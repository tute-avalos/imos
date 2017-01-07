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
 * @file usart.h
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
/** @defgroup USART USART Implementation
 * @ingroup API
 */ 
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Configura los registros para una comunicación serie a 9600bps.
 * 
 * @details
 *  Se configuran los registros de la USART del ATmega328P para una
 * comunicacion serie a 9600bps con 8bits de datos, 1 bit de stop sin
 * control de flujo.
 * 
 * @note Los valores en los registros están "hardcodeados" para la
 * implementación de IMOS: para una F_clk = 11.0592Mhz y un taza de
 * transferencia de 9600bps.
 * 
 * @ingroup USART
 */
void configUSART(void);

/**
 * @brief Se lee un dato del buffer circular de recepción.
 * 
 * @details
 *  Se lee el primer dato disponible del buffer circular donde se almacena
 * la información recibida por la USART. Si no hay datos nuevos en el buffer
 * se devuelve un -1.
 * 
 * @return data el primer dato del buffer circular. -1 si no hay datos.
 * @ingroup USART
 */
int USART_PopRx(void);

/**
 * @brief Carga un byte en el buffer de transmisión.
 * 
 * @details
 *  Esta función pone un byte en el buffer circular de transmisión
 * de la USART. El dato será transmitido cuando los antecesores sean
 * transmitidos.
 * 
 * @param nDato el byte a transmitir por la USART.
 * @ingroup USART
 */
void USART_PushTx(unsigned char nDato);

/**
 * @brief Carga un string en el buffer de transmisión.
 * 
 * @details
 *  Se carga el string completo en el buffer llamando a la función
 * USART_PushTx hasta encontror el el final del string con el '\0'.
 * 
 * @param pszStr string a enviar por la USART.
 * @ingroup USART
 */
void USART_SendStr(const char * pszStr);

#ifdef __cplusplus
}
#endif


#endif // __USART_H__
