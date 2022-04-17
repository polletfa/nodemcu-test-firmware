/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2021-2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#ifndef NODEMCU_TEST_FIRMWARE_EEPROM
#define NODEMCU_TEST_FIRMWARE_EEPROM

#include <Arduino.h>

struct eeprom_t {
    char state;
    char name[100];
    char ssid[32];
    char password[100];
    char pinNames[9][100];
};

class Eeprom {
public:
    Eeprom();
    
    String name() const;
    String ssid() const;
    String password() const;
    String pinName(int index) const;

    void setName(String name);
    void setSsid(String ssid);
    void setPassword(String password);
    void setPinName(int index, String pinName);

    void save();
    
private:
    eeprom_t m_content;

    String getString(const char* addr, int size) const;
};

#endif
