/*************************************************************************************************
ATmega8 & ATmega328P UART functions

    Based on "Alarma para 8051" by Ezequiel Gastón Miravalles
    http://www.neoegm.com/

Copyright (C) 2013,2017 Matías Sebastián Ávalos

This file is part of ATmega8 & ATmega328P UART functions.

    ATmega8 & ATmega328P UART functions is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ATmega8 & ATmega328P UART functions is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ATmega8 & ATmega328P UART functions.  If not, see <http://www.gnu.org/licenses/>.

    site: http://electronlinux.wordpress.com/2012/10/23/funciones-utiles-para-la-uart-en-un-avr-atmega8/
    mail: msavalos@gmail.com
*************************************************************************************************/
/**
 * @file usart.c
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Funciones para el manejode la comunicación serie USART.
 * 
 * @details
 *  Esta librería fue ligeramente adaptada para IMOS, que utiliza técnicas conocidas
 * de buffers circulares e interrupciones para no perder tiempo en la comunicación
 * serie.
 * 
 * @warning Para que la librería funcione deben estar habilitadas las interrupciones
 * globales del micro.
 *          @code sei(); @endcode
 * 
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

/**
 * @brief Tamaño del buffer de Transmisión en bytes.
 * @ingroup USART
 */
#define MAX_BUFFER_TX	128
/**
 * @brief Tamaño del buffer de Recepción en bytes.
 * @ingroup USART
 */
#define MAX_BUFFER_RX	128

#if defined  (__AVR_ATmega8__)
#define REGISTRO_DE_RECEPCION   UDR
#define REGISTRO_DE_TRANSMISION UDR
#define RX_VECTOR USART_RXC_vect
#define TX_VECTOR USART_TXC_vect
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
#define REGISTRO_DE_RECEPCION   UDR0
#define REGISTRO_DE_TRANSMISION UDR0
#define RX_VECTOR USART_RX_vect
#define TX_VECTOR USART_TX_vect
#endif // defined
/**
 * @brief Inicialización de la transmisión.
 * @ingroup USART
 */
#define ARRANCAR_TX SendData()

//Buffers -------------------------------------------------------
/**
 * @brief Buffer de Transmisión.
 * @ingroup USART
 */
volatile unsigned char BufferTx[MAX_BUFFER_TX];	//Buffer de Transmision
/**
 * @brief Buffer de Recepción.
 * @ingroup USART
 */
volatile unsigned char BufferRx[MAX_BUFFER_RX];	//Buffer de Recepcion
//---------------------------------------------------------------
//Índices -------------------------------------------------------
/**
 * @brief Indice HEAD del buffer circular de transmisión.
 * @ingroup USART
 */
volatile unsigned char IndiceTxIn;
/**
 * @brief Indice TAIL del buffer circular de transmisión.
 * @ingroup USART
 */
volatile unsigned char IndiceTxOut;
/**
 * @brief Indice HEAD del buffer circular de recepción.
 * @ingroup USART
 */
volatile unsigned char IndiceRxIn;
/**
 * @brief Indice TAIL del buffer circular de recepción.
 * @ingroup USART
 */
volatile unsigned char IndiceRxOut;
//---------------------------------------------------------------
/**
 * @brief Flag que indica si hay datos enviandose.
 * @ingroup USART
 */
unsigned char fEnviando;

void configUSART(void)
{
    /* Configuración de 9600 con error del 0.0% a 11.0592Mhz de F_CPU */
    UBRR0L = 143;   // Dato sacado de la hoja de datos
    UCSR0A = _BV(U2X0); // Doblar el Baudrate

    /* UART con 8-bits de datos y 1 de STOP sin paridad */
    UCSR0C = (_BV(UCSZ01) | _BV(UCSZ00)); // 8-bits de dato

    /* Habilitación de la Recepcion y la Transmisión e interrupciones
        de recepción y transmisión terminada */
    UCSR0B = (_BV(RXCIE0) | _BV(TXCIE0) | _BV(RXEN0) | _BV(TXEN0));
}

/**
 * @brief Toma el primer dato del buffer circular de transmición para ser enviado.
 * @return nDato El dato que debe ser transmitido por la USART.
 * @ingroup USART
 */
int USART_PopTx(void)
{
    int nDato = -1;

    if (IndiceTxIn != IndiceTxOut)
    {
        nDato = (unsigned int)BufferTx[IndiceTxOut];
        IndiceTxOut++;
        IndiceTxOut %= MAX_BUFFER_TX;
    }

    return nDato;
}

/**
 * @brief Envia el primer dato que encuentre en el buffer por la USART.
 * 
 * @details
 *  Toma el primer dato que hay en disponible en el buffer circular de
 * transmisión y lo pone en el registro del SFR para empezar a enviarlo.
 * 
 * @ingroup USART
 */
void SendData(void)
{
    int nDato;

    nDato = USART_PopTx();    //Tomo el dato a Transmitir

    if (nDato >= 0)
    {
        //Hay dato en la cola
        fEnviando=1;

        REGISTRO_DE_TRANSMISION = (unsigned char)nDato;	//Inicio transmisión
    }
    else
        fEnviando=0;
}

void USART_PushTx(unsigned char nDato)
{
    BufferTx[IndiceTxIn] = nDato;

    IndiceTxIn++;
    IndiceTxIn %= MAX_BUFFER_TX;

    if (!fEnviando)
        ARRANCAR_TX;
}

/**
 * @brief Inserta un dato en el buffer de Recepción.
 * @param nDato dato a insertar en el buffer.
 * @ingroup USART
 */
void USART_PushRx(unsigned char nDato)
{
    BufferRx[IndiceRxIn] = nDato;
    IndiceRxIn++;
    IndiceRxIn %= MAX_BUFFER_RX;
}


int USART_PopRx(void)
{
    int nDato = -1;

    if (IndiceRxIn != IndiceRxOut)
    {
        nDato = (unsigned int)BufferRx[IndiceRxOut];
        IndiceRxOut++;
        IndiceRxOut %= MAX_BUFFER_RX;
    }

    return nDato;
}

void USART_SendStr(const char * pszStr)
{
    for (; *pszStr; pszStr++)
        USART_PushTx(*(pszStr));
}


/**
 * @brief Interrupción que carga el buffer de recepción.
 * 
 * @details
 *  Cada vez que llega un dato por la USART, esta carga el dato
 * recepcionado en el buffer circular de recepción.
 * 
 * @ingroup USART
 */
ISR(RX_VECTOR)
{
    unsigned char nDato;
    nDato = REGISTRO_DE_RECEPCION;      //Lectura del dato
    USART_PushRx(nDato); //Guardo el dato
}

/**
 * @brief Interrupción que envía los datos en el buffer de transmisión.
 * 
 * @details
 *  Mientras haya datos en el buffer de transmisión, esta interrupción
 * se ejecutará periodicamente a medida que los datos se terminanan de
 * enviar.
 * 
 * @ingroup USART
 */
ISR(TX_VECTOR)
{
    SendData();
}
