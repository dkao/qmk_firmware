#include QMK_KEYBOARD_H

enum layer_names {
    _DEFAULT,
    _SYMBOL,
    _NAVIGATION,
    _KEYPAD,
    _SHMUP,
    _LAYERS,
};

#define DEFAULT TO(_DEFAULT)
#define KEYPAD  TG(_KEYPAD)
#define LAYERS  MO(_LAYERS)
#define SHMUP   TO(_SHMUP)
#define HCTLESC CTL_T(KC_ESC)
#define CTL_ESC LCTL_T(KC_ESC)
#define GUI_BSP LGUI_T(KC_BSPC)
#define GUI_QUO LGUI_T(KC_QUOT)
#define ALT_BSL RALT_T(KC_BSLS)
#define SHF_BSP LSFT_T(KC_BSPC)
#define SHF_ENT RSFT_T(KC_ENT)
#define MO_NAV MO(_NAVIGATION)
#define MO_SYM MO(_SYMBOL)
#define MO_FUN MO(_FUNCTION)


/****************************************************************************************************
*
* Keymap: Default Layer in Colemak-DH
*
* ,-------------------------------------------------------------------------------------------------------------------.
* | Esc    |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F8  |  F9  |  F10 |  F12 | PSCR | SLCK | PAUS |  FN0 |  BOOT  |
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* | =+     |  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  | -_     |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* | Tab    |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  | \|     |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | Caps   |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |   K  |   L  |  ;:  | '"     |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | Shift  |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  ,.  |  .>  |  /?  | Shift  |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          | `~   | INS  | Left | Right|                                         | Up   | Down |  [{  |  ]}  |
*          `---------------------------'                                         `---------------------------'
*                                        ,-------------.         ,-------------.
*                                        | Ctrl | Alt  |         | Gui  | Ctrl |
*                                 ,------|------|------|         |------+------+------.
*                                 |      |      | Home |         | PgUp |      |      |
*                                 | BkSp | Del  |------|         |------|Return| Space|
*                                 |      |      | End  |         | PgDn |      |      |
*                                 `--------------------'         `--------------------'
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_DEFAULT] = LAYOUT_pretty(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,         KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_SCRL,  KC_PAUS,  KEYPAD,   QK_BOOT,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,                                                                      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_INS,
    KC_TAB,   KC_Q,     KC_W,     KC_F,     KC_P,     KC_B,                                                                      KC_J,     KC_L,     KC_U,     KC_Y,     KC_SCLN,  CW_TOGG,
    CTL_ESC,  KC_A,     KC_R,     KC_S,     KC_T,     KC_G,                                                                      KC_M,     KC_N,     KC_E,     KC_I,     KC_O,     GUI_QUO,
    KC_LALT,  KC_Z,     KC_X,     KC_C,     KC_D,     KC_V,                                                                      KC_K,     KC_H,     KC_COMM,  KC_DOT,   KC_SLSH,  ALT_BSL,
              KC_EQL,   KC_MINS,  KC_LBRC,  KC_RBRC,                                                                                       KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,
                                                      KC_HOME,  KC_END,                                                KC_PGUP,  KC_PGDN,
                                                                KC_TAB,                                                KC_DEL,
                                            KC_BSPC,  KC_LSFT,  MO_NAV,                                                MO_SYM,   SHF_ENT,  KC_SPC
  ),
  [_SYMBOL] = LAYOUT_pretty(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,       _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  KC_UNDS,  KC_MINS,  _______,                                                                   _______,  KC_PLUS,  KC_EQL,   _______,  _______,  _______,
    _______,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,                                                                      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     _______,
    _______,  KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,                                                                   KC_CIRC,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN, _______,
              _______,  _______,  _______,  _______,                                                                                       _______,  _______,  _______,  _______,
                                                      _______,  _______,                                               _______,  _______,
                                                                _______,                                               _______,
                                            _______,  _______,  LAYERS,                                                _______,  _______,  _______
  ),
  [_NAVIGATION] = LAYOUT_pretty(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,       _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  KC_PSCR,  KC_SCRL,  KC_PAUS,  _______,  _______,
    _______,  KC_VOLU,  KC_MPRV,  _______,  _______,  _______,                                                                   KC_INS,   KC_HOME,  KC_UP,    KC_END,   KC_PGUP,  KC_BTN4,
    _______,  KC_VOLD,  KC_MPLY,  KC_F24,   CW_TOGG,  _______,                                                                   KC_DEL,   KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_PGDN,  KC_BTN5,
    _______,  KC_MUTE,  KC_MNXT,  _______,  _______,  _______,                                                                   KC_ESC,   KC_LBRC,  KC_RBRC,  KC_LCBR,  KC_RCBR,  KC_APP,
              _______,  _______,  _______,  _______,                                                                                       _______,  _______,  _______,  _______,
                                                      _______,  _______,                                               _______,  _______,
                                                                _______,                                               _______,
                                            _______,  _______,  _______,                                               LAYERS,   _______,  _______
  ),
  [_KEYPAD] = LAYOUT_pretty(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,       _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  KC_P7,    KC_P8,    KC_P9,    KC_PPLS,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  KC_P4,    KC_P5,    KC_P6,    KC_PCMM,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  KC_P1,    KC_P2,    KC_P3,    KC_PEQL,  _______,
              _______,  _______,  _______,  _______,                                                                                       KC_LPRN,  KC_RPRN,  KC_PDOT,  KC_PENT,
                                                      _______,  _______,                                               _______,  _______,
                                                                _______,                                               _______,
                                            _______,  _______,  _______,                                               _______,  KC_PENT,  KC_P0
  ),
  [_SHMUP] = LAYOUT_pretty(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,       _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  KC_UP,    _______,  _______,                                                                   KC_F4,    KC_TAB,   KC_LCTL,  KC_LALT,  KC_F11,   _______,
    KC_ESC,   _______,  KC_LEFT,  KC_DOWN,  KC_RGHT,  _______,                                                                   _______,  KC_Z,     KC_X,     KC_C,     KC_V,     _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  KC_A,     KC_S,     KC_D,     KC_F,     _______,
              _______,  _______,  _______,  _______,                                                                                       KC_Q,     KC_W,     KC_E,     KC_R,
                                                      _______,  DEFAULT,                                               DEFAULT,  _______,
                                                                _______,                                               _______,
                                            KC_UP,    KC_SPC,   _______,                                               _______,  KC_ENT,   KC_LSFT
  ),
  [_LAYERS] = LAYOUT_pretty(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,       _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                                                                   _______,  _______,  _______,  _______,  _______,  _______,
              DEFAULT,  _______,  _______,  _______,                                                                                       _______,  _______,  _______,  SHMUP,
                                                      _______,  _______,                                               _______,  _______,
                                                                _______,                                               _______,
                                            _______,  _______,  _______,                                               _______,  _______,  _______
  )

};

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        //case LT(1, KC_BSPC):
	    //case SFT_ESC:
	    //case SFT_ENT:
        case SHF_ENT:
        case CTL_ESC:
            // Immediately select the hold action when another key is pressed.
            return true;
        default:
            // Do not select the hold action when another key is pressed.
            return false;
    }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CTL_ESC:
            return 0;
        default:
            return QUICK_TAP_TERM;
    }
}

#define SOCD_CLEANER_LRN_UDN
#if defined(SOCD_CLEANER_LRN_UDU) || defined(SOCD_CLEANER_LRN_UDN) || defined(SOCD_CLEANER_LAST_INPUT_WINS)
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
#if defined(SOCD_CLEANER_LRN_UDU) || defined(SOCD_CLEANER_LRN_UDN)
	/*
	 * Left + Right = Neutral
	 * Up + Down = Up
	 */
	if (keycode == key_up) {
		if (record->event.pressed) {
			pressed_up |= layer_mask;
			if (pressed_down & layer_mask) {
				unregister_code(key_down);
#ifdef SOCD_CLEANER_LRN_UDN
				return false;
#else
				return true;
#endif
			}
		} else {
			pressed_up &= (~layer_mask);
			if (pressed_down & layer_mask) {
				register_code(key_down);
#ifdef SOCD_CLEANER_LRN_UDN
				return false;
#else
				return true;
#endif
			}
		}
	} else if (keycode == key_down) {
		if (record->event.pressed) {
			pressed_down |= layer_mask;
			if (pressed_up & layer_mask) {
#ifdef SOCD_CLEANER_LRN_UDN
				unregister_code(key_up);
#endif
				return false;
			}
		} else {
			pressed_down &= (~layer_mask);
			if (pressed_up & layer_mask) {
#ifdef SOCD_CLEANER_LRN_UDN
				register_code(key_up);
#endif
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

#if 1
#ifndef DEBOUNCE
#    define DEBOUNCE 5
#endif

// Maximum debounce: 255ms
#if DEBOUNCE > UINT8_MAX
#    undef DEBOUNCE
#    define DEBOUNCE UINT8_MAX
#endif

#define ROW_SHIFTER ((matrix_row_t)1)

typedef uint8_t debounce_counter_t;

static debounce_counter_t *debounce_counters;
static fast_timer_t        last_time;
static bool                counters_need_update;
static bool                matrix_need_update;
static bool                cooked_changed;
static bool                last_eager;

#    define DEBOUNCE_ELAPSED 0

static void update_debounce_counters(uint8_t num_rows, uint8_t elapsed_time);
static void transfer_matrix_values(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows);
static void update_debounce_counters_and_transfer_if_expired(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, uint8_t elapsed_time);
static void start_debounce_counters(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows);

// we use num_rows rather than MATRIX_ROWS to support split keyboards
void debounce_init(uint8_t num_rows) {
    debounce_counters = (debounce_counter_t *)malloc(num_rows * MATRIX_COLS * sizeof(debounce_counter_t));
    int i             = 0;
    for (uint8_t r = 0; r < num_rows; r++) {
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            debounce_counters[i++] = DEBOUNCE_ELAPSED;
        }
    }
}

void debounce_free(void) {
    free(debounce_counters);
    debounce_counters = NULL;
}

bool debounce(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, bool changed) {
    bool updated_last = false;
    //bool use_eager = (IS_LAYER_ON(_SHMUP) || IS_LAYER_ON(_FPS1) || IS_LAYER_ON(_FPS2));
    bool use_eager = IS_LAYER_ON(_SHMUP);
    cooked_changed    = false;

    if (use_eager != last_eager) {
        counters_need_update = false;
        matrix_need_update = true;
        int i             = 0;
        for (uint8_t r = 0; r < num_rows; r++) {
            for (uint8_t c = 0; c < MATRIX_COLS; c++) {
                debounce_counters[i++] = DEBOUNCE_ELAPSED;
            }
        }
        last_eager = use_eager;
    }

    if (counters_need_update) {
        fast_timer_t now          = timer_read_fast();
        fast_timer_t elapsed_time = TIMER_DIFF_FAST(now, last_time);

        last_time    = now;
        updated_last = true;
        if (elapsed_time > UINT8_MAX) {
            elapsed_time = UINT8_MAX;
        }

        if (elapsed_time > 0) {
            if (use_eager) {
                update_debounce_counters(num_rows, elapsed_time);
            } else {
                update_debounce_counters_and_transfer_if_expired(raw, cooked, num_rows, elapsed_time);
            }
        }
    }

    if (changed || matrix_need_update) {
        if (!updated_last) {
            last_time = timer_read_fast();
        }

        if (use_eager) {
            transfer_matrix_values(raw, cooked, num_rows);
        } else {
            start_debounce_counters(raw, cooked, num_rows);
        }
    }

    return cooked_changed;
}

// If the current time is > debounce counter, set the counter to enable input.
static void update_debounce_counters(uint8_t num_rows, uint8_t elapsed_time) {
    counters_need_update                 = false;
    matrix_need_update                   = false;
    debounce_counter_t *debounce_pointer = debounce_counters;
    for (uint8_t row = 0; row < num_rows; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            if (*debounce_pointer != DEBOUNCE_ELAPSED) {
                if (*debounce_pointer <= elapsed_time) {
                    *debounce_pointer  = DEBOUNCE_ELAPSED;
                    matrix_need_update = true;
                } else {
                    *debounce_pointer -= elapsed_time;
                    counters_need_update = true;
                }
            }
            debounce_pointer++;
        }
    }
}

// upload from raw_matrix to final matrix;
static void transfer_matrix_values(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows) {
    matrix_need_update                   = false;
    debounce_counter_t *debounce_pointer = debounce_counters;
    for (uint8_t row = 0; row < num_rows; row++) {
        matrix_row_t delta        = raw[row] ^ cooked[row];
        matrix_row_t existing_row = cooked[row];
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            matrix_row_t col_mask = (ROW_SHIFTER << col);
            if (delta & col_mask) {
                if (*debounce_pointer == DEBOUNCE_ELAPSED) {
                    *debounce_pointer    = DEBOUNCE;
                    counters_need_update = true;
                    existing_row ^= col_mask; // flip the bit.
                    cooked_changed = true;
                }
            }
            debounce_pointer++;
        }
        cooked[row] = existing_row;
    }
}

static void update_debounce_counters_and_transfer_if_expired(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, uint8_t elapsed_time) {
    counters_need_update                 = false;
    debounce_counter_t *debounce_pointer = debounce_counters;
    for (uint8_t row = 0; row < num_rows; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            if (*debounce_pointer != DEBOUNCE_ELAPSED) {
                if (*debounce_pointer <= elapsed_time) {
                    *debounce_pointer        = DEBOUNCE_ELAPSED;
                    matrix_row_t cooked_next = (cooked[row] & ~(ROW_SHIFTER << col)) | (raw[row] & (ROW_SHIFTER << col));
                    cooked_changed |= cooked[row] ^ cooked_next;
                    cooked[row] = cooked_next;
                } else {
                    *debounce_pointer -= elapsed_time;
                    counters_need_update = true;
                }
            }
            debounce_pointer++;
        }
    }
}

static void start_debounce_counters(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows) {
    debounce_counter_t *debounce_pointer = debounce_counters;
    for (uint8_t row = 0; row < num_rows; row++) {
        matrix_row_t delta = raw[row] ^ cooked[row];
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            if (delta & (ROW_SHIFTER << col)) {
                if (*debounce_pointer == DEBOUNCE_ELAPSED) {
                    *debounce_pointer    = DEBOUNCE;
                    counters_need_update = true;
                }
            } else {
                *debounce_pointer = DEBOUNCE_ELAPSED;
            }
            debounce_pointer++;
        }
    }
}
#endif
