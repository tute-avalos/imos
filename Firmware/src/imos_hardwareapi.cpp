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
 * @file imos_hardwareapi.cpp
 * @author Matías S. Ávalos <msavalos@gmail.com>
 * @version v0.1
 * @date 06/01/2017 (dd/mm/yyyy)
 * @brief Contenedor de la API de IMOS.
 */
#include "imos_hardwareapi.h"
#include "systemclock.h"
#include "pwm.h"
#include "usart.h"
#include "adc.h"
#include "eeprom.h"
#include "extinterrupts.h"

imos::config_t* imos::HardwareAPI::getConfiguracion()
{
    return &(this->configuracion);
}

bool imos::HardwareAPI::isEepromBusy(void)
{
    return (EEPROM_Busy())? true : false;
}

void imos::HardwareAPI::saveConfiguracion()
{
    EEPROM_WriteData(this->configAddrs,(void*) &(this->configuracion),sizeof(imos::config_t));
}

uint16_t imos::HardwareAPI::readBateria()
{
    return ADC_Read(this->adcBateria);
}

void imos::HardwareAPI::setPower(uint8_t on_off)
{
    digitalWrite(this->powerRelay,on_off);
}

void imos::HardwareAPI::setBocina(uint8_t on_off)
{
    digitalWrite(this->pinBocina,on_off);
}

uint16_t imos::HardwareAPI::readAcelerometro()
{
    return ADC_Read(this->adcAcelerometro);
}

uint8_t imos::HardwareAPI::readAlarmaIn()
{
    return digitalRead(this->pinAlarmaIn);
}

uint16_t imos::HardwareAPI::readLDR()
{
    return ADC_Read(this->adcLDR);
}

void imos::HardwareAPI::setLucesValue(uint8_t value)
{
    _setHFPWM(value);
}

uint16_t imos::HardwareAPI::readTempAmb()
{
    return ADC_Read(this->adcTempAmb);
}

uint16_t imos::HardwareAPI::readTempMotor()
{
    return ADC_Read(this->adcTempMotor);
}

void imos::HardwareAPI::setBluetoothAT(uint8_t on_off)
{
    digitalWrite(this->pinBluetoothMode,on_off);
}

void imos::HardwareAPI::bluetoothSendCmd(const char *strCmd)
{
    USART_SendStr(strCmd);
}

void imos::HardwareAPI::bluetoothSendByte(uint8_t data)
{
    USART_PushTx(data);
}

int imos::HardwareAPI::bluetoothGetByte(void)
{
    return USART_PopRx();
}

uint16_t imos::HardwareAPI::readCombustible()
{
    return ADC_Read(this->adcCombustible);
}

void imos::HardwareAPI::setBombaAguaValue(uint8_t value)
{
    _setLFPWM(value);
}

void imos::HardwareAPI::setInterrupcion(uint8_t nInt, void (*rutina)(void))
{
    attachIntFunction(nInt,rutina);
}

uint32_t imos::HardwareAPI::getMillis()
{
    return ::ticks_ms;
}

uint16_t imos::HardwareAPI::getSeconds()
{
    return ::ticks_s;
}

void imos::HardwareAPI::toSleep()
{
    // Cosas antes de ir a dormir...
    ADC_Off();
    // Sleep()
    ADC_On();
}


imos::HardwareAPI::HardwareAPI()
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

imos::HardwareAPI* imos::HardwareAPI::ptrAPI = NULL;

imos::HardwareAPI* imos::HardwareAPI::getInstance()
{
    if(ptrAPI == NULL)
    {
        ptrAPI = new HardwareAPI();
    }
    return ptrAPI;
}

imos::HardwareAPI::~HardwareAPI()
{
}
