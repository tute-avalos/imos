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
 * @file adc.h
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Funciones para configurar y leer del ADC.
 * 
 * @details
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
 *  En este archivo están las declaraciones de las funciones para
 * interactuar con el módulo ADC y leer los valores promediados.
 * 
 * @warning Para que esta librería funcione deben estar habilitadas las
 * interrupciones globales.
 *          @code sei(); @endcode
 * 
 */ 
/** @defgroup ADC ADC implementation
 * @ingroup API
 */
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Apaga el módulo del ADC.
 * @ingroup ADC
 */
void ADC_Off(void);

/**
 * @brief Enciende el módulo del ADC.
 * @ingroup ADC
 */
void ADC_On(void);

/**
 * @brief Configura los parámetros del ADC. Y enciende el módulo del ADC.
 * 
 * @details
 *  Se configuran los parámetros para las conversiones Analógicas->Digitales,
 * el prescaler para un clock de 172.8Khz.
 * 
 *  Las lecturas se hacen en 13.5 cíclos de ADC, por lo tanto una lectura de
 * los 6 canales ADC disponibles tarda al rededor de 0,47ms. Por lo tanto,
 * los buffers estarán llenos después de 3,75ms de haber empezado las
 * conversiones. Cualquier dato leído antes de este tiempo no podría asegurar
 * la consistencia de los mismos.
 * 
 * @note Los calculos de frecuencia están hechos para la implementación IMOS,
 * F_clk = 11.0592Mhz.
 * 
 * @ingroup ADC
 */
void configADC(void);

/**
 * @brief Empiezan las conversiones ADC.
 * 
 * @details
 *  Empiezan las conversiones del ADC. Una vez iniciadas, las conversiones
 * se emplean consecutivamente, rotando de canal constantemente y llenando
 * los buffers circulares de 8 posiciones.
 * 
 *  Cuando el módulo ADC se apaga, con @ref ADC_Off, las conversiones se
 * detienen. Esto podría pasar, por ejemplo, cuando se manda "a dormir" al
 * micro. Hay que recordar llamar a @ref ADC_On seguido de esta función 
 * para retormar las conversiones.
 * 
 * @warning Si esta función no es llamada, no empiezan las conversiones,
 * aunque el módulo ADC esté encendido y configurado.
 * 
 * @ingroup ADC
 */
void ADC_Start(void);

/**
 * @brief Se lee el promedio del buffer del canal seleccionado.
 * 
 * @details
 *  Cuando se llama a esta función se devuelve el promedio de las últimas
 * 8 lecturas del ADC del canal ch enviado como parámetro.
 * 
 * @param ch Canal que se desea leer.
 * @return adcValue Valor promedio de las últimas 8 lecturas del canal ADC.
 * @ingroup ADC
 */
uint16_t ADC_Read(uint8_t ch);

#ifdef __cplusplus
}
#endif

#endif // __ADC_H__
