# EspnowUARTBridge for ESP8266 

Receiver/transmitter based on the ESP-NOW protocol.

It uses a serial interface as a data source and can be configured using commands typed in the serial console.

## How it works?

In short, it takes any information given by the serial port, except commands, and sends it to the indicated receiver or to anything within range.

## What can this be used for?
Personally, I created this as a solution to the problem of esphome not being compatible with espnow.


## Features

- **Customizable baud rate**: Set your baud rate using simple command
- **Customizable receiver MAC address**: Set your receiver MAC address
- **Encryption**: using built-in ESP-NOW encryption
- **Persistent Storage**: Save configuration settings in flash memory using `Preferences.h`.
- **ESP-NOW Support**: Seamlessly send and receive data across ESP8266 devices.
- **Acknowledgment Messages**: Reliable delivery with acknowledgment for received messages.
- **Broadcast and Private Modes**: Choose between broadcasting messages or sending to specific devices.
- **Native OLED Support**: Display some basic info on OLED (pin D5, D6)
## Commands List

| Command          |Arg  |Description                                                                 |
|-------------------|----------|-------------------------------------------------------------------|
| **ADDRECV**       | MAC|Allow specifying the receiver MAC address. If not specified, it uses the last connected device's address. |
| **RESRECV**       | None |Sets the receiver MAC address to `FF:FF:FF:FF:FF:FF`.                      |
| **SETBR**         | baud rate |Specify the baud rate. Default: 9600                                                    |
| **REJECTUNPAIRED**| true/false|`True` - Reject messages from unknown peers, `False` - Accept all connections. Default : false |
| **RST**           | None |Reboot the controller.                                                    |
| **INFO**          | None |Print system information.                                                 |
| **?**             | None |Display help.                                                             |
| **AUTORST**       |true/false| Set if controller should reboot automatically after chaning setting. Default: true|
## Supported devices
- ESP12E/F
- ESP01S
- Wemos D1 Mini
- NodeMCU
- NodeMCU with Oled

## Pre-compiled Binaries

Download pre-compiled binaries for ESP12E/F and ESP-01S:  
[Pre-compiled binaries](https://github.com/kurson95/EspnowUARTBridge_ESP8266/releases/)
## Contribution
Contributions are welcome! Please fork the repository and create a pull request for any improvements or bug fixes.

## License
This project is licensed under the MIT License.
