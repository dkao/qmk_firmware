/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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
#include QMK_KEYBOARD_H
#include "drivers/sensors/cirque_pinnacle_gestures.h"
#include "transactions.h"

enum layers {
    _COLEMAK_DH = 0,
    //_QWERTY,
    //_DVORAK,
    _FPS1,
    _FPS2,
    _SHMUP,
    _NAV,
    _SYM,
    _FUNCTION,
    //_QWERTY,
    _ADJUST,
    _MOUSE,
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    _AUTOMOUSE,
#endif
};

enum my_keycodes {
    TP_CAL = SAFE_RANGE, // Recalibrate touchpad
};

//#define _SHMUP _DVORAK
//#define _FPS1  _QWERTY
#define XXXXXXX KC_NO

// Aliases for readability
// #define QWERTY   DF(_QWERTY)
// #define COLEMAK  DF(_COLEMAK_DH)
// #define DVORAK   DF(_DVORAK)
//#define QWERTY   TO(_QWERTY)
//#define COLEMAK  TO(_COLEMAK_DH)
//#define DVORAK   TO(_DVORAK)
#define QWERTY   TO(_FPS1)
#define COLEMAK  TO(_COLEMAK_DH)
#define DVORAK   TO(_SHMUP)
#define FPS2     MO(_FPS2)
#define MOUSE    LT(_MOUSE, KC_SPC)

#ifdef POINTING_DEVICE_SCROLL_ENABLE
#define SCR_NON  SM_TG(SM_NONE)
#define SCR_DRG  SM_TG(SM_DRG)
#define SCR_CRT  SM_TG(SM_CRT)
#define SCR_VOL  SM_TG(SM_VOL)
#define SCR_HST  SM_TG(SM_HST)
#else
#define SCR_NON  _______
#define SCR_DRG  _______
#define SCR_CRT  _______
#define SCR_VOL  _______
#define SCR_HST  _______
#endif

#define SYM      MO(_SYM)
#define NAV      MO(_NAV)
#define FKEYS    MO(_FUNCTION)
#define ADJUST   MO(_ADJUST)

#define CTL_ESC  MT(MOD_LCTL, KC_ESC)
#define CTL_QUOT MT(MOD_RCTL, KC_QUOTE)
#define CTL_MINS MT(MOD_RCTL, KC_MINUS)
#define ALT_ENT  MT(MOD_LALT, KC_ENT)

// Note: LAlt/Enter (ALT_ENT) is not the same thing as the keyboard shortcut Alt+Enter.
// The notation `mod/tap` denotes a key that activates the modifier `mod` when held down, and
// produces the key `tap` when tapped (i.e. pressed and released).

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |Ctrl/' "|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  | [ {  |CapsLk|  |F-keys|  ] } |   N  |   M  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      | Enter|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    // [_QWERTY] = LAYOUT(
    //  KC_TAB  , KC_Q ,  KC_W   ,  KC_E  ,   KC_R ,   KC_T ,                                        KC_Y,   KC_U ,  KC_I ,   KC_O ,  KC_P , KC_BSPC,
    //  CTL_ESC , KC_A ,  KC_S   ,  KC_D  ,   KC_F ,   KC_G ,                                        KC_H,   KC_J ,  KC_K ,   KC_L ,KC_SCLN,CTL_QUOT,
    //  KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , KC_LBRC,KC_CAPS,     FKEYS  , KC_RBRC, KC_N,   KC_M ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
    //                             ADJUST , KC_LGUI, ALT_ENT, KC_SPC , NAV   ,     SYM    , KC_SPC ,KC_RALT, KC_RGUI, KC_APP
    // ),
    // [_FPS1] = LAYOUT(
    //  KC_TAB  , KC_Q ,  KC_W   ,  KC_W  ,   KC_E ,   KC_R ,                                       KC_BTN1, KC_BTN2,  KC_I ,   KC_O ,  KC_P , KC_BSPC,
    //  KC_ESC  , KC_A ,  KC_A   ,  KC_S  ,   KC_D ,   KC_F ,                                        KC_H,   KC_J ,  KC_K ,   KC_L ,KC_SCLN,CTL_QUOT,
    //  KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , KC_LBRC,KC_CAPS,     KC_BTN1, KC_RBRC, KC_N,   KC_M ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
    //                             ADJUST , KC_LGUI, KC_SPC , KC_SPC , NAV   ,     SYM    , KC_SPC ,KC_RALT, KC_RGUI, KC_APP
    // ),
    // [_FPS1] = LAYOUT(
    //   KC_ESC , KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   ,                                     KC_BTN3, KC_BTN2, KC_UP  , KC_ENT , KC_PGUP, KC_HOME,
    //   FPS2   , KC_LSFT, KC_A   , KC_S   , KC_D   , KC_F   ,                                     XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_END ,
    //   KC_1   , KC_LCTL, KC_Z   , KC_X   , KC_C   , KC_V   , KC_3   , KC_5   , KC_BTN1, KC_BTN4, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  ,
    //                              ADJUST , KC_LALT, KC_ENT , KC_SPC , KC_7   , XXXXXXX, KC_BTN5, KC_BSPC, KC_RGUI, KC_APP
    // ),
    [_FPS1] = LAYOUT(
      KC_ESC , KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   ,                                     KC_BTN3, KC_BTN2, KC_UP  , KC_ENT , KC_PGUP, KC_HOME,
      FPS2   , KC_LSFT, KC_A   , KC_S   , KC_D   , KC_F   ,                                     TP_CAL , KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_END ,
      KC_1   , KC_LCTL, KC_Z   , KC_X   , KC_C   , KC_V   , KC_3   , KC_5   , KC_BTN1, KC_BTN4, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  ,
                                 ADJUST , KC_LALT, KC_ENT , KC_SPC , KC_7   , XXXXXXX, KC_BTN5, KC_BSPC, KC_RGUI, KC_APP
    ),
    [_FPS2] = LAYOUT(
      _______, _______, _______, _______, _______, KC_T   ,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, KC_G   ,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, KC_B   , KC_4   , KC_6   , _______, _______, KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 ,
                                 KC_GRV , KC_0   , KC_9   , KC_2   , KC_8   , _______, _______, _______, _______, _______
    ),

/*
 * Base Layer: Dvorak
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   | ' "  | , <  | . >  |   P  |   Y  |                              |   F  |   G  |   C  |   R  |   L  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   O  |   E  |   U  |   I  |                              |   D  |   H  |   T  |   N  |   S  |Ctrl/- _|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift | ; :  |   Q  |   J  |   K  |   X  | [ {  |CapsLk|  |F-keys|  ] } |   B  |   M  |   W  |   V  |   Z  | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      | Enter|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    // [_DVORAK] = LAYOUT(
    //  KC_TAB  ,KC_QUOTE,KC_COMM,  KC_DOT,   KC_P ,   KC_Y ,                                        KC_F,   KC_G ,  KC_C ,   KC_R ,  KC_L , KC_BSPC,
    //  CTL_ESC , KC_A ,  KC_O   ,  KC_E  ,   KC_U ,   KC_I ,                                        KC_D,   KC_H ,  KC_T ,   KC_N ,  KC_S , CTL_MINS,
    //  KC_LSFT ,KC_SCLN, KC_Q   ,  KC_J  ,   KC_K ,   KC_X , KC_LBRC,KC_CAPS,     FKEYS  , KC_RBRC, KC_B,   KC_M ,  KC_W ,   KC_V ,  KC_Z , KC_RSFT,
    //                              ADJUST, KC_LGUI, ALT_ENT, KC_SPC , NAV   ,     SYM    , KC_SPC ,KC_RALT, KC_RGUI, KC_APP
    // ),
    [_SHMUP] = LAYOUT(
      KC_TAB , XXXXXXX, XXXXXXX, KC_UP  , XXXXXXX, XXXXXXX,                                     XXXXXXX, KC_Q   , KC_W   , KC_E   , KC_R   , KC_BSPC,
      KC_ESC , XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX,                                     XXXXXXX, KC_Z   , KC_X   , KC_C   , KC_V   , KC_LCTL,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, FKEYS  , XXXXXXX, XXXXXXX, KC_A   , KC_S   , KC_D   , KC_F   , KC_ENT ,
                                 ADJUST , KC_LGUI, ALT_ENT, KC_UP  , NAV    , SYM    , KC_LSFT, KC_SPC , KC_RGUI, KC_APP
    ),

/*
 * Base Layer: Colemak DH
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   |   Q  |   W  |   F  |   P  |   B  |                              |   J  |   L  |   U  |   Y  | ;  : |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   R  |   S  |   T  |   G  |                              |   M  |   N  |   E  |   I  |   O  |Ctrl/' "|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   D  |   V  | [ {  |CapsLk|  |F-keys|  ] } |   K  |   H  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      | Enter|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    // [_COLEMAK_DH] = LAYOUT(
    //  KC_TAB  , KC_Q ,  KC_W   ,  KC_F  ,   KC_P ,   KC_B ,                                        KC_J,   KC_L ,  KC_U ,   KC_Y ,KC_SCLN, KC_BSPC,
    //  CTL_ESC , KC_A ,  KC_R   ,  KC_S  ,   KC_T ,   KC_G ,                                        KC_M,   KC_N ,  KC_E ,   KC_I ,  KC_O , CTL_QUOT,
    //  KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_D ,   KC_V , KC_LBRC,KC_CAPS,     FKEYS  , KC_RBRC, KC_K,   KC_H ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
    //                              ADJUST, KC_LGUI, ALT_ENT, KC_SPC , NAV   ,     SYM    , KC_SPC ,KC_RALT, KC_RGUI, KC_APP
    // ),
    // [_COLEMAK_DH] = LAYOUT(
    //  KC_TAB  , KC_Q ,  KC_W   ,  KC_F  ,   KC_P ,   KC_B ,                                        KC_J,   KC_L ,  KC_U ,   KC_Y ,KC_SCLN, KC_BSPC,
    //  CTL_ESC , KC_A ,  KC_R   ,  KC_S  ,   KC_T ,   KC_G ,                                        KC_M,   KC_N ,  KC_E ,   KC_I ,  KC_O , CTL_QUOT,
    //  KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_D ,   KC_V , KC_LBRC,CAPSWRD,     FKEYS  , KC_RBRC, KC_K,   KC_H ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
    //                              ADJUST, KC_LGUI, ALT_ENT, KC_SPC , NAV   ,     SYM    , KC_SPC ,KC_RALT, KC_RGUI, KC_APP
    // ),
    [_COLEMAK_DH] = LAYOUT(
     KC_TAB  , KC_Q ,  KC_W   ,  KC_F  ,   KC_P ,   KC_B ,                                        KC_J,   KC_L ,  KC_U ,   KC_Y ,KC_SCLN, KC_BSPC,
     CTL_ESC , KC_A ,  KC_R   ,  KC_S  ,   KC_T ,   KC_G ,                                        KC_M,   KC_N ,  KC_E ,   KC_I ,  KC_O , CTL_QUOT,
     KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_D ,   KC_V , KC_LBRC,CAPSWRD,     FKEYS  , KC_RBRC, KC_K,   KC_H ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
                                 ADJUST, KC_LGUI, ALT_ENT, MOUSE  , NAV   ,     SYM    , KC_SPC ,KC_RALT, KC_RGUI, KC_APP
    ),

/*
 * Nav Layer: Media, navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              | PgUp | Home |   ↑  | End  | VolUp| Delete |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  GUI |  Alt | Ctrl | Shift|      |                              | PgDn |  ←   |   ↓  |   →  | VolDn| Insert |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |ScLck |  |      |      | Pause|M Prev|M Play|M Next|VolMut| PrtSc  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_VOLU, KC_DEL,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_VOLD, KC_INS,
      _______, _______, _______, _______, _______, _______, _______, KC_SLCK, _______, _______,KC_PAUSE, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_PSCR,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Sym Layer: Numbers and symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |    `   |  1   |  2   |  3   |  4   |  5   |                              |   6  |  7   |  8   |  9   |  0   |   =    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |    ~   |  !   |  @   |  #   |  $   |  %   |                              |   ^  |  &   |  *   |  (   |  )   |   +    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |    |   |   \  |  :   |  ;   |  -   |  [   |  {   |      |  |      |   }  |   ]  |  _   |  ,   |  .   |  /   |   ?    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYM] = LAYOUT(
      KC_GRV ,   KC_1 ,   KC_2 ,   KC_3 ,   KC_4 ,   KC_5 ,                                       KC_6 ,   KC_7 ,   KC_8 ,   KC_9 ,   KC_0 , KC_EQL ,
     KC_TILD , KC_EXLM,  KC_AT , KC_HASH,  KC_DLR, KC_PERC,                                     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PLUS,
     KC_PIPE , KC_BSLS, KC_COLN, KC_SCLN, KC_MINS, KC_LBRC, KC_LCBR, _______, _______, KC_RCBR, KC_RBRC, KC_UNDS, KC_COMM,  KC_DOT, KC_SLSH, KC_QUES,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Function Layer: Function keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F9  | F10  | F11  | F12  |      |                              | Btn1 | Btn2 | Btn4 | Btn5 |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  F5  |  F6  |  F7  |  F8  |      |                              | Btn3 | Shift| Ctrl |  Alt |  GUI |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  F1  |  F2  |  F3  |  F4  |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    // [_FUNCTION] = LAYOUT(
    //   _______,  KC_F9 ,  KC_F10,  KC_F11,  KC_F12, _______,                                     KC_BTN1, KC_BTN2, KC_BTN4, KC_BTN5, _______, _______,
    //   _______,  KC_F5 ,  KC_F6 ,  KC_F7 ,  KC_F8 , _______,                                     KC_BTN3, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, _______,
    //   _______,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //                              _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    // ),
    [_FUNCTION] = LAYOUT(
      _______,  KC_F9 ,  KC_F10,  KC_F11,  KC_F12, _______,                                     KC_BTN1, KC_BTN2, KC_BTN4, KC_BTN5, _______, TP_CAL ,
      _______,  KC_F5 ,  KC_F6 ,  KC_F7 ,  KC_F8 , _______,                                     KC_BTN3, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, _______,
      _______,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Adjust Layer: Default layer settings, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |QWERTY|      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |Dvorak|      |      |                              | TOG  | SAI  | HUI  | VAI  | MOD  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |Colmak|      |      |      |      |  |      |      |      | SAD  | HUD  | VAD  | RMOD |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, _______, _______, QWERTY , _______, _______,                                    _______, _______, _______, _______,  _______, _______,
      _______, _______, _______, DVORAK , _______, _______,                                    RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI,  RGB_MOD, _______,
      _______, _______, _______, COLEMAK, _______, _______,_______, _______, _______, _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD, _______,
                                 _______, _______, _______,_______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Mouse Layer: Mouse buttons, would be good if this can auto-switch on touch
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  | Btn1 | Btn3 |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  | Btn2 | Btn4 | Btn5 |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    // [_MOUSE] = LAYOUT(
    //   _______, _______, _______, _______, _______, _______,                                     _______, KC_WH_L, KC_WH_R, _______, KC_WH_U, _______,
    //   _______, _______, _______, _______, KC_F24 , _______,                                     _______, KC_BTN1, KC_BTN3, KC_BTN2, KC_WH_D, _______,
    //   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BTN4, KC_BTN5, _______, _______, _______,
    //                              _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    // ),
    [_MOUSE] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, KC_WH_L, KC_WH_R, _______, KC_WH_U, _______,
      SCR_NON, SCR_DRG, SCR_VOL, SCR_CRT, KC_F24 , _______,                                     _______, KC_BTN1, KC_BTN3, KC_BTN2, KC_WH_D, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BTN4, KC_BTN5, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    [_AUTOMOUSE] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, KC_BTN1, KC_BTN3, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, KC_BTN2, KC_BTN4, KC_BTN5, _______, _______
    ),
#endif

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    if defined(POINTING_DEVICE_LEFT)
#        define POINTING_DEVICE_THIS_SIDE is_keyboard_left()
#    elif defined(POINTING_DEVICE_RIGHT)
#        define POINTING_DEVICE_THIS_SIDE !is_keyboard_left()
#    elif defined(POINTING_DEVICE_COMBINED)
#        define POINTING_DEVICE_THIS_SIDE true
#    endif

static bool calibrate_touchpad = false;

#    if 1

//#        if defined(POINTING_DEVICE_DRIVER_cirque_pinnacle_i2c) || defined(POINTING_DEVICE_DRIVER_cirque_pinnacle_spi)
typedef struct _master_to_slave_t {
    int m2s_data;
} master_to_slave_t;

typedef struct _slave_to_master_t {
    int s2m_data;
} slave_to_master_t;

void user_sync_a_slave_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    const master_to_slave_t *m2s = (const master_to_slave_t *)in_data;
    slave_to_master_t       *s2m = (slave_to_master_t *)out_data;
    if (POINTING_DEVICE_THIS_SIDE && m2s->m2s_data) {
        calibrate_touchpad = true;
    }
    s2m->s2m_data = m2s->m2s_data;
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SYNC_A, user_sync_a_slave_handler);
}
//#        endif

// extern void cirque_pinnacle_enable_tap(bool enable);
// extern void cirque_pinnacle_enable_circular_scroll(bool enable);
// extern void cirque_pinnacle_enable_cursor_glide(bool enable);
// // extern void cirque_pinnacle_configure_circular_scroll(float outer_ring_pct, float movement_pct, float movement_ratio, uint8_t wheel_clicks);
// extern void cirque_pinnacle_configure_circular_scroll(uint8_t outer_ring_pct, uint8_t trigger_px, uint16_t trigger_ang, uint8_t wheel_clicks);
// extern void cirque_pinnacle_configure_cursor_glide(float trigger_inches);
static layer_state_t prev_layer_state;

void pointing_device_init_user(void) {
    // #    define TRACKPAD_DIAMETER_MM 40
    // #    define TRACKPAD_DIAMETER_INCH (TRACKPAD_DIAMETER_MM / 25.4)
    // #    define TRACKPAD_CPI_TO_RANGE(cpi) (roundf((cpi) * TRACKPAD_DIAMETER_INCH))
    //     pointing_device_set_cpi(TRACKPAD_CPI_TO_RANGE(400));
#        if defined(POINTING_DEVICE_DRIVER_cirque_pinnacle_i2c) || defined(POINTING_DEVICE_DRIVER_cirque_pinnacle_spi)
    pointing_device_set_cpi(400);
    // cirque_pinnacle_cursor_smoothing(false);
    // cirque_pinnacle_enable_tap(false);
    // cirque_pinnacle_configure_circular_scroll(100, 0, 0, 9);
    // cirque_pinnacle_configure_circular_scroll(33, 16, 9102, 18*3, false);
    // cirque_pinnacle_configure_circular_scroll(33, 16, 9102, 18*2, false);
    // cirque_pinnacle_enable_circular_scroll(true);
    // cirque_pinnacle_enable_cursor_glide(true);
    cirque_pinnacle_configure_cursor_glide(10);
    // cirque_pinnacle_configure_cursor_glide(0);
#            ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    // set_auto_mouse_layer(_MOUSE); // only required if AUTO_MOUSE_DEFAULT_LAYER is not set to index of _MOUSE_LAYER
    set_auto_mouse_layer(_AUTOMOUSE); // only required if AUTO_MOUSE_DEFAULT_LAYER is not set to index of _MOUSE_LAYER
    set_auto_mouse_state(true);
#            endif
#        endif
    prev_layer_state = layer_state;
}

#        if 0 // no layer_state_set_user() on slave side
layer_state_t layer_state_set_user(layer_state_t state) {
    if (POINTING_DEVICE_THIS_SIDE) {
        if (layer_state_cmp(state, _FPS1)) {
            pointing_device_set_cpi(1200);
            cirque_pinnacle_cursor_smoothing(false);
            cirque_pinnacle_enable_circular_scroll(false);
	        cirque_pinnacle_configure_cursor_glide(0);
        } else if (layer_state_is(_FPS1) && !layer_state_cmp(state, _FPS1)) {
            pointing_device_set_cpi(400);
            cirque_pinnacle_cursor_smoothing(true);
            cirque_pinnacle_enable_circular_scroll(true);
	        // cirque_pinnacle_configure_cursor_glide(2);
	        // cirque_pinnacle_configure_cursor_glide(0.0315);
	        cirque_pinnacle_configure_cursor_glide(10);
        }
    }
    return state;
}
#        elif 1
void housekeeping_task_user(void) {
    /* Reset touchpad baseline */
    if (calibrate_touchpad) {
#        if defined(POINTING_DEVICE_DRIVER_cirque_pinnacle_i2c) || defined(POINTING_DEVICE_DRIVER_cirque_pinnacle_spi)
        if (POINTING_DEVICE_THIS_SIDE) {
            cirque_pinnacle_calibrate();
        } else if (is_keyboard_master())
#        endif
       	{
            master_to_slave_t m2s = {1};
            slave_to_master_t s2m = {0};
            transaction_rpc_exec(USER_SYNC_A, sizeof(m2s), &m2s, sizeof(s2m), &s2m);
        }
        calibrate_touchpad = false;
    }
#        if defined(POINTING_DEVICE_DRIVER_cirque_pinnacle_i2c) || defined(POINTING_DEVICE_DRIVER_cirque_pinnacle_spi)
    /*
     * Change touchpad settings based on layers
     * Pending #16232
     */
    bool is_fps_layer, was_fps_layer;
    if (POINTING_DEVICE_THIS_SIDE) {
        is_fps_layer  = layer_state_is(_FPS1);
        was_fps_layer = layer_state_cmp(prev_layer_state, _FPS1);
        if (is_fps_layer && !was_fps_layer) {
            pointing_device_set_cpi(1200);
            cirque_pinnacle_cursor_smoothing(false);
            cirque_pinnacle_enable_circular_scroll(false);
            // cirque_pinnacle_configure_circular_scroll(100, 0, 0, 18, true);
            cirque_pinnacle_configure_cursor_glide(0);
            // cirque_pinnacle_enable_cursor_glide(false);
        } else if (!is_fps_layer && was_fps_layer) {
            pointing_device_set_cpi(400);
            cirque_pinnacle_cursor_smoothing(true);
            cirque_pinnacle_enable_circular_scroll(true);
            // cirque_pinnacle_configure_circular_scroll(33, 16, 9102, 18, false);
            cirque_pinnacle_configure_cursor_glide(10);
            // cirque_pinnacle_enable_cursor_glide(true);
        }
    }
#        endif
    prev_layer_state = layer_state;
}
#        else
#        endif
#    elif 1
void pointing_device_init_user(void) {
    pointing_device_set_cpi(400);
    // pointing_device_set_cpi(100);
}
#    endif
#endif

/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/kyria/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * For your convenience, here's a copy of those settings so that you can uncomment them if you wish to apply your own modifications.
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */

/* DELETE THIS LINE TO UNCOMMENT (1/2)
#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // QMK Logo and version information
        // clang-format off
        static const char PROGMEM qmk_logo[] = {
            0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
            0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
            0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
        // clang-format on

        oled_write_P(qmk_logo, false);
        oled_write_P(PSTR("Kyria rev1.0\n\n"), false);

        // Host Keyboard Layer Status
        oled_write_P(PSTR("Layer: "), false);
        switch (get_highest_layer(layer_state|default_layer_state)) {
            case _QWERTY:
                oled_write_P(PSTR("QWERTY\n"), false);
                break;
            case _DVORAK:
                oled_write_P(PSTR("Dvorak\n"), false);
                break;
            case _COLEMAK_DH:
                oled_write_P(PSTR("Colemak-DH\n"), false);
                break;
            case _NAV:
                oled_write_P(PSTR("Nav\n"), false);
                break;
            case _SYM:
                oled_write_P(PSTR("Sym\n"), false);
                break;
            case _FUNCTION:
                oled_write_P(PSTR("Function\n"), false);
                break;
            case _ADJUST:
                oled_write_P(PSTR("Adjust\n"), false);
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }

        // Write host Keyboard LED Status to OLEDs
        led_t led_usb_state = host_keyboard_led_state();
        oled_write_P(led_usb_state.num_lock    ? PSTR("NUMLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
    } else {
        // clang-format off
        static const char PROGMEM kyria_logo[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
            0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };
        // clang-format on
        oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
    }
    return false;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {

    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return false;
}
#endif
DELETE THIS LINE TO UNCOMMENT (2/2) */

#define SOCD_CLEANER_LRN_UDU
#if defined(SOCD_CLEANER_LRN_UDU) || defined(SOCD_CLEANER_LAST_INPUT_WINS)
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == TP_CAL) {
        if (record->event.pressed) {
            calibrate_touchpad = true;
        }
        return false;
    }

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
#ifdef SOCD_CLEANER_LRN_UDU
	/*
	 * Left + Right = Neutral
	 * Up + Down = Up
	 */
	if (keycode == key_up) {
		if (record->event.pressed) {
			pressed_up |= layer_mask;
			if (pressed_down & layer_mask) {
				unregister_code(key_down);
				return true;
			}
		} else {
			pressed_up &= (~layer_mask);
			if (pressed_down & layer_mask) {
				register_code(key_down);
				return true;
			}
		}
	} else if (keycode == key_down) {
		if (record->event.pressed) {
			pressed_down |= layer_mask;
			if (pressed_up & layer_mask) {
				return false;
			}
		} else {
			pressed_down &= (~layer_mask);
			if (pressed_up & layer_mask) {
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
