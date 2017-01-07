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
 * @file pwm.h
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
 *  En este archivo están las declaraciones necesarias para la configuración y
 * utilización de los 2 canales PWM que se utilizan.
 * 
 */
/** @defgroup PWM Pulse With Modulation implementation
 * @ingroup API
 */
#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief High Frequency PWM Register. F = 675Hz.
 * @ingroup PWM
 */
#define HFPWMR  OCR1BL

/**
 * @brief Low Frequency PWM Register. F = 165.75Hz.
 * @ingroup PWM
 */
#define LFPWMR  OCR2A

/**
 * @brief Carga un valor x [0~255] para definir ciclo de PWM de alta frecuencia.
 * @param x valor entre 0-255 que define el ciclo del PWM.
 * @ingroup PWM
 */
#define _setHFPWM(x)    HFPWMR = x

/**
 * @brief Carga un valor x [0~255] para definir ciclo de PWM de baja frecuencia.
 * @param x valor entre 0-255 que define el ciclo del PWM.
 * @ingroup PWM
 */
#define _setLFPWM(x)    LFPWMR = x

/**
 * @brief Configura e inicializa los canales PWM de alta y baja frecuiencia.
 * 
 * @details
 *  Antes de intentar utilizar los canales PWM de alta y baja frecuencia
 * debe llamarse a esta función. En la misma se configuran los pines y 
 * los timers asociados.
 * 
 *  La frecuencia del HFPWMR es de 675Hz.
 * 
 *  La frecuencia del LFPWMR es de 165.75Hz.
 * 
 * @note Las frecuencias están fijadas para la implementación específica de
 * IMOS, una F_clk = 11.0592Mhz.
 * 
 * @ingroup PWM
 */
void configPWMChannels(void);

#ifdef __cplusplus
}
#endif

#endif // __PWM_H__