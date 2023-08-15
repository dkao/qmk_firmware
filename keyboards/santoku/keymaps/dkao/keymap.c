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
#include "ps2_mouse.h"
#include "ps2.h"

// Santoku keymap set up
enum santoku_layers {
    _QWERTY,
    _SYMBOL,
    _NAVIGATION,
    _SHMUP,
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
//uint8_t acceleration_setting        = 2;
uint8_t acceleration_setting        = 3;
//float   acceleration_values[6]      = {0.6, 0.8, 1, 1.2, 1.4, 1.6};
//float   acceleration_values[6]      = {0.6, 0.8, 1, 1.5, 2.0, 2.2};
float   acceleration_values[6]      = {0.6, 0.8, 1, 1.5, 1.7777777, 2.0};
//uint8_t linear_reduction_setting    = 2;
uint8_t linear_reduction_setting    = 4;
//float   linear_reduction_values[6]  = {2.4, 2.2, 2.0, 1.8, 1.6, 1.4};
//float   linear_reduction_values[6]  = {4.0, 3.0, 2.0, 1.5, 1.0, 0.5};
//float   linear_reduction_values[6]  = {0.25, 0.5, 1.0, 1.5, 2.0, 2.5};
//float   linear_reduction_values[6]  = {0.5, 0.65, 0.7, 0.75, 1.0, 2.0};
float   linear_reduction_values[6]  = {0.6, 0.7, 0.75, 0.8, 1.0, 2.0};
uint8_t drag_scroll_speed_setting   = 2;
uint8_t drag_scroll_speed_values[6] = {8, 7, 6, 5, 4, 3};
char *  progress_bars[6]            = {"[=     ]", "[==    ]", "[===   ]", "[====  ]", "[===== ]", "[=PLAID]"};
//uint8_t scroll_wheel_test_setting   = 0;
uint8_t scroll_wheel_test_setting   = 3;
enum scroll_wheel_setting{
    DEFAULT,
    DEFAULT_FASTER,
    FANCY,
    FANCY2
};

#if 0
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
#endif

#define CTL_ESC LCTL_T(KC_ESC)
#define GUI_BSP LGUI_T(KC_BSPC)
#define GUI_QUO LGUI_T(KC_QUOT)
#define ALT_BSL RALT_T(KC_BSLS)
#define SHF_BSP LSFT_T(KC_BSPC)
#define SHF_ENT RSFT_T(KC_ENT)
#define MO_NAV MO(_NAVIGATION)
#define MO_SYM MO(_SYMBOL)
#define MO_FUN MO(_FUNCTION)
#define DEFAULT TO(_QWERTY)
#define SHMUP TO(_SHMUP)

// Santoku keymap layout
// TODO: figure out why LALT_T doesn't "stick" when held down. It just presses "ALT" then releases. So, using RALT for everything right now.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] =
    { /* Colemak-DH */
       // XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        {  KC_TAB,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,           KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, KC_BSPC },
        { CTL_ESC,    KC_A,    KC_R,    KC_S,    KC_T,    KC_G,           KC_M,    KC_N,    KC_E,    KC_I,    KC_O, GUI_QUO },
        { KC_LALT,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,           KC_K,    KC_H, KC_COMM,  KC_DOT, KC_SLSH, ALT_BSL },
        { XXXXXXX, XXXXXXX, XXXXXXX, SHF_BSP,  KC_SPC,  MO_NAV,         MO_SYM,  KC_SPC, SHF_ENT, XXXXXXX, XXXXXXX, XXXXXXX }
        //{ XXXXXXX, XXXXXXX, XXXXXXX,  SHF_BSP, MO_NAV,  KC_SPC,         KC_SPC,  MO_SYM, SHF_ENT, XXXXXXX, XXXXXXX, XXXXXXX }
    },

    //[_SYMBOL] =
    //{ /*SYMBOL*/
    //   // XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    //    {  KC_GRV,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,          KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11 },
    //    { KC_TILD,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,           KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_F12 },
    //    { _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,        KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______ },
    //    { XXXXXXX, XXXXXXX, XXXXXXX, _______, _______,  MO_FUN,        _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX }
    //},
    [_SYMBOL] =
    { /*SYMBOL*/
       // XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        {  KC_GRV, _______, _______, KC_UNDS, KC_MINS, _______,        _______, KC_PLUS,  KC_EQL, _______, _______, _______ },
        { KC_TILD,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,           KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______ },
        { _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,        KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______ },
        { XXXXXXX, XXXXXXX, XXXXXXX, _______, _______,  MO_FUN,        _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX }
        //{ XXXXXXX, XXXXXXX, XXXXXXX, _______,  MO_FUN, _______,        _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX }
    },

    [_NAVIGATION] =
    { /*NAVIGATION*/
     // { _______, _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______, _______ },
        { _______, KC_VOLU, KC_MPRV, _______, _______, _______,         KC_INS, KC_HOME,   KC_UP,  KC_END, KC_PGUP, KC_BTN4 },
        { _______, KC_VOLD, KC_MPLY,  KC_F24, CW_TOGG,  KC_EQL,         KC_DEL, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_BTN5 },
        { _______, KC_MUTE, KC_MNXT, KC_UNDS, KC_MINS, KC_PLUS,        KC_PSCR, KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR,  KC_APP },
        { XXXXXXX, XXXXXXX, XXXXXXX, _______, _______, _______,         MO_FUN, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX }
        //{ XXXXXXX, XXXXXXX, XXXXXXX, _______, _______, _______,        _______,  MO_FUN, _______, XXXXXXX, XXXXXXX, XXXXXXX }
    },

    [_SHMUP] =
    { /*SHMUP*/
        { XXXXXXX, XXXXXXX, XXXXXXX,   KC_UP, XXXXXXX, XXXXXXX,          KC_F4,  KC_TAB, KC_LCTL, KC_LALT,  KC_F11, XXXXXXX },
        {  KC_ESC, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX,           KC_Q,    KC_Z,    KC_X,    KC_C,    KC_V, XXXXXXX },
        { XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           KC_W,    KC_A,    KC_S,    KC_D,    KC_F, XXXXXXX },
        { XXXXXXX, XXXXXXX, XXXXXXX, _______,  KC_SPC, _______,        _______, KC_LSFT,  KC_ENT, XXXXXXX, XXXXXXX, XXXXXXX }
    },

    [_FUNCTION] =
    { /*FUNCTION*/
        { XXXXXXX,  QK_BOOT, DEC_ACCL, INC_ACCL, DEC_SPED, INC_SPED,        DEC_DRGS, INC_DRGS, A_B_TEST, OVERVIEW,  QK_BOOT,  XXXXXXX },
        { DEFAULT,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,           KC_F6,    KC_F7,    KC_F8,    KC_F9,   KC_F10,    SHMUP },
        { XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,   KC_F11,  XXXXXXX,         XXXXXXX,   KC_F12,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX },
        { XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______,         _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX }
        //{ XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______,  XXXXXXX,         XXXXXXX,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX }
    },
    //[_FUNCTION] =
    //{ /*FUNCTION*/
    //    { XXXXXXX,  XXXXXXX, DEC_ACCL, INC_ACCL, DEC_SPED, INC_SPED,        DEC_DRGS, INC_DRGS, A_B_TEST,  XXXXXXX,  XXXXXXX,  XXXXXXX },
    //    { XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  QK_BOOT,  XXXXXXX,         XXXXXXX, OVERVIEW,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX },
    //    { XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,         XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX },
    //    { XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______,         _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX }
    //},
};
#if 0
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
#endif

bool socd_check(uint16_t keycode, keyrecord_t *record);
bool socd_cleaner(uint16_t keycode, keyrecord_t *record);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (socd_check(keycode, record)) {
        return socd_cleaner(keycode, record);
    }
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

#if 0
// This is currently only used for the TAPALTTB feature
void matrix_scan_user(void) {
    if (!is_alt_tab_pressed && timer_elapsed(alt_tab_timer) > ALT_TAB_TIMEOUT) {
        unregister_code(KC_LALT);
    }
}
#endif

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    static bool show_vanity_text = true;
    //static bool caps = false;
    static uint8_t prev_layer;
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
        0x90, 0x91, 0x92, 0x93, 0x94, 0xA0, 0xA1, 0xA2,
        0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA,
        0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2,
        0xB3, 0xB4, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5,
        0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD,
        0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00};
    uint8_t current_layer = get_highest_layer(layer_state);
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
        prev_layer = current_layer;
    }
    else if (is_alt_tab_pressed ) {
        oled_write_ln_P(PSTR("   ALT-TAB ACTIVE   "), true);
    }
    else {
    //else if (current_layer != prev_layer) {
        //switch (get_highest_layer(layer_state)) {
        switch (current_layer) {
            case _QWERTY:
                if ((host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK))) {
                    oled_write_P(PSTR("      Caps Lock     \n"), true);
                } else if ( is_caps_word_on() ) {
                    oled_write_P(PSTR("      Caps Word     \n"), true);
                } else {
                    //oled_write_P(PSTR("       QWERTY\n"), false);
                    oled_write_P(PSTR("\n"), false);
                }
                if (current_layer == prev_layer) {
                    break;
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

            case _SHMUP:
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_P(   PSTR("        Shmup       \n"), true);
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
    prev_layer = current_layer;

    return false;
}
#endif

#if 0
void ps2_mouse_init_user(void) {
    // Doesn't work
    //// Set default configuration
    //PS2_MOUSE_SEND(0xE2, "default config: 0xE2");
    //PS2_MOUSE_SEND(0x2C, "default config: 0x2C");
    //PS2_MOUSE_SEND(0x00, "default config: 0x00");

    //// Toggle Press to Select off because it's on by default for some reason
    //PS2_MOUSE_SEND(0xE2, "pts: 0xE2");
    //PS2_MOUSE_SEND(0x47, "pts: 0x47");
    //PS2_MOUSE_SEND(0x2C, "pts: 0x2C");
    //PS2_MOUSE_SEND(0x01, "pts: 0x01");
    uint8_t rcv;

    //// set TrackPoint sensitivity
    //PS2_MOUSE_SEND(0xE2, "tpsens: 0xE2");
    //PS2_MOUSE_SEND(0x81, "tpsens: 0x81");
    //PS2_MOUSE_SEND(0x4A, "tpsens: 0x4A");
    //PS2_MOUSE_SEND(0x49, "tpsens: 0x59");

    //// set TrackPoint Negative Inertia factor
    //PS2_MOUSE_SEND(0xE2, "tpnegin: 0xE2");
    //PS2_MOUSE_SEND(0x81, "tpnegin: 0x81");
    //PS2_MOUSE_SEND(0x4D, "tpnegin: 0x4D");
    //PS2_MOUSE_SEND(0x06, "tpnegin: 0x06");

    //// set TrackPoint speed
    //// (transfer function upper plateau speed)
    //PS2_MOUSE_SEND(0xE2, "tpsp: 0xE2");
    //PS2_MOUSE_SEND(0x81, "tpsp: 0x81");
    //PS2_MOUSE_SEND(0x60, "tpsp: 0x60");
    //PS2_MOUSE_SEND(0x61, "tpsp: 0x61");

    // inquire pts status
    rcv = ps2_host_send(0xE2);
    rcv = ps2_host_send(0x2C);
    rcv = ps2_host_recv_response();
    if ((rcv & 1) == 1) {
        // if on, disable pts
        rcv = ps2_host_send(0xE2);
        rcv = ps2_host_send(0x47);
        rcv = ps2_host_send(0x2C);
        rcv = ps2_host_send(0x01);
    }
}
#else
void ps2_mouse_init_user(void) {
    uint8_t rcv;

#define TRACKPOINT_DEFAULT_CONFIG_PTSON   0
#define TRACKPOINT_DEFAULT_CONFIG_BUTTON2 2
#define TRACKPOINT_DEFAULT_CONFIG_FLIPX   3
#define TRACKPOINT_DEFAULT_CONFIG_FLIPY   4
#define TRACKPOINT_DEFAULT_CONFIG_FLIPZ   5
#define TRACKPOINT_DEFAULT_CONFIG_SWAPXY  6
#define TRACKPOINT_DEFAULT_CONFIG_FTRANS  7

    // Inquire pts status from Default Configuration register
    rcv = ps2_host_send(0xE2);
    rcv = ps2_host_send(0x2C);
    rcv = ps2_host_recv_response();
    if (rcv & (1 << TRACKPOINT_DEFAULT_CONFIG_PTSON)) {
        // If on, disable pts
        rcv = ps2_host_send(0xE2);
        rcv = ps2_host_send(0x47);
        rcv = ps2_host_send(0x2C);
        rcv = ps2_host_send(0x01);
    }

#if 0
    // Set sensitivity
    rcv = ps2_host_send(0xE2);
    rcv = ps2_host_send(0x80);
    rcv = ps2_host_send(0x4A);
    //rcv = ps2_host_send(150);
    rcv = ps2_host_send(255);

    // Set speed
    rcv = ps2_host_send(0xE2);
    rcv = ps2_host_send(0x80);
    rcv = ps2_host_send(0x60);
    //rcv = ps2_host_send(120);
    //rcv = ps2_host_send(195);
    rcv = ps2_host_send(255);
#endif
}
#endif

#if 1
void ps2_mouse_moved_user(report_mouse_t *mouse_report) {
    static float carry_x, carry_y;
    // The math below turns the Trackpoint x and y reports (movements) into a vector and scales the vector with some trigonometry.
    // This allows the user to dynamically adjust the mouse cursor sensitivity to their liking.
    // It also results in arguably smoother movement than just multiplying the x and y values by some fixed value.
    // (and yeah, there's some unnecessary/redundant math going here. I'm hoping to lay the foundation for things like software adjustable negative inertia.)
    if (mouse_report->x != 0 || mouse_report->y != 0) {
        float hypotenuse        = sqrtf((mouse_report->x * mouse_report->x) + (mouse_report->y * mouse_report->y));
        //float scaled_hypotenuse = powf(hypotenuse, acceleration_values[acceleration_setting]) / linear_reduction_values[linear_reduction_setting];
        float scaled_hypotenuse = powf(hypotenuse * linear_reduction_values[linear_reduction_setting], acceleration_values[acceleration_setting]);
        float angle             = atan2f(mouse_report->y, mouse_report->x);
        //mouse_report->x += (scaled_hypotenuse * cos(angle));
        //mouse_report->y += (scaled_hypotenuse * sin(angle));
        carry_x += (scaled_hypotenuse * cos(angle));
        carry_y += (scaled_hypotenuse * sin(angle));
        //carry_x += (scaled_hypotenuse * cos(angle)) + mouse_report->x;
        //carry_y += (scaled_hypotenuse * sin(angle)) + mouse_report->y;
        mouse_report->x = (mouse_xy_report_t)carry_x;
        mouse_report->y = (mouse_xy_report_t)carry_y;
        carry_x -= mouse_report->x;
        carry_y -= mouse_report->y;
    }
#if 0
    // Drag scrolling with the Trackpoint is reported so often that it makes the feature is unusable without slowing it down.
    // The below code only reports when the counter is evenly divisible by the chosen integer speed.
    static uint16_t drag_scroll_counter = 0;
    drag_scroll_counter == 40320 ? drag_scroll_counter = 0 : drag_scroll_counter++ ; // Because 8!==40320 (allows clean mod divisibility and avoids scrolling surge when resetting to 0)
    if ((mouse_report->v != 0 || mouse_report->h != 0) && drag_scroll_counter % drag_scroll_speed_values[drag_scroll_speed_setting] != 0) {
        mouse_report->v = 0;
        mouse_report->h = 0;
    }
#endif
}
#else
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
#if 0
    // Drag scrolling with the Trackpoint is reported so often that it makes the feature is unusable without slowing it down.
    // The below code only reports when the counter is evenly divisible by the chosen integer speed.
    static uint16_t drag_scroll_counter = 0;
    drag_scroll_counter == 40320 ? drag_scroll_counter = 0 : drag_scroll_counter++ ; // Because 8!==40320 (allows clean mod divisibility and avoids scrolling surge when resetting to 0)
    if ((mouse_report->v != 0 || mouse_report->h != 0) && drag_scroll_counter % drag_scroll_speed_values[drag_scroll_speed_setting] != 0) {
        mouse_report->v = 0;
        mouse_report->h = 0;
    }
#endif
}
#endif


#if 1
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
    //static uint16_t hard_delay_max   = 0;
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
    //pointing_device_send();
    encoder_timer = timer_read();
    previous_direction = clockwise;
    return true;
    //return false;
}
#endif
#else
bool encoder_update_user(uint8_t index, bool clockwise) {
    static uint16_t encoder_timer    = 0;
    static bool previous_direction;
    uint16_t timer_difference = timer_elapsed(encoder_timer);
    report_mouse_t currentReport = pointing_device_get_report();

    if (clockwise != previous_direction && timer_difference < 100 ) {
        clockwise = previous_direction;
    }
    encoder_timer = timer_read();
    previous_direction = clockwise;

    if (clockwise) {
	    currentReport.v = 1;
    } else {
	    currentReport.v = -1;
    }
    pointing_device_set_report(currentReport);
    pointing_device_send();
    return false;
}
#endif

#if 0
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        //case LT(1, KC_BSPC):
	    //case SFT_ESC:
	    //case SFT_ENT:
        case SHF_ENT:
        case CTL_ESC:
            // Immediately select the hold action when another key is pressed.
            return true;
        default:
            // Do not select the hold action when another key is pressed.
            return false;
    }
}
#endif

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case CTL_ESC:
            return 0;
        default:
            return QUICK_TAP_TERM;
    }
}

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable   = false;
    debug_matrix   = false;
    debug_keyboard = false;
    debug_mouse    = false;
}

#define SOCD_CLEANER_LRN_UDN
#if defined(SOCD_CLEANER_LRN_UDU) || defined(SOCD_CLEANER_LRN_UDN) || defined(SOCD_CLEANER_LAST_INPUT_WINS)
/*
static keypos_t keypos_up = {0, 0};
static keypos_t keypos_down = {1, 0};
static keypos_t keypos_left = {2, 0};
static keypos_t keypos_right = {3, 0};
*/

static uint8_t pressed_up = 0;
static uint8_t pressed_down = 0;
static uint8_t pressed_left = 0;
static uint8_t pressed_right = 0;

#ifdef SOCD_CLEANER_LAST_INPUT_WINS
static uint8_t up_wins = 0;
static uint8_t left_wins = 0;
#endif

bool socd_check(uint16_t keycode, keyrecord_t *record) {
	uint8_t layer = layer_switch_get_layer(record->event.key);
	if (layer == _SHMUP) {
		return true;
	}
    return false;
}

bool socd_cleaner(uint16_t keycode, keyrecord_t *record) {
	uint8_t layer = layer_switch_get_layer(record->event.key);
	if (layer != _SHMUP) {
		return true;
	}
	/*
	uint16_t key_up = keymap_key_to_keycode(layer, keypos_up);
	uint16_t key_down = keymap_key_to_keycode(layer, keypos_down);
	uint16_t key_left = keymap_key_to_keycode(layer, keypos_left);
	uint16_t key_right = keymap_key_to_keycode(layer, keypos_right);
	*/
	uint16_t key_up = KC_UP;
	uint16_t key_down = KC_DOWN;
	uint16_t key_left = KC_LEFT;
	uint16_t key_right = KC_RGHT;
	uint8_t layer_mask = (1 << layer);
#if defined(SOCD_CLEANER_LRN_UDU) || defined(SOCD_CLEANER_LRN_UDN)
	/*
	 * Left + Right = Neutral
	 * Up + Down = Up
	 */
	if (keycode == key_up) {
		if (record->event.pressed) {
			pressed_up |= layer_mask;
			if (pressed_down & layer_mask) {
				unregister_code(key_down);
#ifdef SOCD_CLEANER_LRN_UDN
				return false;
#else
				return true;
#endif
			}
		} else {
			pressed_up &= (~layer_mask);
			if (pressed_down & layer_mask) {
				register_code(key_down);
#ifdef SOCD_CLEANER_LRN_UDN
				return false;
#else
				return true;
#endif
			}
		}
	} else if (keycode == key_down) {
		if (record->event.pressed) {
			pressed_down |= layer_mask;
			if (pressed_up & layer_mask) {
#ifdef SOCD_CLEANER_LRN_UDN
				unregister_code(key_up);
#endif
				return false;
			}
		} else {
			pressed_down &= (~layer_mask);
			if (pressed_up & layer_mask) {
#ifdef SOCD_CLEANER_LRN_UDN
				register_code(key_up);
#endif
				return false;
			}
		}
	} else if (keycode == key_left) {
		if (record->event.pressed) {
			pressed_left |= layer_mask;
			if (pressed_right & layer_mask) {
				unregister_code(key_right);
				return false;
			}
		} else {
			pressed_left &= (~layer_mask);
			if (pressed_right & layer_mask) {
				register_code(key_right);
				return false;
			}
		}
	} else if (keycode == key_right) {
		if (record->event.pressed) {
			pressed_right |= layer_mask;
			if (pressed_left & layer_mask) {
				unregister_code(key_left);
				return false;
			}
		} else {
			pressed_right &= (~layer_mask);
			if (pressed_left & layer_mask) {
				register_code(key_left);
				return false;
			}
		}
	}
#else
	/* Last input wins */
	if (keycode == key_up) {
		if (record->event.pressed) {
			pressed_up |= layer_mask;
			if (pressed_down & layer_mask) {
				unregister_code(key_down);
				up_wins |= layer_mask;
			}
		} else {
			pressed_up &= (~layer_mask);
			if (pressed_down & layer_mask) {
				if (up_wins & layer_mask) {
					register_code(key_down);
				} else {
					return false;
				}
			}
		}
	} else if (keycode == key_down) {
		if (record->event.pressed) {
			pressed_down |= layer_mask;
			if (pressed_up & layer_mask) {
				unregister_code(key_up);
				up_wins &= (~layer_mask);
			}
		} else {
			pressed_down &= (~layer_mask);
			if (pressed_up & layer_mask) {
				if (!(up_wins & layer_mask)) {
					register_code(key_up);
				} else {
					return false;
				}
			}
		}
	} else if (keycode == key_left) {
		if (record->event.pressed) {
			pressed_left |= layer_mask;
			if (pressed_right & layer_mask) {
				unregister_code(key_right);
				left_wins |= layer_mask;
			}
		} else {
			pressed_left &= (~layer_mask);
			if (pressed_right & layer_mask) {
				if (left_wins & layer_mask) {
					register_code(key_right);
				} else {
					return false;
				}
			}
		}
	} else if (keycode == key_right) {
		if (record->event.pressed) {
			pressed_right |= layer_mask;
			if (pressed_left & layer_mask) {
				unregister_code(key_left);
				left_wins &= (~layer_mask);
			}
		} else {
			pressed_right &= (~layer_mask);
			if (pressed_left & layer_mask) {
				if (!(left_wins & layer_mask)) {
					register_code(key_left);
				} else {
					return false;
				}
			}
		}
	}
#endif
	return true;
}
#endif

#if 1
#ifndef DEBOUNCE
#    define DEBOUNCE 5
#endif

// Maximum debounce: 255ms
#if DEBOUNCE > UINT8_MAX
#    undef DEBOUNCE
#    define DEBOUNCE UINT8_MAX
#endif

#define ROW_SHIFTER ((matrix_row_t)1)

typedef uint8_t debounce_counter_t;

static debounce_counter_t *debounce_counters;
static fast_timer_t        last_time;
static bool                counters_need_update;
static bool                matrix_need_update;
static bool                cooked_changed;
static bool                last_eager;

#    define DEBOUNCE_ELAPSED 0

static void update_debounce_counters(uint8_t num_rows, uint8_t elapsed_time);
static void transfer_matrix_values(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows);
static void update_debounce_counters_and_transfer_if_expired(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, uint8_t elapsed_time);
static void start_debounce_counters(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows);

// we use num_rows rather than MATRIX_ROWS to support split keyboards
void debounce_init(uint8_t num_rows) {
    debounce_counters = (debounce_counter_t *)malloc(num_rows * MATRIX_COLS * sizeof(debounce_counter_t));
    int i             = 0;
    for (uint8_t r = 0; r < num_rows; r++) {
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            debounce_counters[i++] = DEBOUNCE_ELAPSED;
        }
    }
}

void debounce_free(void) {
    free(debounce_counters);
    debounce_counters = NULL;
}

bool debounce(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, bool changed) {
    bool updated_last = false;
    //bool use_eager = (IS_LAYER_ON(_SHMUP) || IS_LAYER_ON(_FPS1) || IS_LAYER_ON(_FPS2));
    bool use_eager = IS_LAYER_ON(_SHMUP);
    cooked_changed    = false;

    if (use_eager != last_eager) {
        counters_need_update = false;
        matrix_need_update = true;
        int i             = 0;
        for (uint8_t r = 0; r < num_rows; r++) {
            for (uint8_t c = 0; c < MATRIX_COLS; c++) {
                debounce_counters[i++] = DEBOUNCE_ELAPSED;
            }
        }
        last_eager = use_eager;
    }

    if (counters_need_update) {
        fast_timer_t now          = timer_read_fast();
        fast_timer_t elapsed_time = TIMER_DIFF_FAST(now, last_time);

        last_time    = now;
        updated_last = true;
        if (elapsed_time > UINT8_MAX) {
            elapsed_time = UINT8_MAX;
        }

        if (elapsed_time > 0) {
            if (use_eager) {
                update_debounce_counters(num_rows, elapsed_time);
            } else {
                update_debounce_counters_and_transfer_if_expired(raw, cooked, num_rows, elapsed_time);
            }
        }
    }

    if (changed || matrix_need_update) {
        if (!updated_last) {
            last_time = timer_read_fast();
        }

        if (use_eager) {
            transfer_matrix_values(raw, cooked, num_rows);
        } else {
            start_debounce_counters(raw, cooked, num_rows);
        }
    }

    return cooked_changed;
}

// If the current time is > debounce counter, set the counter to enable input.
static void update_debounce_counters(uint8_t num_rows, uint8_t elapsed_time) {
    counters_need_update                 = false;
    matrix_need_update                   = false;
    debounce_counter_t *debounce_pointer = debounce_counters;
    for (uint8_t row = 0; row < num_rows; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            if (*debounce_pointer != DEBOUNCE_ELAPSED) {
                if (*debounce_pointer <= elapsed_time) {
                    *debounce_pointer  = DEBOUNCE_ELAPSED;
                    matrix_need_update = true;
                } else {
                    *debounce_pointer -= elapsed_time;
                    counters_need_update = true;
                }
            }
            debounce_pointer++;
        }
    }
}

// upload from raw_matrix to final matrix;
static void transfer_matrix_values(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows) {
    matrix_need_update                   = false;
    debounce_counter_t *debounce_pointer = debounce_counters;
    for (uint8_t row = 0; row < num_rows; row++) {
        matrix_row_t delta        = raw[row] ^ cooked[row];
        matrix_row_t existing_row = cooked[row];
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            matrix_row_t col_mask = (ROW_SHIFTER << col);
            if (delta & col_mask) {
                if (*debounce_pointer == DEBOUNCE_ELAPSED) {
                    *debounce_pointer    = DEBOUNCE;
                    counters_need_update = true;
                    existing_row ^= col_mask; // flip the bit.
                    cooked_changed = true;
                }
            }
            debounce_pointer++;
        }
        cooked[row] = existing_row;
    }
}

static void update_debounce_counters_and_transfer_if_expired(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, uint8_t elapsed_time) {
    counters_need_update                 = false;
    debounce_counter_t *debounce_pointer = debounce_counters;
    for (uint8_t row = 0; row < num_rows; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            if (*debounce_pointer != DEBOUNCE_ELAPSED) {
                if (*debounce_pointer <= elapsed_time) {
                    *debounce_pointer        = DEBOUNCE_ELAPSED;
                    matrix_row_t cooked_next = (cooked[row] & ~(ROW_SHIFTER << col)) | (raw[row] & (ROW_SHIFTER << col));
                    cooked_changed |= cooked[row] ^ cooked_next;
                    cooked[row] = cooked_next;
                } else {
                    *debounce_pointer -= elapsed_time;
                    counters_need_update = true;
                }
            }
            debounce_pointer++;
        }
    }
}

static void start_debounce_counters(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows) {
    debounce_counter_t *debounce_pointer = debounce_counters;
    for (uint8_t row = 0; row < num_rows; row++) {
        matrix_row_t delta = raw[row] ^ cooked[row];
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            if (delta & (ROW_SHIFTER << col)) {
                if (*debounce_pointer == DEBOUNCE_ELAPSED) {
                    *debounce_pointer    = DEBOUNCE;
                    counters_need_update = true;
                }
            } else {
                *debounce_pointer = DEBOUNCE_ELAPSED;
            }
            debounce_pointer++;
        }
    }
}
#endif
