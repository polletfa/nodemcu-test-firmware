/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2021-2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#include "Eeprom.hpp"

#include <EEPROM.h>

Eeprom::Eeprom() {
    EEPROM.begin(sizeof(EepromData));
    EEPROM.get(0, m_content);
    if(m_content.state == 0xff) {
        m_content.state = 0;
        setName("test-uctrl");
        setSsid("voodoo");
        setPassword("werner2020");
        save();
    }
}

String Eeprom::name() const { return getString(m_content.name, sizeof(m_content.name)); }
String Eeprom::ssid() const { return getString(m_content.ssid, sizeof(m_content.ssid)); }
String Eeprom::password() const { return getString(m_content.password, sizeof(m_content.password)); }
String Eeprom::pinName(int index) const {
    if(index < 0 || index > 8) return String("invalid");
    return getString(m_content.pinNames[index], sizeof(m_content.pinNames[index]));
}

void Eeprom::setName(String name) { strncpy(m_content.name, name.c_str(), sizeof(m_content.name)); }
void Eeprom::setSsid(String ssid) { strncpy(m_content.ssid, ssid.c_str(), sizeof(m_content.ssid)); }
void Eeprom::setPassword(String password) { strncpy(m_content.password, password.c_str(), sizeof(m_content.password)); }
void Eeprom::setPinName(int index, String pinName) {
    if(index < 0 || index > 8) return;
    strncpy(m_content.pinNames[index], pinName.c_str(), sizeof(m_content.pinNames[index]));
}

void Eeprom::save() {
    EEPROM.put(0, m_content);
    EEPROM.commit();
}

String Eeprom::getString(const char* addr, int size) const {
    char buffer[size+1];
    strncpy(buffer, (char*)addr, size);
    buffer[size] = 0;
    return String(buffer);
}

