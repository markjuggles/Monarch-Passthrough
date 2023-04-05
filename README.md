# Monarch-Passthrough

This project allows a Microchip AVR-IoT Mini to redirect the Sequans Monarch 2 GM02S serial I/O to the boards pins.  This will allow another processor to control the cellular modem.

A psuedo-command was added to allow the external processor to do a hardware reset on the cellular modem:

GM02S^RESET

It will respond immediately with an "OK\r\n" and later with a "+SYSSTART\r\n"
