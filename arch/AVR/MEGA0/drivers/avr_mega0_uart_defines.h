#ifndef AVR_MEGA0_UART_DEFINES_H
#define AVR_MEGA0_UART_DEFINES_H

#define EVEN 0x02
#define ODD 0x01
#define NONE 0x00

#define SERIAL_8N1 ( (0x8 << 4) | ( NONE << 2 ) | 0x01 )
#define SERIAL_8E1 ( (0x8 << 4) | ( EVEN << 2 ) | 0x01 )
#define SERIAL_8O1 ( (0x8 << 4) | ( ODD << 2 ) | 0x01 )
#define SERIAL_7N1 ( (0x7 << 4) | ( NONE << 2 ) | 0x01 )
#define SERIAL_7E1 ( (0x7 << 4) | ( EVEN << 2 ) | 0x01 )
#define SERIAL_7O1 ( (0x7 << 4) | ( ODD << 2 ) | 0x01 )
#define SERIAL_6N1 ( (0x6 << 4) | ( NONE << 2 ) | 0x01 )
#define SERIAL_6E1 ( (0x6 << 4) | ( EVEN << 2 ) | 0x01 )
#define SERIAL_6O1 ( (0x6 << 4) | ( ODD << 2 ) | 0x01 )
#define SERIAL_5N1 ( (0x5 << 4) | ( NONE << 2 ) | 0x01 )
#define SERIAL_5E1 ( (0x5 << 4) | ( EVEN << 2 ) | 0x01 )
#define SERIAL_5O1 ( (0x5 << 4) | ( ODD << 2 ) | 0x01 )

#define SERIAL_8N2 ( (0x8 << 4) | ( NONE << 2 ) | 0x02 )
#define SERIAL_8E2 ( (0x8 << 4) | ( EVEN << 2 ) | 0x02 )
#define SERIAL_8O2 ( (0x8 << 4) | ( ODD << 2 ) | 0x02 )
#define SERIAL_7N2 ( (0x7 << 4) | ( NONE << 2 ) | 0x02 )
#define SERIAL_7E2 ( (0x7 << 4) | ( EVEN << 2 ) | 0x02 )
#define SERIAL_7O2 ( (0x7 << 4) | ( ODD << 2 ) | 0x02 )
#define SERIAL_6N2 ( (0x6 << 4) | ( NONE << 2 ) | 0x02 )
#define SERIAL_6E2 ( (0x6 << 4) | ( EVEN << 2 ) | 0x02 )
#define SERIAL_6O2 ( (0x6 << 4) | ( ODD << 2 ) | 0x02 )
#define SERIAL_5N2 ( (0x5 << 4) | ( NONE << 2 ) | 0x02 )
#define SERIAL_5E2 ( (0x5 << 4) | ( EVEN << 2 ) | 0x02 )
#define SERIAL_5O2 ( (0x5 << 4) | ( ODD << 2 ) | 0x02 )

#endif