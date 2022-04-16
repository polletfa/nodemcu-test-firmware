/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2021-2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#include "WebServer.hpp"

void setup(void) {
    WebServer::instance().setup();
}

void loop(void) {
    WebServer::instance().loop();
}
