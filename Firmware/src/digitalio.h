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
 * @file digitalio.h
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
/** @defgroup Digital_IO Digital I/O implementation
 * @ingroup API
 */
#ifndef __DIGITALIO_H__
#define __DIGITALIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Digital_IO
 * @{
 */
//#define _PORTA  (1 << 4)
#define _PORTB  (1 << 5)    //!< @brief Selección para el PORTB.
#define _PORTC  (1 << 6)    //!< @brief Selección para el PORTC.
#define _PORTD  (1 << 7)    //!< @brief Selección para el PORTD.

#define _PIN0   0   //!< @brief Selección del pin 0.
#define _PIN1   1   //!< @brief Selección del pin 1.
#define _PIN2   2   //!< @brief Selección del pin 2.
#define _PIN3   3   //!< @brief Selección del pin 3.
#define _PIN4   4   //!< @brief Selección del pin 4.
#define _PIN5   5   //!< @brief Selección del pin 5.
#define _PIN6   6   //!< @brief Selección del pin 6.
#define _PIN7   7   //!< @brief Selección del pin 7.

#define _PB0 (_PORTB | _PIN0)    //!< @brief Pin PORTB0.
#define _PB1 (_PORTB | _PIN1)    //!< @brief Pin PORTB1.
#define _PB2 (_PORTB | _PIN2)    //!< @brief Pin PORTB2.
#define _PB3 (_PORTB | _PIN3)    //!< @brief Pin PORTB3.
#define _PB4 (_PORTB | _PIN4)    //!< @brief Pin PORTB4.
#define _PB5 (_PORTB | _PIN5)    //!< @brief Pin PORTB5.
#define _PB6 (_PORTB | _PIN6)    //!< @brief Pin PORTB6.
#define _PB7 (_PORTB | _PIN7)    //!< @brief Pin PORTB7.

#define _PC0 (_PORTC | _PIN0)    //!< @brief Pin PORTC0.
#define _PC1 (_PORTC | _PIN1)    //!< @brief Pin PORTC1.
#define _PC2 (_PORTC | _PIN2)    //!< @brief Pin PORTC2.
#define _PC3 (_PORTC | _PIN3)    //!< @brief Pin PORTC3.
#define _PC4 (_PORTC | _PIN4)    //!< @brief Pin PORTC4.
#define _PC5 (_PORTC | _PIN5)    //!< @brief Pin PORTC5.
#define _PC6 (_PORTC | _PIN6)    //!< @brief Pin PORTC6.
#define _PC7 (_PORTC | _PIN7)    //!< @brief Pin PORTC7.

#define _PD0 (_PORTD | _PIN0)    //!< @brief Pin PORTD0.
#define _PD1 (_PORTD | _PIN1)    //!< @brief Pin PORTD1.
#define _PD2 (_PORTD | _PIN2)    //!< @brief Pin PORTD2.
#define _PD3 (_PORTD | _PIN3)    //!< @brief Pin PORTD3.
#define _PD4 (_PORTD | _PIN4)    //!< @brief Pin PORTD4.
#define _PD5 (_PORTD | _PIN5)    //!< @brief Pin PORTD5.
#define _PD6 (_PORTD | _PIN6)    //!< @brief Pin PORTD6.
#define _PD7 (_PORTD | _PIN7)    //!< @brief Pin PORTD7.

#define INPUT       0x00    //!< @brief Configurado como entrada.
#define OUTPUT      0x01    //!< @brief Configurado como salida.
#define TRISTATE    0x00    //!< @brief Configurado en Tri-state (Hi-Z)
#define PULLUP      0x02    //!< @brief Configurado con pull-up.

#define LOW     0   //!< @brief Valor/estado del pin en "bajo".
#define HIGH    1   //!< @brief Valor/estado del pin en "alto".
/**
 * @}
 */

/**
 * @brief Indica si el pin es de entrada o salida.
 * @param pin Pin del puerto que se desea configurar.
 * @param mode Modo en que se desea configurar 
 * @ingroup Digital_IO
 */
void pinMode(uint8_t pin, uint8_t mode);

/**
 * @brief Pone un estado en un pin que esté configurado como salida.
 * @param pin Pin al que se desea cambiar el estado.
 * @param state Estado en que se desea poner el pin.
 * @ingroup Digital_IO
 */
void digitalWrite(uint8_t pin, uint8_t state);

/**
 * @brief Estado actual del pin que está configurado como entrada.
 * @param pin Pin que se deasea leer.
 * @return state en que está el pin actualmente.
 * @ingroup Digital_IO
 */
uint8_t digitalRead(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif // __DIGITALIO_H__
 
 