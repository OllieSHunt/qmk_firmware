// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Include image files
#include "assets/static_ui.qgf.h"

// The address of the SSD1306 128x65 OLED
#define DISPLAY_I2C_ADDRESS 0x3C

// Display for quantum painter
static painter_device_t display;

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
        EE_CLR,  QK_BOOT, _______, _______, _______, _______, _______, _______, _______, _______,
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

// Run on startup
void keyboard_post_init_kb(void) {
    // Setup quantum painter
    display = qp_sh1106_make_i2c_device(128, 64, DISPLAY_I2C_ADDRESS);
    qp_init(display, QP_ROTATION_0);

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

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case TMP:
            // rgblight_setrgb (0x00,  0x00, 0xFF);
            break;
        case ABC:
            // rgblight_setrgb (0xFF,  0x00, 0x00);
            break;
        default:
            // rgblight_setrgb (0x00,  0xFF, 0xFF);
            break;
    }

    return state;
}
