// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Include image files
#include "assets/mode_dvorak.qgf.h"
#include "assets/mode_qwerty.qgf.h"
#include "assets/mode_stenography.qgf.h"
#include "assets/speed_indicator.qgf.h"

// The address of the SSD1306 128x65 OLED
#define DISPLAY_I2C_ADDRESS 0x3C

// Display for quantum painter
static painter_device_t display;

static uint16_t display_sleep_timer; // I handle display sleep manually because QUANTUM_PAINTER_DISPLAY_TIMEOUT was not working for some reason
#define DISPLAY_TIMEOUT 3000
#define DISPLAY_TIMEOUT_CHECK_FREQ 1000

// Handles to images
static painter_image_handle_t mode_dvorak;
static painter_image_handle_t mode_qwerty;
static painter_image_handle_t mode_stenography;
static painter_image_handle_t speed_indicator;

// All layers on the keyboard
enum KeyboardLayers {
    DVK, // Dvorak base layer
    QWT, // QWERTY base layer
    SYM, // Symbols layer
    CTL, // Control layer
};

// Keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DVK] = LAYOUT_ortho_5x10(
        EE_CLR,  QK_BOOT, PDF(QWT),_______, _______, _______, _______, _______, _______, KC_D,
        TG(SYM), _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_NO,   KC_NO,   _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO
    ),

    [QWT] = LAYOUT_ortho_5x10(
        _______, _______, PDF(DVK),_______, _______, _______, _______, _______, _______, KC_Q,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_NO,   KC_NO,   _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO
    ),

    [SYM] = LAYOUT_ortho_5x10(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_S,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_NO,   KC_NO,   _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO
    ),

    [CTL] = LAYOUT_ortho_5x10(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_C,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_NO,   KC_NO,   _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO
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
    const int DEFAULT_STATE_IMAGE_X = 0;
    const int DEFAULT_STATE_IMAGE_Y = 0;
    
    // Find what the default layer is
    if (IS_LAYER_ON_STATE(state, DVK))      qp_drawimage(display, DEFAULT_STATE_IMAGE_X, DEFAULT_STATE_IMAGE_Y, mode_dvorak);
    else if (IS_LAYER_ON_STATE(state, QWT)) qp_drawimage(display, DEFAULT_STATE_IMAGE_X, DEFAULT_STATE_IMAGE_Y, mode_qwerty);
    else                                    qp_drawimage(display, DEFAULT_STATE_IMAGE_X, DEFAULT_STATE_IMAGE_Y, mode_stenography);

    qp_flush(display);
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
    defer_exec(DISPLAY_TIMEOUT_CHECK_FREQ, display_sleep_check, NULL);

    // Load images
    mode_dvorak = qp_load_image_mem(gfx_mode_dvorak);
    mode_qwerty = qp_load_image_mem(gfx_mode_qwerty);
    mode_stenography = qp_load_image_mem(gfx_mode_stenography);
    speed_indicator = qp_load_image_mem(gfx_speed_indicator);

    // Draw display for the first time
    draw_default_layer_image(default_layer_state);
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

// Called on layer change
layer_state_t layer_state_set_kb(layer_state_t state) {
    // TODO
    // switch (get_highest_layer(state)) {
    //     case DVK:
    //         break;
    //     case QWT:
    //         break;
    //     case SYM:
    //         break;
    //     case CTL:
    //         break;
    //     default:
    //         break;
    // }

    return state;
}

// Called on default layer change and on keyboard init
layer_state_t default_layer_state_set_kb(layer_state_t state) {
    draw_default_layer_image(state);

    return state;
}

// Called on key press
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // Reset OLED sleep timer
    display_sleep_timer = timer_read();
    qp_power(display, true);
    
    return true;
}
