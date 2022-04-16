cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME     arduino.nodemcu-test-firmware.firmware)
set(SOURCES_TYPE    cpp)
set(HEADERS_TYPE    hpp h)
set(TARGET_TYPE     firmware)

set(LIBRARIES
)

set(DEPENDENCIES
)

set(DEFINITIONS
)

include(./esp8266.cmake)

set(BOARD_TYPE "ESP8266_NODEMCU_ESP12E")
set(CORE_NAME "esp8266")
set(CPU_SPEED "16000000")
set(PORT "/dev/ttyUSB0")
set(PORT_SPEED "57600")
set(PIN_VARIANT "nodemcu")
set(PROGRAMMER "arduino")
set(TOOLCHAIN_PATH "/home/fabien/.arduino15/packages/esp8266/tools/xtensa-lx106-elf-gcc/3.0.4-gcc10.3-1757bed")
set(ARDUINO_PATH "/home/fabien/.arduino15/packages/esp8266/hardware/esp8266/3.0.2/")
set(AR_AVRDUDE_CFG /etc/avrdude.conf)

include(BuildTarget)
