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
 * @file imos_systemclock.h
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief 
 */
/** @defgroup SCHEDULING Scheduler Events
 * @ingroup IMPLEMENTATION
 */
#ifndef __IMOS_SYSCLK__
#define __IMOS_SYSCLK__

#include "imos_hardwareapi.h"

#define MAX_EVENT_COUNT 20  //!< @brief Cantidad máxima de eventos que pueden estar coexistiendo.

namespace imos
{
/**
 * @struct eventNode
 * @brief Nodo a evento.
 * 
 * @details
 *  Los eventos regidos por tiempo se ejecutan en intervalos de 
 * 'ticks' milisegundos. Los eventos tiene una rutina asociada
 * la cual debe ser una función con la siguiente firma: 
 *      @code void rutine(void) @endcode
 *  De esta forma, esta rutina se ejecutará cada vez que pase el
 * intervalo de tiempo 'ticks', apartir de que es agregada en la
 * lista de eventos de la clase SystemClock. 
 * 
 * @see ticks_ms, @ref SysClk, listOfEvents
 * @ingroup SCHEDULING
 */
typedef struct eventNode
{
    uint8_t id;                     //!< @brief id único del evento.
    uint16_t ticks;                 //!< @brief invervalo de tiempo en que se ejecuta el evento.
    uint32_t lastTick;              //!< @brief último tick en que se ejecutó el evento.
    void (*rutine)(void);           //!< @brief rutina que se ejecuta cuando lleva el evento.
    struct eventNode *nextEvent;    //!< @brief próximo evento, si hay.
} event_t;

/**
 * @class SystemClock
 *
 * @brief Clase que maneja el semi-scheduler.
 * 
 * @details
 *  En esta clase se implementan los manejos de los eventos, y los 
 * métodos necesarios para agregar, quitar y modificarlos. Implementa
 * una lista simplemente enlazada concatenando nodos de eventos cada
 * vez que se agerga uno.
 *  Es recomendable no agregar muchos eventos, de todas formas hay un
 * límite hardcodeado MAX_EVENT_COUNT. Para evitar una cantidad
 * exesiva de eventos.
 * 
 * @note
 *  Esta clase sólo utiliza la API para acceder a los ticks del SysClk.
 * 
 * @ingroup SCHEDULING
 */
class SystemClock
{
    private:
        HardwareAPI* API;                  //!< @brief Puntero a la API de IMOS.
        uint8_t eventsCount = 0;        //!< @brief Cantidad de eventos que coexisten.
        uint8_t eventsIDs = 0;          //!< @brief Generador de IDs de eventos.
        uint32_t msTicks;               //!< @brief Ticks que se leen de ticks_ms.
        event_t* listOfEvents = NULL;   //!< @brief Lista de eventos.

    // Actualización de datos:
    public:
        /**
         * @brief Refresca el timer y ejecuta los eventos que haya alcansado su tick.
         * 
         * @details
         *  Esta función básicamente lee el ticks_ms y compara todos los lastTick de
         * todos los eventos que estén cargados en la lista de eventos (listOfEvents).
         *  Para ello debe recorrer la lista, entre más eventos haya cargados, más se
         * demorará en recorrerlos. Por eso no es bueno abusar de ellos.
         * 
         * @ingroup SCHEDULING
         */
        void refresh(void);
    private:
        /**
         * @brief Verifica si hay eventos que deban ejecutarse.
         * 
         * @details
         *  Este método privado es llamado por refresh().
         *  Es un bucle que va recorriendo toda la lista de eventos y ejecutando,
         * si corresponde, las rutinas asociadas a los mismos. Entre más eventos
         * haya en la lista de eventos, más se tardará en recorrerlos, por eso
         * no es recomendable cargar muchos.
         * 
         * @see refresh(), eventNode, listOfEvents
         * @ingroup SCHEDULING
         */
        void checkEvents(void);
        
    // Manejadores de eventos:
    public:
        /**
         * @brief Agrega un evento a la lista de eventos.
         * 
         * @details
         *  Este método es el encargado de agregar un evento a la lista de eventos
         * manejados por el reloj del sistema. 
         * 
         * @param time Intervalo de tiempo en que se ejecuta la rutina.
         * @param rutine Rutina que se ejecuta cada vez que se genera el evento.
         * @return el id del evento generado, o -1 si la lista está completa o no pudo
         * encontrarse memoria que asignar.
         * @see listOfEvents, @ref eventNode
         * @ingroup SCHEDULING
         */
        int16_t newEvent(uint16_t time, void (*rutine)(void));
        //bool changeEvent(uint8_t id, uint16_t time, void (*rutine)(void));
        /**
         * @brief Burra el evento con la id pasada por parámetro.
         * @param id ID del evento a eliminar de la lista de eventos.
         * @return true si se pudo borrar el elemento, false si no se encontró el elemento.
         * @ingroup SCHEDULING
         */
        bool delEvent(uint8_t id);
    
        #ifdef __DEBUG__
        public:
        /**
         * @brief Imprime por la terminal serie la lista de eventos...
         * @todo borrar esta función cuando ya esté depurado
         * @ingroup SCHEDULING
         */
        void debugListOfEvents(void);
        #endif
    
    // Constructores y destructores:
    public:
        /**
         * @brief Inicializa los parámetros para el manejo de eventos.
         * 
         * @details
         *  El contructor inicializa msTicks para tener la referencia del tiempo
         * referente al hardware.
         * 
         * @param API Una instancia de la API de IMOS, para acceder al hardware relacionado al SysClk.
         * @see SysClk, ticks_ms, API
         */
        SystemClock(HardwareAPI* API);
        /**
         * @brief Destructor de la clase.
         */
        virtual ~SystemClock();

};

}

#endif // __IMOS_SYSCLK__
