// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Include images for quantum painter
#include "assets/rubikscube.qgf.h"

// The address of the SSD1306 128x65 OLED
#define DISPLAY_I2C_ADDRESS 0x3C

// Display for quantum painter
static painter_device_t display;

// TEMP: image
static painter_image_handle_t temp_image;
static deferred_token temp_anim;

enum KeyboardLayers {
    // QWT, // QWERTY base layer
    // DVK, // Dvorak base layer
    TMP,
    ABC,
};

// Keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [TMP] = LAYOUT_ortho_5x10(
        EE_CLR,  QK_BOOT, KC_C,    KC_D,    KC_E,    KC_F,    KC_G,    KC_H,    KC_I,    KC_J,
        KC_K,    KC_L,    KC_M,    KC_N,    KC_O,    KC_P,    KC_Q,    KC_R,    KC_S,    KC_T,
        KC_U,    KC_V,    KC_W,    KC_X,    KC_Y,    KC_Z,    KC_A,    KC_B,    KC_C,    KC_E,
        KC_F,    KC_G,    KC_H,    KC_I,    KC_J,    KC_K,    KC_L,    KC_M,    KC_N,    TG(ABC),
        KC_NO,   KC_NO,   KC_P,    KC_Q,    KC_R,    KC_S,    KC_T,    KC_U,    KC_NO,   KC_NO
    ),

    [ABC] = LAYOUT_ortho_5x10(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_NO,   KC_NO,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_NO,   KC_NO
    )

    // /* NEW LAYER TEMPLATE
    //  * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    //  * │   │   │   │   │   │   │   │   │   │   │
    //  * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
    //  * │   │   │   │   │   │   │   │   │   │   │
    //  * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
    //  * │   │   │   │   │   │   │   │   │   │   │
    //  * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
    //  * │   │   │   │   │   │   │   │   │   │   │
    //  * └───┴───┼───┼───┼───┼───┼───┼───┼───┴───┘
    //  *         │   │   │   │   │   │   │        
    //  *         └───┴───┴───┴───┴───┴───┘        
    //  */
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

    // Start temp animation
    temp_image = qp_load_image_mem(gfx_rubikscube);
    if (temp_image != NULL) {
        temp_anim = qp_animate(display, (128 / 2) - (temp_image->width / 2), (64 / 2) - (temp_image->height / 2), temp_image);
    }
}

// This will run multiple times while keyboard is suspended
void suspend_power_down_user(void) {
    // Disable the display
    qp_power(display, false);
}

// This will run on keyboard wakeup
void suspend_wakeup_init_user(void) {
    // Re-enable the display
    qp_power(display, true);
}
