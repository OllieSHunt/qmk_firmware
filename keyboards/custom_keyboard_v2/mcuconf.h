#pragma once

#include_next <mcuconf.h>

#undef STM32_I2C_USE_I2C2
#define STM32_I2C_USE_I2C2 TRUE

#undef RP_I2C_USE_I2C0
#define RP_I2C_USE_I2C0 TRUE
