// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Include image files
#include "assets/static_ui.qgf.h"
#include "assets/mode_dvorak.qgf.h"
#include "assets/mode_qwerty.qgf.h"
#include "assets/mode_stenography.qgf.h"
#include "assets/checkbox_no.qgf.h"
#include "assets/checkbox_yes.qgf.h"

// The address of the SSD1306 128x65 OLED
#define DISPLAY_I2C_ADDRESS 0x3C

// Display for quantum painter
static painter_device_t display;

static uint16_t display_sleep_timer; // I handle display sleep manually because QUANTUM_PAINTER_DISPLAY_TIMEOUT was not working for some reason
#define DISPLAY_TIMEOUT 3000
#define DISPLAY_TIMEOUT_CHECK_FREQ 1000

// Handles to images
static painter_image_handle_t static_ui;
static painter_image_handle_t mode_dvorak;
static painter_image_handle_t mode_qwerty;
static painter_image_handle_t mode_stenography;
static painter_image_handle_t checkbox_no;
static painter_image_handle_t checkbox_yes;

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
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_NO,   KC_NO,   _______, _______, _______, _______, TT(SYM), _______, KC_NO,   KC_NO
    ),

    [QWT] = LAYOUT_ortho_5x10(
        _______, _______, PDF(DVK),_______, _______, _______, _______, _______, _______, _______,
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
        KC_NO,   KC_NO,   _______, TT(CTL), _______, _______, _______, _______, KC_NO,   KC_NO
    ),

    [CTL] = LAYOUT_ortho_5x10(
        _______, _______, _______, _______, _______, _______, _______, _______, EE_CLR,  QK_BOOT,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, PDF(DVK),PDF(QWT),_______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_NO,   KC_NO,   _______, TT(CTL), _______, _______, _______, _______, KC_NO,   KC_NO
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

// Draws the indicators to show what layers are active (not to be confused with
// the default layer indicator which shows what the current default layer is).
void draw_layer_image(layer_state_t state) {
    // Symbol layer
    if (IS_LAYER_ON_STATE(state, SYM)) qp_drawimage(display, 30, 29, checkbox_yes);
    else                               qp_drawimage(display, 30, 29, checkbox_no);

    // Control layer
    if (IS_LAYER_ON_STATE(state, CTL)) qp_drawimage(display, 30, 40, checkbox_yes);
    else                               qp_drawimage(display, 30, 40, checkbox_no);
}

// Redraw the whole screen
void draw_whole_screen(void) {
    // Static UI elements that do not change
    qp_drawimage(display, 0, 0, static_ui);
    
    // The default layer image
    draw_default_layer_image(default_layer_state);

    // The layer indicators
    draw_layer_image(default_layer_state);
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
    static_ui = qp_load_image_mem(gfx_static_ui);
    mode_dvorak = qp_load_image_mem(gfx_mode_dvorak);
    mode_qwerty = qp_load_image_mem(gfx_mode_qwerty);
    mode_stenography = qp_load_image_mem(gfx_mode_stenography);
    checkbox_no = qp_load_image_mem(gfx_checkbox_no);
    checkbox_yes = qp_load_image_mem(gfx_checkbox_yes);

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

// Called on key press
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // Reset OLED sleep timer
    display_sleep_timer = timer_read();
    qp_power(display, true);
    
    return true;
}
