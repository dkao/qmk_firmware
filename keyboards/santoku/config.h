/* Copyright 2021 Tye (@tyetye)
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

//#include "config_common.h"

/* USB Device descriptor parameter */
/*
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6464
#define DEVICE_VER      0x0001
#define MANUFACTURER    Tye
#define PRODUCT         Santoku
*/

/* diode direction */
//#define DIODE_DIRECTION COL2ROW

/* key matrix size */
#define MATRIX_ROWS 4
#define MATRIX_COLS 12

#define LAYER_STATE_8BIT  //  Tells QMK that keymap has fewer than 8 layers. Saves about 660 bytes. Remove if using more than 8 layers

// Configure the SSD1306OLED display
//#define USE_I2C
#define OLED_DISPLAY_128X64

#define PS2_MOUSE_SCROLL_BTN_SEND 500 /* Default is 300 */

#undef PS2_MOUSE_SCROLL_BTN_MASK
#define PS2_MOUSE_SCROLL_BTN_MASK (1<<PS2_MOUSE_BTN_MIDDLE) /* Default */
#define PS2_MOUSE_BTN_LEFT      0
#define PS2_MOUSE_BTN_RIGHT     1
#define PS2_MOUSE_BTN_MIDDLE    2

#define PS2_MOUSE_INIT_DELAY 1000 /* Default */



#ifdef PS2_USE_USART
#define PS2_CLOCK_PIN D5
#define PS2_DATA_PIN  D2
/* synchronous, odd parity, 1-bit stop, 8-bit data, sample at falling edge */
/* set DDR of CLOCK as input to be slave */
#define PS2_USART_INIT() do {   \
	PS2_CLOCK_DDR &= ~(1<<PS2_CLOCK_BIT);   \
	PS2_DATA_DDR &= ~(1<<PS2_DATA_BIT);     \
	UCSR1C = ((1 << UMSEL10) |  \
			(3 << UPM10)   |  \
			(0 << USBS1)   |  \
			(3 << UCSZ10)  |  \
			(0 << UCPOL1));   \
	UCSR1A = 0;                 \
	UBRR1H = 0;                 \
	UBRR1L = 0;                 \
} while (0)
#define PS2_USART_RX_INT_ON() do {  \
	UCSR1B = ((1 << RXCIE1) |       \
			(1 << RXEN1));        \
} while (0)
#define PS2_USART_RX_POLL_ON() do { \
	UCSR1B = (1 << RXEN1);          \
} while (0)
#define PS2_USART_OFF() do {    \
	UCSR1C = 0;                 \
	UCSR1B &= ~((1 << RXEN1) |  \
			(1 << TXEN1));  \
} while (0)
#define PS2_USART_RX_READY      (UCSR1A & (1<<RXC1))
#define PS2_USART_RX_DATA       UDR1
#define PS2_USART_ERROR         (UCSR1A & ((1<<FE1) | (1<<DOR1) | (1<<UPE1)))
#define PS2_USART_RX_VECT       USART1_RX_vect
#endif

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
//#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

#define ENCODER_RESOLUTION 1
#define ENCODER_MAP_DELAY 0

//#define MK_3_SPEED
#define MK_KINETIC_SPEED
// #define MK_MOMENTARY_ACCEL
// #define MK_1_SPEED
// #define MK_2_SPEED
// #define MK_3_SPEED
#define MOUSEKEY_WHEEL_INTERVAL                 8   // 80 Time between wheel movements
#define MOUSEKEY_WHEEL_DELAY                    5   // 5  Delay between pressing a movement key and cursor movement
#define MOUSEKEY_WHEEL_INITIAL_MOVEMENTS	    128	// 16 Initial number of movements of the mouse wheel
#define MOUSEKEY_WHEEL_BASE_MOVEMENTS	        128	// 32 Maximum number of movements at which acceleration stops
#define MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS	128	// 48 Accelerated wheel movements
#define MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS	128	// 8  Decelerated wheel movements
#define MOUSEKEY_WHEEL_MAX_SPEED	            128	// 8  Maximum number of scroll steps per scroll action
#define MOUSEKEY_WHEEL_TIME_TO_MAX	            5	// 40 Time until maximum scroll speed is reached

#define MK_W_OFFSET_0	1	                        // 1	Scroll steps per scroll action (KC_ACL0)
#define MK_W_INTERVAL_0	100	                        // 360	Time between scroll steps (KC_ACL0)
#define MK_W_OFFSET_1	1	                        // 1	Scroll steps per scroll action (KC_ACL1)
#define MK_W_INTERVAL_1	40	                        // 120	Time between scroll steps (KC_ACL1)
#define MK_W_OFFSET_2	4	                        // 1	Scroll steps per scroll action (KC_ACL2)
#define MK_W_INTERVAL_2	0	                        // 20	Time between scroll steps (KC_ACL2)
