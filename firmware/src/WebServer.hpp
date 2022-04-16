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

struct eeprom_t {
    char name[1024];
    char ssid[33];
    char password[1024];
};

class WebServer {
public:
    static constexpr char* NAME { "test-uctrl" };
    static constexpr char* SSID { "voodoo" };
    static constexpr char* PASSWORD { "werner2020" };

    static WebServer& instance();
    
    void setup(void);
    void loop(void);

private:
    static WebServer* m_instance;

    eeprom_t m_eeprom;
    int m_modes[17];
    int m_states[17];
    ESP8266WiFiMulti m_wifiMulti;
    ESP8266WebServer m_server{80};
    IPAddress m_ip;
    bool m_mdns {false};

    WebServer() {}

    static void handleRoot();
    static void handleFavicon();
    static void handleAPI();

    String getPinStatus(int pin);
    
    void _handleRoot();
    void _handleFavicon();    
    void _handleAPI();
};

#endif
