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
 * @file imos_timed.h
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 30/01/2017 (dd/mm/yyyy)
 * @brief Interfaz para utilizar el SystemClock.
 *
 */
/** @defgroup TIME_RELATED Relacionado con el tiempo.
 * @brief Interfaces e implementaciones relacionadas con el tiempo.
 * @ingroup IMPLEMENTATION
 */
#ifndef __IMOS_TIMED__
#define __IMOS_TIMED__

#include <stdint.h>

namespace imos
{

/**
 * @class Timed
 * @brief Interfaz para la aplicación del SystemClock.
 * 
 * @details
 *  Timed pide en el contructor el valor en ms para el intervalo
 * de tiempo en que se ejecuta el método timedExec por medio del
 * cuasi-scheduler implementado en SystemClock.
 * 
 * @see SystemClock
 * @ingroup TIME_RELATED
 */
class Timed
{
    private:
        uint8_t id; //!< @brief ID entragada por SystemClock.
        
    public:
        /**
         * @brief Método virtual a implementar por las clases derivadas.
         * 
         * @details
         *  Este método es llamado en el intervalo de tiempo "time" pasado
         * como parametro en el contructor.
         * @ingroup TIME_RELATED
         */
        virtual void timedExec() = 0;
        
    public:
        
        /**
         * @brief Constructor de la clase donde se ingresa al SystemClock.
         * @param time Intervalo de tiempo en que se ejecuta timeExec().
         * @ingroup TIME_RELATED
         */
        Timed(const uint16_t time);
        
        /**
         * @brief Destructor de la clase.
         * @ingroup TIME_RELATED
         */
        virtual ~Timed();

};

}

#endif // __IMOS_TIMED__
