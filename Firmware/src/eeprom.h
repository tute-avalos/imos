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
 * @file eeprom.h
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
 *  En este archivo se encuentran las declaraciones de las funciones para
 * escribir y leer de la memoria E²PROM del ATmega328P.
 * 
 * @warning Esta librería necesita para funcionar que las interrupciones
 * globales estén habilitadas:
 *          @code sei(); @endcode
 * 
 */ 
/** @defgroup E²PROM E²PROM implementation
 * @ingroup API
 */
#ifndef __EEPROM_H__
#define __EEPROM_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Indica si la E²PROM se puede escribir/leer.
 * 
 * @details
 *  Si la E²PROM no está ocupada la función devuelve un 0.
 *  Si la E²PROM está ocupada la función devuelve un 1.
 * 
 * @return fBusy Si la E²PROM está ocupada para leer/escribir.
 * @ingroup E²PROM
 */
uint8_t EEPROM_Busy(void);


/**
 * @brief Escribe la cantidad de datos que son enviados a la E²PROM.
 * 
 * @details
 *  Esta función guarda en un puntero el dato a ser almacenado, y lo
 * va gurdando en sucesivas escrituras en la E²PROM, según el tamaño
 * del dato a ser gurdado.
 * 
 *  Cada vez que la E²PROM termina de escribir un byte, se ejecuta
 * la interrupción EE_READY y se procede a almacenar el próximo byte,
 * en caso de que sea necesario. De esta forma el programa prosigue
 * mientras se escribe la E²PROM.
 * 
 * @warning Es importante que el dato no cambie mientras se está
 * almacenando en al E²PROM o esto causará datos corruptos.
 * 
 * @param addrs dirección donde se desea almacenar los datos.
 * @param data referencia a los datos a almacenar.
 * @param size tamaño en bytes del dato a almacenar.
 * @return nextAddrs próxima dirección (después del dato que se está
 * guardando).
 * @ingroup E²PROM
 */
int16_t EEPROM_WriteData(uint16_t addrs, void *data, const uint16_t size);

/**
 * @brief Lee los datos de la E²PROM en el buffer dado como parametro.
 * 
 * @details
 *  Para leer los datos de la E²PROM no hay demoras, por lo tanto, el
 * programa debe esperar a que se termine de llenar el buffer para 
 * proseguir. 
 * 
 * @param addrs Dirección de los datos a leer.
 * @param dataBuffer Buffer donde se almacenan los datos leídos.
 * @param size Cantidad de datos a leer.
 * @return nextAddrs La dirección siguiente a la de los datos leídos.
 * @ingroup E²PROM
 */
int16_t EEPROM_ReadData(uint16_t addrs, void *dataBuffer, const uint16_t size);

#ifdef __cplusplus
}
#endif

#endif // __EEPROM_H__