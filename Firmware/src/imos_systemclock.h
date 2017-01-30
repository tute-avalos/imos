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
/** @defgroup SCHEDULING Scheduler Events
 * @ingroup IMPLEMENTATION
 */
#ifndef __IMOS_SYSCLK__
#define __IMOS_SYSCLK__

#include "imos_timed.h"
#include <stdint.h>
#include <stdlib.h>

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
 *      @code void routine(void) @endcode
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
    imos::Timed* timedObj;          //!< @brief Interfaz con método virtual timeExec.
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
        /**
         * @brief Genera un nuevo ID único para los eventos.
         * @return Un ID para un nuevo evento.
         * @ingroup SCHEDULING
         */
        uint8_t getNewID();
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
         * @param timedObj Objeto derivado de Timed, con la implementación del método timeExec.
         * @return el id del evento generado, o -1 si la lista está completa o no pudo
         * encontrarse memoria que asignar.
         * @see listOfEvents, @ref eventNode
         * @ingroup SCHEDULING
         */
        int16_t newEvent(uint16_t time, imos::Timed* timedObj);
        /**
         * @brief Cambia al tiempo y/o rutina asociada al evento con el ID pasado por parámetro
         * @param id ID del evento a modificar.
         * @param time Nuevo tiempo para el evento.
         * @param timedObj Objeto derivado de Timed, con la implementación del método timeExec.
         * @return true si se pudo modificar el evento, false si no.
         * @ingroup SCHEDULING
         */
        bool changeEvent(uint8_t id, uint16_t time, imos::Timed* timedObj);
        /**
         * @brief Borra el evento con la id pasada por parámetro.
         * @param id ID del evento a eliminar de la lista de eventos.
         * @return true si se pudo borrar el elemento, false si no se encontró el elemento.
         * @ingroup SCHEDULING
         */
        bool delEvent(uint8_t id);
    
    
    // Constructores y destructores:
    private:
        static SystemClock* ptrSysClk; //!< @brief Puntero estático para aplicar patrón @b singleton.
        /**
         * @brief Inicializa los parámetros para el manejo de eventos.
         * 
         * @details
         *  El contructor inicializa msTicks para tener la referencia del tiempo
         * referente al hardware.
         * 
         * @see SysClk, ticks_ms, API, getInstance()
         */
        SystemClock();
    public:
        /**
         * @brief Obtiene la única instancia posible de la SystemClock.
         * @param 
         * @return Única referencia posible a la SystemClock.
         * @see ptrSysClk
         */
        static SystemClock* getInstance();
        /**
         * @brief Destructor de la clase.
         */
        virtual ~SystemClock();

};

}

#endif // __IMOS_SYSCLK__
