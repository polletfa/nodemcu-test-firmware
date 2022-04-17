/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2021-2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#include "Firmware.hpp"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

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

    // connect to WLAN
    wifi_station_set_hostname(m_eeprom.name().c_str());
    m_wifiMulti.addAP(m_eeprom.ssid().c_str(), m_eeprom.password().c_str());

    int i = 0;
    while (m_wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
        digitalWrite(LED_BUILTIN, LOW);
        delay(200);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(200);
    }
    digitalWrite(LED_BUILTIN, LOW); // connected
    m_ip = WiFi.localIP();
    m_mdns = MDNS.begin(m_eeprom.name());

    // read pins
    const int pins[] = { D0, D1, D2, D3, D4, D5, D6, D7, D8 };
    for(int i = 0; i < 9; ++i) {
        m_modes[pins[i]] = INPUT;
        m_states[pins[i]] = digitalRead(pins[i]);
    }
}

void Firmware::loop(void) {
    m_server.serve();
}

WebServer& Firmware::webServer() {
    return m_server;
}

Eeprom& Firmware::eeprom() {
    return m_eeprom;
}

String Firmware::getPinStatus(int pin) {
    if(m_modes[pin] == INPUT) {
        int val = digitalRead(pin);
        return val == LOW ? "l" : "h";
    } else {
        return m_states[pin] == LOW ? "L" : "H";
    }
}
    
void Firmware::setPinMode(int pin, int mode) {
    pinMode(pin, mode);
    m_modes[pin] = mode;
}

void Firmware::writeDigitalPin(int pin, int value) {
    digitalWrite(pin, value);
    m_states[pin] = value;
}
