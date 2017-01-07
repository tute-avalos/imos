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
 * @file imos_api.h
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Contenedor de la API de IMOS.
 * 
 * @details
 *  En esta clase se concentra toda la API de IMOS que maneja las partes
 * específicas de Hardware, facilitando el desarrollo de la aplicaión en
 * sí. Se definen los pines y los diferentes módulos de hardware.
 * 
 * @note
 *  Note que este archivo incluye varios archivos propios del
 * microcontrolador
 *      @code
 *        #include <avr/io.h>
 *        #include <avr/interrupt.h>
 *        #include <avr/sleep.h>
 *        #include <stdlib.h>
 *      @endcode   
 * 
 */
/** @defgroup API Hardware API
 * 
 * @brief En este módulo se clasifican las diferentes parte de bajo
 * nivel del Firmware.
 * 
 * @details
 *  La API es la primera capa de abstración de IMOS, y Consta de 7
 * módulos: @ref ADC, @ref Digital_IO, @ref E²PROM, 
 * @ref Interrupts, @ref PWM, @ref SysClk y @ref USART. Los que se
 * programaron en C, y están pensados para un proposito un poco más
 * general que la implementación específica de IMOS, y puede ser
 * fácilmente adaptable para otros proyectos con un mictro controlador
 * AVR de 8-bits.
 * 
 *  Está orientado a la @b no @b pérdida de tiempo en la ejecución de
 * las aplicaciones, tratando de aprovechar al máximo las variadas
 * interrupciones que se proporcionan en el microcontrolador. Es decir
 * que se evitó el método de polling todo lo que se pudo.
 *
 */
#ifndef __IMOS_API__
#define __IMOS_API__

/** @def __AVR_ATmega328P__
 * @brief Microcontrolador seleccionado para el proyecto.
 */
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif // __AVR_ATmega328P__
/** @def F_CPU
 * @brief Frecuencia de trabajo para el CPU.
 */
#ifndef F_CPU
#define F_CPU   11059200
#endif // F_CPU
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include "digitalio.h"

/** @defgroup IMPLEMENTATION
 * @brief Implementación de la @ref API del sistema.
 * 
 * @details
 *  Todos los módulos aquí implementan la @ref API del sistema, es
 * decir, que son una abstracción del microcontrolador. Donde se
 * utiliza C++ y programación orientada a objetos.
 * 
 */
namespace imos_api
{
    
/**
 * @struct configStruct
 * @brief Datos de configuración.
 * 
 * @details
 *  Estos datos son los de configuración del usuario, y se almacenan
 * en la EEPROM.
 *  
 * @ingroup E²PROM
 */
typedef struct configStruct
{
    uint8_t km_mile;            //!< @brief Switch km/h o millas/h.
    char bluetoothPin[5];       //!< @brief PIN de enlace bluetooth, por defecto es "8080".
    uint8_t bluetoothMAC[6];    //!< @brief Dirección MAC del dispositivo asociado a IMOS.
} config_t;

    
/**
 * @class Imos_api
 * @brief Clase contenedora de la API de IMOS.
 * 
 * @details
 *  La calse contiene todos las abstracciones de Hardware para agilizar
 * la códificación del firmware. Ejecuta todas las configuraciones para
 * los módulos de la @ref API. Para poder centrarse en la codificación
 * de la implementación de IMOS.
 * 
 *  Funciona como una @a façade  @a wrapper para las funciones específicas
 * del microcontrolador.
 * 
 * @ingroup IMPLEMENTATION
 */
class Imos_api
{   
    
    /** @addtogroup CONFIGURATION
     * @{
     */
    // EEPROM:
    private:
        config_t configuracion;         //!< @brief Configuración de la aplicación.
        const uint16_t configAddrs = 0; //!< @brief Dirección de la E²PROM donde se guardan las configuraciones.
    public:
        /**
         * @brief Verifica si la EEPROM todavía está escribiendo datos.
         * @return true si la E²PROM está ocupada. false si no.
         */
        inline bool isEepromBusy(void);
        /**
         * @brief Obtiene las configuraciones del sistema de la EEPROM.
         * @return Un puntero a imos_api::config_t donde están las configuraciones
         * del sistema.
         */
        config_t* getConfiguracion();
        /**
         * @brief Guarda las configuraciones del sistema.
         * 
         * @details
         *  Cualquiera haya sido la modificación en la estructura configuracon,
         * se guarda en la EEPROM, para ser leída por quien pida el puntero
         * a la misma.
         * 
         * @see getConfiguracion()
         */
        inline void saveConfiguracion();
    /**
     *@}
     */
     
    
    /** @addtogroup PWR_SUPPLY
     * @{
     */
    // Alimentación
    private:
        static const uint8_t adcBateria = 5;    //!< @brief canal ADC al que se conecta la batería.
        static const uint8_t ledPower = _PD6;   //!< @brief pin digital del led indicador.
        static const uint8_t powerRelay = _PD7; //!< @brief pin que cierra el relé de las alimentaciones.
    public:
        /**
         * @brief Verifica el estado de la alimentación de la batería.
         * @return Voltage de la batería.
         */
        inline uint16_t readBateria();
        /**
         * @brief Activa o desactiva el relé que maneja las alimentaciones de los periféricos.
         * @param on_off 1 cierra el relé que alimenta el resto del circuito.
         * 0 apaga las alimentaciones del circuito.
         */
        inline void setPower(uint8_t on_off);
    /**
     *@}
     */
     
    /** @addtogroup ALARM
     * @{
     */
    // Alarma
    private:
        static const uint8_t pinBocina = _PB1;      //!< @brief pin digital al que se asocia la bocina y las luces.
        static const uint8_t adcAcelerometro = 4;   //!< @brief canal ADC para leer el acelerómetro.
        static const uint8_t pinAlarmaIn = _PB0;    //!< @brief pin digital al que se conecta la entrada para activar/desactivar la moto.
    public:
        /**
         * @brief Enciende o apaga la bocina y las luces de la moto.
         * @param on_off 1 prende la bocina, 0 la apaga.
         */
        inline void setBocina(uint8_t on_off);
        /**
         * @brief Lee el valor del acelerómetro para saber si alguien movió la moto.
         * @return El valor promedio del acelerómetro de las últimas 8 lecturas.
         * @see ADC_Read()
         */
        inline uint16_t readAcelerometro();
        /**
         * @brief Entrada para activar/desactivar la alarma mecánicamente.
         * @return Valor digital para saber si se cerró el contacto o no para la alarma.
         */
        inline uint8_t readAlarmaIn();
    /**
     *@}
     */
    
    /** @addtogroup LIGHTING
     * @{
     */
    // Luminaria
    private:
        static const uint8_t adcLDR = 3;    //!< @brief Canal ADC para leer el LDR.
    public:
        /**
         * @brief Lee el valor del LDR para determinar si hace falta encender las luces.
         * @return Valor promedio de las últimas 8 lecturas del LDR.
         * @see ADC_Read()
         */
        inline uint16_t readLDR();
        /**
         * @brief Enciende las luces con una determinada intensidad.
         * @param value Valor entre [0~255] para determinar la intensidad de las luces.
         * @see HFPWMR
         * @see @ref PWM
         */
        void setLucesValue(uint8_t value);
    /**
     *@}
     */
    
    /** @addtogroup TEMP
     * @{
     */ 
    // Temperatura
    private:
        static const uint8_t adcTempAmb = 1;    //!< @brief Canal ADC del sensor de temperatura ambiente.
        static const uint8_t adcTempMotor =  2; //!< @brief Canal ADC del sensor de temperatura del motor.
    public:
        /**
         * @brief Lectura de la temperatura ambiente.
         * @return Valor del sensor de la temperatura ambiente.
         * @see ADC_Read()
         */
        inline uint16_t readTempAmb();
        /**
         * @brief Lectura de la temperatura del motor.
         * @return Valor del sensor de la temperatura del motor. 
         * @see ADC_Read()
         */
        inline uint16_t readTempMotor();
    /**
     *@}
     */
     
    /** @addtogroup COMM
     * @{
     */ 
    // Comunicaciones
    private:
        static const uint8_t pinBluetoothMode = _PD5;       //!< @brief pin del micro asociado a habilitar el modo AT del módulo bluetooth.
    public:
        /**
         * @brief Pone al módulo bluetooth en modo AT.
         * @param on_off 1 Modo AT, 0 modo de cómunicación entre dispositivos.
         */
        inline void setBluetoothAT(uint8_t on_off);
        /**
         * @brief Enviar un comando en modo AT o un String en modo comunicación.
         * @param strCmd Comando AT o String a enviar por el módulo bluetooth.
         * @see @ref USART, USART_SendStr()
         */
        inline void bluetoothSendCmd(const char *strCmd);
        /**
         * @brief Envía un dato por la comunicación serie bluetooth.
         * @param data byte a ser enviado por el módulo bluetooth.
         * @see @ref USART, USART_PushTx()
         */
        inline void bluetoothSendByte(uint8_t data);
        /**
         * @brief Obtiene un dato del buffer de recepción.
         * @return El primer dato del buffer de recepción. @b -1 si no hay datos en el buffer.
         * @see @ref USART, USART_PopRx()
         */
        inline int bluetoothGetByte(void);
    /**
     *@}
     */

    /** @addtogroup FUEL
     * @{
     */     
    // Combustible
    private:
        static const uint8_t adcCombustible = 0;    //!< @brief Canal ADC asociado a al sensor de combustible.
    public:
        /**
         * @brief Valor del combustible en el tanque.
         * @return Valor de la presión en el tanque de combustible.
         * @see @ref ADC, ADC_Read()
         */
        inline uint16_t readCombustible();
    /**
     *@}
     */

    /** @addtogroup WATER_PUMP
     * @{
     */         
    // Bomba de Agua:
    public:
        /**
         * @brief Valor a enviar a la bomba de agua para mejorar el rendimiento del motor.
         * @param value Valor de PWM a enviarle al motor de la bomba de agua.
         * @see @ref PWM, LFPWMR
         */
        void setBombaAguaValue(uint8_t value);
    /**
     *@}
     */

    /** @addtogroup TIME_RELATED
     * @{
     */
    // Mediciones de Tiempo    
    public:
        /**
         * @brief Asocia rutinas de medición de velocidad y vueltas de motor a interrupciones externas.
         * @param nInt Número de interrupción a asociar la rutina.
         * @param rutina Puntero a la rutina de interrupción que debe ejecutarse.
         * @see @ref Interrupts
         */
        inline void setInterrupcion(uint8_t nInt, void (*rutina)(void));
    /**
     *@}
     */

    /** @addtogroup SCHEDULING
     * @{
     */     
    // SystemClock:
    public:
        /**
         * @brief Lleva la cuenta en milisegundos desde la activación del sistema.
         * @return milisegundos transcurridos desde que se instanció la API.
         * @see @ref SysClk, getInstance()
         */
        uint32_t getMillis();
        /**
         * @brief Lleva la cuenta en segundos desde la actiación del sistema.
         * @return segundos transucrridos desde que se instanció la API.
         * @see @ref SysClk, getInstance()
         */
        uint16_t getSeconds();
    /**
     *@}
     */

    /** @addtogroup PWR_SAVING
     * @{
     */     
    // PowerMode:
    public:
        /**
         * @brief Activa el microcontrolador en modo @b bajo @b consumo.
         */
        void toSleep();
    /**
     *@}
     */

    // Constructores & destructores
    private:
        static Imos_api* ptrAPI;        //!< @brief Puntero estático para aplicar patrón @b singleton.
        /**
         * @brief El constructor de la clase.
         * 
         * @details
         *  El contrsuctor es privado, ya que se está utilizando un <b>patrón de diseño
         * singleton</b>. Ya que no tendría sentido tener múltiples instancias de la API.
         * 
         * @see getInstance()
         */
        Imos_api();
    public:
        /**
         * @brief Obtiene la única instancia posible de la API.
         * @return Única referencia posible a la API.
         * @see ptrAPI
         */
        static Imos_api* getInstance();
        /**
         * @brief Destructor de la clase.
         * 
         * @warning
         *  Siendo un proyecto embebido @b NUNCA debería ejecutarse el destructor de la
         * clase que contiene la API.
         */
        virtual ~Imos_api();

}; // Imos_api class

} // namespace imos_api

#endif // __IMOS_API__
