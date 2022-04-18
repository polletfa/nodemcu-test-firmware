/*****************************************************
 *
 * NodeMCU Test Firmware
 *
 * (c) 2021-2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

#ifndef NODEMCU_TEST_FIRMWARE_APIREQUEST
#define NODEMCU_TEST_FIRMWARE_APIREQUEST

#include <Arduino.h>

enum class ApiMethod { INVALID, STATUS, MODE, WRITE };

class ApiRequest {
public:
    ApiRequest(const String& uri);

    bool isValid() const       { return m_valid;  }
    ApiMethod method() const { return m_method; }
    int pin() const            { return m_pin;    }
    int mode() const           { return m_mode;   }
    int value() const          { return m_value;  }

private:
    bool m_valid         {        false       };
    ApiMethod m_method { ApiMethod::INVALID };
    int m_pin            {         -1         };
    int m_mode           {         -1         };
    int m_value          {         -1         };

    void parseMethod(const String& uri, int start, int end);

    void parsePin(const String& uri, int start, int end);

    void parseMode(const String& uri, int start, int end);

    void parseValue(const String& uri, int start, int end);

    void validateParameters(int count);
};

#endif
