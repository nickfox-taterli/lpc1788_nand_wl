/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

#include "EMC.h"

/* Definitions of physical drive number for each drive */
#define DEV_NAND		0	/* Example: Map Nand to physical drive 0 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    switch (pdrv)
    {
    case DEV_NAND :
        return 0x00;
    }
    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    switch (pdrv)
    {
    case DEV_NAND :
        NandFlash_Init();
        return 0x00;
    }
    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,		/* Physical drive nmuber to identify the drive */
    BYTE *buff,		/* Data buffer to store read data */
    DWORD sector,	/* Start sector in LBA */
    UINT count		/* Number of sectors to read */
)
{

    switch (pdrv)
    {
    case DEV_NAND :
        for(uint8_t i = sector; i < (count + sector); i++)
        {
            NandFlash_ReadOneSector(i * NandFlash_SECTOR_SIZE , buff, 0);
            buff += 512;
        }

        return RES_OK;
    }

    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
DRESULT disk_write (
    BYTE pdrv,			/* Physical drive nmuber to identify the drive */
    const BYTE *buff,	/* Data to be written */
    DWORD sector,		/* Start sector in LBA */
    UINT count			/* Number of sectors to write */
)
{

    switch (pdrv)
    {
    case DEV_NAND :
        for(uint8_t i = sector; i < (count + sector); i++)
        {
            NandFlash_WriteOneSector(i * NandFlash_SECTOR_SIZE, (uint8_t *)buff, 0);
            buff += 512;
        }

        return RES_OK;
    }

    return RES_PARERR;
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

    switch (pdrv)
    {
    case DEV_NAND :
        switch(cmd)
        {
        case CTRL_SYNC:
            return RES_OK;
				case GET_SECTOR_SIZE:
            *(WORD *)buff = (WORD)NandFlash_SECTOR_SIZE;
            return RES_OK;
        case GET_BLOCK_SIZE:
            *(WORD *)buff = (WORD)NandFlash_BLOCK_SIZE;
            return RES_OK;
        case GET_SECTOR_COUNT:
            *(DWORD *)buff = (DWORD)NandFlash_MAX_SECTOR_ADDR/(WORD)NandFlash_SECTOR_SIZE;
            return RES_OK;
        default:
            return RES_PARERR;
        }

    }

    return RES_PARERR;
}

