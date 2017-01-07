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
 * @file pwm.c
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Configuración de los canales PWM.
 * 
 * @details
 *  IMOS cuenta con 2 canales PWM, uno de alta y otro de baja frecuencia. Esto
 * no hay un motivo especial para ello. Es importante que haya uno de baja
 * frecuencia ya que manejará un motor (bomba de agua), y a una frecuencia muy
 * alta no es recomendable. El otro canal PWM manejará la intencidad de las
 * luces de la moto, por lo cual no es importante la frecuencia, siempre y
 * cuando no sea muy baja.
 * 
 *  En este archivo están las implementaciones necesarias para la configuración
 * y utilización de los 2 canales PWM que se utilizan.
 * 
 */  
#include <avr/io.h>
#include "pwm.h"

void configPWMChannels(void)
{
    // Se fijan los pines como salida.
    DDRB  |= (_BV(PINB3) | _BV(PINB2));
    // Salida OCR1B del Timer1 como fast PWM de 8 bits.
    TCCR1A = (_BV(COM1B1) | _BV(WGM10));
    // Salida OCR2A del Timer2 como fast PWM (8 bits).
    TCCR2A = (_BV(COM2A1) | _BV(WGM21) | _BV(WGM20));
    // Se setean los prescalers y empiezan a correr los timers.
    TCCR1B = (_BV(WGM12) | _BV(CS11) | _BV(CS10));
    TCCR2B = (_BV(CS22) | _BV(CS21));
}
