/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2021-2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#include "WebServer.hpp"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#include <EEPROM.h>

#include "ApiRequest.hpp"
#include "../_gen/frontend.hpp"

WebServer* WebServer::m_instance = nullptr;

WebServer& WebServer::instance() {
    if(m_instance == nullptr) {
        m_instance = new WebServer();
    }
    return *m_instance;
}
    
void WebServer::setup(void) {
    // initialize and read EEPROM
    EEPROM.begin(sizeof(eeprom_t));
    EEPROM.get(0, m_eeprom);
        
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // connect to WLAN
    wifi_station_set_hostname(NAME);
    m_wifiMulti.addAP(SSID, PASSWORD);

    int i = 0;
    while (m_wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
        digitalWrite(LED_BUILTIN, LOW);
        delay(200);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(200);
    }
    digitalWrite(LED_BUILTIN, LOW); // connected
    m_ip = WiFi.localIP();
    m_mdns = MDNS.begin(NAME);

    // read pins
    const int pins[] = { D0, D1, D2, D3, D4, D5, D6, D7, D8 };
    for(int i = 0; i < 9; ++i) {
        m_modes[pins[i]] = INPUT;
        m_states[pins[i]] = digitalRead(pins[i]);
    }

    // start web server
    m_server.on("/", WebServer::handleRoot);
    m_server.on("/favicon.svg", WebServer::handleFavicon);
    m_server.onNotFound(WebServer::handleAPI);
    m_server.begin();
}

void WebServer::loop(void) {
    m_server.handleClient();
}

void WebServer::handleRoot() {
    WebServer::instance()._handleRoot();
}

void WebServer::handleFavicon() {
    WebServer::instance()._handleFavicon();
}
    
void WebServer::handleAPI() {
    WebServer::instance()._handleAPI();
}

String WebServer::getPinStatus(int pin) {
    if(m_modes[pin] == INPUT) {
        int val = digitalRead(pin);
        return val == LOW ? "l" : "h";
    } else {
        return m_states[pin] == LOW ? "L" : "H";
    }
}
    
void WebServer::_handleRoot() {
    m_server.send(200, "text/html", HTML_CONTENT);
}

void WebServer::_handleFavicon() {
    m_server.send(200, "image/svg+xml", FAVICON);
}
    
void WebServer::_handleAPI() {
    ApiRequest req(m_server.uri());
    if(req.isValid()) {
        String res;
        switch(req.method()) {
        case API_STATUS:
            res = getPinStatus(D0) + getPinStatus(D1) + getPinStatus(D2) + getPinStatus(D3)
                + getPinStatus(D4) + getPinStatus(D5) + getPinStatus(D6) + getPinStatus(D7)
                + getPinStatus(D8);
            break;
        case API_MODE:
            pinMode(req.pin(), req.mode());
            m_modes[req.pin()] = req.mode();
            res = "success";
            break;
        case API_WRITE:
            digitalWrite(req.pin(), req.value());
            m_states[req.pin()] = req.value();
            res = "success";
            break;
        }
        m_server.send(200, "text/plain", res);
    } else {
        m_server.send(404, "text/plain", "404 - Not Found");
    }
}
