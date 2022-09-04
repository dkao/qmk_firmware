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

/* initialize static functions */
static report_mouse_t process_scroll_mode(uint8_t scroll_mode, report_mouse_t mouse_report);

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

/* set up local context for storing current values */
static scroll_context_t scroll_context = {.mode.active = SCROLL_MODE_DEFAULT, .mode.toggle = SCROLL_MODE_DEFAULT};

/**
 * @brief Reset scroll data
 *
 * Sets scroll data to defaults
 */
void scroll_reset(void) {
    memset(&scroll_context, 0, sizeof(scroll_context));
    scroll_context.mode.active = SCROLL_MODE_DEFAULT;
    scroll_context.mode.toggle = SCROLL_MODE_DEFAULT;
}

/**
 * @brief access active scroll mode
 *
 * @return uint8_t active scroll mode
 */
uint8_t get_scroll_mode(void) {
    return scroll_context.mode.active;
}

/* access current toggled mode */
uint8_t get_scroll_mode_toggle(void) {
    return scroll_context.mode.toggle;
}

/* get scroll values */
int16_t get_scroll(bool axis) {
    if (axis) {
        return scroll_context.v;
    }
    return scroll_context.h;
}

/* set active scroll mode */
void set_scroll_mode(uint8_t scroll_mode) {
    if (scroll_context.mode.active != scroll_mode) {
        scroll_context.mode.active = scroll_mode;
    }
}

/* set current toggled mode */
void scroll_mode_toggle(uint8_t scroll_mode) {
    if (scroll_context.mode.toggle == scroll_mode) {
        scroll_context.mode.toggle = SM_NONE;
    } else {
        scroll_context.mode.toggle = scroll_mode;
    }
    set_scroll_mode(scroll_context.mode.toggle);
}

/* set scroll value */
void set_scroll(bool axis, int16_t val) {
    if (axis) {
        scroll_context.v = val;
        return;
    }
    scroll_context.h = val;
}

/* accumulate scroll */
void accumulate_scroll(bool axis, int16_t val) {
    if (axis) {
#    ifdef POINTING_DEVICE_INVERT_V
        SET_SCROLL_V(SCROLL_V - val);
#    else
        SET_SCROLL_V(SCROLL_V + val);
#    endif
        return;
    }
#    ifdef POINTING_DEVICE_INVERT_H
    SET_SCROLL_H(SCROLL_H - val);
#    else
    SET_SCROLL_H(SCROLL_H + val);
#    endif
}

/* store and reset mouse_report x & y */
void store_cursor(report_mouse_t mouse_report) {
    scroll_context.cursor.x = mouse_report.x;
    scroll_context.cursor.y = mouse_report.y;
}

/* restore mouse_report x & y */
report_mouse_t restore_cursor(report_mouse_t mouse_report) {
    mouse_report.x = scroll_context.cursor.x;
    mouse_report.y = scroll_context.cursor.y;
    return mouse_report;
}

/* weak function to convert axes using accumulation */
__attribute__((weak)) report_mouse_t scroll_axes_conv(report_mouse_t mouse_report) {
    ACCUMULATE_H(mouse_report.x);
    ACCUMULATE_V(mouse_report.y);
    store_cursor(mouse_report);
    mouse_report.x = 0;
    mouse_report.y = 0;
    return mouse_report;
}

/**
 * @brief Tap keycodes based on scroll input
 *
 * Will translate scroll h & v input into keycode taps.
 * Uses vim arrow ordered input (LEFT, DOWN, UP, RIGHT) and will output 1 keytap/divisor
 *
 * NOTE: favours staying on axis and weakly favours the horizontal over the vertical axis
 *
 * @params kc_h_neg[in] uint16_t negative h keycode (LEFT)
 * @params kc_v_neg[in] uint16_t negative v keycode (DOWN)
 * @params kc_v_pos[in] uint16_t positive v keycode (UP)
 * @params kc_h_pos[in] uint16_t positive h keycode (RIGHT)
 * @params divisor[in]  uint8_t
 */
void scroll_tap_codes(uint16_t kc_h_neg, uint16_t kc_v_neg, uint16_t kc_v_pos, uint16_t kc_h_pos, uint8_t divisor) {
    // Ensure larger than divisor before commiting key presses
    // Horizontal handling (RIGHT/LEFT)
    if (abs(SCROLL_H) >= divisor) {
        if (SCROLL_H > 0) {
            // RIGHT
            for (int8_t i = 0; i < abs(SCROLL_H) / divisor; i++) {
                tap_code16_delay(kc_h_pos, SCROLL_TAP_DELAY);
            }
        } else {
            // LEFT
            for (int8_t i = 0; i < abs(SCROLL_H) / divisor; i++) {
                tap_code16_delay(kc_h_neg, SCROLL_TAP_DELAY);
            }
        }
        // Collect residual and clear other axis
        SET_SCROLL_H(SCROLL_H % divisor);
        SET_SCROLL_V(0);
        return;
    }
    // Vertical handling (UP/DOWN)
    if (abs(SCROLL_V) >= divisor) {
        if (SCROLL_V > 0) {
            // UP
            for (int8_t i = 0; i < abs(SCROLL_V) / divisor; i++) {
                tap_code16_delay(kc_v_pos, SCROLL_TAP_DELAY);
            }

        } else {
            // DOWN
            for (int8_t i = 0; i < abs(SCROLL_V) / divisor; i++) {
                tap_code16_delay(kc_v_neg, SCROLL_TAP_DELAY);
            }
        }
        // Collect residual and clear other axis
        SET_SCROLL_V(SCROLL_V % divisor);
        SET_SCROLL_H(0);
    }
}

/* Callback functions for adding processing of additional scroll modes */
__attribute__((weak)) bool process_scroll_mode_user(uint8_t scroll_mode, report_mouse_t* mouse_report) {
    return true;
}

__attribute__((weak)) bool process_scroll_mode_kb(uint8_t scroll_mode, report_mouse_t* mouse_report) {
    if (!process_scroll_mode_user(scroll_mode, mouse_report)) return false;
    return true;
}

/* process scroll modes */
static report_mouse_t process_scroll_mode(uint8_t scroll_mode, report_mouse_t mouse_report) {
    if (!process_scroll_mode_kb(scroll_mode, &mouse_report)) return mouse_report;

    switch (scroll_mode) {
        // Drag scroll mode (sets scroll axes to mouse_report h & v)
        case SM_DRAG:
            // Ensure larger than divisor to avoid collect residuals unless actually written
            if (abs(SCROLL_H) >= SCROLL_DRAG_DIVISOR) {
                mouse_report.h = pointing_device_hv_clamp(SCROLL_H / SCROLL_DRAG_DIVISOR);
                SET_SCROLL_H(SCROLL_H % SCROLL_DRAG_DIVISOR);
            }
            if (abs(SCROLL_V) >= SCROLL_DRAG_DIVISOR) {
                mouse_report.v = pointing_device_hv_clamp(SCROLL_V / SCROLL_DRAG_DIVISOR);
                SET_SCROLL_V(SCROLL_V % SCROLL_DRAG_DIVISOR);
            }
            break;

        // Caret scroll mode (uses arrow keys to move cursor)
        case SM_CARET:
            scroll_tap_codes(KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, SCROLL_CARET_DIVISOR);
            break;

        // Volume scroll mode (adjusts audio volume)
        case SM_VOLUME:
            scroll_tap_codes(KC_NO, KC_VOLD, KC_VOLU, KC_NO, SCROLL_VOL_DIVISOR);
            break;

        // History scroll mode (scroll through recent edit history)
        case SM_HISTORY:
            // attempt to handle macOS
            if (keymap_config.swap_lctl_lgui && keymap_config.swap_rctl_rgui) {
                scroll_tap_codes(SCMD(KC_Z), KC_NO, KC_NO, LCMD(KC_Z), SCROLL_HIST_DIVISOR);
            } else {
                scroll_tap_codes(C(KC_Z), KC_NO, KC_NO, C(KC_Y), SCROLL_HIST_DIVISOR);
            }
            break;

        // assume erroneous scroll mode on default
        default:
            mouse_report         = restore_cursor(mouse_report);
            uint8_t store_toggle = get_scroll_mode_toggle();
            // reset
            scroll_reset();
            if (store_toggle == scroll_mode) break;
            // return toggle mode
            scroll_mode_toggle(store_toggle);
    }
    return mouse_report;
}

/* initiate scrolling task */
report_mouse_t pointing_device_task_scroll(report_mouse_t mouse_report) {
    // skip all processing if scroll_mode is SM_NONE
    if (get_scroll_mode() == SM_NONE) return mouse_report;

    mouse_report = scroll_axes_conv(mouse_report);
    mouse_report = process_scroll_mode(get_scroll_mode(), mouse_report);

    return mouse_report;
}

/* Change scroll mode while held */
void scroll_key_momentary(uint8_t scroll_mode, keyrecord_t* record) {
    if (record->event.pressed) {
        // blindly set mode
        set_scroll_mode(scroll_mode);
    } else {
        // return to toggled mode only if the active mode matches (in case mode switched before release)
        if (get_scroll_mode() == scroll_mode) set_scroll_mode(get_scroll_mode_toggle());
    }
}

/* handle scroll mode buttons: toggle*/
void scroll_key_toggle(uint8_t scroll_mode, keyrecord_t* record) {
    if (!record->event.pressed) scroll_mode_toggle(scroll_mode);
}

/* handle scroll mode buttons: tap-toggle */
void scroll_key_tap_toggle(uint8_t scroll_mode, keyrecord_t* record) {
#    if TAPPING_TOGGLE != 0 && !defined(NO_ACTION_TAPPING)
    if (record->event.pressed) {
        if (record->tap.count < TAPPING_TOGGLE) scroll_mode_toggle(scroll_mode);
    } else {
        if (record->tap.count <= TAPPING_TOGGLE) scroll_mode_toggle(scroll_mode);
    }
#    elif !defined(NO_ACTION_TAPPING)
    scroll_key_toggle(scroll_mode, record);
#    else
    scroll_key_momentary(scroll_mode, record);
#    endif
}

/* handle scroll buttons: hold key while scroll mode is active */
void scroll_key_with_hold(uint8_t scroll_mode, uint16_t kc_hold, keyrecord_t* record) {
    if (record->event.pressed) {
        register_code16(kc_hold);
        set_scroll_mode(scroll_mode);
    } else {
        if (get_scroll_mode() == scroll_mode) set_scroll_mode(get_scroll_mode_toggle());
        unregister_code16(kc_hold);
    }
}

/* Add processing for built in keys */
bool process_record_scroll(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        // handle built in keycods for bottom 16 scroll modes
        // momentary
        case SCROLL_MODE_MO_START ... SCROLL_MODE_MO_END:
            scroll_key_momentary(((keycode - SCROLL_MODE_MO_START) + 1) & 0x0f, record);
            return true;
        // toggle
        case SCROLL_MODE_TG_START ... SCROLL_MODE_TG_END:
            scroll_key_toggle(((keycode - SCROLL_MODE_TG_START) + 1) & 0x0f, record);
            return true;
        // tap toggle
        case SCROLL_MODE_TT_START ... SCROLL_MODE_TT_END:
            scroll_key_tap_toggle(((keycode - SCROLL_MODE_TT_START) + 1) & 0x0f, record);
            return true;

        default:
            return true;
    }
}

#endif // POINTING_DEVICE_SCROLL_ENABLE