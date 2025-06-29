# Bootloader selection
BOOTLOADER = rp2040
BOARD = GENERIC_RP_RP2040

# RGB LEDs
WS2812_DRIVER = vendor

# Quantum painter config
QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DISPLAY_TIMEOUT = 10000

# This is technicaly the wrong driver, but it seems to work (I am using an SSD1306 display not SH1106)
QUANTUM_PAINTER_DRIVERS += sh1106_i2c

# Quantum painter images
SRC += assets/rubikscube.qgf.c
