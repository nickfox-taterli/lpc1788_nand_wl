#ifndef _BSP_EMC_H_
#define _BSP_EMC_H_

#include "LPC177x_8x.h"

//FLASH最大能够达到的地址，是128M
#define NandFlash_MAX_ADDR 0x7FFFFFF

//FLASH块大小，为128KB
#define NandFlash_BLOCK_SIZE 0x20000

//FLASH页大小，为2KB
#define NandFlash_PAGE_SIZE 0x800

//扇区大小
#define NandFlash_SECTOR_SIZE 0x200

//用做交换区的块数
#define NandFlash_SWAP_BLOCKS 10

//用做坏块重新影射的块数
#define NandFlash_BAD_BLOCKS_REMAP 50

//保存坏块表的块数
#define NandFlash_BLOCKS_TABLE 3

//最大能够达到的扇区地址
#define NandFlash_MAX_SECTOR_ADDR (NandFlash_MAX_ADDR-(NandFlash_BAD_BLOCKS_REMAP+NandFlash_SWAP_BLOCKS+NandFlash_BLOCKS_TABLE)*NandFlash_BLOCK_SIZE)

//交换区的起始地址
#define NandFlash_SWAP_BLOCK_ADDR (NandFlash_MAX_ADDR+1-NandFlash_SWAP_BLOCKS*NandFlash_BLOCK_SIZE)

//重影射坏块的起始地址
#define NandFlash_BAD_BLOCK_REMAP_ADDR (NandFlash_MAX_ADDR+1-(NandFlash_BAD_BLOCKS_REMAP+NandFlash_SWAP_BLOCKS)*NandFlash_BLOCK_SIZE)

//定义保存坏块表的起始地址
#define NandFlash_BLOCK_TABLE_ADDR (NandFlash_MAX_ADDR+1-(NandFlash_BAD_BLOCKS_REMAP+NandFlash_SWAP_BLOCKS+NandFlash_BLOCKS_TABLE)*NandFlash_BLOCK_SIZE)

//影射区的状态常量定义
#define NandFlash_BLOCK_OK    0
#define NandFlash_BLOCK_BAD   1
#define NandFlash_BLOCK_USED  2

void NORRAM_Init(void);
void NORFLASH_MassErase( void );
void NORFLASH_WriteWord( uint32_t Addr, uint16_t Data );
uint32_t NandFlash_WriteOneSector(uint32_t Addr, uint8_t *pBuf, uint32_t Remain);
uint32_t NandFlash_ReadOneSector(uint32_t Addr, uint8_t  *pBuf, uint32_t Remain);
void NandFlash_Init(void);
void NandFlash_ReadId(uint8_t *Buf);

#endif
