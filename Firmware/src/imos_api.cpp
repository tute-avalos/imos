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
 * @file imos_api.cpp
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Contenedor de la API de IMOS.
 */
#include "imos_api.h"
#include "systemclock.h"
#include "pwm.h"
#include "usart.h"
#include "adc.h"
#include "eeprom.h"
#include "extinterrupts.h"

imos_api::config_t* imos_api::Imos_api::getConfiguracion()
{
    return &(this->configuracion);
}

bool imos_api::Imos_api::isEepromBusy(void)
{
    return (EEPROM_Busy())? true : false;
}

void imos_api::Imos_api::saveConfiguracion()
{
    EEPROM_WriteData(this->configAddrs,(void*) &(this->configuracion),sizeof(imos_api::config_t));
}

uint16_t imos_api::Imos_api::readBateria()
{
    return ADC_Read(this->adcBateria);
}

void imos_api::Imos_api::setPower(uint8_t on_off)
{
    digitalWrite(this->powerRelay,on_off);
}

void imos_api::Imos_api::setBocina(uint8_t on_off)
{
    digitalWrite(this->pinBocina,on_off);
}

uint16_t imos_api::Imos_api::readAcelerometro()
{
    return ADC_Read(this->adcAcelerometro);
}

uint8_t imos_api::Imos_api::readAlarmaIn()
{
    return digitalRead(this->pinAlarmaIn);
}

uint16_t imos_api::Imos_api::readLDR()
{
    return ADC_Read(this->adcLDR);
}

void imos_api::Imos_api::setLucesValue(uint8_t value)
{
    _setHFPWM(value);
}

uint16_t imos_api::Imos_api::readTempAmb()
{
    return ADC_Read(this->adcTempAmb);
}

uint16_t imos_api::Imos_api::readTempMotor()
{
    return ADC_Read(this->adcTempMotor);
}

void imos_api::Imos_api::setBluetoothAT(uint8_t on_off)
{
    digitalWrite(this->pinBluetoothMode,on_off);
}

void imos_api::Imos_api::bluetoothSendCmd(const char *strCmd)
{
    USART_SendStr(strCmd);
}

void imos_api::Imos_api::bluetoothSendByte(uint8_t data)
{
    USART_PushTx(data);
}

int imos_api::Imos_api::bluetoothGetByte(void)
{
    return USART_PopRx();
}

uint16_t imos_api::Imos_api::readCombustible()
{
    return ADC_Read(this->adcCombustible);
}

void imos_api::Imos_api::setBombaAguaValue(uint8_t value)
{
    _setLFPWM(value);
}

void imos_api::Imos_api::setInterrupcion(uint8_t nInt, void (*rutina)(void))
{
    attachIntFunction(nInt,rutina);
}

uint32_t imos_api::Imos_api::getMillis()
{
    return ::ticks_ms;
}

uint16_t imos_api::Imos_api::getSeconds()
{
    return ::ticks_s;
}

void imos_api::Imos_api::toSleep()
{
    // Cosas antes de ir a dormir...
    ADC_Off();
    // Sleep()
    ADC_On();
}


imos_api::Imos_api::Imos_api()
{
    // Entradas y salidas digitales:
    pinMode(this->powerRelay,OUTPUT);
    pinMode(this->pinAlarmaIn,INPUT);
    pinMode(this->pinBluetoothMode,OUTPUT);
    pinMode(this->pinBocina,OUTPUT);
    
    // Valores iniciales:
    digitalWrite(this->powerRelay, LOW);
    digitalWrite(this->pinBluetoothMode, LOW);
    digitalWrite(this->pinBocina, LOW);
    
    _setHFPWM(0);
    _setLFPWM(0);
    
    // Configuración de módulos:
    configPWMChannels();
    configADC();
    ADC_Start();
    configUSART();
    configSysClk();
    
    // Habilitación de interrupciones:
    sei();
}

imos_api::Imos_api* imos_api::Imos_api::ptrAPI = NULL;

imos_api::Imos_api* imos_api::Imos_api::getInstance()
{
    if(ptrAPI == NULL)
    {
        ptrAPI = new Imos_api();
    }
    return ptrAPI;
}

imos_api::Imos_api::~Imos_api()
{
}
