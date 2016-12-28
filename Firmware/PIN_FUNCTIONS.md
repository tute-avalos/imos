Declaración de pines
====================

Conrtol de temperatura
----------------------

* **PORTC1 (ADC1):** Temperatura ambiernte.
* **PORTC2 (ADC2):** Temperatura del motor.

Comunicaciones
--------------

* **PORTD0 y PORTD1 (RX y TX):** Terminal serie para *debug* con RS-232 y conxión *Bluetooth*.
* **PORTD5 (T1):** Modo AT del *Bluetooth*.

Mediciones de tiempo
--------------------

* **PORTD2 (INT0):** Velocimetro, sensor efecto hall optoacoplado.
* **PORTD3 (INT1):** Tacómetro.

Alimentación
------------

* **PORTC5 (ADC5):** Tensión de la batería.
* **PORTD6 (AIN0):** Led indicador.
* **PORTD7 (AIN1):** Rele de activación de alimentación.

Alarma
------

* **PORTB0 (ICP):** Activación de la alarma.
* **PORTB1 (OC1A):** Luces y bocina.
* **PORTC4 (ADC4):** Acelerómetro (eje Z).

Luminaria
---------

* **PORTB2 (OC1B):** Control de luces PWM.
* **PORTC3 (ADC3):** Sensor LDR.

Bomba de agua
-------------

* **PORTB3 (OC2A):** Bomba de agua (*PWM*).

Conector ISP (no hace falta declararlos)
----------------------------------------

* **PORTB3 (MOSI)**
* **PORTB4 (MISO)**
* **PORTB5 (SCK)**

Pines no disponibles/no implementados (aún)
-------------------------------------------

* **PORTC0 (ADC0)** (*Posible implementación*: Sensar tanque de combustible)
* **PORTD4 (T0)**

