#include QMK_KEYBOARD_H

enum trackball_layers {
    _NORMAL,
    _SCROLL,
    _GAME
};

// Define a type for as many tap dance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
td_state_t cur_dance(tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(tap_dance_state_t *state, void *user_data);
void ql_reset(tap_dance_state_t *state, void *user_data);

// Determine the current tap dance state
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) return TD_DOUBLE_TAP;
    else return TD_UNKNOWN;
}

// Initialize tap structure associated with example tap dance key
static td_tap_t ql_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Functions that control what our tap dance key does
void ql_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            if (layer_state_is(_SCROLL)) {
                // If already set, then switch it off
                layer_off(_SCROLL);
            } else {
                // If not already set, then switch the layer on
                layer_on(_SCROLL);
            }
            break;
        case TD_SINGLE_HOLD:
            tap_code(KC_BTN3);
            break;
        case TD_DOUBLE_TAP:
            // Check to see if the layer is already set
            if (layer_state_is(_GAME)) {
                // If already set, then switch it off
                layer_off(_GAME);
            } else {
                // If not already set, then switch the layer on
                layer_on(_GAME);
            }
            break;
        default:
            break;
    }
}

void ql_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    ql_tap_state.state = TD_NONE;
}
// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    //[0] = ACTION_TAP_DANCE_LAYER_TOGGLE(TG(_SCROLL), _GAME),
    [0] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset),
    [1] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_BTN2, _GAME),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    //[_NORMAL] = LAYOUT(
    //    KC_BTN1, TG(_SCROLL)
    //),
    //[_SCROLL] = LAYOUT(
    //    _______, _______
    //),
    [_NORMAL] = LAYOUT(
        KC_BTN1, TD(0)
    ),
    [_SCROLL] = LAYOUT(
        _______, _______
    ),
    [_GAME] = LAYOUT(
        KC_BTN1, TD(1)
    ),
};

void pointing_device_init_user(void) {
    pointing_device_set_cpi(400);
}

// Modify these values to adjust the scrolling speed
#define SCROLL_DIVISOR_H 40.0
#define SCROLL_DIVISOR_V 40.0

// Variables to store accumulated scroll values
float scroll_accumulated_h = 0;
float scroll_accumulated_v = 0;

// Function to handle mouse reports and perform drag scrolling
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
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
    }
    return mouse_report;
}
