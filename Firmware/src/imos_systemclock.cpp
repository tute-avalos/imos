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
 * @brief Se proporciona la funcionalidad de ejecutar rutinas cada una 
 * x cantidad de ms.
 * 
 * @details
 *  En la clase SystemClock se manejan los eventos temporales tipo event_t,
 * que estan compuestas por un tick de tiempo, que es el invervalo en el 
 * cual se ejecuta la rutina asociada con el puntero routine. Para ello
 * utiliza una lista simplemente enlazada para agregar y eliminar los mismos.
 *
 *  Cada vez que se crea un evento, se genera un ID único para el mismo,
 * con este es posible luego modificar el evento o eliminarlo según la
 * necesidad.
 * 
 *  De esta manera se puede hacer una programación basada en tiempos, es decir,
 * ejecutando rutinas cada un intervalo fijo de tiempo, lo cual da una
 * gran flexibilidad y es más tecnificado que hacer una "maquina de timers".
 * 
 * 
 */
#include "imos_hardwareapi.h"
#include "imos_systemclock.h"

void imos::SystemClock::refresh()
{
    checkEvents();
}

void imos::SystemClock::checkEvents(void)
{
    static uint32_t lastTick = this->msTicks;
    static HardwareAPI* API = imos::HardwareAPI::getInstance();
    this->msTicks = API->getMillis();
    if(this->msTicks != lastTick)
    {
        imos::event_t* tmp = this->listOfEvents;
        while(tmp)
        {
            if((this->msTicks-tmp->lastTick) >= tmp->ticks)
            {
                tmp->timedObj->timedExec();
                tmp->lastTick = this->msTicks;
            }
            tmp = tmp->nextEvent;
        }
    }
    lastTick = this->msTicks;
}

uint8_t imos::SystemClock::getNewID()
{
    event_t* aux = this->listOfEvents;
    this->eventsIDs++;
    if(aux)
    {
        while(aux)
        {
            if(aux->id == this->eventsIDs)
            {
                this->eventsIDs++;
                aux = this->listOfEvents;
            }
            else aux = aux->nextEvent;
        }
    }
    return this->eventsIDs;
}

int16_t imos::SystemClock::newEvent(uint16_t time, imos::Timed* timedObj)
{
    int16_t result = -1;
    if(this->eventsCount < MAX_EVENT_COUNT && time && timedObj != NULL)
    {
        imos::event_t* nuevoEvento = new imos::event_t;
        if(nuevoEvento)
        {
            this->eventsCount++;
            result = nuevoEvento->id = this->getNewID();
            nuevoEvento->ticks = time;
            nuevoEvento->timedObj = timedObj;
            nuevoEvento->nextEvent = this->listOfEvents;
            this->listOfEvents = nuevoEvento;
        }
    }
    return result;
}

bool imos::SystemClock::changeEvent(uint8_t id, uint16_t time, imos::Timed* timedObj)
{
    bool result = false;
    imos::event_t* aux = this->listOfEvents;
    if(time)
    {
        while(aux && !result)
        {
            if(aux->id == id)
            {
                aux->ticks = time;
                if(timedObj!=NULL) aux->timedObj = timedObj;
                result = true;
            }
            else aux = aux->nextEvent;
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

imos::SystemClock::SystemClock()
{
    HardwareAPI* API = imos::HardwareAPI::getInstance();
    this->msTicks = API->getMillis();
}

imos::SystemClock* imos::SystemClock::ptrSysClk = NULL;

imos::SystemClock* imos::SystemClock::getInstance()
{
    if(ptrSysClk == NULL)
    {
        ptrSysClk = new SystemClock();
    }
    return ptrSysClk;
}

imos::SystemClock::~SystemClock()
{
}
