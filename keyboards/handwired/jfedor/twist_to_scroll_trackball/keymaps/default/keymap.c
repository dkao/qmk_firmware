#include QMK_KEYBOARD_H
#include <math.h>

enum trackball_layers {
    _NORMAL,
    _SCROLL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_NORMAL] = LAYOUT(
        KC_BTN1, KC_BTN2,
        MO(_SCROLL), KC_BTN3
    ),
    [_SCROLL] = LAYOUT(
        XXXXXXX, KC_BTN4,
        _______, KC_BTN5
    ),
};

void setup_sensor0(void) {
    // Try releasing special pins for a short time
    setPinInput(PMW3360_SENSOR1_SCK_PIN);
    setPinInput(PMW3360_SENSOR1_MOSI_PIN);
    setPinInput(PMW3360_SENSOR1_MISO_PIN);

    //chThdSleepMilliseconds(10);
#if defined(USE_GPIOV1)
    palSetPadMode(PAL_PORT(SPI_SCK_PIN), PAL_PAD(SPI_SCK_PIN), SPI_SCK_PAL_MODE);
    palSetPadMode(PAL_PORT(SPI_MOSI_PIN), PAL_PAD(SPI_MOSI_PIN), SPI_MOSI_PAL_MODE);
    palSetPadMode(PAL_PORT(SPI_MISO_PIN), PAL_PAD(SPI_MISO_PIN), SPI_MISO_PAL_MODE);
#else
    palSetPadMode(PAL_PORT(SPI_SCK_PIN), PAL_PAD(SPI_SCK_PIN), SPI_SCK_FLAGS);
    palSetPadMode(PAL_PORT(SPI_MOSI_PIN), PAL_PAD(SPI_MOSI_PIN), SPI_MOSI_FLAGS);
    palSetPadMode(PAL_PORT(SPI_MISO_PIN), PAL_PAD(SPI_MISO_PIN), SPI_MISO_FLAGS);
#endif
}

void setup_sensor1(void) {
    // Try releasing special pins for a short time
    setPinInput(SPI_SCK_PIN);
    setPinInput(SPI_MOSI_PIN);
    setPinInput(SPI_MISO_PIN);

    //chThdSleepMilliseconds(10);
#if defined(USE_GPIOV1)
    palSetPadMode(PAL_PORT(PMW3360_SENSOR1_SCK_PIN), PAL_PAD(PMW3360_SENSOR1_SCK_PIN), SPI_SCK_PAL_MODE);
    palSetPadMode(PAL_PORT(PMW3360_SENSOR1_MOSI_PIN), PAL_PAD(PMW3360_SENSOR1_MOSI_PIN), SPI_MOSI_PAL_MODE);
    palSetPadMode(PAL_PORT(PMW3360_SENSOR1_MISO_PIN), PAL_PAD(PMW3360_SENSOR1_MISO_PIN), SPI_MISO_PAL_MODE);
#else
    palSetPadMode(PAL_PORT(PMW3360_SENSOR1_SCK_PIN), PAL_PAD(PMW3360_SENSOR1_SCK_PIN), SPI_SCK_FLAGS);
    palSetPadMode(PAL_PORT(PMW3360_SENSOR1_MOSI_PIN), PAL_PAD(PMW3360_SENSOR1_MOSI_PIN), SPI_MOSI_FLAGS);
    palSetPadMode(PAL_PORT(PMW3360_SENSOR1_MISO_PIN), PAL_PAD(PMW3360_SENSOR1_MISO_PIN), SPI_MISO_FLAGS);
#endif
}

void pointing_device_init_user(void) {
    pointing_device_set_cpi(400);

#if 0
    setup_sensor1();
    pmw33xx_init(1);
    pmw33xx_set_cpi(1, 400);
    setup_sensor0();
#endif
}

// Modify these values to adjust the scrolling speed
#define SCROLL_DIVISOR_H 40.0
#define SCROLL_DIVISOR_V 40.0

// Variables to store accumulated scroll values
float scroll_accumulated_h = 0;
float scroll_accumulated_v = 0;

typedef struct {
    float sensor0_x;
    float sensor0_y;
    float sensor1_x;
    float sensor1_y;
    bool scrolling;
} twist_scroll_accumulator;
twist_scroll_accumulator accum;

#define TWIST_TO_SCROLL_THRESHOLD 40.0
#define TWIST_TO_SCROLL_SLOPE 2
#define TWIST_TO_SCROLL_BREAKOUT_SLOPE 1

// Function to handle mouse reports and perform drag scrolling
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
#if 0
    pmw33xx_report_t report;
#if 1
    float sensor1_x_abs;
    float sensor0_x_abs;
    float sensor0_y_abs;
#endif
    setup_sensor1();
    report = pmw33xx_read_burst(1);
    setup_sensor0();
#endif

#if 0
    if (report.motion.b.is_lifted) {
        memset(&accum, 0, sizeof(accum));
    }
#endif
    // Check if drag scrolling is active
    //if (set_scrolling) {
    if (IS_LAYER_ON(_SCROLL)) {
        // Calculate and accumulate scroll values based on mouse movement and divisors
        scroll_accumulated_h += (float)mouse_report.x / SCROLL_DIVISOR_H;
        scroll_accumulated_v += (float)mouse_report.y / SCROLL_DIVISOR_V;

        // Assign integer parts of accumulated scroll values to the mouse report
        mouse_report.h = (mouse_xy_report_t)scroll_accumulated_h;
        mouse_report.v = -(mouse_xy_report_t)scroll_accumulated_v;

        // Update accumulated scroll values by subtracting the integer parts
        scroll_accumulated_h -= (mouse_xy_report_t)scroll_accumulated_h;
        scroll_accumulated_v -= (mouse_xy_report_t)scroll_accumulated_v;

        // Clear the X and Y values of the mouse report
        mouse_report.x = 0;
        mouse_report.y = 0;
#if 0
    } else {
        if (report.motion.b.is_motion) {
            accum.sensor1_x += report.delta_x;
#if 0
            if (fabs(report.delta_x) >= (TWIST_TO_SCROLL_SLOPE * fabs(mouse_report.x)) &&
                fabs(report.delta_x) >= (TWIST_TO_SCROLL_SLOPE * fabs(mouse_report.y))) {
                mouse_report.v = (mouse_xy_report_t)(accum.sensor1_x / SCROLL_DIVISOR_V);
                accum.sensor1_x -= (mouse_report.v * SCROLL_DIVISOR_V);
                //accum.sensor1_x = 0;
                accum.scrolling = true;
                mouse_report.v = -mouse_report.v;
            } else {
                accum.scrolling = false;
            }

#else
            accum.sensor0_x += mouse_report.x;
            accum.sensor0_y += mouse_report.y;
            accum.sensor1_y += report.delta_y;
            sensor1_x_abs = fabs(accum.sensor1_x);
            sensor0_x_abs = fabs(accum.sensor0_x);
            sensor0_y_abs = fabs(accum.sensor0_y);
            // breakout condition
            if (sensor0_x_abs > (TWIST_TO_SCROLL_BREAKOUT_SLOPE * sensor1_x_abs) ||
                sensor0_y_abs > (TWIST_TO_SCROLL_BREAKOUT_SLOPE * sensor1_x_abs)) {
                memset(&accum, 0, sizeof(accum));
            }
            sensor1_x_abs = fabs(accum.sensor1_x);
            sensor0_x_abs = fabs(accum.sensor0_x);
            sensor0_y_abs = fabs(accum.sensor0_y);
            //if (accum.scrolling) {
            //    mouse_report.v = (mouse_xy_report_t)(accum.sensor1_x / SCROLL_DIVISOR_V);
            //    // need some way to clear remainders to use this, maybe timeout?
            //    accum.sensor1_x -= (mouse_report.v * SCROLL_DIVISOR_V);
            //    //accum.sensor1_x = 0;
            //    accum.scrolling = true;
            //    mouse_report.v = -mouse_report.v;
            //    accum.sensor0_x = 0;
            //    accum.sensor0_y = 0;
            //    accum.sensor1_y = 0;
            //} else
            if (sensor1_x_abs >= TWIST_TO_SCROLL_THRESHOLD) {
                if (accum.scrolling ||
                    (sensor1_x_abs >= (TWIST_TO_SCROLL_SLOPE * sensor0_x_abs) &&
                     sensor1_x_abs >= (TWIST_TO_SCROLL_SLOPE * sensor0_y_abs) &&
                     sensor1_x_abs >= (TWIST_TO_SCROLL_SLOPE * fabs(accum.sensor1_y)))) {
                    // now we're scrolling
                    mouse_report.v = (mouse_xy_report_t)(accum.sensor1_x / SCROLL_DIVISOR_V);
                    // need some way to clear remainders to use this, maybe timeout?
                    //accum.sensor1_x -= (mouse_report.v * SCROLL_DIVISOR_V);
                    accum.sensor1_x = 0;
                    accum.scrolling = true;
                    mouse_report.v = -mouse_report.v;
                } else {
                    accum.sensor1_x = 0;
                    //accum.scrolling = false;
                }
                //memset(&accum, 0, sizeof(accum));
                accum.sensor0_x = 0;
                accum.sensor0_y = 0;
                accum.sensor1_y = 0;
            }
#endif
        }

        if (accum.scrolling) {
            mouse_report.x = 0;
            mouse_report.y = 0;
        }
#endif
    }
    return mouse_report;
}
