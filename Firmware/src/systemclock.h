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
 * @file systemclock.h
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Define un clock para el sistema con ticks de 1ms y 1s.
 * 
 *  Utilizando el Timer0 de 8-bits se genera una interrupción cada 1ms aprox.
 * Esta funcionalidad está pensada para hacer un reloj de sistema que sirva
 * para controlar los eventos o en forma de símil-scheduler.
 * 
 *  En este archivo se encuentran las declaraciones de los elementos.
 * 
 * @warning Debe tenerse en cuenta que para que este sistema funcine, deben
 * habilitarse las interrupciones globales.
 *      @code sei(); @endcode
 * 
 */
/** @defgroup SysClk System Clock implementation
 * @ingroup API
 */ 
#ifndef __SYSCLK_H__
#define __SYSCLK_H__

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief ticks en milisgundos.
 * @ingroup SysClk
 */
extern volatile uint32_t ticks_ms;
/**
 * @brief ticks en segundos.
 * @ingroup SysClk
 */
extern volatile uint16_t ticks_s;

/**
 * @brief inicializa el Timer2 para generar una interrupción cada 1ms
 * 
 * @details
 *  Se calcula el Timer0 para generar una interrupción cada 1ms aprox.
 * con el modo Clear Timer on Compare Match (CTC). Al alcanzar el valor
 * en OCR0A, el Timer se resetea, al habilitar el flag de interrupción
 * ante la comparación se aumenta una variable que cuenta milisegundos
 * y otra cuenta segundos.
 * 
 *  Con el cristal de 11.0592Mhz la mejor combinación fue la siguiente:
 * @code
 
 *  Fclk/Prescaler: 11059200Hz/64 = 172800Hz 
 * 
 *  172800Hz/1000Hz = 172,8 ~> 173 
 * 
 *  F_int = 172800Hz/173 = 998,84Hz -> T_int = 1,001ms 
 * 
 * @endcode
 
 * @note Los tiempos están calculados para la implementación de IMOS,
 * que tiene F_clk = 11.0592Mhz.
 * 
 * @ingroup SysClk
 */
void configSysClk(void);

#ifdef __cplusplus
}
#endif

#endif // __SYSCLK_H__
