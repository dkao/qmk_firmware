/* Copyright 2017 Joshua Broekhuijsen <snipeye+qmk@gmail.com>
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2021 Dasky (@daskygit)
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

#include "pointing_device.h"
#include "debug.h"
#include "wait.h"
#include "timer.h"
#include <stddef.h>

// hid mouse reports cannot exceed -127 to 127, so constrain to that value
#define constrain_hid(amt) ((amt) < -127 ? -127 : ((amt) > 127 ? 127 : (amt)))

// get_report functions should probably be moved to their respective drivers.
#if defined(POINTING_DEVICE_DRIVER_adns5050)
report_mouse_t adns5050_get_report(report_mouse_t mouse_report) {
    report_adns5050_t data = adns5050_read_burst();

    if (data.dx != 0 || data.dy != 0) {
#    ifdef CONSOLE_ENABLE
        if (debug_mouse) dprintf("Raw ] X: %d, Y: %d\n", data.dx, data.dy);
#    endif

        mouse_report.x = data.dx;
        mouse_report.y = data.dy;
    }

    return mouse_report;
}

// clang-format off
const pointing_device_driver_t pointing_device_driver = {
    .init         = adns5050_init,
    .get_report   = adns5050_get_report,
    .set_cpi      = adns5050_set_cpi,
    .get_cpi      = adns5050_get_cpi,
};
// clang-format on
#elif defined(POINTING_DEVICE_DRIVER_adns9800)

report_mouse_t adns9800_get_report_driver(report_mouse_t mouse_report) {
    report_adns9800_t sensor_report = adns9800_get_report();

    int8_t clamped_x = constrain_hid(sensor_report.x);
    int8_t clamped_y = constrain_hid(sensor_report.y);

    mouse_report.x = clamped_x;
    mouse_report.y = clamped_y;

    return mouse_report;
}

// clang-format off
const pointing_device_driver_t pointing_device_driver = {
    .init       = adns9800_init,
    .get_report = adns9800_get_report_driver,
    .set_cpi    = adns9800_set_cpi,
    .get_cpi    = adns9800_get_cpi
};
// clang-format on
#elif defined(POINTING_DEVICE_DRIVER_analog_joystick)
report_mouse_t analog_joystick_get_report(report_mouse_t mouse_report) {
    report_analog_joystick_t data = analog_joystick_read();

#    ifdef CONSOLE_ENABLE
    if (debug_mouse) dprintf("Raw ] X: %d, Y: %d\n", data.x, data.y);
#    endif

    mouse_report.x = data.x;
    mouse_report.y = data.y;

    mouse_report.buttons = pointing_device_handle_buttons(mouse_report.buttons, data.button, POINTING_DEVICE_BUTTON1);

    return mouse_report;
}

// clang-format off
const pointing_device_driver_t pointing_device_driver = {
    .init       = analog_joystick_init,
    .get_report = analog_joystick_get_report,
    .set_cpi    = NULL,
    .get_cpi    = NULL
};
// clang-format on
#elif defined(POINTING_DEVICE_DRIVER_cirque_pinnacle_i2c) || defined(POINTING_DEVICE_DRIVER_cirque_pinnacle_spi)
#    ifndef CIRQUE_PINNACLE_DISABLE_TAP
#        ifndef CIRQUE_PINNACLE_TAPPING_TERM
#            ifdef TAPPING_TERM_PER_KEY
#                include "action.h"
#                include "action_tapping.h"
#                define CIRQUE_PINNACLE_TAPPING_TERM get_tapping_term(KC_BTN1, &(keyrecord_t){})
#            else
#                ifdef TAPPING_TERM
#                    define CIRQUE_PINNACLE_TAPPING_TERM TAPPING_TERM
#                else
#                    define CIRQUE_PINNACLE_TAPPING_TERM 200
#                endif
#            endif
#        endif
#        ifndef CIRQUE_PINNACLE_TOUCH_DEBOUNCE
#            define CIRQUE_PINNACLE_TOUCH_DEBOUNCE (CIRQUE_PINNACLE_TAPPING_TERM * 8)
#        endif
typedef struct {
    uint16_t timer;
    bool     z;
} trackpad_tap_context_t;

static trackpad_tap_context_t tap;

report_mouse_t trackpad_tap(report_mouse_t mouse_report, pinnacle_data_t touchData) {
    if ((bool)touchData.zValue != tap.z) {
        tap.z = (bool)touchData.zValue;
        if (!touchData.zValue) {
            if (timer_elapsed(tap.timer) < CIRQUE_PINNACLE_TAPPING_TERM && tap.timer != 0) {
                mouse_report.buttons = pointing_device_handle_buttons(mouse_report.buttons, true, POINTING_DEVICE_BUTTON1);
                pointing_device_set_report(mouse_report);
                pointing_device_send();
#        if TAP_CODE_DELAY > 0
                wait_ms(TAP_CODE_DELAY);
#        endif
                mouse_report.buttons = pointing_device_handle_buttons(mouse_report.buttons, false, POINTING_DEVICE_BUTTON1);
                pointing_device_set_report(mouse_report);
                pointing_device_send();
            }
        }
        tap.timer = timer_read();
    }
    if (timer_elapsed(tap.timer) > (CIRQUE_PINNACLE_TOUCH_DEBOUNCE)) {
        tap.timer = 0;
    }

    return mouse_report;
}
#    endif

#    ifdef CIRQUE_PINNACLE_ENABLE_CURSOR_GLIDE
typedef struct {
    int8_t dx;
    int8_t dy;
    bool   valid;
} cursor_glide_t;

typedef struct {
    float    coef;
    float    v0;
    int16_t  x;
    int16_t  y;
    uint16_t z;
    uint16_t timer;
    uint16_t interval;
    uint16_t counter;
    int8_t   dx0;
    int8_t   dy0;
} cursor_glide_context_t;

static cursor_glide_context_t glide;

cursor_glide_t cursor_glide(void) {
    cursor_glide_t report;
    float   p;
    int32_t x, y;

    glide.counter++;
    // calculate current position
    p            = glide.v0 * glide.counter - glide.coef * glide.counter * glide.counter / 2;
    x            = (int16_t)(p * glide.dx0 / glide.v0);
    y            = (int16_t)(p * glide.dy0 / glide.v0);
    report.dx    = (int8_t)(x - glide.x);
    report.dy    = (int8_t)(y - glide.y);
    report.valid = true;
    if (report.dx <= 1 && report.dx >= -1 && report.dy <= 1 && report.dy >= -1) {
        glide.dx0 = 0;
        glide.dy0 = 0;
    }
    glide.x     = x;
    glide.y     = y;
    glide.timer = timer_read();

    return report;
}

cursor_glide_t cursor_glide_check(void) {
    cursor_glide_t invalid_report = {0, 0, false};
    if (glide.z || (glide.dx0 == 0 && glide.dy0 == 0) || timer_elapsed(glide.timer) < glide.interval)
        return invalid_report;
    else
        return cursor_glide();
}

cursor_glide_t cursor_glide_start(void) {
    glide.coef = 0.4; // good enough default
    glide.interval = 10; // hardcode for 100sps
    glide.timer = timer_read();
    glide.counter = 0;
    glide.v0 = hypotf(glide.dx0, glide.dy0);
    glide.x = 0;
    glide.y = 0;
    glide.z = 0;

    return cursor_glide();
}

void cursor_glide_update(int8_t dx, int8_t dy, uint16_t z) {
    glide.dx0 = dx;
    glide.dy0 = dy;
    glide.z = z;
}
#    endif

#    ifdef CIRQUE_PINNACLE_ENABLE_CIRCULAR_SCROLL
#        ifndef CIRQUE_PINNACLE_SCROLL_RING_PERCENTAGE
#            define CIRQUE_PINNACLE_SCROLL_RING_PERCENTAGE 33
#        endif
#        ifndef CIRQUE_PINNACLE_SCROLL_MOVEMENT_PERCENTAGE
#            define CIRQUE_PINNACLE_SCROLL_MOVEMENT_PERCENTAGE 6
#        endif
#        ifndef CIRQUE_PINNACLE_SCROLL_MOVEMENT_RATIO
#            define CIRQUE_PINNACLE_SCROLL_MOVEMENT_RATIO 1.2
#        endif
#        ifndef CIRQUE_PINNACLE_SCROLL_WHEEL_CLICKS
#            define CIRQUE_PINNACLE_SCROLL_WHEEL_CLICKS 9
#        endif

typedef enum {
    SCROLL_UNINITIALIZED,
    SCROLL_DETECTING,
    SCROLL_VALID,
    NOT_SCROLL,
} circular_scroll_status_t;

typedef struct {
    int8_t v;
    int8_t h;
    bool   suppress_touch;
} circular_scroll_t;

typedef struct {
    float                    mag;
    int16_t                  x;
    int16_t                  y;
    uint16_t                 z;
    circular_scroll_status_t state;
    bool                     axis;
} circular_scroll_context_t;

static circular_scroll_context_t scroll;

circular_scroll_t circular_scroll(pinnacle_data_t touchData) {
    circular_scroll_t report = {0, 0, false};
    int16_t           x, y;
    uint16_t          center = cirque_pinnacle_get_scale() / 2;
    int8_t            wheel_clicks;
    float             mag, ang, scalar_projection, scalar_rejection, parallel_movement, perpendicular_movement;
    int32_t           dot, det;

    if (touchData.zValue) {
        // place origin at center of trackpad
        x = (int16_t)(touchData.xValue - center);
        y = (int16_t)(touchData.yValue - center);

        // check if first touch
        if (!scroll.z) {
            report.suppress_touch = false;
            // check if touch falls within outer ring
            mag = hypotf(x, y);
            if (mag / center >= (100.0 - CIRQUE_PINNACLE_SCROLL_RING_PERCENTAGE) / 100.0) {
                scroll.state = SCROLL_DETECTING;
                scroll.x     = x;
                scroll.y     = y;
                scroll.mag   = mag;
                // decide scroll axis:
                //   vertical if started from righ half
                //   horizontal if started from left half
                // reverse for left hand? TODO?
#        if defined(POINTING_DEVICE_ROTATION_90)
                scroll.axis = y < 0;
#        elif defined(POINTING_DEVICE_ROTATION_180)
                scroll.axis = x > 0;
#        elif defined(POINTING_DEVICE_ROTATION_270)
                scroll.axis = y > 0;
#        else
                scroll.axis = x < 0;
#        endif
            }
        } else if (scroll.state == SCROLL_DETECTING) {
            report.suppress_touch = true;
            // already detecting scroll, check movement from touchdown location
            mag = hypotf(x - scroll.x, y - scroll.y);
            if (mag >= CIRQUE_PINNACLE_SCROLL_MOVEMENT_PERCENTAGE / 100.0 * center) {
                // check ratio of movement towards center vs. along perimeter
                // this distinguishes circular scroll from swipes that start from edge of trackpad
                dot                    = (int32_t)scroll.x * (int32_t)x + (int32_t)scroll.y * (int32_t)y;
                det                    = (int32_t)scroll.x * (int32_t)y - (int32_t)scroll.y * (int32_t)x;
                scalar_projection      = dot / scroll.mag;
                scalar_rejection       = det / scroll.mag;
                parallel_movement      = fabs(scroll.mag - fabs(scalar_projection));
                perpendicular_movement = fabs(scalar_rejection);
                if (parallel_movement * CIRQUE_PINNACLE_SCROLL_MOVEMENT_RATIO > perpendicular_movement) {
                    // not a scroll, release coordinates
                    report.suppress_touch = false;
                    scroll.state          = NOT_SCROLL;
                } else {
                    // scroll detected
                    scroll.state = SCROLL_VALID;
                }
            }
        }
        if (scroll.state == SCROLL_VALID) {
            report.suppress_touch = true;
            dot                   = (int32_t)scroll.x * (int32_t)x + (int32_t)scroll.y * (int32_t)y;
            det                   = (int32_t)scroll.x * (int32_t)y - (int32_t)scroll.y * (int32_t)x;
            ang                   = atan2f(det, dot);
            wheel_clicks          = roundf(ang * CIRQUE_PINNACLE_SCROLL_WHEEL_CLICKS / M_PI);
            if (wheel_clicks >= 1 || wheel_clicks <= -1) {
                if (scroll.axis == 0) {
                    report.v = -wheel_clicks;
                } else {
                    report.h = wheel_clicks;
                }
                scroll.x = x;
                scroll.y = y;
            }
        }
    }

    scroll.z = touchData.zValue;
    if (!scroll.z) scroll.state = SCROLL_UNINITIALIZED;

    return report;
}
#    endif

report_mouse_t cirque_pinnacle_get_report(report_mouse_t mouse_report) {
    pinnacle_data_t touchData;
    static uint16_t x = 0, y = 0;
    int8_t          report_x = 0, report_y = 0;
#    ifdef CIRQUE_PINNACLE_ENABLE_CURSOR_GLIDE
    cursor_glide_t glide = cursor_glide_check();
#    endif
#    ifdef CIRQUE_PINNACLE_ENABLE_CIRCULAR_SCROLL
    circular_scroll_t scroll;
#    endif

#    ifndef POINTING_DEVICE_MOTION_PIN
    if (!cirque_pinnacle_data_ready()) {
#        ifdef CIRQUE_PINNACLE_ENABLE_CURSOR_GLIDE
        if (!glide.valid)
#        endif
            goto exit;
    } else
#    endif
    {
        // Always read data and clear status flags if available
        touchData = cirque_pinnacle_read_data();
        cirque_pinnacle_scale_data(&touchData, cirque_pinnacle_get_scale(), cirque_pinnacle_get_scale()); // Scale coordinates to arbitrary X, Y resolution

#    ifdef CIRQUE_PINNACLE_ENABLE_CIRCULAR_SCROLL
        scroll = circular_scroll(touchData);
        mouse_report.v = scroll.v;
        mouse_report.h = scroll.h;
        if (!scroll.suppress_touch)
#    endif
        {

            if (x && y && touchData.xValue && touchData.yValue) {
                report_x = (int8_t)(touchData.xValue - x);
                report_y = (int8_t)(touchData.yValue - y);
            }
            x = touchData.xValue;
            y = touchData.yValue;
        }

#    ifndef CIRQUE_PINNACLE_DISABLE_TAP
        mouse_report = trackpad_tap(mouse_report, touchData);
#    endif
#    ifdef CIRQUE_PINNACLE_ENABLE_CURSOR_GLIDE
        if (touchData.touchDown) {
            cursor_glide_update(report_x, report_y, touchData.zValue);
        } else if (!glide.valid) {
            glide = cursor_glide_start();
        }
    }

    if (glide.valid) {
        report_x = glide.dx;
        report_y = glide.dy;
#    endif
    }
    mouse_report.x = report_x;
    mouse_report.y = report_y;

exit:
    return mouse_report;
}

// clang-format off
const pointing_device_driver_t pointing_device_driver = {
    .init       = cirque_pinnacle_init,
    .get_report = cirque_pinnacle_get_report,
    .set_cpi    = cirque_pinnacle_set_scale,
    .get_cpi    = cirque_pinnacle_get_scale
};
// clang-format on

#elif defined(POINTING_DEVICE_DRIVER_pimoroni_trackball)
report_mouse_t pimoroni_trackball_get_report(report_mouse_t mouse_report) {
    static uint16_t debounce      = 0;
    static uint8_t  error_count   = 0;
    pimoroni_data_t pimoroni_data = {0};
    static int16_t  x_offset = 0, y_offset = 0;

    if (error_count < PIMORONI_TRACKBALL_ERROR_COUNT) {
        i2c_status_t status = read_pimoroni_trackball(&pimoroni_data);

        if (status == I2C_STATUS_SUCCESS) {
            error_count = 0;

            if (!(pimoroni_data.click & 128)) {
                mouse_report.buttons = pointing_device_handle_buttons(mouse_report.buttons, false, POINTING_DEVICE_BUTTON1);
                if (!debounce) {
                    x_offset += pimoroni_trackball_get_offsets(pimoroni_data.right, pimoroni_data.left, PIMORONI_TRACKBALL_SCALE);
                    y_offset += pimoroni_trackball_get_offsets(pimoroni_data.down, pimoroni_data.up, PIMORONI_TRACKBALL_SCALE);
                    pimoroni_trackball_adapt_values(&mouse_report.x, &x_offset);
                    pimoroni_trackball_adapt_values(&mouse_report.y, &y_offset);
                } else {
                    debounce--;
                }
            } else {
                mouse_report.buttons = pointing_device_handle_buttons(mouse_report.buttons, true, POINTING_DEVICE_BUTTON1);
                debounce             = PIMORONI_TRACKBALL_DEBOUNCE_CYCLES;
            }
        } else {
            error_count++;
        }
    }
    return mouse_report;
}

// clang-format off
const pointing_device_driver_t pointing_device_driver = {
    .init       = pimoroni_trackball_device_init,
    .get_report = pimoroni_trackball_get_report,
    .set_cpi    = pimoroni_trackball_set_cpi,
    .get_cpi    = pimoroni_trackball_get_cpi
};
// clang-format on
#elif defined(POINTING_DEVICE_DRIVER_pmw3360)
static void pmw3360_device_init(void) {
    pmw3360_init();
}

report_mouse_t pmw3360_get_report(report_mouse_t mouse_report) {
    report_pmw3360_t data        = pmw3360_read_burst();
    static uint16_t  MotionStart = 0; // Timer for accel, 0 is resting state

    if (data.isOnSurface && data.isMotion) {
        // Reset timer if stopped moving
        if (!data.isMotion) {
            if (MotionStart != 0) MotionStart = 0;
            return mouse_report;
        }

        // Set timer if new motion
        if ((MotionStart == 0) && data.isMotion) {
#    ifdef CONSOLE_ENABLE
            if (debug_mouse) dprintf("Starting motion.\n");
#    endif
            MotionStart = timer_read();
        }
        mouse_report.x = constrain_hid(data.dx);
        mouse_report.y = constrain_hid(data.dy);
    }

    return mouse_report;
}

// clang-format off
const pointing_device_driver_t pointing_device_driver = {
    .init       = pmw3360_device_init,
    .get_report = pmw3360_get_report,
    .set_cpi    = pmw3360_set_cpi,
    .get_cpi    = pmw3360_get_cpi
};
// clang-format on
#elif defined(POINTING_DEVICE_DRIVER_pmw3389)
static void pmw3389_device_init(void) {
    pmw3389_init();
}

report_mouse_t pmw3389_get_report(report_mouse_t mouse_report) {
    report_pmw3389_t data        = pmw3389_read_burst();
    static uint16_t  MotionStart = 0; // Timer for accel, 0 is resting state

    if (data.isOnSurface && data.isMotion) {
        // Reset timer if stopped moving
        if (!data.isMotion) {
            if (MotionStart != 0) MotionStart = 0;
            return mouse_report;
        }

        // Set timer if new motion
        if ((MotionStart == 0) && data.isMotion) {
#    ifdef CONSOLE_ENABLE
            if (debug_mouse) dprintf("Starting motion.\n");
#    endif
            MotionStart = timer_read();
        }
        mouse_report.x = constrain_hid(data.dx);
        mouse_report.y = constrain_hid(data.dy);
    }

    return mouse_report;
}

// clang-format off
const pointing_device_driver_t pointing_device_driver = {
    .init       = pmw3389_device_init,
    .get_report = pmw3389_get_report,
    .set_cpi    = pmw3389_set_cpi,
    .get_cpi    = pmw3389_get_cpi
};
// clang-format on
#else
__attribute__((weak)) void           pointing_device_driver_init(void) {}
__attribute__((weak)) report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    return mouse_report;
}
__attribute__((weak)) uint16_t pointing_device_driver_get_cpi(void) {
    return 0;
}
__attribute__((weak)) void pointing_device_driver_set_cpi(uint16_t cpi) {}

// clang-format off
const pointing_device_driver_t pointing_device_driver = {
    .init       = pointing_device_driver_init,
    .get_report = pointing_device_driver_get_report,
    .get_cpi    = pointing_device_driver_get_cpi,
    .set_cpi    = pointing_device_driver_set_cpi
};
// clang-format on
#endif
