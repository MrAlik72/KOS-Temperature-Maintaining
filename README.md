# Project Name

DIPLOMA PROJECT
Temperature Control System on Kaspersky OS

## Overview

This project implements a temperature control system on Kaspersky OS. The program receives current temperature data via the I2C protocol from a BMP180 sensor. The temperature is then controlled by a PID controller, which adjusts the output based on the desired temperature set by the user. The heater is activated using the GPIO interface of the Raspberry Pi 4, connected to pins 3 and 6. Communication between the user and the system is facilitated through a Telegram bot acting as an MQTT Publisher, while the program itself contains MQTT Subscriber functionality to update the desired temperature upon receiving messages.

## Features

- Real-time temperature monitoring using the I2C protocol and BMP180 sensor.
- Precise temperature control through a PID controller.
- Heater activation and deactivation via GPIO interface.
- User interaction and temperature adjustment via Telegram bot acting as MQTT Publisher.
- MQTT Subscriber functionality to update the desired temperature upon receiving messages.

## Installation

### Prerequisites

- Kaspersky OS
- Raspberry Pi 4
- BMP180 sensor
- Heater
- Internet connectivity

### Running on QE
1. Clone the repository:

```bash
$ git clone https://github.com/your-username/temperature-control.git
```

2. Run the cross-build script to compile the program for Kaspersky OS on QEMU: 
```bash
sudo bash ./cross-build.sh
```
### Running on Raspberry Pi 4

1. Clone the repository:

```bash
$ git clone https://github.com/your-username/temperature-control.git
```

2. Build the image using the image-build script:

```bash
sudo bash ./image-build.sh
```

3. load the resulting image onto an SD card.

4. Insert the SD card into the Raspberry Pi 4 and power it on.

## Configuration

1. Connect the BMP180 temperature sensor to the Raspberry Pi 4 via the I2C pin.

2. Connect the heater to the Raspberry Pi 4 via the GPIO pins 3 (BCM2) and 6 (GND).

3. Configure the Telegram bot and MQTT Publisher settings according to the provided documentation.

## Usage

1. Start the program on the Kaspersky OS system.

2. Send a message to the Telegram bot with the desired temperature.

3. The program will receive the desired temperature via MQTT Subscriber functionality and adjust the heater's operation accordingly.

4. The current temperature will be continuously monitored, and the PID controller will regulate the heater's activation to maintain the desired temperature.
