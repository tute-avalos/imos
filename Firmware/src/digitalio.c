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
 * @file digitalio.c
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Manejo de entradas y salidas digitales.
 * 
 * @details
 *  Inspirado en Arduino, pero en vez de usar números de pines numericos, 
 * se usan los nombres originales del integrado.
 *
 */
#include <avr/io.h>
#include "digitalio.h"

void pinMode(uint8_t pin, uint8_t mode)
{
    volatile uint8_t * ddr = 0;
    volatile uint8_t * portr = 0;
    
    switch(pin & 0xF0)
    {
        case _PORTB:
            ddr = &DDRB;
            portr = &PORTB;
            break;
        case _PORTC:
            ddr = &DDRC;
            portr = &PORTC;
            break;
        case _PORTD:
            ddr = &DDRD;
            portr = &PORTD;
            break;
    }
    pin &= 0x07;
    *ddr |= ((mode & 1) << pin);
    *portr |= ((mode >> 1) << pin);
}

void digitalWrite(uint8_t pin, uint8_t state)
{
    volatile uint8_t * portr = 0;
    
    switch(pin & 0xF0)
    {
        case _PORTB:
            portr = &PORTB;
            break;
        case _PORTC:
            portr = &PORTC;
            break;
        case _PORTD:
            portr = &PORTD;
            break;
    }
    pin &= 0x07;
    if(state) *portr |= _BV(pin);
    else *portr &= ~_BV(pin);
}

uint8_t digitalRead(uint8_t pin)
{
    volatile uint8_t * pinr = 0;
    switch(pin & 0xF0)
    {
        case _PORTB:
            pinr = &PINB;
            break;
        case _PORTC:
            pinr = &PINC;
            break;
        case _PORTD:
            pinr = &PIND;
            break;
    }
    pin &= 0x07;
    return (*pinr & _BV(pin))? HIGH : LOW;
}