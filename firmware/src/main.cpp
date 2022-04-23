/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#include "Firmware.hpp"

void setup(void) {
    Firmware::instance().setup();
}

void loop(void) {
    Firmware::instance().loop();
}
