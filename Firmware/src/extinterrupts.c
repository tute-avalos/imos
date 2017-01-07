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
 * @file extinterrupts.c
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Configuración de las interrupciones externas.
 * 
 * @details
 *  El ATmega328P posee 2 interrupciones externas que cuentan con variadas
 * configuraciones. En este caso, se utilizan en el modo que se dispara cuando
 * detecta un flanco descendente.
 * 
 *  Esta librería permite asociar en tiempo de ejecución cualquier función o
 * sub-rutina con el formato:
 *      
 *      @code void func(void) @endcode
 *
 *  De esta manera, la interrupción se habilita cuando se llama a la función
 * attachIntFunction() y puede ser inhabilitada con la misma función si
 * se le pasa un NULL como parámetro.
 * 
 *  En este archivo se encuentra la implementación de la función
 * attachIntFunction().
 * 
 * @warning Es importante que las interrupciones globales estén habilitadas,
 * sino nunca se ejecutarán las funciones asociadas a las interrupciones.
 *          @code sei(); @endcode
 * 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "extinterrupts.h"

/**
 * @brief Función que no hace nada.
 * 
 * @details
 *  Esta función está asociada a las interrupciones externas, en caso de que
 * se produzca inesperadamente una interrupción.
 * @ingroup Interrupts
 */
void dummyFunction(void) {}

/**
 * @brief Puntero a la función que se ejecuta cuando se genera la interrupción
 * asociada a INT0.
 * 
 * @ingroup Interrupts
 */
void (*int0Function)(void) = dummyFunction;
/**
 * @brief Puntero a la función que se ejecuta cuando se genera la interrupción
 * asociada a INT1.
 * 
 * @ingroup Interrupts
 */
void (*int1Function)(void) = dummyFunction;

void attachIntFunction(uint8_t nInt, void (*func)(void))
{
    if(func != 0)
    {
        switch(nInt)
        {
            case INT0:
                EICRA |= _BV(ISC01);
                EIMSK |= _BV(INT0);
                int0Function = func;
                break;
            case INT1:
                EICRA |= _BV(ISC11);
                EIMSK |= _BV(INT1);
                int1Function = func;
                break;
        }
    }
    else
    {
        switch(nInt)
        {
            case INT0:
                EIMSK &= ~_BV(INT0);
                int0Function = dummyFunction;
                break;
            case INT1:
                EIMSK &= ~_BV(INT1);
                int1Function = dummyFunction;
                break;
        }
    }
}

/**
 * @brief Ejecución de la interrupción externa 0.
 * 
 * @details
 *  Cuando está configurada, y llega un flanco descendente,
 * se ejecuta la función a la que esté apuntando int0Function().
 * 
 * @see int0Function
 * 
 * @ingroup Interrupts
 */
ISR(INT0_vect)
{
    int0Function();
}

/**
 * @brief Ejecución de la interrupción externa 1.
 * 
 * @details
 *  Cuando está configurada, y llega un flanco descendente,
 * se ejecuta la función a la que esté apuntando int1Function().
 * 
 * @see int1Function
 * 
 * @ingroup Interrupts
 */
ISR(INT1_vect)
{
    int1Function();
}
