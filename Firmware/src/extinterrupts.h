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
 * @file extinterrupts.h
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
 *  En este archivo se encuentra la declaración de la función
 * attachIntFunction().
 * 
 * @warning Es importante que las interrupciones globales estén habilitadas,
 * sino nunca se ejecutarán las funciones asociadas a las interrupciones.
 *          @code sei(); @endcode
 * 
 */ 
/** @defgroup Interrupts Interrupts implementation
 * @ingroup API
 */
#ifndef __EXTINT_H__
#define __EXTINT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief Asocia y habilita la interrupción externa INTx con la rutina func.
 * 
 * @details
 *  Con esta función se asocia una función o sub-rutina func a la interrupción
 * nInt externa. En el caso del ATmega328P las únicas posibilidades son INT0 o
 * INT1. 
 * 
 *  Por seguridad, si la interrupción se dispara por algún motivo las subrutinas
 * están asociadas a una rutina que no hace nada.
 * 
 *  @note Si se pasa NULL como func, la interrupción se inhabilita.
 * 
 *  @param nInt Número de interrupción a la cual asociar la rutina func.
 *  @param func Puntero a la sub-rutina a ejecutarse cuando se dispare la interrupción.
 *  @ingroup Interrupts
 */
void attachIntFunction(uint8_t nInt, void (*func)(void));

#ifdef __cplusplus
}
#endif

#endif // __EXTINT_H__