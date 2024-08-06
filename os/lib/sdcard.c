/*-----------------------------------------------------------------------*/
/* MMCv3/SDv1/SDv2 Controls via AVR SPI module                           */
/*-----------------------------------------------------------------------*/
/*
/  Copyright (C) 2016, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   any purpose as you like UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/

#include "sdcard.h"
#include "Digital.h"
#include "util/delay_blocking.h"

/* Peripheral controls (Platform dependent) */
#define CS_LOW()		spi_pointer_temp->slaveSelect();	/* Set MMC_CS = low */
#define	CS_HIGH()		spi_pointer_temp->slaveUnselect();	/* Set MMC_CS = high */
#define MMC_CD			1									/* Test if card detected.   yes:true, no:false, default:true */
#define MMC_WP			0									/* Test if write protected. yes:true, no:false, default:false */
#define	FCLK_SLOW()		spi_pointer_temp->config(SPI_SPEED_LOWEST,SPI_MODE0,SPI_MSB_FIRST)	/* Set SPI clock for initialization (100-400kHz) */
#define	FCLK_FAST()		spi_pointer_temp->config(SPI_SPEED_HIGHEST,SPI_MODE0,SPI_MSB_FIRST)  /* Set SPI clock for read/write (20MHz max) */


/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

/* Definitions for MMC/SDC command */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* ERASE_ER_BLK_START */
#define CMD33	(33)		/* ERASE_ER_BLK_END */
#define CMD38	(38)		/* ERASE */
#define	CMD48	(48)		/* READ_EXTR_SINGLE */
#define	CMD49	(49)		/* WRITE_EXTR_SINGLE */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */

// R1 responses
#define PARAM_ERROR(X)      X & 0b01000000
#define ADDR_ERROR(X)       X & 0b00100000
#define ERASE_SEQ_ERROR(X)  X & 0b00010000
#define CRC_ERROR(X)        X & 0b00001000
#define ILLEGAL_CMD(X)      X & 0b00000100
#define ERASE_RESET(X)      X & 0b00000010
#define IN_IDLE(X)          X & 0b00000001

static mos_uint8_t Stat = STA_NOINIT;				/* Disk status */
static mos_uint8_t CardType;						/* Card type flags (b0:MMC, b1:SDv1, b2:SDv2, b3:Block addressing) */
const struct spi_master_driver * spi_pointer_temp;

/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static int wait_ready (void)	/* 1:OK, 0:Timeout */
{
	mos_uint8_t d;
	mos_uint16_t tmr;
	
	for (tmr = 5000; tmr; tmr--) 					/* Wait for ready in timeout of 500ms */
	{	
		d = spi_pointer_temp->transfer(0xFF);
		if (d == 0xFF) 
			break;
		delay_us_blocking(100);
	}
	
	return tmr ? 1 : 0;
}


/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

static void deselect (void)
{
	CS_HIGH();								/* Set CS# high */
	spi_pointer_temp->transfer(0xFF);		/* Dummy clock (force DO hi-z for multiple slave SPI) */
}

/*-----------------------------------------------------------------------*/
/* Select the card and wait for ready                                    */
/*-----------------------------------------------------------------------*/

static int select (void)	/* 1:OK, 0:Timeout */
{
	CS_LOW();							/* Set CS# low */
	spi_pointer_temp->transfer(0xFF);	/* Dummy clock (force DO enabled) */
	if (wait_ready()) 					/* Wait for card ready */
		return 1;			

	deselect();
	return 0;			/* Failed */
}

/*-----------------------------------------------------------------------*/
/* Receive a data packet from the card                                   */
/*-----------------------------------------------------------------------*/

static int rcvr_datablock (	/* 1:OK, 0:Failed */
	mos_uint8_t *buff,			/* Data buffer to store received data */
	mos_uint16_t btr			/* Byte count */
)
{
	mos_uint8_t d[2];
	mos_uint16_t tmr;

	for (tmr = 1000; tmr; tmr--) 	/* Wait for data packet in timeout of 100ms */
	{
		d[0] = spi_pointer_temp->transfer(0xFF);
		if (d[0] != 0xFF) 
			break;
		delay_us_blocking(100);
	}
	if (d[0] != 0xFE) 				/* If not valid data token, return with error */
		return 0;		

	spi_pointer_temp->receive(0xFF, buff, btr);			/* Receive the data block into buffer */
	spi_pointer_temp->receive(0xFF, d, 2);			/* Discard CRC */

	return 1;						/* Return with success */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to the card                                        */
/*-----------------------------------------------------------------------*/

static int xmit_datablock (		/* 1:OK, 0:Failed */
	const mos_uint8_t *buff,	/* 512 byte data block to be transmitted */
	mos_uint8_t token			/* Data/Stop token */
)
{
	mos_uint8_t d[2];
	
	if (!wait_ready()) 
		return 0;
	
	spi_pointer_temp->transfer(token);				/* Xmit a token */
	if (token != 0xFD) {							/* Is it data token? */
		spi_pointer_temp->transmit((mos_uint8_t *)buff, 512);		/* Xmit the 512 byte data block to MMC */
		spi_pointer_temp->receive(0xFF, d , 2); 	/* Xmit dummy CRC (0xFF,0xFF) */
		d[0] = spi_pointer_temp->transfer(0xFF);	/* Receive data response */
		if ((d[0] & 0x1F) != 0x05)					/* If not accepted, return with error */
			return 0;	
		
	}
	
	return 1;
}

/*-----------------------------------------------------------------------*/
/* Send a command packet to the card                                     */
/*-----------------------------------------------------------------------*/

static mos_uint8_t send_cmd (				/* Returns command response (bit7==1:Send failed)*/
	mos_uint8_t cmd,						/* Command byte */
	mos_uint32_t arg						/* Argument */
)
{
	mos_uint8_t n, d, buf[6];

	if (cmd & 0x80) 	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
	{
		cmd &= 0x7F;
		n = send_cmd(CMD55, 0);
		if (n > 1) 
			return n;
	}

	/* Select the card and wait for ready except to stop multiple block read */
	

	if(cmd == CMD0)
	{
		CS_HIGH();
		CS_LOW();
		spi_pointer_temp->transfer(0xFF);
	}
	
	else if (cmd != CMD12) 
	{
		#if 1
		
		deselect();
		if (!select()) 
			return 0xFF;
		
		#else
			
		CS_HIGH();
		CS_LOW();
		spi_pointer_temp->transfer(0xFF);
		
		#endif
	}

	/* Send a command packet */
	buf[0] = 0x40 | cmd;					/* Start + Command index */
	buf[1] = (mos_uint8_t)(arg >> 24);		/* Argument[31..24] */
	buf[2] = (mos_uint8_t)(arg >> 16);		/* Argument[23..16] */
	buf[3] = (mos_uint8_t)(arg >> 8);		/* Argument[15..8] */
	buf[4] = (mos_uint8_t)arg;				/* Argument[7..0] */
	n = 0x01;						/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;		/* (valid CRC for CMD0(0)) */
	if (cmd == CMD8) n = 0x87;		/* (valid CRC for CMD8(0x1AA)) */
	buf[5] = n;
	spi_pointer_temp->transmit(buf, 6);

	/* Receive command response */
	if (cmd == CMD12) 
		spi_pointer_temp->transfer(0xFF);				/* Skip a stuff byte when stop reading */
	
	n = 10;												/* Wait for a valid response in timeout of 10 attempts */
	do
		d = spi_pointer_temp->transfer(0xFF);
	while ((d & 0x80) && --n);

	return d;			/* Return with the response value */
}

/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/

 /*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

mos_uint8_t sdcard_status (void)
{
	return Stat;
}

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

mos_uint8_t sdcard_begin (const struct spi_master_driver * spi_pointer, mos_uint8_t ss_pin)
{
	mos_uint8_t n, ty, cmd, buf[4];
	mos_uint16_t tmr;
	mos_uint8_t s;
	static mos_uint8_t res[5];
	
	spi_pointer_temp = spi_pointer;
	spi_pointer_temp->begin(ss_pin);
	delay_ms_blocking(10);			/* 10ms */
	
	spi_pointer_temp->transfer(0xFF);
    CS_LOW();
	
	delay_ms_blocking(1);	
	
	for (n = 80; n; n--) 					/* Apply 80 dummy clocks and the card gets ready to receive command */
		spi_pointer_temp->transfer(0xFF);
		
    CS_HIGH();
	spi_pointer_temp->transfer(0xFF);
	
	tmr = 1000;
	do {
		//res[0] = SD_goIdleState();
		res[0] = send_cmd(CMD0, 0);
		tmr--;
		delay_ms_blocking(1);
	} while(res[0] != 1 && n);
	
	ty = 0;
	if (send_cmd(CMD0, 0) == 1) 			/* Enter Idle state */
	{
		if (send_cmd(CMD8, 0x1AA) == 1) 	/* SDv2? */
		{
			spi_pointer_temp->receive(0xFF, buf, 4);		/* Get trailing return value of R7 resp */
			if (buf[2] == 0x01 && buf[3] == 0xAA) 	/* The card can work at vdd range of 2.7-3.6V */
			{
				for (tmr = 1000; tmr; tmr--) 		/* Wait for leaving idle state (ACMD41 with HCS bit) */
				{			
					if (send_cmd(ACMD41, 1UL << 30) == 0)
						break;
					delay_ms_blocking(1);
				}
				if (tmr && send_cmd(CMD58, 0) == 0) /* Check CCS bit in the OCR */
				{	
					spi_pointer_temp->receive(0xFF, buf, 4);
					ty = (buf[0] & 0x40) ? CT_SDC2 | CT_BLOCK : CT_SDC2;	/* SDv2+ */
				}
			}
		} 
		else 	/* SDv1 or MMCv3 */
		{
			if (send_cmd(ACMD41, 0) <= 1) 	
			{
				ty = CT_SDC2; cmd = ACMD41;	/* SDv1 */
			} 
			else 
			{
				ty = CT_MMC3; cmd = CMD1;	/* MMCv3 */
			}
			for (tmr = 1000; tmr; tmr--) 	/* Wait for leaving idle state */
			{
				if (send_cmd(cmd, 0) == 0) 
					break;
				delay_ms_blocking(1);
			}
			if (!tmr || send_cmd(CMD16, 512) != 0)	/* Set R/W block length to 512 */
				ty = 0;
		}
	}
	
	CardType = ty;
	s = ty ? 0 : STA_NOINIT;
	Stat = s;

	deselect();

	return s;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT sdcard_read (mos_uint8_t * buff, mos_uint32_t sector, mos_uint16_t count)
{
	mos_uint8_t cmd;
	
	if (sdcard_status() & STA_NOINIT) return RES_NOTRDY;
	
	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert LBA to byte address if needed */

	cmd = count > 1 ? CMD18 : CMD17;			/*  READ_MULTIPLE_BLOCK : READ_SINGLE_BLOCK */
	if (send_cmd(cmd, sector) == 0) 
	{
		do {
			if (!rcvr_datablock(buff, 512)) 
				break;
			buff += 512;
		} while (--count);
		
		if (cmd == CMD18) 
			send_cmd(CMD12, 0);	/* STOP_TRANSMISSION */
	}
	deselect();

	return count ? RES_ERROR : RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT sdcard_write (const mos_uint8_t * buff, mos_uint32_t sector, mos_uint16_t count)
{	
	if (sdcard_status() & STA_NOINIT) return RES_NOTRDY;
	
	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert LBA to byte address if needed */

	if (count == 1) 							/* Single block write */
	{	
		if ((send_cmd(CMD24, sector) == 0)		/* WRITE_BLOCK */
			&& xmit_datablock(buff, 0xFE))
			count = 0;		
	}
	else 										/* Multiple block write */
	{				
		if (CardType & CT_SDC) 
			send_cmd(ACMD23, count);
		
		if (send_cmd(CMD25, sector) == 0) 		/* WRITE_MULTIPLE_BLOCK */
		{
			do {
				if (!xmit_datablock(buff, 0xFC)) 
					break;
				buff += 512;
			} while (--count);
			
			if (!xmit_datablock(0, 0xFD))		/* STOP_TRAN token */
				count = 1;
		}
	}
	
	deselect();
	return count ? RES_ERROR : RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT sdcard_ioctl (
	mos_uint8_t ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	mos_uint8_t n, csd[16];
	mos_uint32_t cs;

	if (sdcard_status() & STA_NOINIT) return RES_NOTRDY;	/* Check if card is in the socket */

	res = RES_ERROR;
	switch (ctrl) {
		case CTRL_SYNC :		/* Make sure that no pending write process */
			if (select()) 
				res = RES_OK;
			break;

		case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
			if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) 
			{
				if ((csd[0] >> 6) == 1) 	/* SDC ver 2.00 */
				{
					cs = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
					*(LBA_t*)buff = cs << 10;
				} 
				else 						/* SDC ver 1.XX or MMC */
				{
					n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
					cs = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
					*(LBA_t*)buff = cs << (n - 9);
				}
				res = RES_OK;
			}
			break;

		case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
			*(mos_uint32_t*)buff = 128;
			res = RES_OK;
			break;

		default:
			res = RES_PARERR;
	}

	deselect();

	return res;
}


/*

void SD_command(mos_uint8_t cmd, mos_uint32_t arg, mos_uint8_t crc)
{
    // transmit command to sd card
    spi_pointer_temp->transfer(cmd|0x40);

    // transmit argument
    spi_pointer_temp->transfer((mos_uint8_t)(arg >> 24));
    spi_pointer_temp->transfer((mos_uint8_t)(arg >> 16));
    spi_pointer_temp->transfer((mos_uint8_t)(arg >> 8));
    spi_pointer_temp->transfer((mos_uint8_t)(arg));

    // transmit crc
    spi_pointer_temp->transfer(crc|0x01);
}

mos_uint8_t SD_readRes1(void)
{
    mos_uint8_t i = 0, res1;

    // keep polling until actual data received
    while((res1 = spi_pointer_temp->transfer(0xFF)) == 0xFF)
    {
        i++;

        // if no data received for 8 bytes, break
        if(i > 8) break;
    }

    return res1;
}

mos_uint8_t SD_goIdleState(void)
{
    mos_uint8_t res1;
	
	#if 0
	// assert chip select
    spi_pointer_temp->transfer(0xFF);
    CS_LOW();
    spi_pointer_temp->transfer(0xFF);
	
	Serial.println("CMD0");
	
    // send CMD0
    SD_command(CMD0, 0 , 0x95);

    // read response
    res1 = SD_readRes1();

    // deassert chip select
    spi_pointer_temp->transfer(0xFF);
    //CS_HIGH();
    spi_pointer_temp->transfer(0xFF);
	
	#else
			
	Serial.println("CMD0");
	res1 = 	send_cmd(CMD0, 0);
	
	#endif

    return res1;
}

void SD_printR1(mos_uint8_t res)
{
    if(res & 0b10000000)
        Serial.print("\tError: MSB = 1\r\n");
    if(res == 0)
        Serial.print("\tCard Ready\r\n");
    if(PARAM_ERROR(res))
        Serial.print("\tParameter Error\r\n");
    if(ADDR_ERROR(res))
        Serial.print("\tAddress Error\r\n");
    if(ERASE_SEQ_ERROR(res))
        Serial.print("\tErase Sequence Error\r\n");
    if(CRC_ERROR(res))
        Serial.print("\tCRC Error\r\n");
    if(ILLEGAL_CMD(res))
        Serial.print("\tIllegal Command\r\n");
    if(ERASE_RESET(res))
        Serial.print("\tErase Reset Error\r\n");
    if(IN_IDLE(res))
        Serial.print("\tIn Idle State\r\n");
}

*/
