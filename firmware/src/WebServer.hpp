/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2021-2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#ifndef NODEMCU_TEST_FIRMWARE_WEBSERVER
#define NODEMCU_TEST_FIRMWARE_WEBSERVER

#include <Arduino.h>
#include <IPAddress.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h> 

#include "Eeprom.hpp"

class WebServer {
public:
    WebServer();
    
    void serve();

private:
    ESP8266WebServer m_server{80};

    static void handleRoot();
    static void handleFavicon();
    static void handleAPI();

    void _handleRoot();
    void _handleFavicon();    
    void _handleAPI();
};

#endif
