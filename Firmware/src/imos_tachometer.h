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
 * @file imos_tachometer.h
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 30/01/2017 (dd/mm/yyyy)
 * @brief Controlador para el tacómetro.
 *
 */
#ifndef __IMOS_TACHOMETER__
#define __IMOS_TACHOMETER__
#include "imos_timed.h"
#include "imos_eim.h"
#include <stdint.h>

namespace imos
{
/**
 * @class Tachometer
 * @brief Mide las RPM del motor.
 * @ingroup TIME_RELATED
 */
class Tachometer : protected Timed, protected ExternalInterruptManager
{
    private:
        static const uint8_t INTERRUPTION_NUMBER = 1;   //!< @brief Número de la interrupción asociada. 
        static const uint16_t TIME_RANGE = 240;         //!< @brief Rango de tiempo para almacenar en el buffer.
        static const uint8_t BUFFER_SIZE = 8;           //!< @brief Tamaño del buffer donde se almacenan las cuentas.
        static const uint8_t TIME_REFERENCE = 60000 / (TIME_RANGE * BUFFER_SIZE); //!< @brief Referencia para calcular las RPM.
        uint8_t buffer[BUFFER_SIZE];    //!< @brief Buffer circular donde se almacenan las últimas lecturas.
        uint8_t index;                  //!< @brief Indice para almacenar en el buffer circular.
        uint8_t countValue;             //!< @brief Cantidad de chispas del motor contabilizadas en el intervalo de tiempo dado por TIME_RANGE.
        
    private:
        /**
         * @brief Cuenta las vueltas que dio el motor.
         * @details
         *  Este método es llamado por la interrupción asociada a las explosiones del motor
         * de la moto. En este caso el motor da 1 pulso por cada vuelta.
         * @see ExternalInterruptManager
         * @ingroup TIME_RELATED
         */
        void engineCount(void);
        /**
         * @brief Almacena en el buffer la última cuenta al vencer el intervalo de tiempo.
         * @details
         *  Almacena el valor en countValue, al vencer el intervalo de tiempo en TIME_RANGE.
         * Una vez almacenado, el contador vuelve a 0 y se sigue acumulando en la próximo
         * intervalo.
         * @see Timed
         * @ingroup TIME_RELATED
         */
        void setNextBufferValue(void);
        /**
         * @brief Método heredado de Timed.
         * 
         * @details
         *  En este método se llama a setNextBufferValue().
         * @see Timed
         * @ingroup TIME_RELATED
         */
        void timedExec();
        /**
         * @brief Método heredado de ExternalInterruptManager.
         * 
         * @details
         *  Este método llama a engineCount(), para aumentar el contador cuando llegue la
         * interrupción.
         * @see ExternalInterruptManager
         * @ingroup TIME_RELATED
         */
        void fordwarderInterrupt();

    public:
        /**
         * @brief Saca el promedio de RPM de las últimas 8 lecturas.
         * @details
         *  Hace una aproximación teniendo en cuentas las últiams 8 lecturas,
         * en el tiempo del intervalo correspondiente, y lo convierte a RPM.
         * @return RPM aproximando las últimas 8 lecturas.
         * @ingroup TIME_RELATED
         */
        uint16_t getRPM(void);
        
    public:
        /**
         * @brief Constructor de la clase.
         * @details
         *  Inicializa y llama a los constructores de las super clases.
         * @ingroup TIME_RELATED
         */
        Tachometer();
        /**
         * @brief Destructor de la clase
         * @ingroup TIME_RELATED
         */
        virtual ~Tachometer();

};

}

#endif // __IMOS_TACHOMETER__
