/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#include "WebServer.hpp"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#include "Firmware.hpp"
#include "ApiRequest.hpp"
#include "../_gen/frontend.hpp"

WebServer::WebServer() {
    // start web server
    m_server.on("/", [&]() { handleRoot(); });
    m_server.on("/favicon.svg", [&]() { handleFavicon(); });
    m_server.onNotFound([&]() { handleAPI(); });
    m_server.begin();
}

void WebServer::serve() {
    m_server.handleClient();
}

void WebServer::handleRoot() {
    m_server.send(200, "text/html", HTML_CONTENT);
}

void WebServer::handleFavicon() {
    m_server.send(200, "image/svg+xml", FAVICON);
}
    
void WebServer::handleAPI() {
    ApiRequest req(m_server.uri());
    if(req.isValid()) {
        String res;
        bool doReset = false;

        switch(req.method()) {
        case ApiMethod::CONFIG:
            for(int i = 0; i < m_server.args(); ++i) {
                if(m_server.argName(i).equals("name")) { doReset = true; Firmware::instance().eeprom().setName(m_server.arg(i)); }
                else if(m_server.argName(i).equals("ssid")) { doReset = true; Firmware::instance().eeprom().setSsid(m_server.arg(i)); }
                else if(m_server.argName(i).equals("password")) { doReset = true; Firmware::instance().eeprom().setPassword(m_server.arg(i)); }
                else if(m_server.argName(i).startsWith("pinName")) Firmware::instance().eeprom().setPinName(m_server.argName(i).substring(7).toInt(), m_server.arg(i));
                res = "success";
            }
            Firmware::instance().eeprom().save();
            
            res = Firmware::instance().eeprom().name()
                + "\n" + Firmware::instance().eeprom().ssid()
                + "\n" + Firmware::instance().eeprom().password();
            for(int i = 0; i < 9; ++i)
                res += "\n" + Firmware::instance().eeprom().pinName(i);
            break;
        case ApiMethod::STATUS:
            res = Firmware::instance().getPinStatus(D0)
                + Firmware::instance().getPinStatus(D1)
                + Firmware::instance().getPinStatus(D2)
                + Firmware::instance().getPinStatus(D3)
                + Firmware::instance().getPinStatus(D4)
                + Firmware::instance().getPinStatus(D5)
                + Firmware::instance().getPinStatus(D6)
                + Firmware::instance().getPinStatus(D7)
                + Firmware::instance().getPinStatus(D8);
            break;
        case ApiMethod::MODE:
            Firmware::instance().setPinMode(req.pin(), req.mode());
            res = "success";
            break;
        case ApiMethod::WRITE:
            Firmware::instance().writeDigitalPin(req.pin(), req.value());
            res = "success";
            break;
        }
        m_server.send(200, "text/plain", res);
        if(doReset) ESP.restart();
    } else {
        m_server.send(404, "text/plain", "404 - Not Found");
    }
}
