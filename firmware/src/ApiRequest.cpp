/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2021-2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#include "ApiRequest.hpp"

ApiRequest::ApiRequest(const String& uri) {
    int positions[5] = { 0, -1, -1, -1, -1 };
    int count = 0;
    
    // split
    for( ; count < 4 ; ++count) {
        positions[count+1] = uri.indexOf("/", positions[count]+1);
        if(positions[count+1] < 0) {
            if(positions[count] != uri.length()-1) {
                ++count;
                positions[count] = uri.length();
            }
            break;
        }
    }

    // parse
    parseMethod(uri, positions[0]+1, positions[1]);
    if(m_method == ApiMethod::MODE || m_method == ApiMethod::WRITE) parsePin(uri, positions[1]+1, positions[2]);
    if(m_method == ApiMethod::MODE) parseMode(uri, positions[2]+1, positions[3]);
    if(m_method == ApiMethod::WRITE) parseValue(uri, positions[2]+1, positions[3]);
    validateParameters(count);
}

void ApiRequest::parseMethod(const String& uri, int start, int end) {
    if(uri.substring(start, end).equals("config")) {
        m_method = ApiMethod::CONFIG;
    } else if(uri.substring(start, end).equals("status")) {
        m_method = ApiMethod::STATUS;
    } else if(uri.substring(start, end).equals("mode")) {
        m_method = ApiMethod::MODE;
    } else if(uri.substring(start, end).equals("write")) {
        m_method = ApiMethod::WRITE;
    }
}

void ApiRequest::parsePin(const String& uri, int start, int end) {
    for(int i = start; i < end; ++i) {
        if(uri.charAt(i) < '0' || uri.charAt(i) > '9')
            return;
    }
    m_pin = uri.substring(start, end).toInt();
    if(m_pin < 0 || m_pin > 8) m_pin = -1;
}

void ApiRequest::parseMode(const String& uri, int start, int end) {
    if(uri.substring(start, end).equals("in")) {
        m_mode = INPUT;
    } else if(uri.substring(start, end).equals("out")) {
        m_mode = OUTPUT;
    }
}

void ApiRequest::parseValue(const String& uri, int start, int end) {
    if(uri.substring(start, end).equals("high")) {
        m_value = HIGH;
    } else if(uri.substring(start, end).equals("low")) {
        m_value = LOW;
    }
}

void ApiRequest::validateParameters(int count) {
    switch(m_method) {
    case ApiMethod::CONFIG:
        if(count == 1 && m_pin == -1 && m_value == -1 && m_mode == -1) m_valid = true;
        break;
    case ApiMethod::STATUS:
        if(count == 1 && m_pin == -1 && m_value == -1 && m_mode == -1) m_valid = true;
        break;
    case ApiMethod::MODE:
        if(count == 3 && m_pin >= 0 && m_pin <= 8 && m_value == -1 && m_mode != -1) m_valid = true;
        break;
    case ApiMethod::WRITE:
        if(count == 3 && m_pin >= 0 && m_pin <= 8 && m_value != -1 && m_mode == -1) m_valid = true;
        break;
    }

    switch(m_pin) {
    case 0: m_pin = D0; break;
    case 1: m_pin = D1; break;
    case 2: m_pin = D2; break;
    case 3: m_pin = D3; break;
    case 4: m_pin = D4; break;
    case 5: m_pin = D5; break;
    case 6: m_pin = D6; break;
    case 7: m_pin = D7; break;
    case 8: m_pin = D8; break;
    }
}
