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
#define POINTING_DEVICE_RIGHT
#define POINTING_DEVICE_TASK_THROTTLE_MS 1
#define CIRQUE_PINNACLE_ADDR 0x2A
#define CIRQUE_PINNACLE_CURVED_OVERLAY
//#define CIRQUE_PINNACLE_DISABLE_SMOOTHING
#define CIRQUE_PINNACLE_DISABLE_TAP
#define CIRQUE_PINNACLE_ENABLE_CURSOR_GLIDE
#define CIRQUE_PINNACLE_ENABLE_CIRCULAR_SCROLL
/*
 * Default 20ms timeout causes I2C data corruption for some reason.
 * Corruption manifests as random jumps and touch lifts during tracking.
 * Here's a bisect of timout values:
 *   20 ms  - glitches
 *   100 ms - works
 *   60 ms  - works
 *   40 ms  - works
 *   30 ms  - glitches
 *   35 ms  - glitches
 *   38 ms  - glitches
 *   39 ms  - glitches
 * Setting to 100 to get 5 ms time_slice in avr i2c_start(). Don't know if that function is actually guilty, will find out later.
 */
#define CIRQUE_PINNACLE_TIMEOUT 100
