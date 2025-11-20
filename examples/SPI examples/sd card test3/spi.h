#ifndef __SPI_H__
#define __SPI_H__

#include "Digital.h"

// macros
#define CS_ENABLE()		Digital.write(B2,LOW)
#define CS_DISABLE()    Digital.write(B2,HIGH)

// SPI functions
mos_uint8_t SPI_transfer(mos_uint8_t data);

#endif
