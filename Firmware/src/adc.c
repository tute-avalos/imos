/*******************************************************************************
 * 
 * Copyright (C) 2017 Matías Sebastián Ávalos <msavalos@gmail.com>
 *
 * This file is part of Integral Moto Open System (IMOS).
 *
 *    Integral Moto Open System (IMOS) is free software: you can 
 *    redistribute it and/or modify it under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation, either version 3 of the License, or (at your option)
 *    any later version.
 *
 *    Integral Moto Open System (IMOS) is distributed in the hope that
 *    it will be useful, but WITHOUT ANY WARRANTY; without even the 
 *    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 *    PURPOSE. See the GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Integral Moto Open System (IMOS).
 *    If not, see <http://www.gnu.org/licenses/>.
 *  
 * 
 * Fork me on GitHub: https://github.com/tute-avalos/imos/
 * 
 ******************************************************************************/
/**
 * @file adc.c
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Funciones para configurar y leer del ADC.
 * 
 *  En el ATmega328P en formato DIP de 28 pines cuenta con 6 canales ADC
 * externos. Con esta librería se leen constantemente los 6 canales y se
 * almacenan en buffers circulares de 8 posiciones. Así, cuando se hace
 * una lectura, se promedian las últimas 8 lecturas del canal elegido.
 * 
 *  Las conversiones inician inmediatamente cuando se termina la anterior
 * haciendo utilización de la interrupción asociada al ADC.
 * 
 *  Además es posible apagar el módulo del ADC una vez configurado para
 * disminuir el consumo cuando se está en alguno de los modos de bajo
 * consumo que ofrece el micro.
 * 
 *  En este archivo están las implementaciones de las funciones para
 * interactuar con el módulo ADC y leer los valores promediados.
 * 
 * @warning Para que esta librería funcione deben estar habilitadas las
 * interrupciones globales.
 *          @code sei(); @endcode
 * 
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

/**
 * @brief Cantidad de ADC a utilizar (del ADC0 al ADC5)
 * @ingroup ADC
 */
#define CANT_ADC    6

/**
 * @brief Cantidad de lecturas del ADC a promediar.
 * @ingroup ADC
 */
#define MAX_BUFFER  8

/**
 * @brief Buffers de los canales ADC.
 * @ingroup ADC
 */
volatile uint16_t ADCbuffers[CANT_ADC][MAX_BUFFER];
/**
 * @brief Indices de los Buffers ADC.
 * @ingroup ADC
 */
volatile uint8_t bffIndex[CANT_ADC];
/**
 * @brief Canal seleccionado para la lectura.
 * @ingroup ADC
 */
volatile uint8_t channel = 0;

inline void ADC_Off(void) { ADCSRA &= ~_BV(ADEN); }

inline void ADC_On(void) { ADCSRA |= _BV(ADEN); }

void configADC(void)
{
    // Referencia desde AVcc.
    ADMUX |= _BV(REFS0); 
    // Se inhabilitan todos los buffers de los canales ADC.
    DIDR0 = 0x3F; 
    // Prescaler: Fclk/64 = 172,8Khz.
    ADCSRA |= (_BV(ADPS2) | _BV(ADPS1) | _BV(ADIE)); 
    ADC_On();
}

void ADC_Start(void)
{
    channel = 0;
    ADMUX &= 0xF0;
    ADMUX |= channel;
    DIDR0 = (0x3F & _BV(channel));
    ADCSRA |= _BV(ADSC);
}

uint16_t ADC_Read(uint8_t ch)
{
    uint16_t result = 0;
    uint8_t i;
    for(i=0;i<MAX_BUFFER;i++)
        result += ADCbuffers[ch][i];
    return result/8;
}

/**
 * @brief En la interrupción se cargan los buffers de los ADC.
 * 
 * @details
 *  Cada vez que se ejecuta la interrupción es porque terminó
 * la conversión, entonces, se almacena ese dato y se incrementan
 * los indices bffIndex y channel y empieza la próxima conversión.
 *
 * @ingroup ADC
 */
ISR(ADC_vect)
{
    // Se lee el valor del ADC en el buffer del canal seleccionado en
    // la posición de indice indicada.
    ADCbuffers[channel][bffIndex[channel]] = ADC;
    // Se incrementa el indice del buffer del canal seleccionado.
    bffIndex[channel]++;
    bffIndex[channel]%=MAX_BUFFER;
    // Se pasa al siguiente canal.
    channel++;
    channel%=CANT_ADC;
    // Se cambia el canal del ADC.
    ADMUX &= 0xF0;
    ADMUX |= channel;
    // Se habilita solamente el buffer (del micro) que corresponde
    // al canal que se va a leer.
    DIDR0 = (0x3F & _BV(channel));
    // Se inicia una nueva conversión.
    ADCSRA |= _BV(ADSC);
}
