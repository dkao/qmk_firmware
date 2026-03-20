/* Copyright 2021 Kyle McCreery 
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 2 of the License, or 
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */


#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xDC9E
#define PRODUCT_ID      0x1770
#define DEVICE_VER      0x0001
#define MANUFACTURER    DCPEdit
#define PRODUCT         ModMmm


/* key matrix size */
#define MATRIX_ROWS 12
#define MATRIX_COLS 10

/* key matrix pins */
/*                        00   01   02   03   04  05  06  07   08   09  10  11 */
#define MATRIX_ROW_PINS { B10, B3,  B0,  B4,  A7, B5, A4, A6,  B6,  A5, B7, B8 }
#define MATRIX_COL_PINS { B13, B14, B15, A15, B1, A3, B9, C15, C14, C13 }

/* encoder pins */
#define ENCODERS_PAD_A { A2, A2, A13 }
#define ENCODERS_PAD_B { A13, A14, A14 }

/* Buzzer */
#define AUDIO_PIN A8
#define AUDIO_PWM_PAL_MODE 1
#define AUDIO_PWM_DRIVER PWMD1
#define AUDIO_PWM_CHANNEL 1
#define AUDIO_STATE_TIMER GPTD4
#define AUDIO_CLICKY
#define AUDIO_CLICKY_FREQ_RANDOMNESS 1.5f
#define STARTUP_SONG SONG(STARTUP_SOUND)

/* Solenoid */
#define SOLENOID_PIN A10

/* encoder resolution */
#define ENCODER_RESOLUTION 4
#define TAP_CODE_DELAY 10

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

#define LED_NUM_LOCK_PIN A1
#define LED_CAPS_LOCK_PIN B12
#define LED_SCROLL_LOCK_PIN A0