NodeMCU Test Firmware for NodeMCU ESP 8266
------------------------------------------

NodeMCU Test Firmware is a simple firmware designed to quickly test a component on a breadbord.
It provides a web interface served over WiFi to read and write digital pins D0 to D8 on the
NodeMCU ESP 8266.

The firmware is written in C++ with the Arduino library.
The frontend is written in TypeScript and HTML with the Bootstrap framework for the layout.

# Build

Requirements:
- NodeJS / NPM (tested with NodeJS 17.3.0 / NPM 8.3.0)
- Arduino IDE with esp8266 board (tested with Arduino IDE 1.8.19 and esp8266 2.7.0)

## Using the makefile

A very rudimentary makefile is provided than will build both the frontend and the firmware and try to upload. You need to specify the board either adding the required options to the makefile or by selecting it in the IDE.

```
make
```

## Step by step

The frontend is generated as a C++ header file and is then included in the code of the firmware.
The frontend must therefore be built first.

### Frontend

```
cd frontend/
npm run build
npm run deploy
```

### Firmware

To build and upload the firmware, use the Arduino IDE.

- Load the file firmware/firmware.ino in the IDE.
- Select the appropriate board for your hardware.
- Build and upload the firmware.
