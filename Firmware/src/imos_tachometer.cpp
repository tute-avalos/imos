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
 * @file imos_tachometer.cpp
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 30/01/2017 (dd/mm/yyyy)
 * @brief Controlador para el tacómetro.
 *
 */
#include "imos_tachometer.h"

void imos::Tachometer::engineCount(void)
{
    this->countValue++;
}

void imos::Tachometer::setNextBufferValue(void)
{
    this->buffer[this->index++] = this->countValue;
    if(this->index == this->BUFFER_SIZE) index = 0;
    this->countValue = 0;
}

void imos::Tachometer::timedExec()
{
    this->setNextBufferValue();
}

void imos::Tachometer::fordwarderInterrupt()
{
    this->engineCount();
}

uint16_t imos::Tachometer::getRPM(void)
{
    uint16_t sum = 0;
    for(uint8_t i = 0; i < this->BUFFER_SIZE ;i++)
        sum += this->buffer[i];
    return sum*this->TIME_REFERENCE;
}

imos::Tachometer::Tachometer() : Timed(TIME_RANGE), ExternalInterruptManager(INTERRUPTION_NUMBER)
{
    this->countValue = 0;
}

imos::Tachometer::~Tachometer()
{
    
}
