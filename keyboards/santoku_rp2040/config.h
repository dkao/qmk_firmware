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


#define W25Q080    // rp2040 generic
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET // Activates the double-tap behavior
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U // Timeout window in ms in which the double tap can occur.
                                                        //
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

//#define MATRIX_COL_PINS { F1, F0, B6, B2, B3, B1, C7, E6, B7, B5, B4, C6 }
//#define MATRIX_ROW_PINS { F4, F5, F6, F7 }
//#define UNUSED_PINS

#define LAYER_STATE_8BIT  //  Tells QMK that keymap has fewer than 8 layers. Saves about 660 bytes. Remove if using more than 8 layers

// Configure the SSD1306OLED display
//#define USE_I2C
#define OLED_DISPLAY_128X64
#define I2C1_SCL_PIN  GP3
#define I2C1_SDA_PIN  GP2 

// Configure rotary encoder
//#define ENCODERS_PAD_A { D4 }
//#define ENCODERS_PAD_B { D7 }
//#define ENCODER_RESOLUTION 1
//#define ENCODER_DEFAULT_POS 0x1

//#ifdef PS2_MOUSE_ENABLE

//#define PS2_MOUSE_SCROLL_BTN_SEND 800 /* Default is 300 */
//#define PS2_MOUSE_INVERT_BUTTONS

//#undef PS2_MOUSE_SCROLL_BTN_MASK
//#define PS2_MOUSE_SCROLL_BTN_MASK (1<<PS2_MOUSE_BTN_MIDDLE) /* Default */
#define PS2_MOUSE_BTN_LEFT      0
#define PS2_MOUSE_BTN_RIGHT     1
#define PS2_MOUSE_BTN_MIDDLE    2

//#define PS2_MOUSE_INIT_DELAY 1000 /* Default */
//#endif


//#define PS2_CLOCK_PIN GP13
//#define PS2_DATA_PIN  GP1
//#define PS2_CLOCK_PIN D5
//#define PS2_DATA_PIN  D2

#ifdef PS2_USE_USART
//#define PS2_CLOCK_PIN D5
//#define PS2_DATA_PIN  D2
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


#ifdef PS2_USE_BUSYWAIT
//#define PS2_CLOCK_PIN   D5
//#define PS2_DATA_PIN    D2
#endif


/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

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

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION
