// I2C config
#define I2C_DRIVER I2CD0
#define I2C1_SCL_PIN GP17
#define I2C1_SDA_PIN GP16

// LED data pin
#define WS2812_DI_PIN GP15

// Enable LED matrix paterns
#define ENABLE_RGB_MATRIX_NONE
#define ENABLE_RGB_MATRIX_SOLID_COLOR               // 1.  Static single hue, no speed support
#define ENABLE_RGB_MATRIX_ALPHAS_MODS               // 2.  Static dual hue, speed is hue for secondary hue
#define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN          // 3.  Static gradient top to bottom, speed controls how much gradient changes
#define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT       // 4.  Static gradient left to right, speed controls how much gradient changes
#define ENABLE_RGB_MATRIX_BREATHING                 // 5.  Single hue brightness cycling animation
#define ENABLE_RGB_MATRIX_BAND_SAT                  // 6.  Single hue band fading saturation scrolling left to right
#define ENABLE_RGB_MATRIX_BAND_VAL                  // 7.  Single hue band fading brightness scrolling left to right
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT         // 8.  Single hue 3 blade spinning pinwheel fades saturation
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL         // 9.  Single hue 3 blade spinning pinwheel fades brightness
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT           // 10. Single hue spinning spiral fades saturation
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL           // 11. Single hue spinning spiral fades brightness
#define ENABLE_RGB_MATRIX_CYCLE_ALL                 // 12. Full keyboard solid hue cycling through full gradient
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT          // 13. Full gradient scrolling left to right
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN             // 14. Full gradient scrolling top to bottom
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN              // 15. Full gradient scrolling out to in
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL         // 16. Full dual gradients scrolling out to in
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON    // 17. Full gradient Chevron shapped scrolling left to right
#define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL            // 18. Full gradient spinning pinwheel around center of keyboard
#define ENABLE_RGB_MATRIX_CYCLE_SPIRAL              // 19. Full gradient spinning spiral around center of keyboard
#define ENABLE_RGB_MATRIX_DUAL_BEACON               // 20. Full gradient spinning around center of keyboard
#define ENABLE_RGB_MATRIX_RAINBOW_BEACON            // 21. Full tighter gradient spinning around center of keyboard
#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS         // 22. Full dual gradients spinning two halfs of keyboard
#define ENABLE_RGB_MATRIX_FLOWER_BLOOMING           // 23. Full tighter gradient of first half scrolling left to right and second half scrolling right to left
#define ENABLE_RGB_MATRIX_RAINDROPS                 // 24. Randomly changes a single key's hue
#define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS       // 25. Randomly changes a single key's hue and saturation
#define ENABLE_RGB_MATRIX_HUE_BREATHING             // 26. Hue shifts up a slight ammount at the same time, then shifts back
#define ENABLE_RGB_MATRIX_HUE_PENDULUM              // 27. Hue shifts up a slight ammount in a wave to the right, then back to the left
#define ENABLE_RGB_MATRIX_HUE_WAVE                  // 28. Hue shifts up a slight ammount and then back down in a wave to the right
#define ENABLE_RGB_MATRIX_PIXEL_FRACTAL             // 29. Single hue fractal filled keys pulsing horizontally out to edges
#define ENABLE_RGB_MATRIX_PIXEL_FLOW                // 30. Pulsing RGB flow along LED wiring with random hues
#define ENABLE_RGB_MATRIX_PIXEL_RAIN                // 31. Randomly light keys with random hues
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP            // 32. How hot is your WPM!
#define ENABLE_RGB_MATRIX_DIGITAL_RAIN              // 33. That famous computer simulation
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE     // 34. Pulses keys hit to hue & value then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE            // 35. Static single hue, pulses keys hit to shifted hue then fades to current hue
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE       // 36. Hue & value pulse near a single key hit then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE  // 37. Hue & value pulse near multiple key hits then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS      // 38. Hue & value pulse the same column and row of a single key hit then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS // 39. Hue & value pulse the same column and row of multiple key hits then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS      // 40. Hue & value pulse away on the same column and row of a single key hit then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS // 41. Hue & value pulse away on the same column and row of multiple key hits then fades value out
#define ENABLE_RGB_MATRIX_SPLASH                    // 42. Full gradient & value pulse away from a single key hit then fades value out
#define ENABLE_RGB_MATRIX_MULTISPLASH               // 43. Full gradient & value pulse away from multiple key hits then fades value out
#define ENABLE_RGB_MATRIX_SOLID_SPLASH              // 44. Hue & value pulse away from a single key hit then fades value out
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH         // 45. Hue & value pulse away from multiple key hits then fades value out
#define ENABLE_RGB_MATRIX_STARLIGHT                 // 46. LEDs turn on and off at random at varying brightness, maintaining user set color
#define ENABLE_RGB_MATRIX_STARLIGHT_SMOOTH          // 47. LEDs slowly increase and decrease in brightness randomly
#define ENABLE_RGB_MATRIX_STARLIGHT_DUAL_HUE        // 48. LEDs turn on and off at random at varying brightness, modifies user set hue by +- 30
#define ENABLE_RGB_MATRIX_STARLIGHT_DUAL_SAT        // 49. LEDs turn on and off at random at varying brightness, modifies user set saturation by +- 30
#define ENABLE_RGB_MATRIX_RIVERFLOW                 // 50. Modification to breathing animation, offset's animation depending on key location to simulate a river flowing
#define ENABLE_RB_MATRIX_EFFECT_MAX

// #define RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE

// LED config
#define RGB_MATRIX_DEFAULT_ON true
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 255
#define RGB_MATRIX_DEFAULT_HUE 132
#define RGB_MATRIX_DEFAULT_SAT 213
#define RGB_MATRIX_DEFAULT_VAL 150
#define RGB_MATRIX_DEFAULT_SPD 127
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_RIVERFLOW
#define RGB_MATRIX_SLEEP // Disable RGB on sleep
#define RGB_TRIGGER_ON_KEYDOWN // The QMK docs say that this can cuase isues. So if I get any RGB issues, then try disabling this.

// Quantum painter config
#define QUANTUM_PAINTER_NUM_IMAGES 16

// Words per minute config
#define WPM_SAMPLE_SECONDS 4
#define WPM_SAMPLE_PERIODS 32
