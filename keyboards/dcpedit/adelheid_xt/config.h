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
#define PRODUCT_ID      0xAD87
#define DEVICE_VER      0x0001
#define MANUFACTURER    DCPEdit
#define PRODUCT         AdelheidXT

/* key matrix size */
#define MATRIX_ROWS 12
#define MATRIX_COLS 12

/* key matrix pins */
#define MATRIX_ROW_PINS { A4, A13, B4, B9, A15, B15, A3, A14, B5, B8, B3, A8 }
#define MATRIX_COL_PINS { B1, B0, A7, A2, A1, A0, C15, B10, C14, B14, B13, B12 }
#define UNUSED_PINS

/* encoder pins */
#define ENCODERS_PAD_A { A5 }
#define ENCODERS_PAD_B { A6 }

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
