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
Trackpoint:
    - Speed, Acceleration, and Angle are adjustable on-the-fly from the Settings Screen.
    - Drag Scroll (aka Middle Click Scroll) speed is adjustable on-the-fly from the Settings Screen

Settings Screen:
    - Dedicated Settings screen and keyboard layer. Settings are scrollable up/down.

Combos:
    - Combos provide easy web browser tab navigation with only the right hand (no reaching for ctrl-blah).

Home Row Mods:
    - Home Row Keys provide easy to reach mods (SHFT, CTL, ALT, GUI).

One Tap Alt-Tab:
    - TAPALTTB for easy switching to open windows using just one key
        - (an improvement on the "Super Alt Tab" example code from the QMK docs).

Caps Word:
    - "Caps Word" feature intelligently turns off CAPSLOCK when a non alphnumeric key is pressed (wonderful QMK feature).

Pinky Shift Keys:
    - Traditional shift keys in the lower corners are togglable (on or off) from the Settings Screen.
        - This helps the user to ease the transition to home row mods while still allowing the user to be productive during crunch time.

Desired TODOs:
    - IN PROGRESS: Write to memory (EEPROM) so Trackpoint speed and scroll settings stick between keyboard reboots
    - IN PROGRESS/TESTING. Change the scroll wheel to use QMK's Pointing Device feature instead of MouseKeys. In theory, this will make the scroll wheel movement smoother because Mouse Keys expect a held down keyswitch instead of a clicky rotary encoder.
    - IN PROGRESS: Add custom Santoku logo to the OLED.

    - Update the mouse pointer smoothing code to use integer math instead of floating point math. This will probably require some clever lookup tables but could save up to 1000 bytes in the compiled hex.
    - Add sidescroll ability to the scrollwheel.
    - Slowly make options to test the transition to a 36 key layout (make alternatives to the outer columns)

*/

#include QMK_KEYBOARD_H
#include "santoku.h"
#include <stdbool.h>   // This is just to get rid of the linter warning
#include <stdint.h>   // This is just to get rid of the linter warning

#ifdef PS2_MOUSE_ENABLE
#include "ps2_mouse.h"
#include "ps2.h"
#endif

// for EEPROM to save settings between resets
//#include "quantum.h"
//#include "eeprom.h"
//#define EEPROM_CUSTOM_START 32
//bool your_boolean; // The value you want to store
//uint8_t eeprom_value;

#define VANITY_TIMEOUT 5000
#define ___x___ XXXXXXX

void scroll_settings(int8_t direction);
void rotate_mouse_coordinates(uint16_t angle, report_mouse_t *mouse_report);
void update_settings_display(void);
void adjust_setting_uint16(uint16_t *setting, int8_t adjustment, uint16_t min, uint16_t max);
void adjust_setting_uint8(uint8_t *setting, int8_t adjustment, uint8_t min, uint8_t max);

// Santoku keymap set up
enum santoku_layers {
    _QWERTY,
    _SYMBOL,
    _NAVIGATION,
    _FUNCTION,
    _SETTINGS
};

enum santoku_keycodes {
    QWERTY = SAFE_RANGE,
    SYMBOL,
    NAVIGATION,
    FUNC,
    OVERVIEW,
    SHFT_KEY,
    TAPALTTB,
    SETTINGS_UP,
    SETTINGS_DOWN,
    SETTINGS_LEFT,
    SETTINGS_RIGHT,
    SETTINGS_SELECT,
    A_B_TEST
};

//enum settings_screen_choice {
//    ROTATION_ANGLE,
//    TP_ACCELERATION,
//    TP_SPEED,
//    TP_SCROLL_SPEED,
//    PINKY_SHIFT,
//    EXP_SEND_MOUSE_UPDATE, /* whether to send extra mouse update after scrollwheel click */
//    //ALT_TAB_TIMEOUT,
//    //SCROLLWHEEL_DELAY, // need to come up with a better name
//    NUM_SETTINGS };

#define MAX_DISPLAY_ROWS 4

int8_t settings_scroll_position = 0;
int8_t settings_selected_setting = 0;

typedef enum {
    SETTING_TP_ACCELERATION,
    SETTING_TP_SPEED,
    SETTING_TP_SCROLL_SPEED,
    SETTING_ROTATION_ANGLE,
    SETTING_PINKY_SHIFT,
    SETTING_ALT_TAB_TIMEOUT,
    SETTING_DEFAULT_LAYOUT,
    SETTING_OPERATING_SYSTEM,
    SETTING_EXP_SEND_MOUSE_UPDATE,
    SETTING_SCROLLWHEEL_DELAY,
    NUM_SETTINGS  // make sure this is always the final element in the enum
} setting_id_t;
uint8_t current_setting_choice = 0;

typedef struct {
    setting_id_t id;
    const char* name;
    void (*display_func)(setting_id_t id, bool is_current);
} setting_t;

// Forward declarations of display functions
void display_placeholder(setting_id_t id, bool is_current);
void display_int(setting_id_t id, bool is_current);
void display_progress_x6(setting_id_t id, bool is_current);
void display_bool(setting_id_t id, bool is_current);


setting_t settings[NUM_SETTINGS] = {
    {SETTING_TP_ACCELERATION,       "TP Accel    ",      display_progress_x6},
    {SETTING_TP_SPEED,              "TP Speed    ",      display_progress_x6},
    {SETTING_TP_SCROLL_SPEED,       "TP Scroll   ",      display_progress_x6},
    {SETTING_ROTATION_ANGLE,        "TP Rotate      ",   display_int},
    {SETTING_PINKY_SHIFT,           "Pinky Shift      ", display_bool},
    {SETTING_ALT_TAB_TIMEOUT,       "AltTab Time    ",   display_int},
    {SETTING_DEFAULT_LAYOUT,        "Def Layout  ",      display_placeholder},
    {SETTING_OPERATING_SYSTEM,      "Oper System ",      display_placeholder},
    {SETTING_EXP_SEND_MOUSE_UPDATE, "EX MouseUpd ",      display_placeholder},
    {SETTING_SCROLLWHEEL_DELAY,     "ScrlWhlDel     ",   display_int}
};

// One tap alt-tab controls. Improvement to the code example from: https://docs.qmk.fm/#/feature_macros?id=super-alt%e2%86%aftab
bool     is_alt_tab_pressed = false;
uint16_t alt_tab_timer      = 0;
uint16_t alt_tab_timeout    = 300;

// toggles the typical shift keys (in lower corners). Useful when learning to use homerow mod's shift keys but still need to be productive at day job.
bool is_pinky_shift_keys_active = true;

// Trackpoint/mouse pointer dynamic speed controls and GUI/OLED settings
uint8_t acceleration_setting        = 2;
float   acceleration_values[6]      = {0.6f, 0.8f, 1.0f, 1.2f, 1.4f, 1.6f};
uint8_t linear_reduction_setting    = 2;
float   linear_reduction_values[6]  = {2.4f, 2.2f, 2.0f, 1.8f, 1.6f, 1.4f};
uint8_t drag_scroll_speed_setting   = 2;
uint8_t drag_scroll_speed_values[6] = {8, 7, 6, 5, 4, 3};
char *  progress_bars_x6[6]         = {"[=     ]", "[==    ]", "[===   ]", "[====  ]", "[===== ]", "[=PLAID]"};
uint8_t scroll_wheel_test_setting   = 0;
uint16_t mouse_rotation_angle = 350;

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

// This logo uses about 7% of the entire available memory on the atmega32u4
// TODO: Research overwriting QMK's built in font set to make better settings screen and reduce this logo's space requirements
const char PROGMEM santoku_logo[] = {
    // 'Santoku_logo_bold_text', 128x32px
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xd0, 0xe8, 0xf4, 0x7a, 0x3d, 0xfd, 0xfd, 0xfc,
    0xfa, 0xf4, 0xe8, 0xd0, 0xa0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xe0, 0x08, 0xf4, 0xf8, 0xfc, 0x1e, 0xcf, 0xe7, 0xf3, 0xf9, 0x7c, 0xbe, 0xdf, 0xef, 0xe7, 0xf7,
    0xf7, 0x77, 0xf7, 0xf7, 0xef, 0xcf, 0x3e, 0xfd, 0xfa, 0xe4, 0x10, 0x00, 0x00, 0x00, 0x10, 0xfc,
    0x82, 0x01, 0x01, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0xc0, 0x00, 0x00,
    0xf0, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xf0, 0x00, 0xe0, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xe0, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0xf0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0xe0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0xf0, 0x00, 0xf0, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20,
    0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0,
    0x03, 0x10, 0x2f, 0x5f, 0xbf, 0x78, 0xf3, 0xe7, 0xef, 0xef, 0xef, 0xef, 0xef, 0xf7, 0xfb, 0xfd,
    0x7e, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0x7c, 0xbf, 0x5f, 0x27, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x41, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x20, 0x20, 0x30, 0x1c, 0x07, 0x00,
    0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x20, 0x3f, 0x00, 0x7f, 0x01, 0x03, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x30,
    0x20, 0x40, 0x00, 0x00, 0x20, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x20, 0x3f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x05, 0x0b, 0x17, 0x2f, 0x5f, 0xbf, 0xbf, 0xbf, 0x1e,
    0x1f, 0x0f, 0x17, 0x0b, 0x05, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

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
        {KC_TAB,  ___x___,  ___x___,  ___x___,  ___x___,  ___x___,  KC_HOME,       KC_PGDN,       KC_PGUP,            KC_END,               ___x___, ___x___},
        {KC_ESC,  KC_LGUI, KC_RALT, KC_LCTL, KC_LSFT, ___x___, KC_LEFT,       KC_DOWN,       KC_UP,              KC_RGHT,              ___x___, ___x___},
        {_______, ___x___,  ___x___,  ___x___,  ___x___,  ___x___,  LGUI(KC_LBRC), LGUI(KC_RBRC), LGUI(LSFT(KC_EQL)), LGUI(LSFT(KC_MINS)),  ___x___, _______},
        {___x___, ___x___,  ___x___,  KC_DEL,   KC_SPC,   OVERVIEW, _______,       _______,       KC_ENT,             ___x___,              ___x___, ___x___}},

    [_FUNCTION] =
    {/*FUNCTION*/
        {KC_TAB,  ___x___,       ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, A_B_TEST,       ___x___, ___x___, ___x___},
        {KC_ESC,  LGUI_T(KC_F1), RALT_T(KC_F2),          LCTL_T(KC_F3),          LSFT_T(KC_F4),     KC_F5,             KC_F6,                 RSFT_T(KC_F7),         RCTL_T(KC_F8), RALT_T(KC_F9), RGUI_T(KC_F10), ___x___},
        {_______, ___x___,       ___x___,                ___x___,                ___x___,           ___x___,           KC_F11,                KC_F12,                ___x___,       ___x___, TO(_SETTINGS), _______},
        {___x___, ___x___,       ___x___,                KC_DEL,                 KC_SPC,            OVERVIEW,          ___x___,               ___x___,               QK_BOOT,       ___x___, ___x___, ___x___}},
    [_SETTINGS] =
    {/*SETTINGS*/
        {___x___, ___x___, ___x___, SETTINGS_UP, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___},
        {TO(_QWERTY), ___x___, SETTINGS_LEFT, SETTINGS_DOWN, SETTINGS_RIGHT, ___x___, SETTINGS_LEFT, SETTINGS_DOWN, SETTINGS_UP, SETTINGS_RIGHT, ___x___, ___x___},
        {___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, TO(_QWERTY), ___x___},
        {___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, SETTINGS_SELECT, ___x___, ___x___, ___x___}
    }
    ,};


// TODO: Make better way to handle this for uint16_t AND ALSO uint8_t
void adjust_setting_uint16(uint16_t *setting, int8_t adjustment, uint16_t min, uint16_t max) {
    if (adjustment == -1 && *setting == min) {
        *setting = max;
    }
    else if (adjustment == 1 && *setting == max) {
        *setting = min;
    }
    else {
        //*setting += adjustment;
        uint16_t new_value = *setting + adjustment;
        if (new_value > max)
            *setting = min;
        else if (new_value < min)
            *setting = max;
        else
            *setting = new_value;
    }
}

void adjust_setting_uint8(uint8_t *setting, int8_t adjustment, uint8_t min, uint8_t max) {
    if (adjustment == -1 && *setting == min) {
        *setting = max;
    }
    else if (adjustment == 1 && *setting == max) {
        *setting = min;
    }
    else {
        //*setting += adjustment;
        uint16_t new_value = *setting + adjustment;
        if (new_value > max)
            *setting = min;
        else if (new_value < min)
            *setting = max;
        else
            *setting = new_value;
    }
}



// Handles scrolling up and down through the choices on the Settings layer/screen
void scroll_settings(int8_t direction) {
    settings_scroll_position += direction;
    settings_selected_setting += direction;

    int8_t max_scroll_position = NUM_SETTINGS > MAX_DISPLAY_ROWS ? NUM_SETTINGS - MAX_DISPLAY_ROWS : 0;
    int8_t max_selected_setting = NUM_SETTINGS - 1;

    // handle edge cases
    if (settings_scroll_position < 0) settings_scroll_position = 0;
    if (settings_scroll_position > max_scroll_position) settings_scroll_position = max_scroll_position;
    if (settings_selected_setting < 0) settings_selected_setting = 0;
    if (settings_selected_setting > max_selected_setting) settings_selected_setting = max_selected_setting;
}

// Loops through the visible Settings on the Settings screen to display them
void update_settings_display() {
    //static char * temp_progress_bars[9] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    for (uint8_t i = 0; i < MAX_DISPLAY_ROWS; i++) {
        uint8_t setting_index = settings_scroll_position + i;
        if (setting_index < NUM_SETTINGS) {
            oled_write(settings[setting_index].name, settings_selected_setting == setting_index);
            settings[setting_index].display_func(settings[setting_index].id, settings_selected_setting == setting_index);
        }
    }
}

// Used by update_display_settings to show Progress Bar settings
void display_progress_x6(setting_id_t id, bool is_current) {
    int value;
    switch (id) {
        case SETTING_TP_ACCELERATION: value = acceleration_setting; break;
        case SETTING_TP_SPEED: value = linear_reduction_setting; break;
        case SETTING_TP_SCROLL_SPEED: value = drag_scroll_speed_setting; break;
        default: value = 1;
    }

    oled_write_ln(progress_bars_x6[value], is_current);
}

// Used by update_display_settings to show boolean settings
void display_bool(setting_id_t id, bool is_current) {
    bool value;
    switch (id) {
        case SETTING_PINKY_SHIFT: value = is_pinky_shift_keys_active; break;
        default: value = false;
    }
    if (value) {
        oled_write_ln_P(PSTR("Yes"), is_current);
    } else {
        oled_write_ln_P(PSTR(" No"), is_current);
    }
}


// Used by update_display_settings to show integer settings
void display_int(setting_id_t id, bool is_current) {
    int value;
    switch (id) {
        case SETTING_ROTATION_ANGLE: value = mouse_rotation_angle; break;
        case SETTING_ALT_TAB_TIMEOUT: value = alt_tab_timeout; break;
        default: value = 7;
    }

    oled_write(get_u16_str(value, ' '), is_current);
    oled_write_ln_P(PSTR(""), false);
}

// Used by update_display_settings to provide some text for Settings that aren't finished yet.
void display_placeholder(setting_id_t id, bool is_current) {
    oled_write_ln_P(PSTR("Not Done"), is_current);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SHFT_KEY:
            if (is_pinky_shift_keys_active) {
                if (record->event.pressed) {
                    register_code(KC_LSFT);
                } else {
                    unregister_code(KC_LSFT);
                }
            }
            return true; // Let QMK send the press/release events

        case OVERVIEW:
            // Macro to handle overview mode. Enter overview, wait, then skip to window after current window
            if (record->event.pressed) {
                register_code(KC_LGUI);
                tap_code(KC_F5);
                unregister_code(KC_LGUI);
                tap_code(KC_RIGHT);
                tap_code(KC_RIGHT);
            }
            return true;

        case TAPALTTB: // Improved on but inspired by: https://github.com/qmk/qmk_firmware/blob/master/keyboards/dz60/keymaps/_bonfire/not-in-use/super-alt-tab.c
            if (record->event.pressed) {
                is_alt_tab_pressed = true;
                register_code(KC_LALT);
                tap_code(KC_TAB);
            } else {
                is_alt_tab_pressed = false;
                alt_tab_timer      = timer_read();
            }
            return true;

        case SETTINGS_UP:
        case SETTINGS_DOWN:
            if (record->event.pressed) {
                scroll_settings(keycode == SETTINGS_UP ? -1 : 1);
            }
            return true;

        case SETTINGS_LEFT:
        case SETTINGS_RIGHT: {
                                 if (record->event.pressed) {
                                     int8_t adjustment = (keycode == SETTINGS_LEFT) ? -1 : 1;
                                     if (settings_selected_setting == SETTING_ROTATION_ANGLE) {
                                         adjust_setting_uint16(&mouse_rotation_angle, adjustment, 0, 359);
                                     }
                                     else if (settings_selected_setting == SETTING_ALT_TAB_TIMEOUT) {
                                         adjust_setting_uint16(&alt_tab_timeout, adjustment * 25, 100, 1200);
                                     }
                                     else if (settings_selected_setting == SETTING_TP_ACCELERATION) {
                                         adjust_setting_uint8(&acceleration_setting, adjustment, 0, 5);
                                     }
                                     else if (settings_selected_setting == SETTING_TP_SPEED) {
                                         adjust_setting_uint8(&linear_reduction_setting, adjustment, 0, 5);
                                     }
                                     else if (settings_selected_setting == SETTING_TP_SCROLL_SPEED) {
                                         adjust_setting_uint8(&drag_scroll_speed_setting, adjustment, 0, 5);
                                     }
                                     else if (settings_selected_setting == SETTING_PINKY_SHIFT) {
                                         is_pinky_shift_keys_active = !is_pinky_shift_keys_active;
                                     }
                                 }
                                 return true;
                             }

        case A_B_TEST:
                             if (record->event.pressed) {
                                 scroll_wheel_test_setting++;
                                 if (scroll_wheel_test_setting > FANCY2) {
                                     scroll_wheel_test_setting = 0;
                                 }
                             }
                             return true;
    }
    return true;
}

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

// This is currently only used for the TAPALTTB feature
void matrix_scan_user(void) {
    if (!is_alt_tab_pressed && timer_elapsed(alt_tab_timer) > alt_tab_timeout) {
        unregister_code(KC_LALT);
    }
}

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    static bool show_vanity_text = true;
    if (show_vanity_text) {
        uint32_t vanity_timeout = VANITY_TIMEOUT;
        oled_write_ln_P(PSTR("   Santoku Keyboard"), false);
        oled_write_ln_P(PSTR("   gestaltinput.com"), false);
        oled_write_ln_P(PSTR(""), false);
        oled_write_ln_P(PSTR("     Hello, World"), false);
        oled_write_raw_P(santoku_logo, sizeof(santoku_logo) );
        if (timer_read() > vanity_timeout) {
            show_vanity_text = false;
        }
    }
    else {
        switch (get_highest_layer(layer_state)) {
            case _QWERTY:
                if (is_alt_tab_pressed || timer_elapsed(alt_tab_timer) < alt_tab_timeout) {
                    oled_write_ln_P(PSTR("   ALT-TAB ACTIVE   "), true);
                } else if ((host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK))) {
                    oled_write_ln_P(PSTR("      Caps Lock     "), true);
                } else if ( is_caps_word_on() ) {
                    oled_write_ln_P(PSTR("      Caps Word     "), true);
                } else {
                    oled_write_ln_P(PSTR("       QWERTY       "), true);
                }
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR("TB  qwert | yuiop\\"), false);
                oled_write_ln_P(PSTR("ES  asdfg | hjkl;'"), false);
                oled_write_ln_P(PSTR("SH  zxcvb | nm,./"), false);
                oled_write_ln_P(PSTR("Fn Sp Atb | Nv Sy En"), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR("  Fn+/ for Options"), false);
                break;

            case _SYMBOL:
                oled_write_ln_P(PSTR("       Symbol       "), true);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(" `  !@#$% | ^&*()-"), false);
                oled_write_ln_P(PSTR("ES  12345 | 67890="), false);
                oled_write_ln_P(PSTR("SH  \\_+{} | [],./"), false);
                oled_write_ln_P(PSTR("__ Sp Ovw | Nv __ En"), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(""), false);
                break;

            case _NAVIGATION:
                // TODO: Research how to display graphic characters in QMK font instead of using <<, >>, D[, etc
                oled_write_ln_P(PSTR("     Navigation     "), true);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR("       | HM PD PU EN"), false);
                oled_write_ln_P(PSTR("       | << vv ^^ >>"), false);
                oled_write_ln_P(PSTR("       | D[ D] D+ D-"), false);
                oled_write_ln_P(PSTR("Dl Sp Ovw | __ __ En"), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR(""), false);
                break;

            case _FUNCTION:
                oled_write_ln_P(PSTR("      Function      "), true);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR("          |"), false);
                oled_write_ln_P(PSTR("ES F12345 | 67890"), false);
                oled_write_ln_P(PSTR("CL F      | ab  Opt"), false);
                oled_write_ln_P(PSTR("__ Sp Ovw | __ __ Rs"), false);
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR("  Fn+/ for Options"), false);
                break;

            case _SETTINGS:
                oled_write_ln_P(PSTR("      Options       "), true);
                oled_write_ln_P(PSTR(""), false);
                update_settings_display();
                oled_write_ln_P(PSTR(""), false);
                oled_write_ln_P(PSTR("SELECT HJKL,  EXIT /"), false);
                break;
        }
    }

    return false;
}
#endif



// TODO: Move the speed and acceleration code into a separate function to make more modular
// TODO: Move the drag scroll counter code into a separate function to make more modular
void ps2_mouse_moved_user(report_mouse_t *mouse_report) {
    rotate_mouse_coordinates(mouse_rotation_angle, mouse_report);
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
    // Drag scrolling with the Trackpoint is reported so often that it makes the feature unusable without slowing it down.
    // The below code only reports when the counter is evenly divisible by the chosen integer speed.
    // Skipping reports is technically, probably, not ideal. I'd like to find a way to send a slower speed without skipping.
    // As is, however, it works well and is user configurable from the Options screen.
    static uint16_t drag_scroll_counter = 0;
    drag_scroll_counter == 40320 ? drag_scroll_counter = 0 : drag_scroll_counter++ ; // Because 8!==40320 (allows clean mod divisibility and avoids scrolling surge when resetting to 0)
    if ((mouse_report->v != 0 || mouse_report->h != 0) && drag_scroll_counter % drag_scroll_speed_values[drag_scroll_speed_setting] != 0) {
        mouse_report->v = 0;
        mouse_report->h = 0;
    }
}

void rotate_mouse_coordinates(uint16_t angle, report_mouse_t *mouse_report) {
    // because pi/180 = 0.017453
    static const float degree = 0.017453f;

    float radians = angle * degree;

    // Need to save these values because we rewrite mouse_report->x immediately but reuse the value to find the rotated y value
    int current_x = mouse_report->x;
    int current_y = mouse_report->y;

    // Calculate rotated x & y, convert back to an int
    mouse_report->x = round(cos(radians) * current_x - sin(radians) * current_y);
    mouse_report->y = round(sin(radians) * current_x + cos(radians) * current_y);
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
}

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable   = false;
    debug_matrix   = false;
    debug_keyboard = false;
    debug_mouse    = false;

    //    uint8_t eeprom_value = eeprom_read_byte((uint8_t*)EEPROM_CUSTOM_START);
    //    // Save the boolean to EEPROM
    //    if(eeprom_value == 0xFF) {
    //        // The value has not been set yet, so set it to a default value
    //        your_boolean = false;
    //    } else {
    //        your_boolean = eeprom_value == 1;
    //    }
    //    eeprom_update_byte((uint8_t*)EEPROM_CUSTOM_START, !your_boolean ? 1 : 0);
}

