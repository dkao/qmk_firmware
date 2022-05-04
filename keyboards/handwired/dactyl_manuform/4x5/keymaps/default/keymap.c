#include QMK_KEYBOARD_H

#define _BASE 0
#if 1
#define _QWERTY 1
#define _ENGRAM 2
#define _RAISE 3
#define _LOWER 4
#define _UTILITY 5
#define _FPS1 7
#define _FPS2 8
#define _SHMUP 6
#elif 0
#define _QWERTY 1
#define _RAISE 2
#define _LOWER 3
#define _UTILITY 4
#define _FPS1 5
#define _FPS2 6
#define _SHMUP 7
#else
#define _RAISE 1
#define _LOWER 2
#define _UTILITY 3
#define _FPS1 4
#define _FPS2 5
#define _SHMUP 6
#endif

#define SFT_ESC  SFT_T(KC_ESC)
#define CTL_BSPC CTL_T(KC_BSPC)
#define ALT_SPC  ALT_T(KC_SPC)
#define SFT_ENT  SFT_T(KC_ENT)

#define KC_ML KC_MS_LEFT
#define KC_MR KC_MS_RIGHT
#define KC_MU KC_MS_UP
#define KC_MD KC_MS_DOWN
#define KC_MB1 KC_MS_BTN1
#define KC_MB2 KC_MS_BTN2
#define XXXXXXX KC_NO

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)
#define UTILITY MO(_UTILITY)
#define FPS1 TO(_FPS1)
#define FPS2 MO(_FPS2)
#define SHMUP TO(_SHMUP)
#define RETB TO(_BASE)
#if 1
#define QWERTY TO(_QWERTY)
#define ENGRAM TO(_ENGRAM)
#elif 0
#define QWERTY TO(_QWERTY)
#define ENGRAM XXXXXXX
#else
#define QWERTY XXXXXXX
#define ENGRAM XXXXXXX
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base (qwerty)
     * ,----------------------------------,                             ,----------------------------------,
     * |   q  |   w  |   e  |   r  |   t  |                             |   y  |   u  |   i  |   o  |   p  |
     * |------+------+------+------+------|                             |-------------+------+------+------|
     * |   a  |   s  |   d  |   f  |   g  |                             |   h  |   j  |   k  |   l  |   ;  |
     * |------+------+------+------+------|                             |------|------+------+------+------|
     * |   z  |   x  |   c  |   v  |   b  |                             |   n  |   m  |   ,  |   .  |   '  |
     * |------+------+------+-------------,                             ,-------------+------+------+------,
     *        |  [   |   ]  |                                                         |   -  |   =  |
     *        '------+------'-------------'                             '-------------'------+------'
     *                      | ESC  |  BS  |                             | SPACE|ENTER |
     *                      |  +   |   +  |                             |  +   |  +   |
     *                      | SHIFT| CTRL |                             | ALT  |SHIFT |
     *                      '------+------'                             '------+------'
     *                                    '------+------' '------+------'
     *                                    | TAB  | HOME | | END  | DEL  |
     *                                    '------+------' '------+------'
     *                                    | Raise|  ~   | | GUI  | Lower|
     *                                    '------+------' '------+------'
     */
#if 0
    [_BASE] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_QUOT,
                          KC_LBRC, KC_RBRC,                                                        KC_MINS, KC_EQL,
                                                     SFT_ESC, CTL_BSPC, ALT_SPC, SFT_ENT,
                                                     KC_TAB,  KC_HOME,  KC_END,  KC_DEL,
                                                     RAISE,   KC_GRV,   KC_LGUI, LOWER
    ),
#elif 1
    [_BASE] = LAYOUT(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                                         KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN,
        KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                                         KC_M,    KC_N,    KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                                         KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_QUOT,
                          KC_LBRC, KC_RBRC,                                                        KC_MINS, KC_EQL,
                                                     SFT_ESC, CTL_BSPC, ALT_SPC, SFT_ENT,
                                                     KC_TAB,  KC_HOME,  KC_END,  KC_DEL,
                                                     RAISE,   KC_GRV,   KC_LGUI, LOWER
    ),
#else
	/* Colemak-Qi */
    [_BASE] = LAYOUT(
        KC_Q,    KC_L,    KC_W,    KC_M,    KC_K,                                         KC_J,    KC_F,    KC_U,    KC_Y,    KC_SCLN,
        KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                                         KC_P,    KC_N,    KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                                         KC_B,    KC_H,    KC_COMM, KC_DOT,  KC_QUOT,
                          KC_LBRC, KC_RBRC,                                                        KC_MINS, KC_EQL,
                                                     SFT_ESC, CTL_BSPC, ALT_SPC, SFT_ENT,
                                                     KC_TAB,  KC_HOME,  KC_END,  KC_DEL,
                                                     RAISE,   KC_GRV,   KC_LGUI, LOWER
    ),
#endif

#if 1
    /* QWERTY (for bopomofo)
     * ,----------------------------------,                             ,----------------------------------,
     * |   q  |   w  |   e  |   r  |   t  |                             |   y  |   u  |   i  |   o  |   p  |
     * |------+------+------+------+------|                             |-------------+------+------+------|
     * |   a  |   s  |   d  |   f  |   g  |                             |   h  |   j  |   k  |   l  |   ;  |
     * |------+------+------+------+------|                             |------|------+------+------+------|
     * |   z  |   x  |   c  |   v  |   b  |                             |   n  |   m  |   ,  |   .  |   /  |
     * |------+------+------+-------------,                             ,-------------+------+------+------,
     *        |  [   |   ]  |                                                         |   -  |   =  |
     *        '------+------'-------------'                             '-------------'------+------'
     *                      | ESC  |  BS  |                             | SPACE|ENTER |
     *                      |  +   |   +  |                             |  +   |  +   |
     *                      | SHIFT| CTRL |                             | ALT  |SHIFT |
     *                      '------+------'                             '------+------'
     *                                    '------+------' '------+------'
     *                                    | TAB  | HOME | | END  | DEL  |
     *                                    '------+------' '------+------'
     *                                    | Raise|  ~   | | GUI  | Lower|
     *                                    '------+------' '------+------'
     */
#if 0
    [_QWERTY] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_LBRC, KC_RBRC,                                                        KC_MINS, KC_EQL,
                                                     SFT_ESC, CTL_BSPC, ALT_SPC, SFT_ENT,
                                                     KC_TAB,  KC_HOME,  KC_END,  KC_DEL,
                                                     RAISE,   KC_GRV,   KC_LGUI, LOWER
    ),
#else
	/* APTv3 */
    [_QWERTY] = LAYOUT(
        KC_W,    KC_G,    KC_D,    KC_F,    KC_B,                                         KC_Q,    KC_L,    KC_U,    KC_O,    KC_Y,
        KC_R,    KC_S,    KC_T,    KC_H,    KC_K,                                         KC_J,    KC_N,    KC_E,    KC_A,    KC_I,
        KC_X,    KC_C,    KC_M,    KC_P,    KC_V,                                         KC_Z,    KC_COMM, KC_DOT,  KC_QUOT, KC_SCLN,
                          KC_LBRC, KC_RBRC,                                                        KC_MINS, KC_EQL,
                                                     SFT_ESC, CTL_BSPC, ALT_SPC, SFT_ENT,
                                                     KC_TAB,  KC_HOME,  KC_END,  KC_DEL,
                                                     RAISE,   KC_GRV,   KC_LGUI, LOWER
    ),
#endif
#endif

#if 1
    /* engram
     * ,----------------------------------,                             ,----------------------------------,
     * |   q  |   w  |   e  |   r  |   t  |                             |   y  |   u  |   i  |   o  |   p  |
     * |------+------+------+------+------|                             |-------------+------+------+------|
     * |   a  |   s  |   d  |   f  |   g  |                             |   h  |   j  |   k  |   l  |   ;  |
     * |------+------+------+------+------|                             |------|------+------+------+------|
     * |   z  |   x  |   c  |   v  |   b  |                             |   n  |   m  |   ,  |   .  |   /  |
     * |------+------+------+-------------,                             ,-------------+------+------+------,
     *        |  [   |   ]  |                                                         |   -  |   =  |
     *        '------+------'-------------'                             '-------------'------+------'
     *                      | ESC  |  BS  |                             | SPACE|ENTER |
     *                      |  +   |   +  |                             |  +   |  +   |
     *                      | SHIFT| CTRL |                             | ALT  |SHIFT |
     *                      '------+------'                             '------+------'
     *                                    '------+------' '------+------'
     *                                    | TAB  | HOME | | END  | DEL  |
     *                                    '------+------' '------+------'
     *                                    | Raise|  ~   | | GUI  | Lower|
     *                                    '------+------' '------+------'
     */
#if 0
    [_ENGRAM] = LAYOUT(
        KC_B,    KC_Y,    KC_O,    KC_U,    KC_QUOT,                                      KC_SCLN, KC_L,    KC_D,    KC_W,    KC_V,
        KC_C,    KC_I,    KC_E,    KC_A,    KC_COMM,                                      KC_DOT,  KC_H,    KC_T,    KC_S,    KC_N,
        KC_G,    KC_X,    KC_J,    KC_K,    KC_MINS,                                      KC_EQL,  KC_R,    KC_M,    KC_F,    KC_P,
                          KC_LBRC, KC_RBRC,                                                        KC_Z,    KC_Q,
                                                     SFT_ESC, CTL_BSPC, ALT_SPC, SFT_ENT,
                                                     KC_TAB,  KC_HOME,  KC_END,  KC_DEL,
                                                     RAISE,   KC_GRV,   KC_LGUI, LOWER
    ),
#elif 1
    /* flipping it per engram#42 */
    [_ENGRAM] = LAYOUT(
        KC_V,    KC_W,    KC_D,    KC_L,    KC_SCLN,                                      KC_QUOT, KC_U,    KC_O,    KC_Y,    KC_B,
        KC_N,    KC_S,    KC_T,    KC_H,    KC_COMM,                                      KC_DOT,  KC_A,    KC_E,    KC_I,    KC_C,
        KC_P,    KC_F,    KC_M,    KC_R,    KC_MINS,                                      KC_EQL,  KC_K,    KC_J,    KC_X,    KC_G,
                          KC_Z,    KC_Q,                                                           KC_LBRC, KC_RBRC,
                                                     SFT_ESC, CTL_BSPC, ALT_SPC, SFT_ENT,
                                                     KC_TAB,  KC_HOME,  KC_END,  KC_DEL,
                                                     RAISE,   KC_GRV,   KC_LGUI, LOWER
    ),
#else
	/* CTGAP 3.0 */
    [_ENGRAM] = LAYOUT(
        KC_V,    KC_P,    KC_L,    KC_C,    KC_F,                                         KC_K,    KC_U,    KC_O,    KC_Y,    KC_J,
        KC_R,    KC_N,    KC_T,    KC_S,    KC_D,                                         KC_QUOT, KC_A,    KC_E,    KC_I,    KC_H,
        KC_Z,    KC_B,    KC_M,    KC_G,    KC_W,                                         KC_X,    KC_COMM, KC_DOT,  KC_SCLN, KC_Q,
                          KC_Z,    KC_Q,                                                           KC_LBRC, KC_RBRC,
                                                     SFT_ESC, CTL_BSPC, ALT_SPC, SFT_ENT,
                                                     KC_TAB,  KC_HOME,  KC_END,  KC_DEL,
                                                     RAISE,   KC_GRV,   KC_LGUI, LOWER
    ),
#endif
#endif

    /* Raise
     * ,----------------------------------,                             ,----------------------------------,
     * |      |      |  mup |      |      |                             | VOL+ |      |  up  |      | PgUp |
     * |------+------+------+------+------|                             |-------------+------+------+------|
     * |      | mleft| mdown|mright|      |                             | MUTE | left | down |right | PgDn |
     * |------+------+------+------+------|                             |------|------+------+------+------|
     * |      |      |      |      |      |                             | VOL- |  /   |  \   |  ?   |  |   |
     * |------+------+------+-------------,                             ,-------------+------+------+------,
     *        |      |      |                                                         | mbtn |mbtn2 |
     *        '------+------'-------------'                             '-------------'------+------'
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      '------+------'                             '------+------'
     *                                    '------+------' '------+------'
     *                                    |      | Util | |      |      |
     *                                    '------+------' '------+------'
     *                                    |      |      | |      |      |
     *                                    '------+------' '------+------'
     */
    [_RAISE] = LAYOUT(
        _______, _______, KC_MU,   _______, _______,                                      KC_VOLU, _______, KC_UP,   _______, KC_PGUP,
        _______, KC_ML,   KC_MD,   KC_MR,   _______,                                      KC_MUTE, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,
        _______, _______, _______, _______, _______,                                      KC_VOLD, KC_SLSH, KC_BSLS, KC_QUES, KC_PIPE,
                 _______, _______,                                                                          KC_MB1,  KC_MB2,
                                   _______, _______,                                      _______, _______,
                                                     _______, UTILITY,  _______, _______,
                                                     _______, _______,  _______, _______
    ),

    /* Lower
     * ,----------------------------------,                             ,----------------------------------,
     * | F1   | F2   | F3   | F4   | F5   |                             |  F6  | F7   |  F8  |  F9  |  F10 |
     * |------+------+------+------+------|                             |-------------+------+------+------|
     * |  1   |  2   |  3   |  4   |  5   |                             |  6   |  7   |  8   |  9   |  0   |
     * |------+------+------+------+------|                             |------|------+------+------+------|
     * |  !   |  @   |  #   |  $   |  %   |                             |  ^   |  &   |  *   |  (   |  )   |
     * |------+------+------+-------------,                             ,-------------+------+------+------,
     *        | F11  | F12  |                                                         |  -   |  =   |
     *        '------+------'-------------'                             '-------------'------+------'
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      '------+------'                             '------+------'
     *                                    '------+------' '------+------'
     *                                    |      |      | |      | Util |
     *                                    '------+------' '------+------'
     *                                    |      |      | |      |      |
     *                                    '------+------' '------+------'
     */
    [_LOWER] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
                 KC_F11,  KC_F12,                                                                           _______, _______,
                                   _______, _______,                                      _______, _______,
                                                     _______, _______,  UTILITY, _______,
                                                     _______, _______,  _______, _______
    ),

    /* Utility
     * ,----------------------------------,                             ,----------------------------------,
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+------+------|                             |-------------+------+------+------|
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+------+------|                             |------|------+------+------+------|
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+-------------,                             ,-------------+------+------+------,
     *        | RESET|      |                                                         |      | RESET|
     *        '------+------'-------------'                             '-------------'------+------'
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      '------+------'                             '------+------'
     *                                    '------+------' '------+------'
     *                                    |      |      | |      |      |
     *                                    '------+------' '------+------'
     *                                    |      |      | |      |      |
     *                                    '------+------' '------+------'
     */
    [_UTILITY] = LAYOUT(
        QWERTY,  XXXXXXX, XXXXXXX, XXXXXXX, FPS1,                                         SHMUP,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, ENGRAM,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RETB,                                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                 RESET,   XXXXXXX,                                                                          XXXXXXX, RESET,
                                   XXXXXXX, XXXXXXX,                                      XXXXXXX, XXXXXXX,
                                                     XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,
                                                     XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX
    ),

    /* FPS 1
     * ,----------------------------------,                             ,----------------------------------,
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+------+------|                             |-------------+------+------+------|
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+------+------|                             |------|------+------+------+------|
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+-------------,                             ,-------------+------+------+------,
     *        | RESET|      |                                                         |      | RESET|
     *        '------+------'-------------'                             '-------------'------+------'
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      '------+------'                             '------+------'
     *                                    '------+------' '------+------'
     *                                    |      |      | |      |      |
     *                                    '------+------' '------+------'
     *                                    |      |      | |      |      |
     *                                    '------+------' '------+------'
     */
    [_FPS1] = LAYOUT(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,                                         KC_HOME, KC_INS,  KC_UP,   KC_DEL,  KC_PGUP,
        KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,                                         KC_END,  KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,
        FPS2,    KC_Z,    KC_X,    KC_C,    KC_V,                                         KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,
                 KC_LCTL, KC_LALT,                                                                          KC_F6,   KC_F7,
                                   KC_ESC,  KC_SPC,                                       KC_F8,   KC_ENT,
                                                     KC_1,    KC_3,     KC_F11,  KC_F9,
                                                     KC_2,    KC_4,     KC_F12,  KC_F10
    ),

    /* FPS 2
     * ,----------------------------------,                             ,----------------------------------,
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+------+------|                             |-------------+------+------+------|
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+------+------|                             |------|------+------+------+------|
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+-------------,                             ,-------------+------+------+------,
     *        | RESET|      |                                                         |      | RESET|
     *        '------+------'-------------'                             '-------------'------+------'
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      '------+------'                             '------+------'
     *                                    '------+------' '------+------'
     *                                    |      |      | |      |      |
     *                                    '------+------' '------+------'
     *                                    |      |      | |      |      |
     *                                    '------+------' '------+------'
     */
    [_FPS2] = LAYOUT(
        KC_TAB,  KC_Q,    KC_W,    KC_T,    KC_Y,                                         RETB,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_LSFT, KC_A,    KC_S,    KC_D,    KC_G,                                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_TRNS, KC_Z,    KC_X,    KC_C,    KC_B,                                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                 KC_LCTL, KC_LALT,                                                                          XXXXXXX, XXXXXXX,
                                   KC_9,    KC_0,                                         XXXXXXX, XXXXXXX,
                                                     KC_5,    KC_7,     XXXXXXX, XXXXXXX,
                                                     KC_6,    KC_8,     XXXXXXX, XXXXXXX
    ),

    /* Shmup
     * ,----------------------------------,                             ,----------------------------------,
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+------+------|                             |-------------+------+------+------|
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+------+------|                             |------|------+------+------+------|
     * |      |      |      |      |      |                             |      |      |      |      |      |
     * |------+------+------+-------------,                             ,-------------+------+------+------,
     *        | RESET|      |                                                         |      | RESET|
     *        '------+------'-------------'                             '-------------'------+------'
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      |      |      |                             |      |      |
     *                      '------+------'                             '------+------'
     *                                    '------+------' '------+------'
     *                                    |      |      | |      |      |
     *                                    '------+------' '------+------'
     *                                    |      |      | |      |      |
     *                                    '------+------' '------+------'
     */
    [_SHMUP] = LAYOUT(
        XXXXXXX, XXXXXXX, KC_UP,   XXXXXXX, XXXXXXX,                                      KC_F4,   KC_TAB,  KC_LCTL, KC_LALT, KC_F11,
        XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX,                                      XXXXXXX, KC_Z,    KC_X,    KC_C,    KC_V,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                      XXXXXXX, KC_A,    KC_S,    KC_D,    KC_F,
                 XXXXXXX, XXXXXXX,                                                                          KC_Q,    KC_W,
                                   KC_ESC,  KC_UP,                                        KC_LSFT, KC_ENT,
                                                     KC_SPC,  RETB,     RETB,    XXXXXXX,
                                                     XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX
    ),
};

void persistent_default_layer_set(uint16_t default_layer) {
    eeconfig_update_default_layer(default_layer);
    default_layer_set(default_layer);
}

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
