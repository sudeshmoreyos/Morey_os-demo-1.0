/**\addtogroup headers_MEGA2560
* @{ \defgroup controller_macros_MEGA2560_h
* @{
* \brief Documentaion of controller_macros header file for atmega2560 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header file declares controller specific macros for atmega2560 controller.
* @}
*/

#ifndef CONTROLLER_MACROS_H
#define CONTROLLER_MACROS_H

#define PLATFORM_SUPPORT_DIGITAL_IO
#define PORTA_AVAILABLE
#define PORTB_AVAILABLE
#define PORTC_AVAILABLE
#define PORTD_AVAILABLE
#define PORTE_AVAILABLE
#define PORTF_AVAILABLE
#define PORTG_AVAILABLE
#define PORTH_AVAILABLE
#define PORTJ_AVAILABLE
#define PORTK_AVAILABLE
#define PORTL_AVAILABLE

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
#define G5 0x35

#define H0 0x38
#define H1 0x39
#define H2 0x3A
#define H3 0x3B
#define H4 0x3C
#define H5 0x3D
#define H6 0x3E
#define H7 0x3F

#define J0 0x48
#define J1 0x49
#define J2 0x4A
#define J3 0x4B
#define J4 0x4C
#define J5 0x4D
#define J6 0x4E
#define J7 0x4F

#define K0 0x50
#define K1 0x51
#define K2 0x52
#define K3 0x53
#define K4 0x54
#define K5 0x55
#define K6 0x56
#define K7 0x57

#define L0 0x58
#define L1 0x59
#define L2 0x5A
#define L3 0x5B
#define L4 0x5C
#define L5 0x5D
#define L6 0x5E
#define L7 0x5F

// Os pin mapping with controller pins
#define P0 D0
#define P1 D1
#define P2 D2
#define P3 D3
#define P4 D4
#define P5 D5
#define P6 D6
#define P7 D7

#define P8 B0
#define P9 B1
#define P10 B2
#define P11 B3
#define P12 B4
#define P13 B5

#define P14 C0
#define P15 C1
#define P16 C2
#define P17 C3
#define P18 C4
#define P19 C5

#define P20 B6
#define P21 B7

// Declare Macros for Serial UART supported by controller/platform
#define PLATFORM_SUPPORT_UART
#define PLATFORM_SUPPORT_CONST_PRINT

#define UART0_AVAILABLE
#define UART1_AVAILABLE
#define UART2_AVAILABLE
#define UART3_AVAILABLE

#define UART0_PORT 1
#define UART1_PORT 2
#define UART2_PORT 3
#define UART3_PORT 4

// UART port is not available in atmega2560 controller
//#define UART_PORT 0


#ifdef SERIAL_PORT
	#define SERIAL0_PORT SERIAL_PORT
#endif

#ifndef SERIAL0_PORT
	#define SERIAL0_PORT UART0_PORT
#endif

#ifndef SERIAL1_PORT
	#define SERIAL1_PORT UART1_PORT
#endif

#ifndef SERIAL2_PORT
	#define SERIAL2_PORT UART2_PORT
#endif

#ifndef SERIAL3_PORT
	#define SERIAL3_PORT UART3_PORT
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

#define UART0_RX_BUFFER_DEFAULT 256
#define UART0_TX_BUFFER_DEFAULT 256

#define UART1_RX_BUFFER_DEFAULT 256
#define UART1_TX_BUFFER_DEFAULT 256

#define UART2_RX_BUFFER_DEFAULT 256
#define UART2_TX_BUFFER_DEFAULT 256

#define UART3_RX_BUFFER_DEFAULT 256
#define UART3_TX_BUFFER_DEFAULT 256

// Declare Macros for I2C supported by controller/platform
#define PLATFORM_SUPPORT_I2C
#define I2C_AVAILABLE

#define I2C_SLAVE_RX_BUFFER_DEFAULT 256
#define I2C_SLAVE_TX_BUFFER_DEFAULT 256

// Declare Macros for SPI supported by controller/platform
#define PLATFORM_SUPPORT_SPI
#define SPI_AVAILABLE
#define SPI_SS_PIN B0
#define SPI_SCK_PIN	B1
#define SPI_MOSI_PIN B2
#define SPI_MISO_PIN B3

// Declare Macros for Analog Input supported by controller/platform
#define PLATFORM_SUPPORT_ANALOGINPUT
#define ANALOGINPUT_TYPE1
#define AI_REF_INTERNAL1v1 3
#define AI_CHANNEL_BG1V1 14
#define AI_CHANNEL_BG0V0 15

// Declare Macros for Timer Delay supported by controller/platform
#define PLATFORM_SUPPORT_TIMER_DELAY
#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2
#define TIMER_3 3
#define TIMER_4 4
#define TIMER_5 5

#ifndef DISABLE_OS
	#ifdef OS_TIMER_TYPE_CONF
		#define OS_TIMER_TYPE TIMER_TYPE_CONF
	#else
		#define OS_TIMER_TYPE TIMER_5
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
	
	#if OS_TIMER_TYPE != TIMER_5
		#define TIMER_DELAY5 TIMER_5
	#endif

#else
	#define TIMER_DELAY0 TIMER_0
	#define TIMER_DELAY1 TIMER_1
	#define TIMER_DELAY2 TIMER_2
	#define TIMER_DELAY3 TIMER_3
	#define TIMER_DELAY4 TIMER_4
	#define TIMER_DELAY5 TIMER_5
#endif


// Declare Macros for External Interrupt supported by controller/platform
#define PLATFORM_SUPPORT_EXTERNAL_INTERRUPT
#define EXTERNAL_INTERRUPT0_AVAILABLE
#define EXTERNAL_INTERRUPT1_AVAILABLE


#endif