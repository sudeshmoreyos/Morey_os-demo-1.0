#ifndef AVR_MEGA_SPI_DEFINES_H
#define AVR_MEGA_SPI_DEFINES_H

// Portable Macros Across Platforms
#define SPI_MODE0 0		// CPOL = 0, CPHA = 0
#define SPI_MODE1 1		// CPOL = 0, CPHA = 1
#define SPI_MODE2 2		// CPOL = 1, CPHA = 0
#define SPI_MODE3 3		// CPOL = 1, CPHA = 1

// Macros valid for AVR Mega controllers only
#define SPI_MODE_CPOL0_CPHA0 SPI_MODE0
#define SPI_MODE_CPOL0_CPHA1 SPI_MODE1
#define SPI_MODE_CPOL1_CPHA0 SPI_MODE2
#define SPI_MODE_CPOL1_CPHA1 SPI_MODE3

// Portable Macros Across Platforms
#define SPI_SPEED0 0	// OSC_FREQ/4
#define SPI_SPEED1 1	// OSC_FREQ/8
#define SPI_SPEED2 2	// OSC_FREQ/16
#define SPI_SPEED3 3	// OSC_FREQ/32
#define SPI_SPEED4 4	// OSC_FREQ/64
#define SPI_SPEED5 5	// OSC_FREQ/128
#define SPI_SPEED6 6	// OSC_FREQ/2

// Macros valid for AVR Mega controllers only
#define SPI_SPEED_BY2 SPI_SPEED6
#define SPI_SPEED_BY4 SPI_SPEED0
#define SPI_SPEED_BY8 SPI_SPEED1
#define SPI_SPEED_BY16 SPI_SPEED2
#define SPI_SPEED_BY32 SPI_SPEED3
#define SPI_SPEED_BY64 SPI_SPEED4
#define SPI_SPEED_BY128 SPI_SPEED5
#define SPI_SPEED_HIGHEST SPI_SPEED_BY4
#define SPI_SPEED_LOWEST SPI_SPEED_BY128

// Portable Macros Across Platforms
#define SPI_MSB_FIRST 0
#define SPI_LSB_FIRST 1

#endif