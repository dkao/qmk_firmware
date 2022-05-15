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
 * In this configuration, the sporadic false movements show up when using the left half on its own, but works fine when the two are connected.
 */
//#define POINTING_DEVICE_RIGHT
#define POINTING_DEVICE_COMBINED
#define CIRQUE_PINNACLE_ADDR 0x2A
#define CIRQUE_PINNACLE_CURVED_OVERLAY
#define CIRQUE_PINNACLE_DISABLE_SMOOTHING
#define CIRQUE_PINNACLE_DISABLE_TAP
#define CIRQUE_PINNACLE_ENABLE_CURSOR_GLIDE
#define CIRQUE_PINNACLE_ENABLE_CIRCULAR_SCROLL
