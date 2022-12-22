/* Copyright 2022 Thomas Baart <thomas@splitkb.com>
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

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_ANIMATIONS
#    define RGBLIGHT_HUE_STEP  8
#    define RGBLIGHT_SAT_STEP  8
#    define RGBLIGHT_VAL_STEP  8
#    define RGBLIGHT_LIMIT_VAL 150
#endif

// Lets you roll mod-tap keys
#define IGNORE_MOD_TAP_INTERRUPT

#define SPLIT_POINTING_ENABLE
/*
 * For some reason yet to be debugged, POINTING_DEVICE_RIGHT gets sporadic false movements when the two halves are connected.
 * Right half works fine on its own with POINTING_DEVICE_RIGHT.
 * Current workaround is to turn on POINTING_DEVICE_COMBINED and build a separate firmware with POINTING_DEVICE_DRIVER = custom for the left half.
 * In this configuration, the sporadic false movements show up when using the right half on its own, but works fine when the two are connected.
 */
#define POINTING_DEVICE_RIGHT
// #define POINTING_DEVICE_COMBINED
// #define POINTING_DEVICE_ROTATION_180
// #define POINTING_DEVICE_ROTATION_180_RIGHT
#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_A
#define SPLIT_LAYER_STATE_ENABLE
#define POINTING_DEVICE_TASK_THROTTLE_MS 1
#define USB_POLLING_INTERVAL_MS 1
//#define CIRQUE_PINNACLE_ADDR 0x2A
// #define CIRQUE_PINNACLE_ADC_ATTENUATION 0x80
#define CIRQUE_PINNACLE_ATTENUATION 0x80
// #define CIRQUE_PINNACLE_ATTENUATION 0x40
//#define CIRQUE_PINNACLE_DISABLE_SMOOTHING
// #define CIRQUE_PINNACLE_TAP_ENABLE
// #define CIRQUE_PINNACLE_CIRCULAR_SCROLL_ENABLE
#define POINTING_DEVICE_GESTURES_CURSOR_GLIDE_ENABLE
#define POINTING_DEVICE_GESTURES_SCROLL_ENABLE
#define MOUSE_EXTENDED_REPORT
// #define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS
// #define CIRQUE_PINNACLE_POSITION_MODE 0
// #define CIRQUE_PINNACLE_SECONDARY_TAP_ENABLE
// #define CIRQUE_PINNACLE_SIDE_SCROLL_ENABLE
//#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
// #define POINTING_DEVICE_SCROLL_ENABLE
// #define SCROLL_DRAG_DIVISOR 1
// #define SCROLL_CARET_DIVISOR 1
// #define SCROLL_VOL_DIVISOR 1
// #define SCROLL_HIST_DIVISOR 1
