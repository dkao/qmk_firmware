// Copyright 2022 Stefan Kerkmann
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once


/* SPI & PMW3360 settings. */
#define SPI_DRIVER SPID0
#define SPI_SCK_PIN GP2
#define SPI_MISO_PIN GP4
#define SPI_MOSI_PIN GP3
//#define POINTING_DEVICE_CS_PIN GP9
#define PMW33XX_CS_PIN GP9
#define PMW33XX_CS_PINS { GP9, GP25 }
#define PMW3360_SENSOR1_SCK_PIN GP18
#define PMW3360_SENSOR1_MISO_PIN GP20
#define PMW3360_SENSOR1_MOSI_PIN GP23
#define POINTING_DEVICE_INVERT_X
#define PMW33XX_LIFTOFF_DISTANCE 0x03

/* Reset. */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

#define MOUSE_EXTENDED_REPORT
