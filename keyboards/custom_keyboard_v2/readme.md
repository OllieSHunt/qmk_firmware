# custom_keyboard_v2

**IMPORTANT:** See https://github.com/OllieSHunt/custom-keyboard-v2 for information about this project.

* Keyboard Maintainer: [Ollie Hunt](https://github.com/OllieSHunt)
* Hardware Supported: https://github.com/OllieSHunt/custom-keyboard-v2

## Bootloader

Enter the bootloader by going onto the `ctl` (control) layer and pressing the boot key (please see [the keymap cheatsheet](https://github.com/OllieSHunt/custom-keyboard-v2/blob/main/keymap/keymap-exported.pdf)).

## Commands
Lists of useful commands mostly just for my own reference.

Run from anywhere to compile and/or flash (put into boot mode first):
* `qmk flash`
* `qmk compile`

Working with images (run from the directory `~/qmk_firmware/keyboards/custom_keyboard_v2/assets/original`):
* One image: `qmk painter-convert-graphics -f mono2 -o .. -i static_ui.png`
* All images (requires Fish shell): `for file in (ls .); qmk painter-convert-graphics -f mono2 -o .. -i $file; end`

## Update QMK Fork
* First, follow update instructions here: https://docs.qmk.fm/newbs_git_using_your_master_branch
* Then run this: `git submodule update --init --recursive` (not included in the above link for some reason)
