/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#ifndef NODEMCU_TEST_FIRMWARE_NETWORK
#define NODEMCU_TEST_FIRMWARE_NETWORK

#include <Arduino.h>
#include <IPAddress.h>

class Network {
public:
    static void connect();
    static void update();
    static void disconnect();
    
private:
    Network() {};

    static String getAPName();
    
    static void createAccessPoint();
    static void connectWiFi();

    static void startMDNS(IPAddress);
};

#endif
