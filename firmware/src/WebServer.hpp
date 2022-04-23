/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2021-2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

/**
 * \file WebServer.hpp
 * \brief HTTP Server.
 */

#ifndef NODEMCU_TEST_FIRMWARE_WEBSERVER
#define NODEMCU_TEST_FIRMWARE_WEBSERVER

#include <Arduino.h>
#include <IPAddress.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h> 

#include "Eeprom.hpp"

/**
 * \brief HTTP Server implementing the API.
 */
class WebServer {
public:
    /**
     * \brief Constructor.
     */
    WebServer();

    /**
     * \brief Handle requests and serve responses. 
     * 
     * This method must be called in a loop and will handle any pending requests.
     */
    void serve();

private:
    ESP8266WebServer m_server{80}; /**< \brief ESP8266WebServer instance. */

    /**
     * \brief Handle a request for the root URL.
     *
     * This method is called for requests for the URL / and serves
     * the frontend.
     */
    void handleRoot();

    /**
     * \brief Handle a request for the favicon.
     *
     * This method is called for requests for the URL /favicon.svg.
     */
    void handleFavicon();

    /**
     * \brief Handle an API request.
     *
     * This method is called for all URLs for which no other handler applies.
     * If the URL is an API request, the API method is executed. Otherwise a 404 response is sent.
     */
    void handleAPI();
};

#endif
