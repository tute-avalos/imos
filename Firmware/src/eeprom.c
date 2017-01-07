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
 * @file eeprom.c
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Permite leer y escribir en la memoria E²PROM interna del ATmega328P.
 * 
 * @details
 *  Los ATmega328P tienen una memoria interna E²PROM para almacenamiento de
 * datos estáticos, útilies para configuraciones y demás cosas que no deben
 * perderse si el dispositivo deja de estar alimentado.
 * 
 *  En esta librería está pensada para implementaciones en las que el tiempo
 * de ejecución es importante, por ello utiliza la interrupción para escribir
 * en la E²PROM, y donde se desconoce la longitud de los datos a almacenar en
 * la memoria. Por ello, es ideal para almacenar estructuras de datos
 * completas.
 * 
 *  En este archivo se encuentran las implementaciones de las funciones para
 * escribir y leer de la memoria E²PROM del ATmega328P.
 * 
 * @warning Esta librería necesita para funcionar que las interrupciones
 * globales estén habilitadas:
 *          @code sei(); @endcode
 * 
 */  
#include <avr/io.h>
#include <avr/interrupt.h>
#include "eeprom.h"

/**
 *  @brief Flag que indica si la E²PROM está lista para ser leída/escrita.
 *  @ingroup E²PROM
 */
volatile uint8_t fBusy = 0;
/**
 *  @brief Siguiente dirección a escribir en la E²PROM.
 *  @ingroup E²PROM
 */
volatile uint16_t nextAddrs = 0;
/**
 *  @brief Dato a escribir en la E²PROM.
 *  @ingroup E²PROM
 */
volatile uint8_t *actualData = 0;
/**
 *  @brief Cantidad de datos restantes a escribir en la E²PROM.
 *  @ingroup E²PROM
 */
volatile uint16_t remainingData;

uint8_t EEPROM_Busy(void)
{
    return fBusy;
}

/**
 * @brief Instrucciones básicas para escribir en la E²PROM.
 * @param addrs Dirección a escribir el dato.
 * @param data Dato a escribir en la memoria.
 * @ingroup E²PROM
 */
void EEPROM_Write(uint16_t addrs, uint8_t data)
{
    /* Set up address and Data Registers */
    EEAR = addrs;
    EEDR = data;
    /* Write logical one to EEMPE */
    EECR |= _BV(EEMPE);
    /* Start eeprom write by setting EEPE */
    EECR |= _BV(EEPE);
}

/**
 * @brief Instrucciones básicas para leer de la E²PROM.
 * @param addrs Dirección a leer de la memoria.
 * @return data Dato leído de la dirección addrs.
 * @ingroup E²PROM
 */
uint8_t EEPROM_Read(uint16_t addrs)
{
    /* Set up address register */
    EEAR = addrs;
    /* Start eeprom read by writing EERE */
    EECR |= _BV(EERE);
    /* Return data from Data Register */
    return EEDR;
}

int16_t EEPROM_WriteData(uint16_t addrs, void *data, const uint16_t size)
{
    int16_t result = -1; 
    if(!fBusy && (addrs+size) > E2END)
    {
        fBusy = 1;
        nextAddrs = addrs;
        result = addrs + size;
        actualData = (uint8_t*)data;
        remainingData = size;
        EECR |= _BV(EERIE);
    }
    return result;
}

int16_t EEPROM_ReadData(uint16_t addrs, void *dataBuffer, uint16_t size)
{
    int16_t result = -1;
    if(!fBusy)
    {
        result = addrs + size;
        actualData = (uint8_t*) dataBuffer;
        while(!size--)
        {
            *actualData = EEPROM_Read(addrs++);
            actualData++;
        }
    }
    return result;
}

/**
 * @brief Rutina de interrupción de la EEPROM.
 * 
 * @details
 *  La interrupción se ejecuta apenas se habilita la misma,
 * empezando a escribir la primer parte del dato en la EEPROM.
 * Cada vez que termina de escribirse un dato, se ejecuta
 * la interrupción, hasta que no haya más datos que escribir.
 * 
 * @ingroup E²PROM
 */
ISR(EE_READY_vect)
{
    // Si llegué al final de la informaicón inhabilito la
    // interrupción de la EEPROM y bajo el flagBusy.
    if(!remainingData--)
    {
        EECR &= ~_BV(EERIE);
        fBusy = 0;
    }
    else // Sino, escribo el próximo dato.
        EEPROM_Write(nextAddrs++,*(actualData++));
}
