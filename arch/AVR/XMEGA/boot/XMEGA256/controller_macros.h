/** \addtogroup XMEGA256
* @{ \defgroup controller_macros_xmega256_h
* @{ \addtogroup controller_macros_xmega256_h
* @{
* \brief Documentaion of controller_macros header file for atxmega256 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header file declares controller specific macros for atxmega256 controller.
* @}
*/

#ifndef CONTROLLER_MACROS_H
#define CONTROLLER_MACROS_H

// Declare Macros for digital input outputs supported by controller/platform
#define PLATFORM_SUPPORT_DIGITAL_IO
#define PORTA_AVAILABLE
#define PORTB_AVAILABLE
#define PORTC_AVAILABLE
#define PORTD_AVAILABLE
#define PORTE_AVAILABLE
#define PORTF_AVAILABLE
#define PORTR_AVAILABLE

// controller digital IO PIN declaration
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

#define R0 0x40
#define R1 0x41

// Os IO pin mapping with controller IO pins

#define P0 A0
#define P1 A1
#define P2 A2
#define P3 A3
#define P4 A4
#define P5 A5
#define P6 A6
#define P7 A7

#define P8 B0
#define P9 B1
#define P10 B2
#define P11 B3
#define P12 B4
#define P13 B5
#define P14 B6
#define P15 B7

#define P16 C0
#define P17 C1
#define P18 C2
#define P19 C3
#define P20 C4
#define P21 C5
#define P22 C6
#define P23 C7

#define P24 D0
#define P25 D1
#define P26 D2
#define P27 D3
#define P28 D4
#define P29 D5
#define P30 D6
#define P31 D7

#define P32 E0
#define P33 E1
#define P34 E2
#define P35 E3
#define P36 E4
#define P37 E5
#define P38 E6
#define P39 E7

#define P40 F0
#define P41 F1
#define P42 F2
#define P43 F3
#define P44 F4
#define P45 F5
#define P46 F6
#define P47 F7

#define P48 R0
#define P49 R1

// Declare Macros for Serial UART supported by controller/platform
#define PLATFORM_SUPPORT_UART
#define UARTC0_AVAILABLE
#define UARTC1_AVAILABLE
#define UARTD0_AVAILABLE
#define UARTD1_AVAILABLE
#define UARTE0_AVAILABLE
#define UARTE1_AVAILABLE
#define UARTF0_AVAILABLE

#define UARTC0_PORT 0
#define UARTC1_PORT 1
#define UARTD0_PORT 2
#define UARTD1_PORT 3
#define UARTE0_PORT 4
#define UARTE1_PORT 5
#define UARTF0_PORT 6

// If not declared in config.h, declare default OS serial port numbers to controller UART ports
#ifdef SERIAL_PORT
	#define SERIAL0_PORT SERIAL_PORT
#endif

#ifndef SERIAL0_PORT
	#define SERIAL0_PORT UARTC0_PORT
#endif

#ifndef SERIAL1_PORT
	#define SERIAL1_PORT UARTC1_PORT
#endif

#ifndef SERIAL2_PORT
	#define SERIAL2_PORT UARTD0_PORT
#endif

#ifndef SERIAL3_PORT
	#define SERIAL3_PORT UARTD1_PORT
#endif

#ifndef SERIAL4_PORT
	#define SERIAL4_PORT UARTE0_PORT
#endif

#ifndef SERIAL5_PORT
	#define SERIAL5_PORT UARTE1_PORT
#endif

#ifndef SERIAL6_PORT
	#define SERIAL6_PORT UARTF0_PORT
#endif

#ifndef SERIAL7_PORT
	#define SERIAL7_PORT UARTF1_PORT
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

// Declare default RX and TX buffer sizeS for atxmega256 for
// UARTC0, UARTC1, UARTD0, UARTD1, UARTE0, UARTE1, UARTF0, UARTF1
// If user declares UARTxx_RX_BUFFER_CONF or UARTxx_TX_BUFFER_CONF Macros in config.h
// then respective UART drivers will give precendence to user defined settings
// xx = C0 / C1 / D0 / D1 / E0 / E1 / F0 / F1

#define UARTC0_RX_BUFFER_DEFAULT 256
#define UARTC0_TX_BUFFER_DEFAULT 256
#define UARTC1_RX_BUFFER_DEFAULT 256
#define UARTC1_TX_BUFFER_DEFAULT 256

#define UARTD0_RX_BUFFER_DEFAULT 256
#define UARTD0_TX_BUFFER_DEFAULT 256
#define UARTD1_RX_BUFFER_DEFAULT 256
#define UARTD1_TX_BUFFER_DEFAULT 256

#define UARTE0_RX_BUFFER_DEFAULT 256
#define UARTE0_TX_BUFFER_DEFAULT 256
#define UARTE1_RX_BUFFER_DEFAULT 256
#define UARTE1_TX_BUFFER_DEFAULT 256

#define UARTF0_RX_BUFFER_DEFAULT 256
#define UARTF0_TX_BUFFER_DEFAULT 256
#define UARTF1_RX_BUFFER_DEFAULT 256
#define UARTF1_TX_BUFFER_DEFAULT 256

// Declare Macros for I2C supported by controller/platform
#define PLATFORM_SUPPORT_I2C

// Declare Macros for External Interrupt supported by controller/platform
//#define EXTERNAL_INT_AVAILABLE 2

#endif