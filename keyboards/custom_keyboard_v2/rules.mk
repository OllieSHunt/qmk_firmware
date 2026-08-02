# Bootloader selection
BOOTLOADER = rp2040
BOARD = GENERIC_RP_RP2040

# RGB LEDs
WS2812_DRIVER = vendor

# Misc features
WPM_ENABLE = yes
LAYER_LOCK_ENABLE = yes

# Enables N key rollover, but does not turn it on. That is done in config.h
# (enabled in keyboard.json instead)
# NKRO_ENABLE = yes

# Stenography
# https://docs.qmk.fm/features/stenography
# (see also, alternative layout: geminipr)
STENO_ENABLE = yes
STENO_PROTOCOL = txbolt

# Quantum painter
# https://docs.qmk.fm/quantum_painter
QUANTUM_PAINTER_ENABLE = yes

# This is technicaly the wrong driver, but it seems to work (I am using an SSD1306 display not SH1106)
QUANTUM_PAINTER_DRIVERS += sh1106_i2c

# Images
# IMPORTANT: If adding new images, then also update `QUANTUM_PAINTER_NUM_IMAGES` in config.h!!!
#            (I've just wasted a very large chunk of my afternoon with this)
SRC += assets/static_ui.qgf.c
SRC += assets/mode_dvorak.qgf.c
SRC += assets/mode_qwerty.qgf.c
SRC += assets/checkbox_no.qgf.c
SRC += assets/checkbox_yes.qgf.c
SRC += assets/caps_off.qgf.c
SRC += assets/caps_on.qgf.c
SRC += assets/num_off.qgf.c
SRC += assets/num_on.qgf.c
SRC += assets/scroll_off.qgf.c
SRC += assets/scroll_on.qgf.c
SRC += assets/seperator_off_off.qgf.c
SRC += assets/seperator_on_off.qgf.c
SRC += assets/seperator_off_on.qgf.c
SRC += assets/seperator_on_on.qgf.c
SRC += assets/steno_indicator.qgf.c
SRC += assets/layers_list.qgf.c
