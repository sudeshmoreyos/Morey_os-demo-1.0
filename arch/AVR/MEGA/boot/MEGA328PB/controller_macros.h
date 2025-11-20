/**\addtogroup headers_MEGA328PB
* @{ \defgroup controller_macros_MEGA328PB_h
* @{
* \brief Documentaion of controller_macros header file for atmega328PB controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header file declares controller specific macros for atmega328PB controller.
* @}
*/

#ifndef CONTROLLER_MACROS_H
#define CONTROLLER_MACROS_H

/**
* \name MACROS : Mega328PB Digital IOs Macros declaration
* \brief Define OS and platform dependent MACROS for digital pins
* @{
*/
#define PLATFORM_SUPPORT_DIGITAL_IO
#define PORTB_AVAILABLE
#define PORTC_AVAILABLE
#define PORTD_AVAILABLE
#define PORTE_AVAILABLE

// controller port declaration
#define B0 0x08
#define B1 0x09
#define B2 0x0A
#define B3 0x0B
#define B4 0x0C
#define B5 0x0D
#define B6 0x0E
#define B7 0x0F

#define C0 0x10
#define C1 0x11
#define C2 0x12
#define C3 0x13
#define C4 0x14
#define C5 0x15

#define D0 0x18
#define D1 0x19
#define D2 0x1A
#define D3 0x1B
#define D4 0x1C
#define D5 0x1D
#define D6 0x1E
#define D7 0x1F

#define E0 0x20
#define E1 0x21
#define E2 0x22
#define E3 0x23

// User can define custom OS pins for easy portability
// By defining macro DEFINE_CUSTOM_OS_PINS in config.h
// and then defining custom OS pins
#ifndef DEFINE_CUSTOM_OS_PINS

	// if not Default OS pins are mapped with controller pins as below:
	#define OS_pin0 D0
	#define OS_pin1 D1
	#define OS_pin2 D2
	#define OS_pin3 D3
	#define OS_pin4 D4
	#define OS_pin5 D5
	#define OS_pin6 D6
	#define OS_pin7 D7

	#define OS_pin8 B0
	#define OS_pin9 B1
	#define OS_pin10 B2
	#define OS_pin11 B3
	#define OS_pin12 B4
	#define OS_pin13 B5

	#define OS_pin14 C0
	#define OS_pin15 C1
	#define OS_pin16 C2
	#define OS_pin17 C3
	#define OS_pin18 C4
	#define OS_pin19 C5

	#define OS_pin20 B6
	#define OS_pin21 B7

#endif
///@}

// Declare Macros for Serial UART supported by controller/platform
#define PLATFORM_SUPPORT_UART
#define PLATFORM_SUPPORT_CONST_PRINT

#define UART0_AVAILABLE
#define UART1_AVAILABLE

#define UART0_PORT 1
#define UART1_PORT 2

// UART & UART2 to UART3 ports are not available in atmega328PB controller
//#define UART_PORT 0
//#define UART2_PORT 3
//#define UART3_PORT 4

// Throw error is both SERIAL_PORT_CONF and SERIAL0_PORT_CONF are defined
#if defined(SERIAL_PORT_CONF) && defined(SERIAL0_PORT_CONF)
	#error Either SERIAL_PORT_CONF or SERIAL0_PORT_CONF or should be defined
#endif

// If Serial Port is configured by user
#ifdef SERIAL_PORT_CONF
	#define SERIAL_PORT SERIAL_PORT_CONF
#endif

// If Serial0 Port is configured by user
#ifdef SERIAL0_PORT_CONF
	#define SERIAL0_PORT SERIAL0_PORT_CONF
#endif

// If SERIAL_PORT is defined above, assign it to SERIAL0_PORT
#ifdef SERIAL_PORT
	#define SERIAL0_PORT SERIAL_PORT
#endif

// If SERIAL0_PORT is not defined by user or above, give it default value UART0_PORT
#ifndef SERIAL0_PORT
	#define SERIAL0_PORT UART0_PORT
#endif

// If Serial1 Port is configured by user
#ifdef SERIAL1_PORT_CONF
	#define SERIAL1_PORT SERIAL1_PORT_CONF
#endif

// If SERIAL1_PORT is not defined by user or above, give it default value UART1_PORT
#ifndef SERIAL1_PORT
	#define SERIAL1_PORT UART1_PORT
#endif

// Checking for repeat declaration of SERIAL_ENABLE and SERIAL0_ENABLE
#if defined(SERIAL_ENABLE) && defined(SERIAL0_ENABLE)
	#error Either declare SERIAL_ENABLE or SERIAL0_ENABLE to configure Serial0 UART
#endif

#ifdef SERIAL_ENABLE
	#define SERIAL0_ENABLE SERIAL_ENABLE
#endif

// Checking for repeat declaration of SERIAL_RX_BUFFER_CONF and SERIAL0_RX_BUFFER_CONF
#if defined(SERIAL_RX_BUFFER_CONF) && defined(SERIAL0_RX_BUFFER_CONF)
	#error Either declare SERIAL_RX_BUFFER_CONF or SERIAL0_RX_BUFFER_CONF to configure Serial0 UART RX Buffer size
#endif

#ifdef SERIAL_RX_BUFFER_CONF
	#define SERIAL0_RX_BUFFER_CONF SERIAL_RX_BUFFER_CONF
#endif

// Checking for repeat declaration of SERIAL_TX_BUFFER_CONF and SERIAL0_TX_BUFFER_CONF
#if defined(SERIAL_TX_BUFFER_CONF) && defined(SERIAL0_TX_BUFFER_CONF)
	#error Either declare SERIAL_TX_BUFFER_CONF or SERIAL0_TX_BUFFER_CONF to configure Serial0 UART TX Buffer size
#endif

#ifdef SERIAL_TX_BUFFER_CONF
	#define SERIAL0_TX_BUFFER_CONF SERIAL_TX_BUFFER_CONF
#endif

// Checking for repeat declaration of SERIAL_TX_DISABLE and SERIAL0_TX_DISABLE
#if defined(SERIAL_TX_DISABLE) && defined(SERIAL0_TX_DISABLE)
	#error Either declare SERIAL_TX_DISABLE or SERIAL0_TX_DISABLE to configure Serial0 UART
#endif

#ifdef SERIAL_TX_DISABLE
	#define SERIAL0_TX_DISABLE SERIAL_TX_DISABLE
#endif

// Checking for repeat declaration of SERIAL_RX_DISABLE and SERIAL0_RX_DISABLE
#if defined(SERIAL_RX_DISABLE) && defined(SERIAL0_RX_DISABLE)
	#error Either declare SERIAL_RX_DISABLE or SERIAL0_RX_DISABLE to configure Serial0 UART
#endif

#ifdef SERIAL_RX_DISABLE
	#define SERIAL0_RX_DISABLE SERIAL_RX_DISABLE
#endif

// Checking for repeat declaration of SERIAL_TX_INTERRUPT_ENABLE and SERIAL0_TX_INTERRUPT_ENABLE
#if defined(SERIAL_TX_INTERRUPT_ENABLE) && defined(SERIAL0_TX_INTERRUPT_ENABLE)
	#error Either declare SERIAL_TX_INTERRUPT_ENABLE or SERIAL0_TX_INTERRUPT_ENABLE to configure Serial UART0
#endif

#ifdef SERIAL_TX_INTERRUPT_ENABLE
	#define SERIAL0_TX_INTERRUPT_ENABLE SERIAL_TX_INTERRUPT_ENABLE
#endif

// Declare default RX and TX buffer sizes for atmega328PB for UART
// If user declares SERIALx_RX_BUFFER_CONF or SERIALx_TX_BUFFER_CONF Macros in config.h
// then respective UART drivers will give precendence to user defined settings
// x = 0 / 1 / 2 / 3

#define UART0_RX_BUFFER_DEFAULT 64
#define UART0_TX_BUFFER_DEFAULT 64

#define UART1_RX_BUFFER_DEFAULT 64
#define UART1_TX_BUFFER_DEFAULT 64

// Declare Macros for Analog Input supported by controller/platform
//#define PLATFORM_SUPPORT_ANALOGINPUT
#define ANALOGINPUT_TYPE1
#define AI_REF_INTERNAL1v1 3
#define AI_CHANNEL_BG1V1 14
#define AI_CHANNEL_BG0V0 15

// Declare Macros for I2C supported by controller/platform
#define PLATFORM_SUPPORT_I2C
#define I2C0_AVAILABLE
#define I2C1_AVAILABLE

#define I2C0_PORT 1
#define I2C1_PORT 2

// I2C_PORT is not available in atmega328PB controller
// #define I2C_PORT 0

#ifdef I2C_PORT_CONF
	#define I2C0_PORT_CONF I2C_PORT_CONF
#endif

#ifndef I2C0_PORT_CONF
	#define I2C0_PORT_CONF I2C0_PORT
#endif

#ifndef I2C1_PORT_CONF
	#define I2C1_PORT_CONF I2C1_PORT
#endif

// Declare Macros for SPI supported by controller/platform
#define PLATFORM_SUPPORT_SPI
#define SPI0_AVAILABLE
#define SPI1_AVAILABLE

#define SPI0_SS_PIN 	B2
#define SPI0_MOSI_PIN 	B3
#define SPI0_MISO_PIN 	B4
#define SPI0_SCK_PIN 	B5

#define SPI1_SS_PIN 	E2
#define SPI1_MOSI_PIN 	E3
#define SPI1_MISO_PIN 	C0
#define SPI1_SCK_PIN 	C1

#define SPI0_PORT 1
#define SPI1_PORT 2

// SPI_PORT is not available in atmega328PB controller
// #define SPI_PORT 0

#ifdef SPI_PORT_CONF
	#define SPI0_PORT_CONF SPI_PORT_CONF
#endif

#ifndef SPI0_PORT_CONF
	#define SPI0_PORT_CONF SPI0_PORT
#endif

#ifndef SPI1_PORT_CONF
	#define SPI1_PORT_CONF SPI1_PORT
#endif

// Checking for repeat declaration of SPI_SLAVE_ENABLE and SPI0_SLAVE_ENABLE
#if defined(SPI_SLAVE_ENABLE) && defined(SPI0_SLAVE_ENABLE)
	#error Either declare SPI_SLAVE_ENABLE or SPI0_SLAVE_ENABLE to configure SPI0
#endif

#ifdef SPI_SLAVE_ENABLE
	#define SPI0_SLAVE_ENABLE SPI_SLAVE_ENABLE
#endif

// Declare Macros for Timer Delay supported by controller/platform
#define PLATFORM_SUPPORT_TIMER_DELAY
#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2
#define TIMER_3 3
#define TIMER_4 4

#ifndef DISABLE_OS
	#ifdef OS_TIMER_TYPE_CONF
		#define OS_TIMER_TYPE TIMER_TYPE_CONF
	#else
		#define OS_TIMER_TYPE TIMER_4
	#endif

	#if OS_TIMER_TYPE != TIMER_0
		#define TIMER_DELAY0 TIMER_0
	#endif

	#if OS_TIMER_TYPE != TIMER_1
		#define TIMER_DELAY1 TIMER_1
	#endif

	#if OS_TIMER_TYPE != TIMER_2
		#define TIMER_DELAY2 TIMER_2
	#endif
	
	#if OS_TIMER_TYPE != TIMER_3
		#define TIMER_DELAY3 TIMER_3
	#endif
	
	#if OS_TIMER_TYPE != TIMER_4
		#define TIMER_DELAY4 TIMER_4
	#endif

#else
	#define TIMER_DELAY0 TIMER_0
	#define TIMER_DELAY1 TIMER_1
	#define TIMER_DELAY2 TIMER_2
	#define TIMER_DELAY3 TIMER_3
	#define TIMER_DELAY4 TIMER_4
#endif

// Declare Macros for External Interrupt supported by controller/platform
#define PLATFORM_SUPPORT_EXTERNAL_INTERRUPT
#define EXTERNAL_INTERRUPT0_AVAILABLE
#define EXTERNAL_INTERRUPT1_AVAILABLE


#endif