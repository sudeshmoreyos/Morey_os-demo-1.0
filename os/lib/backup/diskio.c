/*-----------------------------------------------------------------------*/
/* Low level disk I/O module glue functions         (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types for FatFs */
#include "diskio.h"		/* FatFs lower layer API */
#include "sdcard.h"

#if defined(PLATFORM_SUPPORT_DIGITAL) || defined(PLATFORM_SUPPORT_SPI)

#include "SpiMaster.h"

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	if (pdrv) 
		return STA_NOINIT;
	
	return (DSTATUS)sdcard_status();
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{	
	if (pdrv) 
		return STA_NOINIT;
	
	return (DSTATUS)sdcard_begin(&SpiMaster,B0);
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{	
	if(pdrv)
		return RES_PARERR;
	
	return sdcard_read(buff,sector,count);
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{	
	if(pdrv)
		return RES_PARERR;
	
	return sdcard_write(buff,sector,count);
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{	
	if(pdrv)
		return RES_PARERR;
	
	return sdcard_ioctl(cmd,buff);
}

#endif //defined(PLATFORM_SUPPORT_DIGITAL) || defined(PLATFORM_SUPPORT_SPI)

