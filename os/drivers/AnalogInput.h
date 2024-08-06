#ifndef ANALOGINPUT_H
#define ANALOGINPUT_H

#include "../../platform.h"

struct analog_input_driver {
void (*begin)(mos_uint8_t speed_mode, mos_uint8_t ref_type);
mos_uint16_t (*read)(mos_uint8_t channel);
#ifdef ANALOGINPUT_SUPPORT_DIFFERENTIAL
mos_uint16_t (*readDifferntial)(mos_uint8_t channel, mos_uint8_t gain);
#endif
};

#include "../../arch/arch_analog_input.h"

#endif
