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

#ifdef POINTING_DEVICE_SCROLL_ENABLE

#    include "pointing_device_scroll.h"

/* Scrolling Functions and Variables */
static scroll_record_t local_scroll_record = {.mode_tg = SM_NONE}; // store scroll mode and accumulated scroll values

/* duplication of inline function from pointing_device.c */
static inline int8_t pointing_device_hv_clamp(mouse_xy_report_t value) {
    if (value < INT8_MIN) {
        return INT8_MIN;
    } else if (value > INT8_MAX) {
        return INT8_MAX;
    } else {
        return value;
    }
}

/* access active scroll mode */
uint8_t get_scroll_mode(void) {
    return local_scroll_record.mode;
}

/* set active scroll mode */
void set_scroll_mode(uint8_t scroll_mode) {
    if (local_scroll_record.mode != scroll_mode) {
        local_scroll_record.mode = scroll_mode;
    }
}

/* set current toggled mode */
void toggle_scroll_mode(uint8_t scroll_mode) {
    if (local_scroll_record.mode_tg == scroll_mode) {
        local_scroll_record.mode_tg = SM_NONE;
    } else {
        local_scroll_record.mode_tg = scroll_mode;
    }
    set_scroll_mode(local_scroll_record.mode_tg);
}

/* access current toggled mode */
uint8_t get_scroll_mode_tg(void) {
    return local_scroll_record.mode_tg;
}

/* Change scroll mode while held */
void scroll_key_momentary(uint8_t scroll_mode, keyrecord_t* record) {
    if (record->event.pressed) {
        set_scroll_mode(scroll_mode);
    } else {
        if (get_scroll_mode() == scroll_mode) set_scroll_mode(get_scroll_mode_tg());
    }
}

/* handle scroll mode puttons: toggle*/
void scroll_key_toggle(uint8_t scroll_mode, keyrecord_t* record) {
    if (record->event.pressed) toggle_scroll_mode(scroll_mode);
}

void scroll_key_tap_toggle(uint8_t scroll_mode, keyrecord_t* record) {
#    if TAPPING_TOGGLE != 0 && !defined(NO_ACTION_TAPPING)
    if (record->event.pressed) {
        set_scroll_mode(scroll_mode);
    } else {
        if (record->tap.count == TAPPING_TOGGLE) {
            toggle_scroll_mode(scroll_mode);
        } else {
            if (get_scroll_mode() == scroll_mode) set_scroll_mode(get_scroll_mode_tg());
        }
    }
#    else
    scroll_mode_momentary(scroll_mode, record);
#    endif
}

/* momentary scroll mode while holding  */
void scroll_key_with_hold(uint8_t scroll_mode, uint16_t kc_hold, keyrecord_t* record) {
    if (record->event.pressed) {
        register_code16(kc_hold);
        set_scroll_mode(scroll_mode);
    } else {
        if (get_scroll_mode() == scroll_mode) set_scroll_mode(get_scroll_mode_tg());
        unregister_code16(kc_hold);
    }
}

/* weak function scroll value accumulation */
__attribute__((weak)) void pointing_device_axes_conv(scroll_record_t* scroll_record, report_mouse_t* mouse_report) {
    // accumulate scroll if scroll key is pressed, inverting if set
#    ifdef POINTING_DEVICE_INVERT_H
    scroll_record->h -= mouse_report->x;
#    else
    scroll_record->h += mouse_report->x;
#    endif

#    ifdef POINTING_DEVICE_INVERT_V
    scroll_record->v -= mouse_report->y;
#    else
    scroll_record->v += mouse_report->y;
#    endif
    // stop cursor movement
    mouse_report->x = mouse_report->y = 0;
}

/* process scroll modes */
static void process_scroll_mode(scroll_record_t* scroll_record, report_mouse_t* mouse_report) {
    if (!process_scroll_mode_kb(scroll_record, mouse_report)) return;

    switch (scroll_record->mode) {
        case SM_DRAG:
            // Ensure larger than divisor to avoid clearing unless actually written
            if (abs(scroll_record->h) > SCROLL_DRAG_DIVISOR) {
                mouse_report->h = pointing_device_hv_clamp(scroll_record->h / SCROLL_DRAG_DIVISOR);
                scroll_record->h %= SCROLL_DRAG_DIVISOR;
            }
            if (abs(scroll_record->v) > SCROLL_DRAG_DIVISOR) {
                mouse_report->v = pointing_device_hv_clamp(scroll_record->v / SCROLL_DRAG_DIVISOR);
                scroll_record->v %= SCROLL_DRAG_DIVISOR;
            }
            break;

        case SM_CARET: // Caret scroll mode (uses arrow keys to move cursor)
            scroll_tap_codes(KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, SCROLL_CARET_DIVISOR);
            break;

        case SM_VOLUME: // Volume scroll mode (adjusts volume)
            scroll_tap_codes(KC_NO, KC_VOLD, KC_VOLU, KC_NO, SCROLL_VOL_DIVISOR);
            break;

        case SM_HISTORY:                                                        // History scroll mode (scroll through edit history)
            if (keymap_config.swap_lctl_lgui && keymap_config.swap_rctl_rgui) { // attempt to handle macOS
                scroll_tap_codes(SCMD(KC_Z), KC_NO, KC_NO, LCMD(KC_Z), SCROLL_HIST_DIVISOR);
            } else {
                scroll_tap_codes(C(KC_Z), KC_NO, KC_NO, C(KC_Y), SCROLL_HIST_DIVISOR);
            }
            break;

        default:
            if (get_scroll_mode_tg() == scroll_record->mode) toggle_scroll_mode(scroll_record->mode);
            set_scroll_mode(get_scroll_mode_tg());
            break;
    }
}

/* initiate scrolling task */
report_mouse_t pointing_device_task_scroll(report_mouse_t mouse_report) {
    // skip all processing if no scroll mode set
    if (local_scroll_record.mode == SM_NONE) return mouse_report;

    pointing_device_axes_conv(&local_scroll_record, &mouse_report);
    process_scroll_mode(&local_scroll_record, &mouse_report);
    return mouse_report;
}

/* allow tapping of 16bit keycodes based on scroll input
 * favours horizontal axis and the previous axis
 */
void scroll_tap_codes(uint16_t kc_h_neg, uint16_t kc_v_neg, uint16_t kc_v_pos, uint16_t kc_h_pos, uint8_t divisor) {
    // Ensure larger than divisor before commiting key presses
    // Horizontal handling (LEFT/RIGHT)
    if (abs(local_scroll_record.h) > divisor) {
        if (local_scroll_record.h > 0) {
            // RIGHT
            for (int8_t i = 0; i < abs(local_scroll_record.h) / divisor; i++) {
                tap_code16_delay(kc_h_pos, SCROLL_TAP_DELAY);
            }
        } else {
            // LEFT
            for (int8_t i = 0; i < abs(local_scroll_record.h) / divisor; i++) {
                tap_code16_delay(kc_h_neg, SCROLL_TAP_DELAY);
            }
        }
        local_scroll_record.h %= divisor;
        local_scroll_record.v = 0;
    }
    // Vertical handling (UP/DOWN)
    if (abs(local_scroll_record.v) > divisor) {
        if (local_scroll_record.v > 0) {
            // UP
            for (int8_t i = 0; i < abs(local_scroll_record.v) / divisor; i++) {
                tap_code16_delay(kc_v_pos, SCROLL_TAP_DELAY);
            }

        } else {
            // DOWN
            for (int8_t i = 0; i < abs(local_scroll_record.v) / divisor; i++) {
                tap_code16_delay(kc_v_neg, SCROLL_TAP_DELAY);
            }
        }
        local_scroll_record.v %= divisor;
        local_scroll_record.h = 0;
    }
}

/* Add processing for built in keys */
bool process_record_scroll(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case SCROLL_MODE_MO_START ... SCROLL_MODE_MO_END:
            scroll_key_momentary(((keycode - SCROLL_MODE_MO_START) + 1) & 0x0f, record);
            return true;

        case SCROLL_MODE_TG_START ... SCROLL_MODE_TG_END:
            scroll_key_toggle(((keycode - SCROLL_MODE_TG_START) + 1) & 0x0f, record);
            return true;

        case SCROLL_MODE_TT_START ... SCROLL_MODE_TT_END:
            scroll_key_tap_toggle(((keycode - SCROLL_MODE_TT_START) + 1) & 0x0f, record);
            return true;

        default:
            return true;
    }
}

/* Callback functions for adding processing of additional scroll modes */
__attribute__((weak)) bool process_scroll_mode_kb(scroll_record_t* scroll_record, report_mouse_t* mouse_report) {
    if (!process_scroll_mode_user(scroll_record, mouse_report)) return false;
    return true;
}

__attribute__((weak)) bool process_scroll_mode_user(scroll_record_t* scroll_record, report_mouse_t* mouse_report) {
    return true;
}

#endif // POINTING_DEVICE_SCROLL_ENABLE
