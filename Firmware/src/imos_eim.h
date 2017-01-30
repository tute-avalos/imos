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
 * @file imos_eim.h
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 30/01/2017 (dd/mm/yyyy)
 * @brief Implementación de las interrupciones externas.
 *
 */
#ifndef __IMOS_EIM__
#define __IMOS_EIM__

#include <stdint.h>

namespace imos
{

/**
 * @class ExternalInterruptManager
 * @brief Manejador para asociar las interrupciones
 * 
 * @details
 *  La clase debe heredarse e implementar el método fordwarderInterrupt() que
 * será el que se ejecute en la interrupción correspondiente al número pasado
 * en el contructor.
 * 
 * @ingroup TIME_RELATED
 */
class ExternalInterruptManager
{
    private:
        uint8_t interrupt_nbr;                          //!< @brief Número de interrupción externa asociada.
        static ExternalInterruptManager* intZeroObj;    //!< @brief Objeto asociado a la interrupción 0.
        static ExternalInterruptManager* intOneObj;     //!< @brief Objeto asociado a la interrupción 1.
        
    private:
        /**
         * @brief Rutina que ejecuta la interrupción 0.
         * @ingroup TIME_RELATED
         */
        static void interruptZero(void);
        /**
         * @brief Rutina que ejecuta la interrupción 1.
         * @ingroup TIME_RELATED
         */
        static void interruptOne(void);
        
    public:
        /**
         * @brief Método abstracto que contiene las instrucciones a ejecutar en la interrupción
         * asociada.
         * 
         * @details
         *  La clase que hereda debe implementar el contenido que se ejecutará
         * cuando el evento de la interrupción ocurra.
         * 
         * @ingroup TIME_RELATED
         */
        virtual void fordwarderInterrupt(void) = 0;
        
    public:
        /**
         * @brief Constructor de la clase, recibe el N° de interrupción a asociar.
         * 
         * @param interrupt_nbr Número de interrupción a la cual asociar el método.
         * @ingroup TIME_RELATED
         */
        ExternalInterruptManager(const uint8_t interrupt_nbr);
        /**
         * @brief Destructor de la clase, libera la interrupción.
         * @ingroup TIME_RELATED
         */
        virtual ~ExternalInterruptManager();

};

}

#endif // __IMOS_EIM__
