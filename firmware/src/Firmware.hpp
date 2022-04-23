/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#ifndef NODEMCU_TEST_FIRMWARE_FIRMWARE
#define NODEMCU_TEST_FIRMWARE_FIRMWARE

#include <Arduino.h>
#include <ESP8266WebServer.h> 

#include "Eeprom.hpp"
#include "WebServer.hpp"

class Firmware {
public:
    static Firmware& instance();
    
    void setup(void);
    void loop(void);

    WebServer& webServer();
    Eeprom& eeprom();

    String getPinStatus(int pin);
    void setPinMode(int pin, int mode);
    void writeDigitalPin(int pin, int value);
    
private:
    static Firmware* m_instance;

    Eeprom m_eeprom;
    int m_modes[17] { INPUT };
    WebServer m_server;

    Firmware() {}
};

#endif
