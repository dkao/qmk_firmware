/* Copyright 2022 Tye (@tyetye)
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

/*
 * Current features of this keymap for the Santoku keyboard:
 Trackpoint speed and acceleration settings are adjustable on-the-fly from the Function layer.
 Trackpoint drag scroll speed is adjustable on-the-fly from the Function layer.
 Mouse input from the Trackpoint is smoothed and scaled in realtime (through the features noted above).
 Combos provide easy web browser tab navigation with only the right hand (no reaching for ctrl-blah).
 Home row mod keys (SHFT, CTL, ALT, GUI).
 TAPALTTB for easy switching to open windows using just one key (an improvement on the "Super Alt Tab" example code from the QMK docs).
 "Caps Word" feature intelligently turns off CAPSLOCK when a non alphnumeric key is pressed (wonderful QMK feature).
 Just for fun: QMK's WPM feature is available on the OLED screen.
 Traditional shift keys in the lower corners are togglable (on or off). This helps the user to ease the transition to home row mods while still allowing the user to be productive during crunch time.

 * Desired TODOs:
 Write to memory so Trackpoint speed and scroll settings stick between keyboard reboots
 IN PROGRESS. Improve SuperAltTab feature so that holding down the key *does not* move to the next window but keeps Alt pressed. Window gets selected *only* after timeout (~1000ms).
 -- This will allow the user to "look through" the AltTab choices without having to worry about quickly selecting one because of the timeout.
 -- But will still also allow the user to easily tap the key once and quickly toggle between the two most recent windows.
 Update the mouse pointer smoothing code to use integer math instead of floating point math. This will probably require some clever log lookup tables but could save up to 1000 bytes in the compiled hex.
 IN TESTING. Change the scroll wheel to use QMK's Pointing Device feature instead of MouseKeys. In theory, this will make the scroll wheel movement smoother because Mouse Keys expect a held down keyswitch instead of a clicky rotary encoder.
 Create a dedicated "help" screen. This will take a lot of bytes because of raw text. Still uncertain how to approach this.
 Add sidescroll ability to the scrollwheel.
 Slowly make options to test the transition to a 36 key layout (make alternatives to the outer columns)
 -- Make ALTTAB delay variable and add ALTTAB timeout setting to settings page (yet another reason to have a dedicated settings page.
 -- Add custom Santoku logo to the OLED.

*/

#include QMK_KEYBOARD_H
#include "santoku.h"
#include <stdbool.h>   // This is just to get rid of the linter warning
#include <stdint.h>   // This is just to get rid of the linter warning

// Santoku keymap set up
enum santoku_layers {
    _QWERTY,
    _SYMBOL,
    _NAVIGATION,
    _FUNCTION };

enum santoku_keycodes { 
    QWERTY = SAFE_RANGE,
    SYMBOL,
    NAVIGATION,
    FUNC,
    OVERVIEW,
    DEC_ACCL,
    INC_ACCL,
    DEC_SPED,
    INC_SPED,
    DEC_DRGS,
    INC_DRGS,
    SHFT_KEY,
    TAPALTTB,
    A_B_TEST };

#define ___x___ XXXXXXX

// One tap alt-tab controls. Improvement to the code example from: https://docs.qmk.fm/#/feature_macros?id=super-alt%e2%86%aftab
bool     is_alt_tab_pressed    = false;
uint16_t alt_tab_timer         = 0;
const uint16_t ALT_TAB_TIMEOUT = 300;

// toggles the typical shift keys (in lower corners). Useful when learning to use homerow mod's shift keys but still need to be productive at day job.
bool normal_shift_key_active = true;

// Trackpoint/mouse pointer dynamic speed controls and GUI/OLED settings
uint8_t acceleration_setting        = 2;
float   acceleration_values[6]      = {0.6, 0.8, 1, 1.2, 1.4, 1.6};
uint8_t linear_reduction_setting    = 2;
float   linear_reduction_values[6]  = {2.4, 2.2, 2.0, 1.8, 1.6, 1.4};
uint8_t drag_scroll_speed_setting   = 2;
uint8_t drag_scroll_speed_values[6] = {8, 7, 6, 5, 4, 3};
char *  progress_bars[6]            = {"[=     ]", "[==    ]", "[===   ]", "[====  ]", "[===== ]", "[=PLAID]"};
uint8_t scroll_wheel_test_setting   = 0;
enum scroll_wheel_setting{
    DEFAULT,
    DEFAULT_FASTER,
    FANCY,
    FANCY2
};

// Combo stuff
enum combos { 
    COMBO_MCOMMADOT_FORWARDHISTORY,
    COMBO_NMCOMM_BACKHISTORY, 
    COMBO_HJK_CLOSETAB, 
    COMBO_YUI_PREVTAB, 
    COMBO_UIO_NEXTTAB, 
    COMBO_GH_CAPSLOCK, 
    COMBO_UI_ESCAPE, 
    COMBO_FG_TAB,
    NUM_COMBOS    // make sure this is always the final element in the combos enum
                  // TODO: get rid of QMK's static value for this because I'm editing it too often and forgetting during debugging!!!!!!
};

const uint16_t PROGMEM combo_yui[]       = {KC_Y, KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM combo_uio[]       = {KC_U, KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM combo_hjk[]       = {KC_H, RSFT_T(KC_J), RCTL_T(KC_K), COMBO_END};
const uint16_t PROGMEM combo_nmcomm[]    = {KC_N, KC_M, KC_COMM, COMBO_END}; 
const uint16_t PROGMEM combo_mcommadot[] = {KC_M, KC_COMMA, KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_gh[]        = {KC_G, KC_H, COMBO_END};
const uint16_t PROGMEM combo_ui[]        = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM combo_fg[]        = {LSFT_T(KC_F), KC_G, COMBO_END};
combo_t key_combos[NUM_COMBOS] = {
    [COMBO_UIO_NEXTTAB] = COMBO_ACTION(combo_uio),
    [COMBO_YUI_PREVTAB] = COMBO_ACTION(combo_yui),
    [COMBO_HJK_CLOSETAB] = COMBO_ACTION(combo_hjk),
    [COMBO_NMCOMM_BACKHISTORY] = COMBO_ACTION(combo_nmcomm),
    [COMBO_MCOMMADOT_FORWARDHISTORY] = COMBO_ACTION(combo_mcommadot),
    [COMBO_GH_CAPSLOCK] = COMBO_ACTION(combo_gh),
    [COMBO_UI_ESCAPE] = COMBO_ACTION(combo_ui),
    [COMBO_FG_TAB] = COMBO_ACTION(combo_fg)
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case COMBO_UI_ESCAPE:
            if (pressed) {
                tap_code16(KC_ESC);
            }
            break;
        case COMBO_UIO_NEXTTAB:
            if (pressed) {
                tap_code16(LCTL(KC_PGDN));
            }
            break;
        case COMBO_YUI_PREVTAB:
            if (pressed) {
                tap_code16(LCTL(KC_PGUP));
            }
            break;
        case COMBO_HJK_CLOSETAB:
            if (pressed) {
                tap_code16(LCTL(KC_W));
            }
            break;
        case COMBO_NMCOMM_BACKHISTORY:
            if (pressed) {
                tap_code16(LALT(KC_LEFT));
            }
            break;
        case COMBO_MCOMMADOT_FORWARDHISTORY:
            if (pressed) {
                tap_code16(LALT(KC_RGHT));
            }
            break;
        case COMBO_GH_CAPSLOCK:
            if (pressed) {
                tap_code16(KC_CAPS);
            }
        case COMBO_FG_TAB:
            if (pressed) {
                tap_code16(KC_TAB);
            }
            break;
    }
}

// Santoku keymap layout
// TODO: figure out why LALT_T doesn't "stick" when held down. It just presses "ALT" then releases. So, using RALT for everything right now.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] =
    {/*QWERTY*/
        {KC_TAB,   KC_Q,         KC_W,         KC_E,                   KC_R,         KC_T,         KC_Y,            KC_U,            KC_I,         KC_O,         KC_P,            KC_BSLS},
        {KC_ESC,   LGUI_T(KC_A), RALT_T(KC_S), LCTL_T(KC_D),           LSFT_T(KC_F), KC_G,         KC_H,            RSFT_T(KC_J),    RCTL_T(KC_K), RALT_T(KC_L), RGUI_T(KC_SCLN), KC_QUOT},
        {SHFT_KEY, KC_Z,         KC_X,         KC_C,                   KC_V,         KC_B,         KC_N,            KC_M,            KC_COMM,      KC_DOT,       KC_SLSH,         SHFT_KEY},
        {___x___,  ___x___,      ___x___,      LT(_FUNCTION, KC_BSPC), KC_SPC,       TAPALTTB,     TT(_NAVIGATION), TT(_SYMBOL),     KC_ENT,       ___x___,      ___x___,         ___x___}},

    [_SYMBOL] =
    {/*SYMBOL*/
        {KC_GRV,  KC_EXLM,      KC_AT,        KC_HASH,      KC_DLR,       KC_PERC,  KC_CIRC, KC_AMPR,      KC_ASTR,      KC_LPRN,      KC_RPRN,      KC_MINS},
        {KC_ESC,  LGUI_T(KC_1), RALT_T(KC_2), LCTL_T(KC_3), LSFT_T(KC_4), KC_5,     KC_6,    RSFT_T(KC_7), RCTL_T(KC_8), RALT_T(KC_9), RGUI_T(KC_0), KC_EQL},
        {_______, KC_BSLS,      KC_UNDS,      KC_PLUS,      KC_LCBR,      KC_RCBR,  KC_LBRC, KC_RBRC,      KC_COMM,      KC_DOT,       KC_SLSH,      _______},
        {___x___, ___x___,      ___x___,      KC_BSPC,      KC_SPC,       OVERVIEW, _______, _______,      KC_ENT,       ___x___,      ___x___,      ___x___}},

    [_NAVIGATION] =
    {/*NAVIGATION*/
        {KC_TAB,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_HOME,       KC_PGDN,       KC_PGUP,            KC_END,               XXXXXXX, XXXXXXX},
        {KC_ESC,  KC_LGUI, KC_RALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_LEFT,       KC_DOWN,       KC_UP,              KC_RGHT,              XXXXXXX, XXXXXXX},
        {_______, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  LGUI(KC_LBRC), LGUI(KC_RBRC), LGUI(LSFT(KC_EQL)), LGUI(LSFT(KC_MINS)),  XXXXXXX, _______},
        {___x___, ___x___,  ___x___,  KC_DEL,   KC_SPC,   OVERVIEW, _______,       _______,       KC_ENT,             ___x___,              ___x___, ___x___}},

    [_FUNCTION] =
    {/*FUNCTION*/
        {KC_TAB,  XXXXXXX,       DEC_ACCL, INC_ACCL, DEC_SPED, INC_SPED, DEC_DRGS, INC_DRGS, A_B_TEST,       XXXXXXX, XXXXXXX, XXXXXXX},
        {KC_ESC,  LGUI_T(KC_F1), RALT_T(KC_F2),          LCTL_T(KC_F3),          LSFT_T(KC_F4),     KC_F5,             KC_F6,                 RSFT_T(KC_F7),         RCTL_T(KC_F8), RALT_T(KC_F9), RGUI_T(KC_F10), XXXXXXX},
        {_______, XXXXXXX,       XXXXXXX,                XXXXXXX,                XXXXXXX,           XXXXXXX,           KC_F11,                KC_F12,                XXXXXXX,       XXXXXXX, XXXXXXX, _______},
        {___x___, ___x___,       ___x___,                KC_DEL,                 KC_SPC,            OVERVIEW,          XXXXXXX,               XXXXXXX,               QK_BOOT,       ___x___, ___x___, ___x___}}
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SHFT_KEY:
            if (normal_shift_key_active) {
                if (record->event.pressed) {
                    register_code(KC_LSFT);
                } else {
                    unregister_code(KC_LSFT);
                }
            }
            return true; // Let QMK send the press/release events
            break;

        case DEC_ACCL:
            if (record->event.pressed) {
                if (acceleration_setting > 0) {
                    acceleration_setting--;
                }
            }
            return true; // Let QMK send the press/release events
            break;

        case INC_ACCL:
            if (record->event.pressed) {
                if (acceleration_setting < 5) {
                    acceleration_setting++;
                }
            }
            return true; // Let QMK send the press/release events
            break;

        case DEC_SPED:
            if (record->event.pressed) {
                if (linear_reduction_setting > 0) {
                    linear_reduction_setting--;
                }
            }
            return true; // Let QMK send the press/release events
            break;

        case INC_SPED:
            if (record->event.pressed) {
                if (linear_reduction_setting < 5) {
                    linear_reduction_setting++;
                }
            }
            return true; // Let QMK send the press/release events
            break;

        case DEC_DRGS:
            if (record->event.pressed) {
                if (drag_scroll_speed_setting > 0) {
                    drag_scroll_speed_setting--;
                }
            }
            return true; // Let QMK send the press/release events
            break;

        case INC_DRGS:
            if (record->event.pressed) {
                if (drag_scroll_speed_setting < 5) {
                    drag_scroll_speed_setting++;
                }
            }
            return true; // Let QMK send the press/release events
            break;

        case OVERVIEW:
            // Macro to handle overview mode. Enter overview, wait, then skip to window after current window
            if (record->event.pressed) {
                register_code(KC_LGUI);
                tap_code(KC_F5);
                unregister_code(KC_LGUI);
                //wait_ms(500);
                tap_code(KC_RIGHT);
                tap_code(KC_RIGHT);
            }
            return true;
            break;

        case TAPALTTB: // Improved on but inspired by: https://github.com/qmk/qmk_firmware/blob/master/keyboards/dz60/keymaps/_bonfire/not-in-use/super-alt-tab.c
            if (record->event.pressed) {
                is_alt_tab_pressed = true;
                register_code(KC_LALT);
                tap_code(KC_TAB);
            } else {
                is_alt_tab_pressed = false;
                alt_tab_timer     = timer_read();
            }
            return true;
            break;

        case A_B_TEST:
            if (record->event.pressed) {
                scroll_wheel_test_setting++;
                if (scroll_wheel_test_setting > FANCY2) {
                    scroll_wheel_test_setting = 0;
                }
            }
            return true;
            break;
    }
    return true;
}

// This is currently only used for the TAPALTTB feature
void matrix_scan_user(void) {
    if (!is_alt_tab_pressed && timer_elapsed(alt_tab_timer) > ALT_TAB_TIMEOUT) {
        unregister_code(KC_LALT);
    }
}

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    static bool show_vanity_text = true;
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
        0x90, 0x91, 0x92, 0x93, 0x94, 0xA0, 0xA1, 0xA2,
        0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA,
        0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2,
        0xB3, 0xB4, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5,
        0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD,
        0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00};
    if (show_vanity_text) {
        uint32_t vanity_timeout = 10000;
        oled_write_ln_P(PSTR("   Santoku Keyboard"), false);
        oled_write_ln_P(PSTR("        by Tye"), false);
        oled_write_ln_P(PSTR("   gestaltinput.com"), false);
        oled_write_ln_P(PSTR(""), false);
        oled_write_ln_P(PSTR("     Hello, World"), false);
        oled_write_P(qmk_logo, false);
        if (timer_read() > vanity_timeout) {
            show_vanity_text = false;
        }
    }
    else if (is_alt_tab_pressed ) {
        oled_write_ln_P(PSTR("   ALT-TAB ACTIVE   "), true);
    }
    else {
        switch (get_highest_layer(layer_state)) {
            case _QWERTY:
                if ((host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK))) {
                    oled_write_P(PSTR("      Caps Lock     \n"), true);
                } else if ( is_caps_word_on() ) {
                    oled_write_P(PSTR("      Caps Word     \n"), true);
                } else {
                    oled_write_P(PSTR("       QWERTY\n"), false);
                }
                oled_write_P(PSTR("WPM:"), false);
                oled_write(get_u8_str(get_current_wpm(), ' '), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR("TB  qwert | yuiop\\"), false);
                oled_write_ln_P(PSTR("ES  asdfg | hjkl;'"), false);
                oled_write_ln_P(PSTR("SH  zxcvb | nm,./"), false);
                //oled_write_P(qmk_logo, false);
                break;

            case _SYMBOL:
                oled_write_P(   PSTR("       Symbol       \n"), true);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(" `  !@#$% | ^&*()-"), false);
                oled_write_ln_P(PSTR("ES  12345 | 67890="), false);
                oled_write_ln_P(PSTR("SH  \\_+{} | [],./"), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(""), false);
                break;

            case _NAVIGATION:
                oled_write_P(   PSTR("      Navigation    \n"), true);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR("   | HM PD PU EN"), false);
                oled_write_ln_P(PSTR("   | << vv ^^ >>"), false);
                oled_write_ln_P(PSTR("   | D[ D] D+ D-"), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(""), false);
                break;

            case _FUNCTION:
                oled_write_P(   PSTR(" Function & Settings\n"), true);
                oled_write_P(   PSTR("SCROLLWHEEL TEST:"), false);
                oled_write(get_u8_str(scroll_wheel_test_setting, ' '), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR("     -+-+ | -+A"), false);
                oled_write_ln_P(PSTR("ES F12345 | 67890"), false);
                oled_write_ln_P(PSTR("CL F      | ab"), false);
                oled_write_P(   PSTR("TP Accl W/E "), false);
                oled_write_ln(progress_bars[acceleration_setting], false);
                oled_write_P(   PSTR("TP Spd  R/T "), false);
                oled_write_ln(progress_bars[linear_reduction_setting], false);
                oled_write_P(   PSTR("TP Scrl Y/U "), false);
                oled_write_ln(progress_bars[drag_scroll_speed_setting], false);
                break;

            default:
                oled_write_ln_P(PSTR("If you see this there's a bug in the layer code :)"), false);
        }
    }

    return false;
}
#endif


void ps2_mouse_moved_user(report_mouse_t *mouse_report) {
    // The math below turns the Trackpoint x and y reports (movements) into a vector and scales the vector with some trigonometry.
    // This allows the user to dynamically adjust the mouse cursor sensitivity to their liking.
    // It also results in arguably smoother movement than just multiplying the x and y values by some fixed value.
    // (and yeah, there's some unnecessary/redundant math going here. I'm hoping to lay the foundation for things like software adjustable negative inertia.)
    if (mouse_report->x != 0 || mouse_report->y != 0) {
        float hypotenuse        = sqrt((mouse_report->x * mouse_report->x) + (mouse_report->y * mouse_report->y));
        float scaled_hypotenuse = pow(hypotenuse, acceleration_values[acceleration_setting]) / linear_reduction_values[linear_reduction_setting];
        float angle             = atan2(mouse_report->y, mouse_report->x);
        mouse_report->x += (scaled_hypotenuse * cos(angle));
        mouse_report->y += (scaled_hypotenuse * sin(angle));
    }
    // Drag scrolling with the Trackpoint is reported so often that it makes the feature is unusable without slowing it down.
    // The below code only reports when the counter is evenly divisible by the chosen integer speed.
    static uint16_t drag_scroll_counter = 0;
    drag_scroll_counter == 40320 ? drag_scroll_counter = 0 : drag_scroll_counter++ ; // Because 8!==40320 (allows clean mod divisibility and avoids scrolling surge when resetting to 0)
    if ((mouse_report->v != 0 || mouse_report->h != 0) && drag_scroll_counter % drag_scroll_speed_values[drag_scroll_speed_setting] != 0) {
        mouse_report->v = 0;
        mouse_report->h = 0;
    }
}


#ifdef ENCODER_ENABLE

/*
   The Trackpoint polling causes small delays in the keyboard/matrix polling.
   This shows up as minor tearing in the OLED redraw and 
   scrollwheel spinning. The code in encoder_update_user is a quick and dirty
   attempt to try a few different methods to smooth out the variations in the 
   scrollwheel readings. (It will *not* increase the actual polling rate)
   I believe this delay is deep in the QMK implementation. Also PS2 is a crotchety old standard.
*/
bool encoder_update_user(uint8_t index, bool clockwise) {
    // float step_values[10] = {2.0, 2.0, 1.8, 1.8, 1.6, 1.6, 1.4, 1.4, 1.2, 1.0};
    // float step_values[10] = {3.0, 3.0, 3.0, 2.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0};
    // float step_values[10] = {2.4, 2.2, 2.0, 1.8, 1.6, 1.4, 1.2, 1.0, 0.8, 0.6};
    // float step_values[10] = {1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};
    // float step_values[10] = {.65, .50, .55, 0.5, 0.45, 0.4, 0.35, 0.3, 0.25, 0.2};
    // float step_values[10] = { .35, 0.35, 0.30, 0.30, 0.25, 0.25, 0.20, 0.20, 0.15, 0.15};
    // float step_values[10] = { .30, 0.20, 0.20, 0.20, 0.20, 0.20, 0.10, 0.10, 0.10, 0.10};
    // float step_values[10] = { .40, .40, .35, 0.35, 0.30, 0.30, 0.25, 0.25, 0.20, 0.20};
    // float step_values[11] = {2.0, 1,8, 1.6, 1.4, 1.2, 1.0, 0.8, 0.6, 0.4, 0.2};
    // float step_values[10] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    //wait_ms(10);
    float step_values[10] = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 0.2, 0.2};
    report_mouse_t currentReport     = pointing_device_get_report();

    static uint16_t encoder_timer    = 0;
    static uint16_t timer_difference = 0;
    static uint16_t hard_delay_max   = 30;
    static bool previous_direction;

    timer_difference = timer_elapsed(encoder_timer);

    //if (timer_difference > 50) return true;

    //if (clockwise != previous_direction && timer_difference < 50 ) {
    if (clockwise != previous_direction && timer_difference < 30 ) {
        clockwise = previous_direction;
    }

    oled_write_P(PSTR("delay:"), false);
    oled_write_ln(get_u8_str(timer_difference, ' '), false);

    if (timer_difference < hard_delay_max) {
        wait_ms(hard_delay_max - timer_difference);
        //wait_ms(hard_delay_max);
        //wait_ms(hard_delay_max);
    }
    /*
    if (timer_difference < hard_delay_max-10) {
        wait_ms(hard_delay_max-10 );
    }
    else if (timer_difference < hard_delay_max-5) {
        wait_ms(hard_delay_max-5 );
    }
    else if (timer_difference < hard_delay_max) {
        wait_ms(hard_delay_max);
    }
    */
    if (scroll_wheel_test_setting == DEFAULT) {
        //currentReport.v = (clockwise ? 1.0 : -1.0);
        //currentReport.v = 0 * (clockwise ? 1.0 : -1.0);
    }
    else if (scroll_wheel_test_setting == DEFAULT_FASTER) {
        currentReport.v = 0 * (clockwise ? 1.0 : -1.0);
    }
    else if (scroll_wheel_test_setting == FANCY) {
        currentReport.v = step_values[ timer_difference / 10] * (clockwise ? 1.0 : -1.0);
    }
    else if (scroll_wheel_test_setting == FANCY2) {
        clockwise ? tap_code(KC_WH_U) : tap_code(KC_WH_D);
    }
    pointing_device_set_report(currentReport);
    pointing_device_send();
    encoder_timer = timer_read();
    previous_direction = clockwise;
    return true;
}
#endif


void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable   = false;
    debug_matrix   = false;
    debug_keyboard = false;
    debug_mouse    = false;
}

