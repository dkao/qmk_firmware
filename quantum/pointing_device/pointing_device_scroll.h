/* Copyright 2022 Alabastard
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

#include <string.h>
#include "quantum.h"
#include "pointing_device.h"

/* Sanity check */
#ifndef POINTING_DEVICE_SCROLL_ENABLE
#    error "POINTING_DEVICE_SCROLL_ENABLE not defined! settings may be incorrect!"
#endif

/* default settings */
#ifndef SCROLL_TAP_DELAY
#    define SCROLL_TAP_DELAY 0
#endif
#ifndef SCROLL_MODE_DEFAULT
#    define SCROLL_MODE_DEFAULT 0
#endif

/* default divisors */
#ifndef SCROLL_DRAG_DIVISOR
#    define SCROLL_DRAG_DIVISOR 8
#endif
#ifndef SCROLL_CARET_DIVISOR
#    define SCROLL_CARET_DIVISOR 32
#endif
#ifndef SCROLL_VOL_DIVISOR
#    define SCROLL_VOL_DIVISOR 32
#endif
#ifndef SCROLL_HIST_DIVISOR
#    define SCROLL_HIST_DIVISOR 64
#endif

/* local data structure */
typedef struct {
    struct {
        uint8_t active;
        uint8_t toggle;
    } mode;
    struct {
        mouse_xy_report_t x;
        mouse_xy_report_t y;
    } cursor;
    int16_t h;
    int16_t v;
} scroll_context_t;

/* ----------Set up functions------------------------------------------------------------------------------------- */
report_mouse_t pointing_device_task_scroll(report_mouse_t mouse_report);       // handles mouse_report processing add to pointing_device_task
bool           process_record_scroll(uint16_t keyrecord, keyrecord_t* record); // handle processing of builtin keyrecords

/* ----------Control functions------------------------------------------------------------------------------------ */
uint8_t        get_scroll_mode(void);                         // returns active scroll mode ID
void           set_scroll_mode(uint8_t scroll_mode);          // set scroll mode to scroll_mode ID
void           scroll_mode_toggle(uint8_t scroll_mode);       // toggle scroll mode
uint8_t        get_scroll_mode_toggle(void);                  // get toggle scroll mode index
int16_t        get_scroll(bool axis);                         // get scroll axis value (true: v, false: h)
void           set_scroll(bool axis, int16_t val);            // set scroll axis value (true: v, false: h)
void           accumulate_scroll(bool axis, int16_t val);     // accumulate val onto local scroll axis (true: v, false: h)
void           store_cursor(report_mouse_t mouse_report);     // store and clear mouse_report x & y
report_mouse_t restore_cursor(report_mouse_t mouse_report);   // restore locally stored mouse_report x & y
report_mouse_t scroll_axes_conv(report_mouse_t mouse_report); // overwritable function that converts x & y axes to local h & v

/* ----------Setting up scroll mode keyrecords-------------------------------------------------------------------- */
void scroll_key_momentary(uint8_t scroll_mode, keyrecord_t* record);                   // momentary change of scroll mode while button is held
void scroll_key_toggle(uint8_t scroll_mode, keyrecord_t* record);                      // toggles scroll mode on/off on keypress
void scroll_key_tap_toggle(uint8_t scroll_mode, keyrecord_t* record);                  // works same as TT with scroll mode
void scroll_key_with_hold(uint8_t scroll_mode, uint16_t kc_hold, keyrecord_t* record); // momentary change of scroll mode while holding keycode

/* ----------Functions for custom scrolling modes----------------------------------------------------------------- */
void scroll_tap_codes(uint16_t kc_h_neg, uint16_t kc_v_neg, uint16_t kc_v_pos, uint16_t kc_h_pos, uint8_t divisor); // turn h/v axis values into key taps

/* ----------Callbacks for modifying and adding scroll modes------------------------------------------------------ */
bool process_scroll_mode_kb(uint8_t scroll_mode, report_mouse_t* mouse_report);
bool process_scroll_mode_user(uint8_t scroll_mode, report_mouse_t* mouse_report);

/* ----------Useful Macros---------------------------------------------------------------------------------------- */
#define SCROLL_H get_scroll(false)
#define SCROLL_V get_scroll(true)
#define SET_SCROLL_H(val) set_scroll(false, val)
#define SET_SCROLL_V(val) set_scroll(true, val)
#define ACCUMULATE_H(val) accumulate_scroll(false, val)
#define ACCUMULATE_V(val) accumulate_scroll(true, val)
