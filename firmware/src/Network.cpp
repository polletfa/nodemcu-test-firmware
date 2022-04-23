/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2021-2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#include "Network.hpp"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#include "Firmware.hpp"

void Network::connect() {
    wifi_station_set_hostname(Firmware::instance().eeprom().name().c_str());
    if(Firmware::instance().eeprom().ssid().equals("")) {
        createAccessPoint();
    } else {
        connectWiFi();
    }
    digitalWrite(LED_BUILTIN, LOW); // connected
}

void Network::update() {
    MDNS.update();
}

void Network::createAccessPoint() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(getAPName());
    startMDNS(WiFi.softAPIP());
}

void Network::connectWiFi() {
    WiFi.begin(Firmware::instance().eeprom().ssid().c_str(), Firmware::instance().eeprom().password().c_str());

    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
        digitalWrite(LED_BUILTIN, LOW);
        delay(200);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(200);
    }
    startMDNS(WiFi.localIP());
}

void Network::startMDNS(IPAddress ip) {
    if(!MDNS.begin(Firmware::instance().eeprom().name(), ip)) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
    }
}

String Network::getAPName() {
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);
    String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
        String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();
    return Firmware::instance().eeprom().name() + "_" + macID;
}
