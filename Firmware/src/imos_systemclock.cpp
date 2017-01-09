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
 * @file imos_systemclock.cpp
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief 
 */

#include "imos_systemclock.h"

void imos::SystemClock::refresh()
{
    checkEvents();
}

void imos::SystemClock::checkEvents(void)
{
    static uint32_t lastTick = this->msTicks;
    this->msTicks = this->API->getMillis();
    if(this->msTicks != lastTick)
    {
        imos::event_t* tmp = this->listOfEvents;
        while(tmp)
        {
            if((this->msTicks-tmp->lastTick) >= tmp->ticks)
            {
                tmp->rutine();
                tmp->lastTick = this->msTicks;
            }
            tmp = tmp->nextEvent;
        }
    }
    lastTick = this->msTicks;
}

int16_t imos::SystemClock::newEvent(uint16_t time, void (*rutine)(void))
{
    int16_t result = -1;
    if(this->eventsCount < MAX_EVENT_COUNT && time)
    {
        imos::event_t* nuevoEvento = new imos::event_t;
        if(nuevoEvento)
        {
            this->eventsCount++;
            result = nuevoEvento->id = this->eventsIDs++;
            nuevoEvento->ticks = time;
            nuevoEvento->rutine = rutine;
            nuevoEvento->nextEvent = this->listOfEvents;
            this->listOfEvents = nuevoEvento;
        }
    }
    return result;
}

bool imos::SystemClock::delEvent(uint8_t id)
{
    bool result = false;
    uint8_t count = 0;
    imos::event_t* tmp = this->listOfEvents;
    imos::event_t* aux = NULL;
    
    while(tmp)
    {
        if(tmp->id == id)
        {
            if(!count) this->listOfEvents = tmp->nextEvent;
            else aux->nextEvent = tmp->nextEvent;
            delete tmp;
            this->eventsCount--;
            result = true;
        }
        else
        {
            aux = tmp;
            tmp = tmp->nextEvent;
            count++;
        }
    }
    return result;
}

#ifdef __DEBUG__
void imos::SystemClock::debugListOfEvents(void)
{
    char s[10];
    event_t * tmp = this->listOfEvents;
    if(!tmp) API->bluetoothSendCmd("Nop");
    while(tmp)
    {
        API->bluetoothSendCmd(itoa((uint16_t)tmp,s,16));
        API->bluetoothSendByte('(');
        API->bluetoothSendCmd(itoa(tmp->id,s,10));
        API->bluetoothSendByte(')');
        API->bluetoothSendCmd(" -> ");
        tmp = tmp->nextEvent;
    }
    API->bluetoothSendCmd("\r\n");
}
#endif // __DEBUG__

imos::SystemClock::SystemClock(imos::HardwareAPI* API)
{
    this->API = API;
    this->msTicks = this->API->getMillis();
}

imos::SystemClock::~SystemClock()
{
}
