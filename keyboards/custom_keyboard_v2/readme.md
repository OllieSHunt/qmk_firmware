# custom_keyboard_v2

IMPORTANT: See https://github.com/OllieSHunt/custom-keyboard-v2 for information about this project.

* Keyboard Maintainer: [Ollie Hunt](https://github.com/OllieSHunt)
* Hardware Supported: https://github.com/OllieSHunt/custom-keyboard-v2

Make example for this keyboard (after setting up your build environment):

    make custom_keyboard_v2:default

Flashing example for this keyboard:

    make custom_keyboard_v2:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Hold down the button on the Raspberry Pi Pico as you plug in the keyboard
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available

## Commands
Lists of useful commands mostly just for my own reference.

Run from anywhere:
* `qmk compile && sudo picotool load ~/qmk_firmware/custom_keyboard_v2_default.uf2 && sudo picotool reboot`

Working with images - run from the directory `~/qmk_firmware/keyboards/custom_keyboard_v2/assets/original`:
* `qmk painter-convert-graphics -f mono2 -o .. -i static_ui.png`
* Requires Fish shell: `for file in (ls .); qmk painter-convert-graphics -f mono2 -o .. -i $file; end`
