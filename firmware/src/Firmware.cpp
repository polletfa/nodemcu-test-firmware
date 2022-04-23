/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#include "Firmware.hpp"
#include "Network.hpp"

Firmware* Firmware::m_instance = nullptr;

Firmware& Firmware::instance() {
    if(m_instance == nullptr) {
        m_instance = new Firmware();
    }
    return *m_instance;
}
    
void Firmware::setup(void) {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // connect to WLAN or create a Hotspot
    Network::connect();
}

void Firmware::loop(void) {
    Network::update();
    m_server.serve();
}

WebServer& Firmware::webServer() {
    return m_server;
}

Eeprom& Firmware::eeprom() {
    return m_eeprom;
}

String Firmware::getPinStatus(int pin) {
    return digitalRead(pin) == LOW
        ? (m_modes[pin] == INPUT ? "l" : "L")
        : (m_modes[pin] == INPUT ? "h" : "H");
}
    
void Firmware::setPinMode(int pin, int mode) {
    pinMode(pin, mode);
    m_modes[pin] = mode;
}

void Firmware::writeDigitalPin(int pin, int value) {
    digitalWrite(pin, value);
}
