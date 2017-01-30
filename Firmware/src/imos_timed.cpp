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
 * @file imos_timed.cpp
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 30/01/2017 (dd/mm/yyyy)
 * @brief Interfaz para utilizar el SystemClock.
 *
 */
#include "imos_timed.h"
#include "imos_systemclock.h"

imos::Timed::Timed(const uint16_t time)
{
    SystemClock* scheduler = SystemClock::getInstance();
    this->id = scheduler->newEvent(time,this);
}

imos::Timed::~Timed()
{
    SystemClock* scheduler = SystemClock::getInstance();
    scheduler->delEvent(this->id);
}
