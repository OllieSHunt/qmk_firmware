// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Include image files
#include "assets/static_ui.qgf.h"

// The address of the SSD1306 128x65 OLED
#define DISPLAY_I2C_ADDRESS 0x3C

// Display for quantum painter
static painter_device_t display;

static uint16_t display_sleep_timer; // I handle display sleep manually because QUANTUM_PAINTER_DISPLAY_TIMEOUT was not working for some reason
#define DISPLAY_TIMEOUT 3000
#define DISPLAY_TIMEOUT_CHECK_FREQ 1000

// Handles to images
static painter_image_handle_t static_ui;

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
        EE_CLR,  QK_BOOT, TG(SYM), _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_NO,   KC_NO,   _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO
    ),

    [QWT] = LAYOUT_ortho_5x10(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_NO,   KC_NO,   _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO
    ),

    [SYM] = LAYOUT_ortho_5x10(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_NO,   KC_NO,   _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO
    ),

    [CTL] = LAYOUT_ortho_5x10(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
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
    static_ui = qp_load_image_mem(gfx_static_ui);

    // Draw images
    qp_drawimage(display, 0, 0, static_ui);
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
    switch (get_highest_layer(state)) {
        case DVK:
            break;
        case QWT:
            break;
        case SYM:
            break;
        case CTL:
            break;
        default:
            break;
    }

    return state;
}

// Called on key press
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // Reset OLED sleep timer
    display_sleep_timer = timer_read();
    qp_power(display, true);
    
    return true;
}
