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
 * @file imos_eim.cpp
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 30/01/2017 (dd/mm/yyyy)
 * @brief Implementación de las interrupciones externas.
 *
 */
#include "imos_eim.h"
#include "imos_hardwareapi.h"
#include <stdlib.h>

imos::ExternalInterruptManager* imos::ExternalInterruptManager::intZeroObj = NULL;
imos::ExternalInterruptManager* imos::ExternalInterruptManager::intOneObj = NULL;

void imos::ExternalInterruptManager::interruptZero(void)
{
    intZeroObj->fordwarderInterrupt();
}

void imos::ExternalInterruptManager::interruptOne(void)
{
    intOneObj->fordwarderInterrupt();
}

imos::ExternalInterruptManager::ExternalInterruptManager(const uint8_t interrupt_nbr)
{
    HardwareAPI* API = HardwareAPI::getInstance();
    this->interrupt_nbr = interrupt_nbr;
    
    switch(interrupt_nbr)
    {
        case 0:
            API->setInterrupcion(0,imos::ExternalInterruptManager::interruptZero);
            intZeroObj = this;
            break;
        case 1:
            API->setInterrupcion(1,imos::ExternalInterruptManager::interruptOne);
            intOneObj = this;
            break;
    }
}

imos::ExternalInterruptManager::~ExternalInterruptManager()
{
    HardwareAPI* API = HardwareAPI::getInstance();
    API->setInterrupcion(interrupt_nbr,NULL);
}

