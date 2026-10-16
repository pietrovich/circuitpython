// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2021 Scott Shawcroft for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include "supervisor/board.h"

// Use the MP_WEAK supervisor/shared/board.c versions of routines not defined here.
#include "shared-bindings/digitalio/DigitalInOut.h"
#include "shared-bindings/usb_host/Port.h"
#include "hardware/gpio.h"

void board_init(void) {
    // Deliberately empty: the USB host port (GPIO12 = D+, GPIO13 = D-) is NOT created at boot.
    // This leaves the USB-A pins free so they can be used as a PIO UART; Python code creates the
    // host with usb_host.Port(board.USB_HOST_DATA_PLUS, board.USB_HOST_DATA_MINUS) when needed.
}