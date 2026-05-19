# STM32 Alarm System

A simple alarm system based on **STM32F103C8T6**, developed using **STM32CubeMX** and **IAR Embedded Workbench**.

## Overview

This project implements an embedded alarm system with:

- **3 sensors** for intrusion/event detection
- **Siren output** for alarm indication
- **SIM/modem support** for communication and remote control

The system monitors sensor inputs, triggers the siren when needed, and can use the SIM module for alerts or commands.

## Features

- STM32F103C8T6 based design
- Developed with STM32CubeMX
- Built with IAR Embedded Workbench
- 3 sensor inputs
- Siren control
- SIM/modem communication over UART
- SMS or modem-based command handling
- Simple modular embedded structure

## Hardware

- **MCU:** STM32F103C8T6
- **Sensors:** 3 digital sensor inputs
- **Siren:** 1 output
- **SIM Module:** UART-connected GSM/SIM modem
- **RTC** : timestamps and time based automations 

## Software

- **Code Generator:** STM32CubeMX
- **IDE/Compiler:** IAR Embedded Workbench
- **Language:** C

## Project Function

The system works as follows:

1. Reads the status of 3 sensors
2. Detects alarm conditions
3. Activates the siren on trigger
4. Communicates through the SIM module
5. Can process modem/SMS-based commands if enabled

## Possible Commands

Depending on implementation, the SIM module can be used for commands such as:

- Arm
- Disarm
- Siren off
- Status request


## License

For educational and personal use.
