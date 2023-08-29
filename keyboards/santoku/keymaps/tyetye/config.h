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

// defines for unused feature (to save space)
#define NO_ACTION_ONESHOT
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE

// defines for combos feature (press two keys at one time to trigger an event)
#define COMBO_TERM  40  /* time limit in milliseconds combos need to be completed */
#define COMBO_COUNT 8

// defines to help make home row mods more reliable
#define TAPPING_TOGGLE 2  /* How many times TT(__layer__) needs to be tapped to stick */
#define TAPPING_TERM 200

// defines for making the Trackpoint more fluid
#define PS2_MOUSE_USE_REMOTE_MODE

// defines for tuning the OLED
#define OLED_TIMEOUT 15000
#define OLED_UPDATE_INTERVAL 100
#define OLED_FADE_OUT
#define OLED_FADE_OUT_INTERVAL 2
#define OLED_UPDATE_PROCESS_LIMIT 4

// defines for the CAPS_WORD feature
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
#define CAPS_WORD_IDLE_TIMEOUT 5000


// defines for the mouse scroll wheel (which probably don't work for the scroll wheel because it's based on clicks instead of holding the key down)
//#define MOUSEKEY_DELAY	10	// Delay between pressing a movement key and cursor movement
//#define MOUSEKEY_INTERVAL	20	// Time between cursor movements in milliseconds
//#define MOUSEKEY_MOVE_DELTA	8	// Step size
//#define MOUSEKEY_MAX_SPEED	10	// Maximum cursor speed at which acceleration stops
//#define MOUSEKEY_TIME_TO_MAX	30	// Time until maximum cursor speed is reached
//#define MOUSEKEY_WHEEL_DELAY	10	// Delay between pressing a wheel key and wheel movement
//#define MOUSEKEY_WHEEL_INTERVAL	80	// Time between wheel movements
//#define MOUSEKEY_WHEEL_MAX_SPEED	8	// Maximum number of scroll steps per scroll action
//#define MOUSEKEY_WHEEL_TIME_TO_MAX	40	// Time until maximum scroll speed is reached
