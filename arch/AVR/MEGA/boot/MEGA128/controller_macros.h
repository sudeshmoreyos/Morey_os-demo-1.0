/**\addtogroup headers_MEGA128
* @{ \defgroup controller_macros_MEGA128
* @{
* \brief Documentaion of controller_macros header file for atmega128 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header file declares controller specific macros for atmega128 controller.
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
#define PORTA_AVAILABLE
#define PORTB_AVAILABLE
#define PORTC_AVAILABLE
#define PORTD_AVAILABLE
#define PORTE_AVAILABLE
#define PORTF_AVAILABLE
#define PORTG_AVAILABLE

// controller port declaration

#define A0 0x00
#define A1 0x01
#define A2 0x02
#define A3 0x03
#define A4 0x04
#define A5 0x05
#define A6 0x06
#define A7 0x07

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
#define C6 0x16
#define C7 0x17

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
#define E4 0x24
#define E5 0x25
#define E6 0x26
#define E7 0x27

#define F0 0x28
#define F1 0x29
#define F2 0x2A
#define F3 0x2B
#define F4 0x2C
#define F5 0x2D
#define F6 0x2E
#define F7 0x2F

#define G0 0x30
#define G1 0x31
#define G2 0x32
#define G3 0x33
#define G4 0x34

// User can define custom OS pins for easy portability
// By defining macro DEFINE_CUSTOM_OS_PINS in config.h
// and then defining custom OS pins
#ifndef DEFINE_CUSTOM_OS_PINS

	// if not Default OS pins are mapped with controller pins as below:
	#define OS_pin0 	A0
	#define OS_pin1 	A1
	#define OS_pin2 	A2
	#define OS_pin3 	A3
	#define OS_pin4 	A4
	#define OS_pin5 	A5
	#define OS_pin6 	A6
	#define OS_pin7 	A7

	#define OS_pin8 	B0
	#define OS_pin9 	B1
	#define OS_pin10 	B2
	#define OS_pin11 	B3
	#define OS_pin12 	B4
	#define OS_pin13 	B5
	#define OS_pin14 	B6
	#define OS_pin15 	B7
	
	#define OS_pin16 	C0
	#define OS_pin17 	C1
	#define OS_pin18 	C2
	#define OS_pin19 	C3
	#define OS_pin20	C4
	#define OS_pin21 	C5
	#define OS_pin22 	C6
	#define OS_pin23 	C7

	#define OS_pin24 	D0
	#define OS_pin25 	D1
	#define OS_pin26 	D2
	#define OS_pin27 	D3
	#define OS_pin28 	D4
	#define OS_pin29 	D5
	#define OS_pin30 	D6
	#define OS_pin31 	D7
	
	#define OS_pin32 	E0
	#define OS_pin33 	E1
	#define OS_pin34 	E2
	#define OS_pin35 	E3
	#define OS_pin36 	E4
	#define OS_pin37 	E5
	#define OS_pin38 	E6
	#define OS_pin39 	E7
	
	#define OS_pin40	F0
	#define OS_pin41 	F1
	#define OS_pin42 	F2
	#define OS_pin43 	F3
	#define OS_pin44 	F4
	#define OS_pin45 	F5
	#define OS_pin46 	F6
	#define OS_pin47 	F7
	
	#define OS_pin48 	G0
	#define OS_pin49 	G1
	#define OS_pin50 	G2
	#define OS_pin51 	G3
	#define OS_pin52 	G4
	
#endif
///@}

// Declare Macros for Serial UART supported by controller/platform
#define PLATFORM_SUPPORT_UART
#define PLATFORM_SUPPORT_CONST_PRINT

#define UART0_AVAILABLE
#define UART1_AVAILABLE

#define UART0_PORT 1
#define UART1_PORT 2

// UART port is not available in atmega2560 controller
//#define UART_PORT 0
//#define UART2_PORT 3
//#define UART3_PORT 4

// Throw error if both SERIAL_PORT_CONF and SERIAL0_PORT_CONF are defined
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
	#error Either declare SERIAL_ENABLE or SERIAL0_ENABLE to configure Serial UART0
#endif

#ifdef SERIAL_ENABLE
	#define SERIAL0_ENABLE SERIAL_ENABLE
#endif

// Checking for repeat declaration of SERIAL_RX_BUFFER_CONF and SERIAL0_RX_BUFFER_CONF
#if defined(SERIAL_RX_BUFFER_CONF) && defined(SERIAL0_RX_BUFFER_CONF)
	#error Either declare SERIAL_RX_BUFFER_CONF or SERIAL0_RX_BUFFER_CONF to configure Serial UART0 RX Buffer size
#endif

#ifdef SERIAL_RX_BUFFER_CONF
	#define SERIAL0_RX_BUFFER_CONF SERIAL_RX_BUFFER_CONF
#endif

// Checking for repeat declaration of SERIAL_TX_BUFFER_CONF and SERIAL0_TX_BUFFER_CONF
#if defined(SERIAL_TX_BUFFER_CONF) && defined(SERIAL0_TX_BUFFER_CONF)
	#error Either declare SERIAL_TX_BUFFER_CONF or SERIAL0_TX_BUFFER_CONF to configure Serial UART0 TX Buffer size
#endif

#ifdef SERIAL_TX_BUFFER_CONF
	#define SERIAL0_TX_BUFFER_CONF SERIAL_TX_BUFFER_CONF
#endif

// Checking for repeat declaration of SERIAL_TX_DISABLE and SERIAL0_TX_DISABLE
#if defined(SERIAL_TX_DISABLE) && defined(SERIAL0_TX_DISABLE)
	#error Either declare SERIAL_TX_DISABLE or SERIAL0_TX_DISABLE to configure Serial UART0
#endif

#ifdef SERIAL_TX_DISABLE
	#define SERIAL0_TX_DISABLE SERIAL_TX_DISABLE
#endif

// Checking for repeat declaration of SERIAL_RX_DISABLE and SERIAL0_RX_DISABLE
#if defined(SERIAL_RX_DISABLE) && defined(SERIAL0_RX_DISABLE)
	#error Either declare SERIAL_RX_DISABLE or SERIAL0_RX_DISABLE to configure Serial UART0
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

// Declare default RX and TX buffer sizes for atmega2560 for UART
// If user declares UARTx_UART_RX_BUFFER_CONF or UARTx_TX_BUFFER_CONF Macros in config.h
// then respective UART drivers will give precendence to user defined settings
// x = 0 / 1 / 2 / 3

#define UART0_RX_BUFFER_DEFAULT 128
#define UART0_TX_BUFFER_DEFAULT 128

#define UART1_RX_BUFFER_DEFAULT 128
#define UART1_TX_BUFFER_DEFAULT 128

// Declare Macros for I2C supported by controller/platform
#define PLATFORM_SUPPORT_I2C
#define I2C_AVAILABLE

#define I2C_SLAVE_RX_BUFFER_DEFAULT 128
#define I2C_SLAVE_TX_BUFFER_DEFAULT 128

// Declare Macros for SPI supported by controller/platform
#define PLATFORM_SUPPORT_SPI
#define SPI_AVAILABLE
#define SPI_SS_PIN 		B0
#define SPI_SCK_PIN		B1
#define SPI_MOSI_PIN 	B2
#define SPI_MISO_PIN 	B3

// Declare Macros for Analog Input supported by controller/platform
#define PLATFORM_SUPPORT_ANALOGINPUT
#define ANALOGINPUT_TYPE0
#define AI_REF_INTERNAL2v56 3
#define AI_CHANNEL_BG1V23 30
#define AI_CHANNEL_BG0V0 31

// Declare Macros for Timer Delay supported by controller/platform
#define PLATFORM_SUPPORT_TIMER_DELAY
#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2
#define TIMER_3 3

#ifndef DISABLE_OS
	#ifdef OS_TIMER_TYPE_CONF
		#define OS_TIMER_TYPE TIMER_TYPE_CONF
	#else
		#define OS_TIMER_TYPE TIMER_1
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

#else
	#define TIMER_DELAY0 TIMER_0
	#define TIMER_DELAY1 TIMER_1
	#define TIMER_DELAY2 TIMER_2
	#define TIMER_DELAY3 TIMER_3
#endif


// Declare Macros for External Interrupt supported by controller/platform
#define PLATFORM_SUPPORT_EXTERNAL_INTERRUPT
#define EXTERNAL_INTERRUPT0_AVAILABLE
#define EXTERNAL_INTERRUPT1_AVAILABLE


#endif