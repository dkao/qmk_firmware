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

#include "quantum.h"
#include "pointing_device.h"
//#include "report.h"

#ifndef POINTING_DEVICE_SCROLL_ENABLE
#    error "POINTING_DEVICE_SCROLL_ENABLE not defined! settings may be incorrect!"
#endif

#ifndef SCROLL_TAP_DELAY
#    define SCROLL_TAP_DELAY 0
#endif

#ifndef SCROLL_DRAG_DIVISOR
#    define SCROLL_DRAG_DIVISOR 32
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
    uint8_t           mode;
    uint8_t           mode_tg;
    mouse_xy_report_t h;
    mouse_xy_report_t v;
} scroll_record_t;

/* Default Scroll Modes */
enum scroll_modes {
    SM_NONE = 0x00, // Null scroll mode
    SM_DRAG,
    SM_CARET,
    SM_VOLUME,
    SM_HISTORY,
    SM_SAFE_RANGE // to add more scroll modes leave at end
};

/* keycode macros */
#define SM_MO(sm) sm > 0 ? (((sm)&0x0f) + SCROLL_MODE_MO_START - 1) : SCROLL_MODE_MO_START
#define SM_TG(sm) sm > 0 ? (((sm)&0x0f) + SCROLL_MODE_TG_START - 1) : SCROLL_MODE_TG_START
#define SM_TT(sm) sm > 0 ? (((sm)&0x0f) + SCROLL_MODE_TT_START - 1) : SCROLL_MODE_TT_START

/* alias scroll modes */
#define SM_DRG SM_DRAG
#define SM_CRT SM_CARET
#define SM_VOL SM_VOLUME
#define SM_HST SM_HISTORY

/* ----------Set up functions------------------------------------------------------------------------------------- */
report_mouse_t pointing_device_task_scroll(report_mouse_t mouse_report);                                // handles mouse_report processing add to pointing_device_task
void           pointing_device_axes_conv(scroll_record_t* scroll_record, report_mouse_t* mouse_report); // overwritable function that converts pointing_device x & y axes to local h & v
bool           process_record_scroll(uint16_t keyrecord, keyrecord_t* record);                          // handle processing of builtin keyrecords

/* ----------Control functions------------------------------------------------------------------------------------ */
uint8_t get_scroll_mode(void);                   // returns active scroll mode ID
void    set_scroll_mode(uint8_t scroll_mode);    // set scroll mode to scroll_mode ID
void    toggle_scroll_mode(uint8_t scroll_mode); // toggle scroll mode
uint8_t get_scroll_mode_tg(void);                // get toggle scroll mode index

/* ----------Setting up scroll mode keyrecords-------------------------------------------------------------------- */
void scroll_key_momentary(uint8_t scroll_mode, keyrecord_t* record);                   // momentary change of scroll mode while button is held
void scroll_key_toggle(uint8_t scroll_mode, keyrecord_t* record);                      // toggles scroll mode on/off on keypress
void scroll_key_tap_toggle(uint8_t scroll_mode, keyrecord_t* record);                  // works same as TT with scroll mode
void scroll_key_with_hold(uint8_t scroll_mode, uint16_t kc_hold, keyrecord_t* record); // momentary change of scroll mode while holding keycode

/* ----------Functions for custom scrolling modes----------------------------------------------------------------- */
void scroll_tap_codes(uint16_t kc_h_neg, uint16_t kc_v_neg, uint16_t kc_v_pos, uint16_t kc_h_pos, uint8_t divisor); // turn h/v axis values into key taps 1 tap per divisor

/* ----------Callbacks for modifying and adding scroll modes------------------------------------------------------ */
bool process_scroll_mode_kb(scroll_record_t* scroll_status, report_mouse_t* mouse_report);
bool process_scroll_mode_user(scroll_record_t* scroll_status, report_mouse_t* mouse_report);
