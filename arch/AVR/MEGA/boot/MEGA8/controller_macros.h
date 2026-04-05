/**\addtogroup headers_MEGA8
* @{ \defgroup controller_macros_MEGA8_h
* @{
* \brief Documentation of controller_macros header file for atmega8 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header file declares controller specific macros for atmega8 controller.
* @}
*/

#ifndef CONTROLLER_MACROS_H
#define CONTROLLER_MACROS_H

#define PLATFORM_SUPPORT_NO_OS_INIT	// Inform OS that controller can run without OS functionality

/**
* \name MACROS : Mega8 Digital IOs Macros declaration
* \brief Defines OS and platform dependent MACROS for digital pins
* @{
*/

#define PLATFORM_SUPPORT_DIGITAL_IO	// Inform OS that controller supports Digital IOs
#define PORTB_AVAILABLE	// Inform Digital driver that the controller has PORTB
#define PORTC_AVAILABLE	// Inform Digital driver that the controller has PORTC
#define PORTD_AVAILABLE	// Inform Digital driver that the controller has PORTD

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

// User can define custom OS pins for easy portability
// By defining macro DEFINE_CUSTOM_OS_PINS in config.h
// and then defining custom OS pins
#ifndef DEFINE_CUSTOM_OS_PINS

// if not then Default OS IO pins are mapped with controller IO pins as below:
#define OS_PIN0 D0
#define OS_PIN1 D1
#define OS_PIN2 D2
#define OS_PIN3 D3
#define OS_PIN4 D4
#define OS_PIN5 D5
#define OS_PIN6 D6
#define OS_PIN7 D7

#define OS_PIN8	B0
#define OS_PIN9 B1
#define OS_PIN10 B2
#define OS_PIN11 B3
#define OS_PIN12 B4
#define OS_PIN13 B5

#define OS_PIN14 C0
#define OS_PIN15 C1
#define OS_PIN16 C2
#define OS_PIN17 C3
#define OS_PIN18 C4
#define OS_PIN19 C5

#define OS_PIN20 B6
#define OS_PIN21 B7
#endif	//DEFINE_CUSTOM_OS_PINS
///@}

/**
* \name MACROS : Mega8 UART Driver Macros declaration
* \brief Defines OS and platform dependent MACROS for UART Driver
* @{
*/

// Declare Macros for Serial UART supported by controller/platform
#define PLATFORM_SUPPORT_UART	// Inform OS that controller support UART Peripheral
#define PLATFORM_SUPPORT_CONST_PRINT	// Inform OS that controller supports const print feature of AVR
#define UART_AVAILABLE	// Inform Digital driver that the controller has UART

#define UART_PORT 0
// UART0 to UART3 ports are not available in atmega8 controller
//#define UART0_PORT 1
//#define UART1_PORT 2
//#define UART2_PORT 3
//#define UART3_PORT 4

// No need to define uart interrupts, they are default values of UART driver
//#define MCU_UART_RX_INTERRUPT_ISR		USART_RXC_vect
//#define MCU_UART_TX_INTERRUPT_ISR1	USART_UDRE_vect

// Check if user has configured SERIAL_PORT in config.h file
#ifdef SERIAL_PORT_CONF
#define SERIAL_PORT SERIAL_PORT_CONF
#endif //SERIAL_PORT_CONF

// Check if user has configured SERIAL_PORT0 in config.h file
#ifdef SERIAL0_PORT_CONF
#define SERIAL0_PORT SERIAL0_PORT_CONF
#endif //SERIAL0_PORT_CONF

// If SERIAL_PORT macro is defined above assign it SERIAL0_PORT macro
#ifdef SERIAL_PORT
#define SERIAL0_PORT SERIAL_PORT
#endif //SERIAL_PORT

// If SERIAL0_PORT macro is not defined assign it UART_PORT by default
#ifndef SERIAL0_PORT
#define SERIAL0_PORT UART_PORT
#endif //SERIAL0_PORT

// Checking for repeat declaration of SERIAL_ENABLE and SERIAL0_ENABLE
#if defined(SERIAL_ENABLE) && defined(SERIAL0_ENABLE)
#error Either declare SERIAL_ENABLE or SERIAL0_ENABLE to configure Serial UART0
#endif

// If SERIAL_ENABLE macro is defined above assign it SERIAL0_ENABLE macro
#ifdef SERIAL_ENABLE
#define SERIAL0_ENABLE SERIAL_ENABLE
#endif //SERIAL_ENABLE

// Checking for repeat declaration of SERIAL_RX_BUFFER_CONF and SERIAL0_RX_BUFFER_CONF
#if defined(SERIAL_RX_BUFFER_CONF) && defined(SERIAL0_RX_BUFFER_CONF)
#error Either declare SERIAL_RX_BUFFER_CONF or SERIAL0_RX_BUFFER_CONF to configure Serial UART0 RX Buffer size
#endif

// If SERIAL_RX_BUFFER_CONF macro is defined above assign it SERIAL0_RX_BUFFER_CONF macro
#ifdef SERIAL_RX_BUFFER_CONF
#define SERIAL0_RX_BUFFER_CONF SERIAL_RX_BUFFER_CONF
#endif //SERIAL_RX_BUFFER_CONF

// Checking for repeat declaration of SERIAL_TX_BUFFER_CONF and SERIAL0_TX_BUFFER_CONF
#if defined(SERIAL_TX_BUFFER_CONF) && defined(SERIAL0_TX_BUFFER_CONF)
#error Either declare SERIAL_TX_BUFFER_CONF or SERIAL0_TX_BUFFER_CONF to configure Serial UART0 TX Buffer size
#endif

// If SERIAL_TX_BUFFER_CONF macro is defined above assign it SERIAL0_TX_BUFFER_CONF macro
#ifdef SERIAL_TX_BUFFER_CONF
#define SERIAL0_TX_BUFFER_CONF SERIAL_TX_BUFFER_CONF
#endif //SERIAL_TX_BUFFER_CONF

// Checking for repeat declaration of SERIAL_TX_DISABLE and SERIAL0_TX_DISABLE
#if defined(SERIAL_TX_DISABLE) && defined(SERIAL0_TX_DISABLE)
#error Either declare SERIAL_TX_DISABLE or SERIAL0_TX_DISABLE to configure Serial UART0
#endif

// If SERIAL_TX_DISABLE macro is defined above assign it SERIAL0_TX_DISABLE macro
#ifdef SERIAL_TX_DISABLE
#define SERIAL0_TX_DISABLE SERIAL_TX_DISABLE
#endif //SERIAL_TX_DISABLE

// Checking for repeat declaration of SERIAL_RX_DISABLE and SERIAL0_RX_DISABLE
#if defined(SERIAL_RX_DISABLE) && defined(SERIAL0_RX_DISABLE)
#error Either declare SERIAL_RX_DISABLE or SERIAL0_RX_DISABLE to configure Serial UART0
#endif

// If SERIAL_RX_DISABLE macro is defined above assign it SERIAL0_RX_DISABLE macro
#ifdef SERIAL_RX_DISABLE
#define SERIAL0_RX_DISABLE SERIAL_RX_DISABLE
#endif //SERIAL_RX_DISABLE

// Checking for repeat declaration of SERIAL_TX_INTERRUPT_ENABLE and SERIAL0_TX_INTERRUPT_ENABLE
#if defined(SERIAL_TX_INTERRUPT_ENABLE) && defined(SERIAL0_TX_INTERRUPT_ENABLE)
#error Either declare SERIAL_TX_INTERRUPT_ENABLE or SERIAL0_TX_INTERRUPT_ENABLE to configure Serial UART0
#endif

// If SERIAL_TX_INTERRUPT_ENABLE macro is defined above assign it SERIAL0_TX_INTERRUPT_ENABLE macro
#ifdef SERIAL_TX_INTERRUPT_ENABLE
#define SERIAL0_TX_INTERRUPT_ENABLE SERIAL_TX_INTERRUPT_ENABLE
#endif //SERIAL_TX_INTERRUPT_ENABLE

// Declare default RX and TX buffer sizes for atmega8 for UART
// If user declares UARTx_UART_RX_BUFFER_CONF or UARTx_TX_BUFFER_CONF Macros in config.h
// then respective UART drivers will give precendence to user defined settings
// x = 0 / 1 / 2 / 3

#define UART_RX_BUFFER_DEFAULT 64	// Default UART RX buffer size, can be overridden by user
#define UART_TX_BUFFER_DEFAULT 64	// Default UART TX buffer size, can be overridden by user
///@}

/**
* \name MACROS : Mega8 ADC Driver Macros declaration
* \brief Defines OS and platform dependent MACROS for ADC Driver
* @{
*/
#define PLATFORM_SUPPORT_ANALOGINPUT	// Inform OS that controller support Analog to Digital conversion
#define ANALOGINPUT_TYPE0	// For easier implementation of ADC driver in AVR, ADC types are defined, Mega8 supports types
#define AI_REF_INTERNAL2v56 3
#define AI_CHANNEL_BG1V3 14
#define AI_CHANNEL_BG0V0 15
///@}

/**
* \name MACROS : Mega8 I2C Driver Macros declaration
* \brief Defines OS and platform dependent MACROS for I2C Driver
* @{
*/
#define PLATFORM_SUPPORT_I2C	// Inform OS that controller support I2C Peripheral
#define I2C_AVAILABLE	// Inform I2C driver that the controller has I2C

#define I2C_SLAVE_RX_BUFFER_DEFAULT	64	// Default I2C SLAVE RX buffer size, can be overridden by user
#define I2C_SLAVE_TX_BUFFER_DEFAULT	64	// Default I2C SLAVE TX buffer size, can be overridden by user
///@}

/**
* \name MACROS : Mega8 I2C Driver Macros declaration
* \brief Defines OS and platform dependent MACROS for I2C Driver
* @{
*/
#define PLATFORM_SUPPORT_SPI	// Inform OS that controller support SPI Peripheral
#define SPI_AVAILABLE	// Inform SPI driver that the controller has SPI

// Inform SPI driver Pin mapping of SPI port
#define SPI_SS_PIN	B2
#define SPI_MOSI_PIN	B3
#define SPI_MISO_PIN	B4
#define SPI_SCK_PIN	B5

#define SPI_PORT 	0
// SPI0_PORT & SPI1_PORT is not available in atmega8 controller
// #define SPI1_PORT 1
// #define SPI2_PORT 2

// If SPI_PORT_CONF macro is defined assign it to SPI0_PORT_CONF macro
#ifdef SPI_PORT_CONF
#define SPI0_PORT_CONF SPI_PORT_CONF
#endif //SPI_PORT_CONF

// If SPI0_PORT_CONF macro is not defined assign it to SPI_PORT by default
#ifndef SPI0_PORT_CONF
#define SPI0_PORT_CONF SPI_PORT
#endif //SPI0_PORT_CONF

// Checking for repeat declaration of SPI_SLAVE_ENABLE and SPI0_SLAVE_ENABLE
#if defined(SPI_SLAVE_ENABLE) && defined(SPI0_SLAVE_ENABLE)
#error Either declare SPI_SLAVE_ENABLE or SPI0_SLAVE_ENABLE to configure SPI0
#endif

// If SPI_SLAVE_ENABLE macro is defined assign it to SPI0_SLAVE_ENABLE macro
#ifdef SPI_SLAVE_ENABLE
#define SPI0_SLAVE_ENABLE SPI_SLAVE_ENABLE
#endif //SPI_SLAVE_ENABLE
///@}

/**
* \name MACROS : Mega8 Timer Driver Macros declaration
* \brief Defines OS and platform dependent MACROS for Timer Driver
* @{
*/
#define PLATFORM_SUPPORT_TIMER_DELAY	// Inform OS that controller support Timer Peripheral
#define TIMER0_AVAILABLE	// Inform Timer driver that the controller has TIMER0
#define TIMER1_AVAILABLE	// Inform Timer driver that the controller has TIMER1
#define TIMER2_AVAILABLE	// Inform Timer driver that the controller has TIMER2

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

#ifndef DISABLE_OS	// If OS functionality is not disabled
#ifdef OS_TIMER_TYPE_CONF	// If user has configured OS_TIMER in config.h file
#define OS_TIMER_TYPE OS_TIMER_TYPE_CONF	// Assign OS TIMER to user configured timer
#else
#define OS_TIMER_TYPE TIMER_1	// Else by default assign it TIMER_1
#endif //OS_TIMER_TYPE_CONF

// If OS Timer is not using it, TIMER0 can be used in TimerDelay driver
#if OS_TIMER_TYPE != TIMER_0
#define TIMER_DELAY0 TIMER_0
#endif

// If OS Timer is not using it, TIMER1 can be used in TimerDelay driver
#if OS_TIMER_TYPE != TIMER_1
#define TIMER_DELAY1 TIMER_1
#endif

// If OS Timer is not using it, TIMER2 can be used in TimerDelay driver
#if OS_TIMER_TYPE != TIMER_2
#define TIMER_DELAY2 TIMER_2
#endif //DISABLE_OS

#else	// If OS functionality is disabled all timers can be used in TimerDelay driver
#define TIMER_DELAY0 TIMER_0
#define TIMER_DELAY1 TIMER_1
#define TIMER_DELAY2 TIMER_2
#endif
///@}

/**
* \name MACROS : Mega8 External Interrupt Driver Macros declaration
* \brief Defines OS and platform dependent MACROS for External Interrupt Driver
* @{
*/
// Declare Macros for External Interrupt supported by controller/platform
#define PLATFORM_SUPPORT_EXTERNAL_INTERRUPT
#define EXTERNAL_INTERRUPT0_AVAILABLE
#define EXTERNAL_INTERRUPT1_AVAILABLE
///@}

#endif	//CONTROLLER_MACROS_H