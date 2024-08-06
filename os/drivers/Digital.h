#ifndef DIGITAL_H
#define DIGITAL_H

#include "../../platform.h"

struct digital_driver {
void (*pinmode)(mos_uint8_t pin, mos_uint8_t mode);
void (*write)(mos_uint8_t pin, mos_uint8_t value);
mos_uint8_t (*read) (mos_uint8_t pin);
};

#include "../../arch/arch_digital.h"

#endif
