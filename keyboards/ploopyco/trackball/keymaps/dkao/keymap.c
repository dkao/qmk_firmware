/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
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


#if 0 /* layer tap doesn't work with KC_BTN* */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( /* Base */
        KC_BTN1, KC_BTN3, KC_BTN2,
          KC_BTN4, LT(2, KC_BTN5)
    ),
    [1] = LAYOUT(
        KC_BTN1, DPI_CONFIG, KC_BTN3,
          KC_BTN2, LT(3, KC_BTN4)
    ),
    [2] = LAYOUT(
        _______, TO(1), DPI_CONFIG,
          _______, _______
    ),
    [3] = LAYOUT(
        _______, TO(0), _______,
          _______, _______
    )
};
#else

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [0] = ACTION_TAP_DANCE_LAYER_MOVE(KC_BTN5, 1),
    [1] = ACTION_TAP_DANCE_LAYER_MOVE(KC_BTN4, 0),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( /* Base */
        KC_BTN1, KC_BTN3, KC_BTN2,
          KC_BTN4, TD(0)
    ),
    [1] = LAYOUT(
        KC_BTN1, DPI_CONFIG, KC_BTN3,
          KC_BTN2, TD(1)
    )
};
#endif
