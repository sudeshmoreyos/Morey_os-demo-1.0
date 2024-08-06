/**\addtogroup headers_MEGA8
* @{ \defgroup controller_macros_MEGA8_h
* @{
* \brief Documentaion of controller_macros header file for atmega8 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header file declares controller specific macros for atmega8 controller.
* @}
*/

#ifndef CONTROLLER_MACROS_H
#define CONTROLLER_MACROS_H

// Declare Macros for digital input outputs supported by controller/platform
#define PLATFORM_SUPPORT_DIGITAL_IO
#define PORTB_AVAILABLE
#define PORTC_AVAILABLE
#define PORTD_AVAILABLE

// controller digital IO PIN declaration
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

// Os IO pin mapping with controller IO pins
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
#define UART_AVAILABLE

#define UART_PORT 0

// No need to define uart interrupts, they are default values of UART driver
//#define MCU_UART_RX_INTERRUPT_ISR	USART_RXC_vect
//#define MCU_UART_TX_INTERRUPT_ISR1	USART_UDRE_vect

// UART0 to UART3 ports are not available in atmega8 controller
//#define UART0_PORT 1
//#define UART1_PORT 2
//#define UART2_PORT 3
//#define UART3_PORT 4

#ifdef SERIAL_PORT_CONF
	#define SERIAL_PORT SERIAL_PORT_CONF
#endif

#ifdef SERIAL0_PORT_CONF
	#define SERIAL0_PORT SERIAL0_PORT_CONF
#endif

#ifdef SERIAL_PORT
	#define SERIAL0_PORT SERIAL_PORT
#endif

#ifndef SERIAL0_PORT
	#define SERIAL0_PORT UART_PORT
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

// Declare default RX and TX buffer sizes for atmega8 for UART
// If user declares UARTx_UART_RX_BUFFER_CONF or UARTx_TX_BUFFER_CONF Macros in config.h
// then respective UART drivers will give precendence to user defined settings
// x = 0 / 1 / 2 / 3

#define UART_RX_BUFFER_DEFAULT 64
#define UART_TX_BUFFER_DEFAULT 64

// Declare Macros for Analog Input supported by controller/platform
#define PLATFORM_SUPPORT_ANALOGINPUT
#define ANALOGINPUT_TYPE0
#define AI_REF_INTERNAL2v56 3
#define AI_CHANNEL_BG1V3 14
#define AI_CHANNEL_BG0V0 15

// Declare Macros for I2C supported by controller/platform
#define PLATFORM_SUPPORT_I2C
#define I2C_AVAILABLE

#define I2C_SLAVE_RX_BUFFER_DEFAULT	64
#define I2C_SLAVE_TX_BUFFER_DEFAULT 	64

// Declare Macros for SPI supported by controller/platform
#define PLATFORM_SUPPORT_SPI
#define SPI_AVAILABLE

#define SPI_SS_PIN 	B2
#define SPI_MOSI_PIN 	B3
#define SPI_MISO_PIN 	B4
#define SPI_SCK_PIN	B5

#define SPI_PORT 	0

// SPI0_PORT & SPI1_PORT is not available in atmega8 controller
// #define SPI1_PORT 1
// #define SPI1_PORT 2

#ifdef SPI_PORT_CONF
	#define SPI0_PORT_CONF SPI_PORT_CONF
#endif

#ifndef SPI0_PORT_CONF
	#define SPI0_PORT_CONF SPI_PORT
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

#ifndef DISABLE_OS
	#ifdef OS_TIMER_TYPE_CONF
		#define OS_TIMER_TYPE OS_TIMER_TYPE_CONF
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

#else
	#define TIMER_DELAY0 TIMER_0
	#define TIMER_DELAY1 TIMER_1
	#define TIMER_DELAY2 TIMER_2
#endif

// Declare Macros for External Interrupt supported by controller/platform
#define PLATFORM_SUPPORT_EXTERNAL_INTERRUPT
#define EXTERNAL_INTERRUPT0_AVAILABLE
#define EXTERNAL_INTERRUPT1_AVAILABLE

#endif
