// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Serial.h"
#include "SpiMaster.h"
#include "Digital.h"
#include "util/delay_blocking.h"

// macros
#define CS_ENABLE()     Digital.write(B2,LOW)
#define CS_DISABLE()    Digital.write(B2,HIGH)

// command definitions
#define CMD0                0
#define CMD0_ARG            0x00000000
#define CMD0_CRC            0x94
#define CMD8                8
#define CMD8_ARG            0x0000001AA
#define CMD8_CRC            0x86
#define CMD55               55
#define CMD55_ARG           0x00000000
#define CMD55_CRC           0x00
#define CMD58               58
#define CMD58_ARG           0x00000000
#define CMD58_CRC           0x00
#define ACMD41              41
#define ACMD41_ARG          0x40000000
#define ACMD41_CRC          0x00

// R1 responses
#define PARAM_ERROR(X)      X & 0b01000000
#define ADDR_ERROR(X)       X & 0b00100000
#define ERASE_SEQ_ERROR(X)  X & 0b00010000
#define CRC_ERROR(X)        X & 0b00001000
#define ILLEGAL_CMD(X)      X & 0b00000100
#define ERASE_RESET(X)      X & 0b00000010
#define IN_IDLE(X)          X & 0b00000001

#define POWER_UP_STATUS(X)  X & 0x40
#define CCS_VAL(X)          X & 0x40
#define VDD_2728(X)         X & 0b10000000
#define VDD_2829(X)         X & 0b00000001
#define VDD_2930(X)         X & 0b00000010
#define VDD_3031(X)         X & 0b00000100
#define VDD_3132(X)         X & 0b00001000
#define VDD_3233(X)         X & 0b00010000
#define VDD_3334(X)         X & 0b00100000
#define VDD_3435(X)         X & 0b01000000
#define VDD_3536(X)         X & 0b10000000

// R7 responses
#define CMD_VER(X)          ((X >> 4) & 0xF0)
#define VOL_ACC(X)          (X & 0x1F)
#define VOLTAGE_ACC_27_33   0b00000001
#define VOLTAGE_ACC_LOW     0b00000010
#define VOLTAGE_ACC_RES1    0b00000100
#define VOLTAGE_ACC_RES2    0b00001000

// UART functions
void UART_init(mos_uint16_t baudRate);
void UART_putc(unsigned char data);
void UART_puts(char* charString);
void UART_puthex8(mos_uint8_t val);
unsigned char UART_getc(void);

// SPI functions
mos_uint8_t SPI_transfer(mos_uint8_t data);

// SD functions
void SD_powerUpSeq(void);
void SD_command(mos_uint8_t cmd, mos_uint32_t arg, mos_uint8_t crc);
mos_uint8_t SD_readRes1(void);
void SD_readRes3_7(mos_uint8_t *res);
mos_uint8_t SD_goIdleState(void);
void SD_sendIfCond(mos_uint8_t *res);
void SD_readOCR(mos_uint8_t *res);
mos_uint8_t SD_sendApp(void);
mos_uint8_t SD_sendOpCond(void);
void SD_printR1(mos_uint8_t res);
void SD_printR3(mos_uint8_t *res);
void SD_printR7(mos_uint8_t *res);

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	SpiMaster.begin(B2);
	Serial.begin(9600,SERIAL_8N1);
}

// Delcare all processes here
TASK_CREATE(test1,"test-1");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&test1);

TASK_RUN(test1)
{
    // array to hold responses
    static mos_uint8_t res[5];
	
    // received char from UART
    static char c;
	
	BEGIN();

    // start power up sequence
    SD_powerUpSeq();
	
	// print menu
	UART_puts("MENU\r\n");
	UART_puts("------------------\r\n");
	UART_puts("0 - Send CMD0\r\n1 - Send CMD8\r\n2 - Send CMD58\r\n");
	UART_puts("3 - Send CMD55\r\n4 - Send ACMD41\r\n");
	UART_puts("------------------\r\n");

		
    while(1)
    {	
		if(Serial.available())
		{
				// get character from user
			c = UART_getc();

			if(c == '0')
			{
				// command card to idle
				UART_puts("Sending CMD0...\r\n");
				res[0] = SD_goIdleState();
				UART_puts("Response:\r\n");
				SD_printR1(res[0]);
			}
			else if(c == '1')
			{
				// send if conditions
				UART_puts("Sending CMD8...\r\n");
				SD_sendIfCond(res);
				UART_puts("Response:\r\n");
				SD_printR7(res);
			}
			else if(c == '2')
			{
				// send if conditions
				UART_puts("Sending CMD58...\r\n");
				SD_readOCR(res);
				UART_puts("Response:\r\n");
				SD_printR3(res);
			}
			else if(c == '3')
			{
				// command card to idle
				UART_puts("Sending CMD55...\r\n");
				res[0] = SD_sendApp();
				UART_puts("Response:\r\n");
				SD_printR1(res[0]);
			}
			else if(c == '4')
			{
				// command card to idle
				UART_puts("Sending ACMD41...\r\n");
				res[0] = SD_sendOpCond();
				UART_puts("Response:\r\n");
				SD_printR1(res[0]);
			}
			else
			{
				UART_puts("Unrecognized command\r\n");
			}				
			
		}
			
		DELAY_SEC(0.1);
    }

	END();
}

mos_uint8_t SPI_transfer(mos_uint8_t data)
{
	return SpiMaster.transfer(data);
}

void SD_powerUpSeq()
{
    // make sure card is deselected
    CS_DISABLE();

    // give SD card time to power up
    delay_ms_blocking(1);

    // select SD card
    SPI_transfer(0xFF);
    CS_ENABLE();

    // send 74 clock cycles to synchronize
    for(mos_uint8_t i = 0; i < 74; i++)
        SPI_transfer(0xFF);

    // deselect SD card
    CS_DISABLE();
    SPI_transfer(0xFF);
}

void SD_command(mos_uint8_t cmd, mos_uint32_t arg, mos_uint8_t crc)
{
    // transmit command to sd card
    SPI_transfer(cmd|0x40);

    // transmit argument
    SPI_transfer((mos_uint8_t)(arg >> 24));
    SPI_transfer((mos_uint8_t)(arg >> 16));
    SPI_transfer((mos_uint8_t)(arg >> 8));
    SPI_transfer((mos_uint8_t)(arg));

    // transmit crc
    SPI_transfer(crc|0x01);
}

mos_uint8_t SD_readRes1()
{
    mos_uint8_t i = 0, res1;

    // keep polling until actual data received
    while((res1 = SPI_transfer(0xFF)) == 0xFF)
    {
        i++;

        // if no data received for 8 bytes, break
        if(i > 8) break;
    }

    return res1;
}

void SD_readRes3_7(mos_uint8_t *res)
{
    // read response 1 in R7
    res[0] = SD_readRes1();

    // if error reading R1, return
    if(res[0] > 1) return;

    // read remaining bytes
    res[1] = SPI_transfer(0xFF);
    res[2] = SPI_transfer(0xFF);
    res[3] = SPI_transfer(0xFF);
    res[4] = SPI_transfer(0xFF);
}

mos_uint8_t SD_goIdleState()
{
    // assert chip select
    SPI_transfer(0xFF);
    CS_ENABLE();
    SPI_transfer(0xFF);

    // send CMD0
    SD_command(CMD0, CMD0_ARG, CMD0_CRC);

    // read response
    mos_uint8_t res1 = SD_readRes1();

    // deassert chip select
    SPI_transfer(0xFF);
    CS_DISABLE();
    SPI_transfer(0xFF);

    return res1;
}

void SD_sendIfCond(mos_uint8_t *res)
{
    // assert chip select
    SPI_transfer(0xFF);
    CS_ENABLE();
    SPI_transfer(0xFF);

    // send CMD8
    SD_command(CMD8, CMD8_ARG, CMD8_CRC);

    // read response
    SD_readRes3_7(res);

    // deassert chip select
    SPI_transfer(0xFF);
    CS_DISABLE();
    SPI_transfer(0xFF);
}

void SD_readOCR(mos_uint8_t *res)
{
    // assert chip select
    SPI_transfer(0xFF);
    CS_ENABLE();
    SPI_transfer(0xFF);

    // send CMD58
    SD_command(CMD58, CMD58_ARG, CMD58_CRC);

    // read response
    SD_readRes3_7(res);

    // deassert chip select
    SPI_transfer(0xFF);
    CS_DISABLE();
    SPI_transfer(0xFF);
}

mos_uint8_t SD_sendApp()
{
    // assert chip select
    SPI_transfer(0xFF);
    CS_ENABLE();
    SPI_transfer(0xFF);

    // send CMD0
    SD_command(CMD55, CMD55_ARG, CMD55_CRC);

    // read response
    mos_uint8_t res1 = SD_readRes1();

    // deassert chip select
    SPI_transfer(0xFF);
    CS_DISABLE();
    SPI_transfer(0xFF);

    return res1;
}

mos_uint8_t SD_sendOpCond()
{
    // assert chip select
    SPI_transfer(0xFF);
    CS_ENABLE();
    SPI_transfer(0xFF);

    // send CMD0
    SD_command(ACMD41, ACMD41_ARG, ACMD41_CRC);

    // read response
    mos_uint8_t res1 = SD_readRes1();

    // deassert chip select
    SPI_transfer(0xFF);
    CS_DISABLE();
    SPI_transfer(0xFF);

    return res1;
}

void SD_printR1(mos_uint8_t res)
{
    if(res & 0b10000000)
        UART_puts("\tError: MSB = 1\r\n");
    if(res == 0)
        UART_puts("\tCard Ready\r\n");
    if(PARAM_ERROR(res))
        UART_puts("\tParameter Error\r\n");
    if(ADDR_ERROR(res))
        UART_puts("\tAddress Error\r\n");
    if(ERASE_SEQ_ERROR(res))
        UART_puts("\tErase Sequence Error\r\n");
    if(CRC_ERROR(res))
        UART_puts("\tCRC Error\r\n");
    if(ILLEGAL_CMD(res))
        UART_puts("\tIllegal Command\r\n");
    if(ERASE_RESET(res))
        UART_puts("\tErase Reset Error\r\n");
    if(IN_IDLE(res))
        UART_puts("\tIn Idle State\r\n");
}

void SD_printR3(mos_uint8_t *res)
{
    SD_printR1(res[0]);

    if(res[0] > 1) return;

    UART_puts("\tCard Power Up Status: ");
    if(POWER_UP_STATUS(res[1]))
    {
        UART_puts("READY\r\n");
        UART_puts("\tCCS Status: ");
        if(CCS_VAL(res[1])){ UART_puts("1\r\n"); }
        else UART_puts("0\r\n");
    }
    else
    {
        UART_puts("BUSY\r\n");
    }

    UART_puts("\tVDD Window: ");
    if(VDD_2728(res[3])) UART_puts("2.7-2.8, ");
    if(VDD_2829(res[2])) UART_puts("2.8-2.9, ");
    if(VDD_2930(res[2])) UART_puts("2.9-3.0, ");
    if(VDD_3031(res[2])) UART_puts("3.0-3.1, ");
    if(VDD_3132(res[2])) UART_puts("3.1-3.2, ");
    if(VDD_3233(res[2])) UART_puts("3.2-3.3, ");
    if(VDD_3334(res[2])) UART_puts("3.3-3.4, ");
    if(VDD_3435(res[2])) UART_puts("3.4-3.5, ");
    if(VDD_3536(res[2])) UART_puts("3.5-3.6");
    UART_puts("\r\n");
}

void SD_printR7(mos_uint8_t *res)
{
    SD_printR1(res[0]);

    if(res[0] > 1) return;

    UART_puts("\tCommand Version: ");
    UART_puthex8(CMD_VER(res[1]));
    UART_puts("\r\n");

    UART_puts("\tVoltage Accepted: ");
    if(VOL_ACC(res[3]) == VOLTAGE_ACC_27_33)
        UART_puts("2.7-3.6V\r\n");
    else if(VOL_ACC(res[3]) == VOLTAGE_ACC_LOW)
        UART_puts("LOW VOLTAGE\r\n");
    else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES1)
        UART_puts("RESERVED\r\n");
    else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES2)
        UART_puts("RESERVED\r\n");
    else UART_puts("NOT DEFINED\r\n");

    UART_puts("\tEcho: ");
    UART_puthex8(res[4]);
    UART_puts("\r\n");
}

void UART_putc(unsigned char data)
{
	Serial.write(data);
}

void UART_puts(char* charString)
{
	Serial.print(charString);
}

void UART_puthex8(mos_uint8_t val)
{
    // extract upper and lower nibbles from input value
    mos_uint8_t upperNibble = (val & 0xF0) >> 4;
    mos_uint8_t lowerNibble = val & 0x0F;

    // convert nibble to its ASCII hex equivalent
    upperNibble += upperNibble > 9 ? 'A' - 10 : '0';
    lowerNibble += lowerNibble > 9 ? 'A' - 10 : '0';

    // print the characters
    UART_putc(upperNibble);
    UART_putc(lowerNibble);
}

unsigned char UART_getc(void)
{
	return Serial.read();
}
