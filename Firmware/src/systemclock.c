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
 * @file systemclock.c
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Define un clock para el sistema con ticks de 1ms y 1s.
 * 
 * @details
 *  Utilizando el Timer0 de 8-bits se genera una interrupción cada 1ms aprox.
 * Esta funcionalidad está pensada para hacer un reloj de sistema que sirva
 * para controlar los eventos o en forma de símil-scheduler.
 * 
 *  En este archivo se encuentran las implementaciones de las funciones.
 * 
 * @warning Debe tenerse en cuenta que para que este sistema funcine, deben
 * habilitarse las interrupciones globales.
 *      @code sei(); @endcode
 * 
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "systemclock.h"

/**
 * @brief Valor a pasarle al Timer0 para generar una interrupcion cada 1ms aprox.
 * @ingroup SysClk
 */
#define T0_TOP  172

volatile uint32_t ticks_ms = 0;
volatile uint16_t ticks_s = 0;

void configSysClk(void)
{
    // Valor en que se produce la interrupción del Timer0
    OCR0A = T0_TOP;
    // OCR0A interrupt enable.
    TIMSK0 = _BV(OCIE0A);
    // CTC MODE.
    TCCR0A = _BV(WGM01);
    // clk_TS0 / 64.
    TCCR0B = (_BV(CS00)|_BV(CS01));     
}

/**
 * @brief Interrupción del timer que se ejecuta contantemente a 1ms aprox.
 * 
 * @details
 *  Cada vez que pasa 1ms aprox. se ejecuta esta interrupción que va 
 * incrementando ticks_ms. Así se puede tener una referencia de tiempo.
 *
 *  Además, cada 1000 se incrementa ticks_s, para tener una referencia
 * en segundos. 
 * 
 * @see ticks_ms, ticks_s
 * 
 * @ingroup SysClk
 */
ISR(TIMER0_COMPA_vect)
{
    // Cada 1ms aprox.
    ticks_ms++;
    if(!(ticks_ms%1000)) ticks_s++; // Cada 1s aprox.
}
