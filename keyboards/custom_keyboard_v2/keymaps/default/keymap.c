// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

// TODO:
// - Make the RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE togglable at runtime using custom code
// - Change some specific LEDs when on different layers or states (e.g. caps lock)
// - Stenography: https://docs.qmk.fm/features/stenography
//   - Update the features list in the hardware repository to include this feature
// - Autocorrect? https://docs.qmk.fm/features/autocorrect

#include QMK_KEYBOARD_H

// Include image files
#include "assets/static_ui.qgf.h"
#include "assets/mode_dvorak.qgf.h"
#include "assets/mode_qwerty.qgf.h"
#include "assets/mode_stenography.qgf.h"
#include "assets/checkbox_no.qgf.h"
#include "assets/checkbox_yes.qgf.h"
#include "assets/caps_off.qgf.h"
#include "assets/caps_on.qgf.h"
#include "assets/num_off.qgf.h"
#include "assets/num_on.qgf.h"
#include "assets/scroll_off.qgf.h"
#include "assets/scroll_on.qgf.h"
#include "assets/seperator_off_off.qgf.h"
#include "assets/seperator_on_off.qgf.h"
#include "assets/seperator_off_on.qgf.h"
#include "assets/seperator_on_on.qgf.h"

// The address of the SSD1306 128x65 OLED
#define DISPLAY_I2C_ADDRESS 0x3C

// Display for quantum painter
static painter_device_t display;

static uint16_t display_sleep_timer; // I handle display sleep manually because QUANTUM_PAINTER_DISPLAY_TIMEOUT was not working for some reason
#define DISPLAY_TIMEOUT 5000
#define DISPLAY_TIMEOUT_CHECK_FREQ 1000

// Handles to images
static painter_image_handle_t static_ui_img;
static painter_image_handle_t mode_dvorak_img;
static painter_image_handle_t mode_qwerty_img;
static painter_image_handle_t mode_stenography_img;
static painter_image_handle_t checkbox_no_img;
static painter_image_handle_t checkbox_yes_img;
static painter_image_handle_t caps_off_img;
static painter_image_handle_t caps_on_img;
static painter_image_handle_t num_off_img;
static painter_image_handle_t num_on_img;
static painter_image_handle_t scroll_off_img;
static painter_image_handle_t scroll_on_img;
static painter_image_handle_t seperator_off_off_img;
static painter_image_handle_t seperator_on_off_img;
static painter_image_handle_t seperator_off_on_img;
static painter_image_handle_t seperator_on_on_img;

// Deferred executor tokens
deferred_token display_sleep_check_token;
deferred_token draw_wpm_bar_token;

#define WPM_BAR_REDRAW_FREQ 100

// All layers on the keyboard
enum KeyboardLayers {
    DVK, // Dvorak base layer
    QWT, // QWERTY base layer
    SYM, // Symbols layer
    CTL, // Control layer
};

// Keymap
// IMPORTANT: This keymap is intended to work with an OS keyboard layout of UK QWERTY not US QWERTY.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DVK] = LAYOUT_ortho_5x10(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,
        KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,
        KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,
        KC_NO,   KC_NO,   KC_LALT, KC_SPC,  KC_LCMD, KC_LCTL, MO(SYM), SFT_T(KC_CAPS),KC_NO,   KC_NO
    ),

    [QWT] = LAYOUT_ortho_5x10(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_QUOT,
        KC_NO,   KC_NO,   _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO
    ),

    [SYM] = LAYOUT_ortho_5x10(
        KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,   KC_PSCR, KC_PSLS, KC_PAST, KC_NO,   KC_APP,
        KC_BSLS, KC_GRV,  KC_LBRC, KC_RBRC, KC_ESC,  KC_NUM,  KC_P7,   KC_P8,   KC_P9,   KC_PMNS,
        KC_NUBS, KC_SLSH, KC_DEL,  KC_BSPC, KC_ENT,  KC_P0,   KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_NO,   KC_MINS, KC_EQL,  KC_TAB,  KC_NO,   KC_PDOT, KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        KC_NO,   KC_NO,   _______, QK_LLCK, _______, _______, MO(CTL), _______, KC_NO,   KC_NO
    ),

    [CTL] = LAYOUT_ortho_5x10(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   RM_HUEU, RM_SATU, RM_VALU, RM_SPDU, RM_NEXT,
        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  RM_HUED, RM_SATD, RM_VALD, RM_SPDD, RM_PREV,
        KC_F11,  KC_F12,  KC_INS,  KC_HOME, KC_PGUP, KC_RALT, PDF(QWT),PDF(DVK),KC_NO,   RM_TOGG,
        KC_SCRL, KC_PAUS, KC_NO,   KC_END,  KC_PGDN, KC_NO,   KC_NO,   KC_NO,   EE_CLR,  QK_BOOT,
        KC_NO,   KC_NO,   _______, QK_LLCK, _______, _______, KC_NO,   _______, KC_NO,   KC_NO
    ),

    // NEW LAYER TEMPLATE
    // [?] = LAYOUT_ortho_5x10(
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //     KC_NO,   KC_NO,   _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO
    // )
};

// Draws the default state indicator
void draw_default_layer_image(layer_state_t state) {
    // Find what the default layer is
    if (IS_LAYER_ON_STATE(state, DVK))      qp_drawimage(display, 0, 0, mode_dvorak_img);
    else if (IS_LAYER_ON_STATE(state, QWT)) qp_drawimage(display, 0, 0, mode_qwerty_img);
    else                                    qp_drawimage(display, 0, 0, mode_stenography_img);

    qp_flush(display);
}

// Draws the indicators to show what layers are active (not to be confused with
// the default layer indicator which shows what the current default layer is).
void draw_layer_image(layer_state_t state) {
    // Symbol layer
    if (IS_LAYER_ON_STATE(state, SYM)) qp_drawimage(display, 30, 29, checkbox_yes_img);
    else                               qp_drawimage(display, 30, 29, checkbox_no_img);

    // Control layer
    if (IS_LAYER_ON_STATE(state, CTL)) qp_drawimage(display, 30, 40, checkbox_yes_img);
    else                               qp_drawimage(display, 30, 40, checkbox_no_img);

    qp_flush(display);
}

// Draws the caps lock, num lock, and scroll lock indicators.
//
// This function uses the QMK's led indicator feature to get information about
// the state of the lock keys.
void draw_lock_indicators(led_t led_state) {
    // Caps lock indicator
    if (led_state.caps_lock) qp_drawimage(display, 0, 52, caps_on_img);
    else                     qp_drawimage(display, 0, 52, caps_off_img);

    // Draw the correct separator between the caps lock and num lock indicators
    if      (led_state.caps_lock && led_state.num_lock)  qp_drawimage(display, 13, 52, seperator_on_on_img);
    else if (led_state.caps_lock && !led_state.num_lock) qp_drawimage(display, 13, 52, seperator_on_off_img);
    else if (!led_state.caps_lock && led_state.num_lock) qp_drawimage(display, 13, 52, seperator_off_on_img);
    else                                                 qp_drawimage(display, 13, 52, seperator_off_off_img);

    // Num lock indicator
    if (led_state.num_lock) qp_drawimage(display, 16, 52, num_on_img);
    else                    qp_drawimage(display, 16, 52, num_off_img);

    // Draw the correct separator between the num lock and scroll lock indicators
    if      (led_state.num_lock && led_state.scroll_lock)  qp_drawimage(display, 26, 52, seperator_on_on_img);
    else if (led_state.num_lock && !led_state.scroll_lock) qp_drawimage(display, 26, 52, seperator_on_off_img);
    else if (!led_state.num_lock && led_state.scroll_lock) qp_drawimage(display, 26, 52, seperator_off_on_img);
    else                                                   qp_drawimage(display, 26, 52, seperator_off_off_img);

    // Scroll lock indicator
    if (led_state.scroll_lock) qp_drawimage(display, 29, 52, scroll_on_img);
    else                       qp_drawimage(display, 29, 52, scroll_off_img);

    qp_flush(display);
}

// Uses a deferred callback to redraw the bar that measures words per minute.
// https://docs.qmk.fm/custom_quantum_functions#deferred-executor-callbacks
uint32_t draw_wpm_bar(uint32_t trigger_time, void *cb_arg) {
    const uint8_t max_bar_length = 83; // Max bar length in pixels
    const float max_wpm = 100.0;       // The highest wpm value on the scale
    float wpm = (float)get_current_wpm();

    uint8_t bar_length = (uint8_t)((wpm / max_wpm) * (float)max_bar_length);

    // Draw black box over old bar to erase it
    qp_rect(display, 44, 61, 44 + max_bar_length - 1, 63, 0, 0, 0, true);

    // Draw WPM bar
    qp_rect(display, 44, 61, 44 + bar_length - 1, 63, 0, 255, 255, true);

    qp_flush(display);

    // Call this function again after the same amount of time
    return WPM_BAR_REDRAW_FREQ;
}

// Draws the bars that show hue saturation, lightness, and speed.
void draw_hsl_bars(void) {
    const uint8_t horizontal_bar_length = 64; // Max length of horizontal bars in pixels
    const uint8_t vertical_bar_length = 39;   // Max length of vertical bars in pixels

    uint8_t h_bar_length = (uint8_t)(((float)rgb_matrix_get_hue() / 255.0) * (float)horizontal_bar_length);
    uint8_t s_bar_length = (uint8_t)(((float)rgb_matrix_get_sat() / 255.0) * (float)horizontal_bar_length);
    uint8_t l_bar_length = (uint8_t)(((float)rgb_matrix_get_val() / 255.0) * (float)horizontal_bar_length);
    uint8_t speed_bar_length = (uint8_t)(((float)rgb_matrix_get_speed() / 255.0) * (float)vertical_bar_length);

    // Draw black box over old bars to erase them
    qp_rect(display, 49, 13, 49 + horizontal_bar_length - 1, 15, 0, 0, 0, true);
    qp_rect(display, 49, 28, 49 + horizontal_bar_length - 1, 30, 0, 0, 0, true);
    qp_rect(display, 49, 42, 49 + horizontal_bar_length - 1, 44, 0, 0, 0, true);
    qp_rect(display, 119, 6 + vertical_bar_length, 121, 6, 0, 0, 0, true);

    // Draw bars
    qp_rect(display, 49, 13, 49 + h_bar_length - 1, 15, 0, 255, 255, true);
    qp_rect(display, 49, 28, 49 + s_bar_length - 1, 30, 0, 255, 255, true);
    qp_rect(display, 49, 42, 49 + l_bar_length - 1, 44, 0, 255, 255, true);
    qp_rect(display, 119, 6 + vertical_bar_length, 121, (6 + vertical_bar_length) - speed_bar_length - 1, 0, 255, 255, true);

    qp_flush(display);
}

// Draw binary number as an indicator for what RGB matrix effect is active.
void draw_rgb_mode_indicator(void) {
    const uint8_t x = 94;
    const uint8_t y = 1;
    const uint8_t bit_size = 3; // Size of each square in pixels
    const uint8_t bit_quantity = 6;
    const uint8_t x_end = x + (bit_size * bit_quantity);
    const uint8_t y_end = y + bit_size;
    uint8_t mode = rgb_matrix_get_mode();

    // Draw black rectangle to erase previous render
    qp_rect(display, x, y, x_end - 1, y_end - 1, 0, 0, 0, true);

    // Draw a square for each bit
    for (uint8_t i=0; i<bit_quantity; i++) {
        if ((mode & ( 1 << i )) >> i) {
            uint8_t bit_x = x_end - (bit_size * i);
            qp_rect(display, bit_x - bit_size, y, bit_x - 1, y_end - 1, 0, 255, 255, true);
        }
    }
}

// Redraw the whole screen
//
// The deferred executor for draw_wpm_bar() must before you call this function.
void draw_whole_screen(void) {
    // Static UI elements that do not change
    qp_drawimage(display, 0, 0, static_ui_img);

    // The default layer image
    draw_default_layer_image(default_layer_state);

    // The layer indicators
    draw_layer_image(default_layer_state);

    // The caps lock, num lock, and scroll lock indicators.
    draw_lock_indicators(host_keyboard_led_state());

    // The bars that show hue saturation and lightness.
    draw_hsl_bars();

    // Binary number as an indicator for what RGB matrix effect is active.
    draw_rgb_mode_indicator();

    // The bar that measures words per minute
    // Draw the WPM bar by making the deferred callback run early
    extend_deferred_exec(draw_wpm_bar_token, 1);
}

// Uses a deferred callback to check if the display should go to sleep
// https://docs.qmk.fm/custom_quantum_functions#deferred-executor-callbacks
uint32_t display_sleep_check(uint32_t trigger_time, void *cb_arg) {
    // Check if display needs to go to sleep
    if (timer_elapsed(display_sleep_timer) > DISPLAY_TIMEOUT) {
        // Reset timer
        display_sleep_timer = timer_read();

        // Turn off the display
        qp_power(display, false);
    }

    // Call this function again after the same amount of time
    return DISPLAY_TIMEOUT_CHECK_FREQ;
}

// Run on startup
void keyboard_post_init_kb(void) {
    // Setup quantum painter
    display = qp_sh1106_make_i2c_device(128, 64, DISPLAY_I2C_ADDRESS);
    qp_init(display, QP_ROTATION_0);

    // Display sleeping
    display_sleep_timer = timer_read();
    display_sleep_check_token = defer_exec(DISPLAY_TIMEOUT_CHECK_FREQ, display_sleep_check, NULL);

    // Load images
    static_ui_img = qp_load_image_mem(gfx_static_ui);
    mode_dvorak_img = qp_load_image_mem(gfx_mode_dvorak);
    mode_qwerty_img = qp_load_image_mem(gfx_mode_qwerty);
    mode_stenography_img = qp_load_image_mem(gfx_mode_stenography);
    checkbox_no_img = qp_load_image_mem(gfx_checkbox_no);
    checkbox_yes_img = qp_load_image_mem(gfx_checkbox_yes);
    caps_off_img = qp_load_image_mem(gfx_caps_off);
    caps_on_img = qp_load_image_mem(gfx_caps_on);
    num_off_img = qp_load_image_mem(gfx_num_off);
    num_on_img = qp_load_image_mem(gfx_num_on);
    scroll_off_img = qp_load_image_mem(gfx_scroll_off);
    scroll_on_img = qp_load_image_mem(gfx_scroll_on);
    seperator_off_off_img = qp_load_image_mem(gfx_seperator_off_off);
    seperator_on_off_img = qp_load_image_mem(gfx_seperator_on_off);
    seperator_off_on_img = qp_load_image_mem(gfx_seperator_off_on);
    seperator_on_on_img = qp_load_image_mem(gfx_seperator_on_on);

    // Regularly update the WPM bar on the screen
    draw_wpm_bar_token = defer_exec(WPM_BAR_REDRAW_FREQ, draw_wpm_bar, NULL);

    // Draw display for the first time
    draw_whole_screen();
}

// This will run multiple times while keyboard is suspended
void suspend_power_down_kb(void) {
    // Disable the display
    qp_power(display, false);
}

// This will run on keyboard wakeup
void suspend_wakeup_init_kb(void) {
    // Re-enable the display
    qp_power(display, true);
}

// This will run when entering the keyboard is powered off
bool shutdown_kb(bool jump_to_bootloader) {
    // Clear the display and turn off all LEDs
    qp_power(display, false);
    rgb_matrix_set_color_all(RGB_OFF);

    // Force update the LEDs
    ws2812_flush();

    return true;
}

// Called on layer change
layer_state_t layer_state_set_kb(layer_state_t state) {
    // Draw the layer indicators
    draw_layer_image(state);

    return state;
}

// Called on default layer change and on keyboard init
layer_state_t default_layer_state_set_kb(layer_state_t state) {
    draw_default_layer_image(state);

    return state;
}

// Called when one of the following indicator LEDs is changed:
// Num Lock, Caps Lock, Scroll Lock, Compose, Kana
bool led_update_kb(led_t led_state) {
    // Draws the caps lock, num lock, and scroll lock indicators.
    draw_lock_indicators(led_state);

    return true;
}

// Called on key press
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // Reset OLED sleep timer
    display_sleep_timer = timer_read();
    qp_power(display, true);

    // Redraw the WPM bar by making the deferred callback run early
    extend_deferred_exec(draw_wpm_bar_token, 1);

    // TEMP: Move somewhere else so it is not run every keystroke
    draw_hsl_bars();
    draw_rgb_mode_indicator();

    return true;
}
