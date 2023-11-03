/* Copyright 2022 Dennis Kruyt (dennis@kruyt.org)
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

enum layer_names {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST
};

#define CTL_ESC LCTL_T(KC_ESC)
#define GUI_QUO LGUI_T(KC_QUOT)
#define ALT_BSL RALT_T(KC_BSLS)
#define SHF_TAB LSFT_T(KC_TAB)
#define SHF_SPC LSFT_T(KC_SPC)
#define SHF_ENT RSFT_T(KC_ENT)
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_ortho_4x12(
        //KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        //KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        //KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_UP,   KC_ENT,
        //KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        //CTL_ESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, GUI_QUO,
        //KC_LALT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS,
        //KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC,
        //CTL_ESC, KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
        //KC_LALT, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS,
        KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC,
        CTL_ESC, KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,    KC_O,    GUI_QUO,
        KC_LALT, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, ALT_BSL,
        //KC_LALT, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS,
        //KC_CAPS, KC_LCTL, KC_LALT, KC_LGUI, KC_SPC,  MO(1),   MO(2),   KC_SPC,  KC_UP,   KC_LEFT, KC_DOWN, KC_RGHT
        //KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_GRV,  KC_QUOT, KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN,
        //KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_MINS, KC_EQL,  KC_M,    KC_N,    KC_E,    KC_I,    KC_O,
        //KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_LBRC, KC_RBRC, KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH,
        //CTL_ESC, KC_LALT, KC_LGUI, SHF_TAB, KC_BSPC,  MO(1),   MO(2),   KC_SPC, SHF_ENT, KC_RCTL, KC_RALT, KC_BSLS
        //CTL_ESC, KC_LALT, KC_LGUI, SHF_SPC, KC_SPC,  MO(1),   MO(2),   KC_BSPC, SHF_ENT, KC_RCTL, KC_RALT, KC_BSLS
        //CTL_ESC, KC_LALT, KC_LGUI, MO(1), SHF_SPC, KC_SPC,  KC_BSPC, SHF_ENT, MO(2), KC_RCTL, KC_RALT, KC_BSLS
        //CTL_ESC, KC_LALT, KC_LGUI, MO(1), SHF_SPC, KC_SPC,  KC_SPC, SHF_ENT, MO(2), KC_RCTL, KC_RALT, KC_BSLS
        //CW_TOGG, KC_F24, KC_LGUI, KC_LSFT,  KC_SPC, MO(1),   MO(2),   KC_SPC, SHF_ENT, KC_RALT, KC_RGUI, KC_RCTL
        CW_TOGG, KC_F24, KC_LGUI, KC_LSFT,  KC_SPC, MO(1),   MO(2),   KC_SPC, SHF_ENT, KC_BTN1, KC_BTN3, KC_BTN2
        //CTL_ESC, KC_LALT, KC_LGUI, KC_BSPC,  SHF_TAB, MO(1),   MO(2),   SHF_ENT, KC_SPC, KC_RCTL, KC_RALT, KC_BSLS
        //CTL_ESC, KC_LALT, KC_LGUI, KC_SPC,  SHF_TAB, MO(1),   MO(2),   SHF_ENT, KC_BSPC, KC_RCTL, KC_RALT, KC_BSLS
        //KC_LCTL, KC_LALT, KC_LGUI, KC_SPC,  KC_TAB, MO(1),     MO(2),   KC_ENT,  KC_BSPC,  KC_RGHT,   KC_LEFT, KC_DOWN
    ),
    [_LOWER] = LAYOUT_ortho_4x12(
        //KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,    KC_ASTR,    KC_LPRN, KC_RPRN, KC_BSPC,
        //KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS,    KC_PLUS,    KC_LCBR, KC_RCBR, KC_PIPE,
        //_______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  S(KC_NUHS), S(KC_NUBS), KC_HOME, KC_END,  _______,
        //_______, _______, _______, _______, _______, _______, _______, _______,    KC_MNXT,    KC_VOLD, KC_VOLU, KC_MPLY
        _______, KC_VOLU, KC_MPRV, _______, _______, _______,         KC_INS, KC_HOME,   KC_UP,  KC_END, KC_PGUP, KC_BTN4,
        _______, KC_VOLD, KC_MPLY,  KC_F24, CW_TOGG,  KC_EQL,         KC_DEL, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_BTN5,
        _______, KC_MUTE, KC_MNXT, KC_UNDS, KC_MINS, KC_PLUS,        KC_PSCR, KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR,  KC_APP,
        //KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_MPRV, KC_VOLU, KC_INS,  KC_HOME, KC_UP,   KC_END,  KC_PGUP,
        //KC_F5,   KC_F6,   KC_F7,   KC_F8,   CW_TOGG, KC_MPLY, KC_VOLD, KC_DEL,  KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,
        //KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_ESC,  KC_MNXT, KC_MUTE, KC_PSCR, KC_TAB,  KC_ESC,  _______, KC_APP,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_RAISE] = LAYOUT_ortho_4x12(
        //KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        //KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
        //_______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NUHS, KC_NUBS, KC_PGUP, KC_PGDN, _______,
        //_______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY
         KC_GRV, _______, _______, KC_UNDS, KC_MINS, _______,        _______, KC_PLUS,  KC_EQL, _______, _______, _______,
        KC_TILD,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,           KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
        _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,        KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
        //_______, _______, _______, _______, _______, KC_UNDS, KC_EQL,  _______, _______, _______, _______, _______,
        //KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_MINS, KC_PLUS, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        //KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, _______, _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_ADJUST] = LAYOUT_ortho_4x12(
        QK_BOOT,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,         XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
        XXXXXXX,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,           KC_F6,    KC_F7,    KC_F8,    KC_F9,   KC_F10,  XXXXXXX,
        XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,   KC_F11,  XXXXXXX,         XXXXXXX,   KC_F12,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
        //QK_BOOT, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, _______, _______, _______, _______, DB_TOGG,
        //_______, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, AG_NORM, AG_SWAP, _______, _______, _______, _______, _______,
        //_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    )
};

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case CTL_ESC:
            return 0;
        default:
            return QUICK_TAP_TERM;
    }
}
