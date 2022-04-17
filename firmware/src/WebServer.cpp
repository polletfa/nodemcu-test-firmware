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

#include "Firmware.hpp"
#include "ApiRequest.hpp"
#include "../_gen/frontend.hpp"

WebServer::WebServer() {
    // start web server
    m_server.on("/", WebServer::handleRoot);
    m_server.on("/favicon.svg", WebServer::handleFavicon);
    m_server.onNotFound(WebServer::handleAPI);
    m_server.begin();
}

void WebServer::serve() {
    m_server.handleClient();
}

void WebServer::handleRoot() {
    Firmware::instance().webServer()._handleRoot();
}

void WebServer::handleFavicon() {
    Firmware::instance().webServer()._handleFavicon();
}
    
void WebServer::handleAPI() {
    Firmware::instance().webServer()._handleAPI();
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
        case API_MODE:
            Firmware::instance().setPinMode(req.pin(), req.mode());
            res = "success";
            break;
        case API_WRITE:
            Firmware::instance().writeDigitalPin(req.pin(), req.value());
            res = "success";
            break;
        }
        m_server.send(200, "text/plain", res);
    } else {
        m_server.send(404, "text/plain", "404 - Not Found");
    }
}
