set(ROOT_PATH /home/fabien/.arduino15/packages/esp8266/)
set(ESP_VERSION 2.7.0)
set(ESP_PATH ${ROOT_PATH}/hardware/esp8266/${ESP_VERSION})

set(ARDUINO_CORE     ${ESP_PATH}/cores/esp8266/)
set(ARDUINO_VARIANT  ${ESP_PATH}/variants/nodemcu)
set(TOOLCHAIN_PREFIX ${ROOT_PATH}/tools/xtensa-lx106-elf-gcc/2.5.0-4-b40a506/bin/xtensa-lx106-elf-)

list(APPEND _INCLUDE_DIRS ${ESP_PATH}/tools/sdk/include/)
list(APPEND _INCLUDE_DIRS ${ESP_PATH}/tools/sdk/lwip2/include)
list(APPEND _INCLUDE_DIRS ${ESP_PATH}/tools/sdk/libc/xtensa-lx106-elf/include)

add_definitions(
  -U__STRICT_ANSI__
  -D__ets__
  -DF_CPU=80000000L
  -DLWIP_OPEN_SRC
  -DTCP_MSS=536
  -DLWIP_FEATURES=1
  -DLWIP_IPV6=0
  -DMMU_IRAM_SIZE=0x8000
  -DARDUINO=10819
  -DARDUINO_ESP8266_NODEMCU_ESP12E
  -DARDUINO_ARCH_ESP8266
  "-DARDUINO_BOARD=\"ESP8266_NODEMCU_ESP12E\""
)

# todo: linking funktioniert nicht
# todo: version für arduino nano
# todo: digitalis.build anpassen
