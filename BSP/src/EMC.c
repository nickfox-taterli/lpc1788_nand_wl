#include "EMC.h"

static uint32_t NandFlash_CurrentWriteSectorAddr; //当前写的扇区地址
static uint32_t NandFlash_CurrentReadSectorAddr;  //当前读的扇区地址
static uint32_t NandFlash_NeedWriteBack; //需要回写的标志

static uint32_t NandFlash_BadBlockTable[2][NandFlash_BAD_BLOCKS_REMAP + 1]; //用来管理坏块的表格
static uint32_t NandFlash_BadBlocksCount;  //有多少个坏块
static uint8_t NandFlash_RemapBlockStatu[NandFlash_BAD_BLOCKS_REMAP + 1]; //用来重影射用的块的状况
static uint32_t NandFlash_LastAccessAddr; //最后一次访问过的地址

/********************************************************************
函数功能：初始化EMC配置
入口参数：无
返    回：无
备    注：这只是一个赋值寄存器函数,肯定会成功的.
********************************************************************/
void NORRAM_Init(void)
{
    uint32_t i = 0;

    LPC_SC->PCONP = 0xFFFFFFFF;
    LPC_SC->EMCDLYCTL  = 0x000001f1f;
    LPC_EMC->Control = 0x00000001;
    LPC_EMC->Config  = 0x00000000;

    LPC_IOCON->P3_0 = 0x201; /* D0 @ P3.0 */
    LPC_IOCON->P3_1 = 0x201; /* D1 @ P3.1 */
    LPC_IOCON->P3_2 = 0x201; /* D2 @ P3.2 */
    LPC_IOCON->P3_3 = 0x201; /* D3 @ P3.3 */

    LPC_IOCON->P3_4 = 0x201; /* D4 @ P3.4 */
    LPC_IOCON->P3_5 = 0x201; /* D5 @ P3.5 */
    LPC_IOCON->P3_6 = 0x201; /* D6 @ P3.6 */
    LPC_IOCON->P3_7 = 0x201; /* D7 @ P3.7 */

    LPC_IOCON->P3_8 = 0x201; /* D8 @ P3.8 */
    LPC_IOCON->P3_9 = 0x201; /* D9 @ P3.9 */
    LPC_IOCON->P3_10 = 0x201; /* D10 @ P3.10 */
    LPC_IOCON->P3_11 = 0x201; /* D11 @ P3.11 */

    LPC_IOCON->P3_12 = 0x201; /* D12 @ P3.12 */
    LPC_IOCON->P3_13 = 0x201; /* D13 @ P3.13 */
    LPC_IOCON->P3_14 = 0x201; /* D14 @ P3.14 */
    LPC_IOCON->P3_15 = 0x201; /* D15 @ P3.15 */

    LPC_IOCON->P3_16 = 0x201; /* D16 @ P3.16 */
    LPC_IOCON->P3_17 = 0x201; /* D17 @ P3.17 */
    LPC_IOCON->P3_18 = 0x201; /* D18 @ P3.18 */
    LPC_IOCON->P3_19 = 0x201; /* D19 @ P3.19 */

    LPC_IOCON->P3_20 = 0x201; /* D20 @ P3.20 */
    LPC_IOCON->P3_21 = 0x201; /* D21 @ P3.21 */
    LPC_IOCON->P3_22 = 0x201; /* D22 @ P3.22 */
    LPC_IOCON->P3_23 = 0x201; /* D23 @ P3.23 */

    LPC_IOCON->P3_24 = 0x201; /* D24 @ P3.24 */
    LPC_IOCON->P3_25 = 0x201; /* D25 @ P3.25 */
    LPC_IOCON->P3_26 = 0x201; /* D26 @ P3.26 */
    LPC_IOCON->P3_27 = 0x201; /* D27 @ P3.27 */

    LPC_IOCON->P3_28 = 0x201; /* D28 @ P3.28 */
    LPC_IOCON->P3_29 = 0x201; /* D29 @ P3.29 */
    LPC_IOCON->P3_30 = 0x201; /* D30 @ P3.30 */
    LPC_IOCON->P3_31 = 0x201; /* D31 @ P3.31 */

    LPC_IOCON->P4_0 = 0x201; /* A0 @ P4.0 */
    LPC_IOCON->P4_1 = 0x201; /* A1 @ P4.1 */
    LPC_IOCON->P4_2 = 0x201; /* A2 @ P4.2 */
    LPC_IOCON->P4_3 = 0x201; /* A3 @ P4.3 */

    LPC_IOCON->P4_4 = 0x201; /* A4 @ P4.4 */
    LPC_IOCON->P4_5 = 0x201; /* A5 @ P4.5 */
    LPC_IOCON->P4_6 = 0x201; /* A6 @ P4.6 */
    LPC_IOCON->P4_7 = 0x201; /* A7 @ P4.7 */

    LPC_IOCON->P4_8 = 0x201; /* A8 @ P4.8 */
    LPC_IOCON->P4_9 = 0x201; /* A9 @ P4.9 */
    LPC_IOCON->P4_10 = 0x201; /* A10 @ P4.10 */
    LPC_IOCON->P4_11 = 0x201; /* A11 @ P4.11 */

    LPC_IOCON->P4_12 = 0x201; /* A12 @ P4.12 */
    LPC_IOCON->P4_13 = 0x201; /* A13 @ P4.13 */
    LPC_IOCON->P4_14 = 0x201; /* A14 @ P4.14 */
    LPC_IOCON->P4_15 = 0x201; /* A15 @ P4.15 */

    LPC_IOCON->P4_16 = 0x201; /* A16 @ P4.16 */
    LPC_IOCON->P4_17 = 0x201; /* A17 @ P4.17 */
    LPC_IOCON->P4_18 = 0x201; /* A18 @ P4.18 */
    LPC_IOCON->P4_19 = 0x201; /* A19 @ P4.19 */

    LPC_IOCON->P4_20 = 0x201; /* A20 @ P4.20 */
    LPC_IOCON->P4_21 = 0x201; /* A21 @ P4.21 */
    LPC_IOCON->P4_22 = 0x201; /* A22 @ P4.22 */
    LPC_IOCON->P5_0 = 0x201; /* A24 @ P5.0 */
    LPC_IOCON->P5_1 = 0x201; /* A25 @ P5.1 */

    LPC_IOCON->P4_25 = 0x201; /* WEN @ P4.25 */
    LPC_IOCON->P4_24 = 0x201; /* OEN @ P4.24 */
    LPC_IOCON->P4_30 = 0x201; /* CSN[0] @ P4.30 */
    LPC_IOCON->P4_31 = 0x201; /* CSN[1] @ P4.31 */
    LPC_IOCON->P2_28 = 0x201; /* DQM[0] @ P2.28 */
    LPC_IOCON->P2_29 = 0x201; /* DQM[1] @ P2.29 */
    LPC_IOCON->P2_30 = 0x201; /* DQM[2] @ P2.30 */
    LPC_IOCON->P2_31 = 0x201; /* DQM[3] @ P2.31 */
    LPC_IOCON->P2_16 = 0x201; /* CASN @ P2.16 */
    LPC_IOCON->P2_17 = 0x201; /* RASN @ P2.17 */
    LPC_IOCON->P2_18 = 0x201; /* CLK[0] @ P2.18 */
    LPC_IOCON->P2_20 = 0x201; /* DYCSN[0] @ P2.20 */
    LPC_IOCON->P2_24 = 0x201; /* CKE[0] @ P2.24 */

    LPC_IOCON->P2_21 = 0x000; /* GPIO/NAND_RB @ P2.21 */

    LPC_EMC->DynamicRP         = 0x00000002;
    LPC_EMC->DynamicRAS        = 0x0000000F;
    LPC_EMC->DynamicSREX       = 0x00000000;
    LPC_EMC->DynamicAPR        = 0x00000001;
    LPC_EMC->DynamicDAL        = 0x00000004;
    LPC_EMC->DynamicWR         = 0x00000001;
    LPC_EMC->DynamicRC         = 0x00000004;
    LPC_EMC->DynamicRFC        = 0x00000004;
    LPC_EMC->DynamicXSR        = 0x0000000F;
    LPC_EMC->DynamicRRD        = 0x00000004;
    LPC_EMC->DynamicMRD        = 0x00000001;
    LPC_EMC->DynamicReadConfig = 0x00000001;
    LPC_EMC->DynamicRasCas0    = 0x00000303;
    LPC_EMC->DynamicConfig0	= 0x00004680;

    for(i = 100 * 30; i; i--);

    LPC_EMC->DynamicControl    = 0x00000183;

    for(i = 200 * 30; i; i--);

    LPC_EMC->DynamicControl    = 0x00000103;
    LPC_EMC->DynamicRefresh    = 0x00000002;
    for(i = 0; i < 0x80; i++);
    LPC_EMC->DynamicRefresh    = 0x0000001D;
    LPC_EMC->DynamicControl    = 0x00000083;
    LPC_EMC->DynamicControl    = 0x00000000;
    LPC_EMC->DynamicConfig0    = 0x00084680;
    for(i = 10 * 30; i; i--);
    LPC_SC->EMCDLYCTL = 0x00001F11;

    /* NOR Flash */

    LPC_EMC->StaticConfig0 = 0x00000081;
    LPC_EMC->StaticWaitWen0 = 0x00000002;
    LPC_EMC->StaticWaitOen0 = 0x00000002;
    LPC_EMC->StaticWaitRd0 = 0x0000001F;
    LPC_EMC->StaticWaitPage0 = 0x0000001F;
    LPC_EMC->StaticWaitWr0 = 0x0000001F;
    LPC_EMC->StaticWaitTurn0 = 0x0000000F;

    /* NAND Flash */

    LPC_EMC->StaticConfig1 = 0x00000080;
    LPC_EMC->StaticWaitWen1 = 0x00000002;
    LPC_EMC->StaticWaitOen1 = 0x00000002;
    LPC_EMC->StaticWaitRd1 = 0x0000001F;
    LPC_EMC->StaticWaitPage1 = 0x0000001F;
    LPC_EMC->StaticWaitWr1 = 0x0000001F;
    LPC_EMC->StaticWaitTurn1 = 0x0000000F;

		LPC_GPIO2_BB->DIR.M21 = 0;
	
}

/********************************************************************
函数功能：全片擦除NOR
入口参数：无
返    回：无
备    注：执行后,请自行延迟50毫秒以上.
********************************************************************/
void NORFLASH_MassErase( void )
{
    uint32_t i = 0;

    *(volatile uint16_t *)(0x8000AAAA) = 0x00AA;
    *(volatile uint16_t *)(0x80005554) = 0x0055;
    *(volatile uint16_t *)(0x8000AAAA) = 0x0080;
    *(volatile uint16_t *)(0x8000AAAA) = 0x00AA;
    *(volatile uint16_t *)(0x80005554) = 0x0055;
    *(volatile uint16_t *)(0x8000AAAA) = 0x0010;

}

/********************************************************************
函数功能：给NOR写数据
入口参数：地址 + 数据
返    回：无
备    注：NOR数据直接通过指针读取就可以了.
********************************************************************/
void NORFLASH_WriteWord( uint32_t Addr, uint16_t Data )
{
    *(volatile uint16_t *)(0x8000AAAA) = 0x00AA;
    *(volatile uint16_t *)(0x80005554) = 0x0055;
    *(volatile uint16_t *)(0x8000AAAA) = 0x00A0;

    *(volatile uint16_t *)(0x80000000 | (Addr << 1)) = Data; /* Program 16-bit word */
    while (*(volatile uint16_t *)(0x80000000 | (Addr << 1)) != Data);
}

/********************************************************************
函数功能：NAND底层函数 - 写指令
入口参数：指令
返    回：无
备    注：无
********************************************************************/
__INLINE static void NandFlash_WriteCommand(uint8_t Cmd)
{
    *((volatile uint8_t *)0x92000000) = Cmd;
}

/********************************************************************
函数功能：NAND底层函数 - 读数据
入口参数：无
返    回：数据
备    注：无
********************************************************************/
__INLINE static uint8_t NandFlash_ReadDataByte()
{
    return  *(((volatile uint8_t *)0x90000000));
}

/********************************************************************
函数功能：NAND底层函数 - 写数据
入口参数：数据
返    回：无
备    注：无
********************************************************************/
__INLINE static void NandFlash_WriteDataByte(uint8_t Data)
{
    *((volatile uint8_t *)0x90000000) = Data;
}

/********************************************************************
函数功能：NAND底层函数 - 写地址
入口参数：地址
返    回：无
备    注：无
********************************************************************/
__INLINE static void NandFlash_WriteAddr(uint8_t Addr)
{
    *((volatile uint8_t *)0x91000000) = Addr;
}

/********************************************************************
函数功能：NAND底层函数 - 等待R/B引脚跳变
入口参数：无
返    回：无
备    注：R/B引脚是用来判断忙碌的.
********************************************************************/
static void NandFlash_Wait( void )
{
    while( LPC_GPIO2_BB->PIN.M21 );
    while( !LPC_GPIO2_BB->PIN.M21 );
}

/********************************************************************
函数功能：NAND底层函数 - 写4字节地址
入口参数：地址
返    回：无
备    注：无
********************************************************************/
static void NandFlash_WriteAddr4Byte(uint32_t Addr)
{
    uint8_t i;

    //注意：该FLASH的地址中间有5bit必须置0
    //并将高16位左移5位
    Addr = ((Addr << 5) & (~0xFFFF)) | (Addr & 0x07FF);

    for(i = 0; i < 4; i++) //写入四字节的地址
    {
        NandFlash_WriteAddr(Addr);
        Addr >>= 8;
    }

}

/********************************************************************
函数功能：NAND底层函数 - 写2字节地址
入口参数：地址
返    回：无
备    注：无
********************************************************************/
static void NandFlash_WriteAddr2Byte(uint32_t Addr)
{
    uint8_t i;

    //只需要低11位
    Addr = Addr & (NandFlash_PAGE_SIZE - 1);

    for(i = 0; i < 2; i++) //写入二字节的地址
    {
        NandFlash_WriteAddr(Addr);
        Addr >>= 8;
    }

}

/********************************************************************
函数功能：NAND底层函数 - 写页地址
入口参数：地址
返    回：无
备    注：无
********************************************************************/
static void NandFlash_WritePageAddr(uint32_t Addr)
{
    uint8_t i;

    //计算页地址
    Addr /= NandFlash_PAGE_SIZE;

    for(i = 0; i < 2; i++) //写入二字节的地址
    {
        NandFlash_WriteAddr(Addr); //写一字节数据
        Addr >>= 8;
    }
}

/********************************************************************
函数功能：NAND底层函数 - 返回状态寄存器
入口参数：地址
返    回：无
备    注：无
********************************************************************/
static uint8_t NandFlash_ReadStatus(void)
{
    NandFlash_WriteCommand(0x70);  //写查询命令
    return *((volatile uint8_t *)0x91000000);  //读回状态
}

/********************************************************************
函数功能：NAND底层函数 - 页写指令
入口参数：无
返    回：无
备    注：返回状态寄存器.
********************************************************************/
static uint8_t NandFlash_WritePage(void)
{
    NandFlash_WriteCommand(0x10);  //页写命令
    NandFlash_Wait();  //等待写完成
    return NandFlash_ReadStatus();
}

/********************************************************************
函数功能：NAND底层函数 - 擦除一个块
入口参数：页地址
返    回：无
备    注：返回状态寄存器.
********************************************************************/
static uint8_t NandFlash_EraseBlock(uint32_t Addr)
{
    //擦除命令第一字节命令
    NandFlash_WriteCommand(0x60);
    //写入两字节的页地址
    NandFlash_WritePageAddr(Addr);
    //写入擦除命令第二字节
    NandFlash_WriteCommand(0xD0);
    //等待擦除完成
    NandFlash_Wait();
    //返回操作状态
    return NandFlash_ReadStatus();
}

/********************************************************************
函数功能：NAND底层函数 - 内部搬移
入口参数：源地址 + 目标地址
返    回：无
备    注：返回状态寄存器.
********************************************************************/
static uint8_t NandFlash_CopyPage(uint32_t sAddr, uint32_t dAddr)
{
    //读源数据到内部缓存
    NandFlash_WriteCommand(0x00);
    NandFlash_WriteAddr4Byte(sAddr);
    NandFlash_WriteCommand(0x35);
    NandFlash_Wait();

    //将数据写到目标页
    NandFlash_WriteCommand(0x85);
    NandFlash_WriteAddr4Byte(dAddr);
    NandFlash_WriteCommand(0x10);
    NandFlash_Wait();
    return NandFlash_ReadStatus();
}

/********************************************************************
函数功能：NAND底层函数 - 读取ID
入口参数：ID缓冲区
返    回：无
备    注：常用语判断芯片是否正确.
********************************************************************/
void NandFlash_ReadId(uint8_t *Buf)
{
    uint8_t i;
    NandFlash_WriteCommand(0x90);
    NandFlash_WriteAddr(0); //写地址

    for(i = 0; i < 5; i++) //读5字节的ID
    {
        Buf[i] = NandFlash_ReadDataByte();
    }
}

/********************************************************************
函数功能：NAND FTL函数 - 获取映射块
入口参数：无
返    回：无
备    注：返回映射块地址.
********************************************************************/
static uint32_t NandFlash_GetNewRemapBlock(void)
{
    uint32_t i, Addr;
    for(i = 0; i < NandFlash_BAD_BLOCKS_REMAP; i++)
    {
        if(NandFlash_BLOCK_OK == NandFlash_RemapBlockStatu[i]) //如果该块还未用
        {
            Addr = NandFlash_BAD_BLOCK_REMAP_ADDR + i * NandFlash_BLOCK_SIZE; //计算地址
            if(0x01 == (NandFlash_EraseBlock(Addr) & 0x01)) //如果擦除失败
            {
                NandFlash_RemapBlockStatu[i] = NandFlash_BLOCK_BAD; //标志该块为已经损坏
            }
            else //否则，擦除成功
            {
                NandFlash_RemapBlockStatu[i] = NandFlash_BLOCK_USED; //标志为该块已被使用
                return Addr; //返回地址
            }
        }
    }
    return ~0; //如果找不到，则返回-1。
}

/********************************************************************
函数功能：NAND FTL函数 - 标记坏块
入口参数：坏块地址
返    回：无
备    注：无
********************************************************************/
static void NandFlash_MarkRemapBlockBad(uint32_t Addr)
{
    uint32_t i;
    i = (Addr - NandFlash_BAD_BLOCK_REMAP_ADDR) / NandFlash_BLOCK_SIZE; //计算偏移量
    if(i >= NandFlash_BAD_BLOCKS_REMAP)return; //出错
    NandFlash_RemapBlockStatu[i] = NandFlash_BLOCK_BAD; //标志为已经损坏
}

/********************************************************************
函数功能：NAND FTL函数 - 地址重映射
入口参数：原始地址
返    回：无
备    注：映射后地址
********************************************************************/
static uint32_t NandFlash_AddrRemap(uint32_t Addr)
{
    static uint32_t CurrentRemapBlockAddr;
    uint32_t i, j;

    if(0 == NandFlash_BadBlocksCount) //如果坏块数量为0，则不需要处理，直接返回地址
    {
        return Addr;
    }

    //如果最后一次访问的地址和本次访问的地址属于同一个块地址，那么不需要重新影射
    if(0 == ((Addr - NandFlash_LastAccessAddr) & (NandFlash_BLOCK_SIZE - 1)))
    {
        return CurrentRemapBlockAddr + (Addr & (NandFlash_BLOCK_SIZE - 1)); //由当前块地址加上块内偏移得到完整地址
    }

    NandFlash_LastAccessAddr = Addr; //保存最后一次访问过的地址

    if(1 == NandFlash_BadBlocksCount) //如果坏块数量为1，则直接影射
    {
        if((Addr & (~(NandFlash_BLOCK_SIZE - 1))) == NandFlash_BadBlockTable[0][0]) //如果块地址相等，则直接影射
        {
            CurrentRemapBlockAddr = NandFlash_BadBlockTable[1][0];
            return CurrentRemapBlockAddr + (Addr & (NandFlash_BLOCK_SIZE - 1)); //由当前块地址加上块内偏移得到完整地址
        }
        else //不用影射
        {
            CurrentRemapBlockAddr = Addr & (~(NandFlash_BLOCK_SIZE - 1)); //获取当前块地址
            return Addr;  //直接返回原来的地址
        }
    }
    else //坏块数量大于1
    {
        //如果地址比第一个坏块的地址还小或者比最后一个坏块的地址还大，
        //那么肯定不会是坏快，不需要重新影射
        if((Addr < NandFlash_BadBlockTable[0][0])
                || ((Addr & (NandFlash_BLOCK_SIZE - 1)) > NandFlash_BadBlockTable[0][NandFlash_BadBlocksCount - 1]))
        {
            CurrentRemapBlockAddr = Addr & (~(NandFlash_BLOCK_SIZE - 1)); //获取当前块地址
            return Addr;  //直接返回原来的地址
        }
        else //属于坏块区间，使用二分查表法决定是否需要影射
        {
            i = 0;
            j = NandFlash_BadBlocksCount - 1;
            while(1)
            {
                if((Addr & (~(NandFlash_BLOCK_SIZE - 1))) == NandFlash_BadBlockTable[0][(i + j) / 2]) //如果相等，则影射
                {
                    CurrentRemapBlockAddr = NandFlash_BadBlockTable[1][(i + j) / 2];
                    return CurrentRemapBlockAddr + (Addr & (NandFlash_BLOCK_SIZE - 1)); //由当前块地址加上块内偏移得到完整地址
                }
                if(i == j)break; //如果i和j相等，则退出查找
                if((Addr & (~(NandFlash_BLOCK_SIZE - 1))) < NandFlash_BadBlockTable[0][(i + j) / 2]) //如果小于
                {
                    j = (i + j) / 2 - 1; //搜索前半段
                }
                else //如果大于
                {
                    i = (i + j) / 2 + 1; //搜索后半段
                }
            }
        }
    }
    //没有在坏块表中找到，则说明不是坏块
    CurrentRemapBlockAddr = Addr & (~(NandFlash_BLOCK_SIZE - 1)); //获取当前块地址
    return Addr;  //直接返回原来的地址
}

/********************************************************************
函数功能：NAND FTL函数 - 保存坏块表
入口参数：无
返    回：无
备    注：NandFlash_BadBlockTable 是全局变量.
********************************************************************/
static void NandFlash_SaveBadBlockTable(void)
{
    uint32_t i, j, k, Sum;

    for(i = 0; i < NandFlash_BLOCKS_TABLE; i++) //标志为准备擦除
    {
        NandFlash_WriteCommand(0x80);
        NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * (i + 1) - NandFlash_PAGE_SIZE);
        NandFlash_WriteDataByte(0x00);  //将第一字节设置为0，表示准备擦除
        //剩余字节写0xFF
        for(j = 1; j < NandFlash_PAGE_SIZE; j++)
        {
            NandFlash_WriteDataByte(0xFF);
        }
        NandFlash_WritePage(); //写页
    }

    for(i = 0; i < NandFlash_BLOCKS_TABLE; i++) //将坏块表写入这三块
    {
        NandFlash_EraseBlock(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * i); //擦除一块
        NandFlash_WriteCommand(0x80);
        NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * i); //写入第一块的开始位置
        NandFlash_WriteDataByte(0x00);  //将第1字节设置为0
        NandFlash_WriteDataByte(0x55);  //将第2字节设置为0x55
        NandFlash_WriteDataByte(0xAA);  //将第3字节设置为0xAA
        NandFlash_WriteDataByte(0xFF);  //将第4字节设置为0xFF
        Sum = 0x1FE;
        //接着写坏块数量，并统计校验和
        NandFlash_WriteDataByte((NandFlash_BadBlocksCount >> 24) & 0xFF);
        Sum += (NandFlash_BadBlocksCount >> 24) & 0xFF;
        NandFlash_WriteDataByte((NandFlash_BadBlocksCount >> 16) & 0xFF);
        Sum += (NandFlash_BadBlocksCount >> 16) & 0xFF;
        NandFlash_WriteDataByte((NandFlash_BadBlocksCount >> 8) & 0xFF);
        Sum += (NandFlash_BadBlocksCount >> 8) & 0xFF;
        NandFlash_WriteDataByte((NandFlash_BadBlocksCount) & 0xFF);
        Sum += (NandFlash_BadBlocksCount) & 0xFF;
        j = 8; //写了8字节
        //保存坏块表
        for(k = 0; k < sizeof(NandFlash_BadBlockTable[0][0])*NandFlash_BAD_BLOCKS_REMAP * 2; k++)
        {
            if(0 == (j & (NandFlash_PAGE_SIZE - 1))) //如果超过了页，则需要重新写新页
            {
                NandFlash_WritePage(); //写页
                NandFlash_WriteCommand(0x80);
                NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * i + j);
            }
            Sum += ((uint8_t *)NandFlash_BadBlockTable)[k]; //求校验和
            NandFlash_WriteDataByte(((uint8_t *)NandFlash_BadBlockTable)[k]);  //写一字节
            j++;
        }
        //保存重影射区的状态表
        for(k = 0; k < sizeof(NandFlash_RemapBlockStatu[0])*NandFlash_BAD_BLOCKS_REMAP; k++)
        {
            if(0 == (j & (NandFlash_PAGE_SIZE - 1))) //如果超过了页，则需要重新写新页
            {
                NandFlash_WritePage(); //写页
                NandFlash_WriteCommand(0x80);
                NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * i + j);
            }
            Sum += ((uint8_t *)NandFlash_RemapBlockStatu)[k]; //求校验和
            NandFlash_WriteDataByte(((uint8_t *)NandFlash_RemapBlockStatu)[k]);  //写一字节
            j++;
        }
        for(; 0 != (j & (NandFlash_PAGE_SIZE - 1)); j++) //将剩余部分写入0xFF
        {
            NandFlash_WriteDataByte(0xFF);
        }
        NandFlash_WritePage();   //写页

        //已完成写状态及校验和写入到该块的倒数第二页
        NandFlash_WriteCommand(0x80);
        NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * (i + 1) - 2 * NandFlash_PAGE_SIZE);
        NandFlash_WriteDataByte(0x00);  //将第一字节设置为0，表示已经写入
        //将校验和取反加1，这样累加结果就为0
        Sum = (~Sum) + 1;
        //写校验和
        NandFlash_WriteDataByte((Sum >> 24) & 0xFF);
        NandFlash_WriteDataByte((Sum >> 16) & 0xFF);
        NandFlash_WriteDataByte((Sum >> 8) & 0xFF);
        NandFlash_WriteDataByte((Sum) & 0xFF);
        //剩余字节写0xFF
        for(j = 5; j < NandFlash_PAGE_SIZE; j++)
        {
            NandFlash_WriteDataByte(0xFF);
        }
        NandFlash_WritePage(); //写页
    }
}

/********************************************************************
函数功能：NAND FTL函数 - 加载坏块表
入口参数：无
返    回：无
备    注：NandFlash_BadBlockTable 是全局变量.
********************************************************************/
static void NandFlash_LoadBadBlockTable(void)
{
    uint32_t i, j, k, Sum, Ok;
    uint8_t Data;

    Ok = 0; //设置为不成功
    for(i = 0; i < NandFlash_BLOCKS_TABLE; i++) //查找没有准备擦除的块
    {
        //从该块中最后一页读回第一字节，看是否为0xFF，如果为0xFF，表示该块没有准备擦除
        NandFlash_WriteCommand(0x00);
        NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * (i + 1) - NandFlash_PAGE_SIZE);
        NandFlash_WriteCommand(0x30);
        NandFlash_Wait(); //等待数据读回
        Data = NandFlash_ReadDataByte();
        if(Data == 0xFF) //表示该块数据还未准备擦除
        {
            //从该块中倒数第二页读回第一字节，看是否为0，如果为0，表示该块已经写入了数据
            NandFlash_WriteCommand(0x00);
            NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * (i + 1) - 2 * NandFlash_PAGE_SIZE);
            NandFlash_WriteCommand(0x30);
            NandFlash_Wait(); //等待数据读回
            Data = NandFlash_ReadDataByte();
            if(Data == 0) //表示数据有效
            {
                Data = NandFlash_ReadDataByte(); //读出校验和
                Sum = Data;
                Data = NandFlash_ReadDataByte(); //读出校验和
                Sum = (Sum << 8) + Data;
                Data = NandFlash_ReadDataByte();//读出校验和
                Sum = (Sum << 8) + Data;
                Data = NandFlash_ReadDataByte();//读出校验和
                Sum = (Sum << 8) + Data;
                //从该块开始位置读
                NandFlash_WriteCommand(0x00);
                NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * i);
                NandFlash_WriteCommand(0x30);
                NandFlash_Wait(); //等待数据读回
                //检查第1字节是否为0
                Data = NandFlash_ReadDataByte();
                if(Data != 0)continue;
                //检查第2字节是否为0x55
                Data = NandFlash_ReadDataByte();
                if(Data != 0x55)continue;
                //检查第3字节是否为0xAA
                Data = NandFlash_ReadDataByte();
                if(Data != 0xAA)continue;
                //检查第4字节是否为0xFF
                Data = NandFlash_ReadDataByte();
                if(Data != 0xFF)continue;
                Sum += 0x1FE;

                //读坏块数量
                Data = NandFlash_ReadDataByte();
                NandFlash_BadBlocksCount = Data;
                Sum += Data;
                Data = NandFlash_ReadDataByte();
                NandFlash_BadBlocksCount = (NandFlash_BadBlocksCount << 8) + Data;
                Sum += Data;
                Data = NandFlash_ReadDataByte();
                NandFlash_BadBlocksCount = (NandFlash_BadBlocksCount << 8) + Data;
                Sum += Data;
                Data = NandFlash_ReadDataByte();
                NandFlash_BadBlocksCount = (NandFlash_BadBlocksCount << 8) + Data;
                Sum += Data;
                j = 8;
                //读回坏块表
                for(k = 0; k < sizeof(NandFlash_BadBlockTable[0][0])*NandFlash_BAD_BLOCKS_REMAP * 2; k++)
                {
                    if(0 == (j & (NandFlash_PAGE_SIZE - 1))) //如果超过了页，则需要重新读新页
                    {
                        NandFlash_WriteCommand(0x00);
                        NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * i + j);
                        NandFlash_WriteCommand(0x30);
                        NandFlash_Wait(); //等待数据读回
                    }
                    Data = NandFlash_ReadDataByte();
                    Sum += Data; //求校验和
                    ((uint8_t *)NandFlash_BadBlockTable)[k] = Data; //读回一字节到坏块表中
                    j++;
                }
                //读回重影射区的状态表
                for(k = 0; k < sizeof(NandFlash_RemapBlockStatu[0])*NandFlash_BAD_BLOCKS_REMAP; k++)
                {
                    if(0 == (j & (NandFlash_PAGE_SIZE - 1))) //如果超过了页，则需要重新读新页
                    {
                        NandFlash_WriteCommand(0x00);
                        NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * i + j);
                        NandFlash_WriteCommand(0x30);
                        NandFlash_Wait(); //等待数据读回
                    }
                    Data = NandFlash_ReadDataByte();
                    Sum += Data; //求校验和
                    ((uint8_t *)NandFlash_RemapBlockStatu)[k] = Data; //读回一字节到重影射区状态表中
                    j++;
                }
                if(Sum == 0) //如果校验成功，则说明数据正确
                {
                    Ok = 0xFF; //设置为成功
                    break;   //并退出循环
                }
            }
        }
    }

    if(Ok == 0) //如果在已写入的表中找不到好的坏块表，再去准备擦除的中去找
    {
        for(i = 0; i < NandFlash_BLOCKS_TABLE; i++) //查找准备擦除的块
        {
            //从该块中最后一页读回第一字节，看是否为0，如果为0，表示该块已经准备擦除了
            NandFlash_WriteCommand(0x00);
            NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * (i + 1) - NandFlash_PAGE_SIZE);
            NandFlash_WriteCommand(0x30);
            NandFlash_Wait(); //等待数据读回
            Data = NandFlash_ReadDataByte();
            if(Data == 0x00) //表示该块数据准备擦除
            {
                //从该块中倒数第二页读回第一字节，看是否为0，如果为0，表示该块已经写入了数据
                NandFlash_WriteCommand(0x00);
                NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * (i + 1) - 2 * NandFlash_PAGE_SIZE);
                NandFlash_WriteCommand(0x30);
                NandFlash_Wait(); //等待数据读回
                Data = NandFlash_ReadDataByte();
                if(Data == 0) //表示数据有效
                {
                    Data = NandFlash_ReadDataByte(); //读出校验和
                    Sum = Data;
                    Data = NandFlash_ReadDataByte(); //读出校验和
                    Sum = (Sum << 8) + Data;
                    Data = NandFlash_ReadDataByte(); //读出校验和
                    Sum = (Sum << 8) + Data;
                    Data = NandFlash_ReadDataByte(); //读出校验和
                    Sum = (Sum << 8) + Data;
                    //从该块开始位置读
                    NandFlash_WriteCommand(0x00);
                    NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * i);
                    NandFlash_WriteCommand(0x30);
                    NandFlash_Wait(); //等待数据读回
                    //检查第1字节是否为0
                    Data = NandFlash_ReadDataByte();
                    if(Data != 0)continue;
                    //检查第2字节是否为0x55
                    Data = NandFlash_ReadDataByte();
                    if(Data != 0x55)continue;
                    //检查第3字节是否为0xAA
                    Data = NandFlash_ReadDataByte();
                    if(Data != 0xAA)continue;
                    //检查第4字节是否为0xFF
                    Data = NandFlash_ReadDataByte();
                    if(Data != 0xFF)continue;
                    Sum += 0x1FE;

                    //读坏块数量
                    Data = NandFlash_ReadDataByte();
                    NandFlash_BadBlocksCount = Data;
                    Sum += Data;
                    Data = NandFlash_ReadDataByte();
                    NandFlash_BadBlocksCount = (NandFlash_BadBlocksCount << 8) + Data;
                    Sum += Data;
                    Data = NandFlash_ReadDataByte();
                    NandFlash_BadBlocksCount = (NandFlash_BadBlocksCount << 8) + Data;
                    Sum += Data;
                    Data = NandFlash_ReadDataByte();
                    NandFlash_BadBlocksCount = (NandFlash_BadBlocksCount << 8) + Data;
                    Sum += Data;
                    j = 8;
                    //读回坏块表
                    for(k = 0; k < sizeof(NandFlash_BadBlockTable[0][0])*NandFlash_BAD_BLOCKS_REMAP * 2; k++)
                    {
                        if(0 == (j & (NandFlash_PAGE_SIZE - 1))) //如果超过了页，则需要重新读新页
                        {
                            NandFlash_WriteCommand(0x00);
                            NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * i + j);
                            NandFlash_WriteCommand(0x30);
                            NandFlash_Wait(); //等待数据读回
                        }
                        Data = NandFlash_ReadDataByte();
                        Sum += Data; //求校验和
                        ((uint8_t *)NandFlash_BadBlockTable)[k] = Data; //读回一字节到坏块表中
                        j++;
                    }
                    //读回重影射区的状态表
                    for(k = 0; k < sizeof(NandFlash_RemapBlockStatu[0])*NandFlash_BAD_BLOCKS_REMAP; k++)
                    {
                        if(0 == (j & (NandFlash_PAGE_SIZE - 1))) //如果超过了页，则需要重新读新页
                        {
                            NandFlash_WriteCommand(0x00);
                            NandFlash_WriteAddr4Byte(NandFlash_BLOCK_TABLE_ADDR + NandFlash_BLOCK_SIZE * i + j);
                            NandFlash_WriteCommand(0x30);
                            NandFlash_Wait(); //等待数据读回
                        }
                        Data = NandFlash_ReadDataByte();
                        Sum += Data; //求校验和
                        ((uint8_t *)NandFlash_RemapBlockStatu)[k] = Data; //读回一字节到重影射区状态表中
                        j++;
                    }
                    if(Sum == 0) //如果校验成功，则说明数据正确
                    {
                        NandFlash_SaveBadBlockTable(); //将其保存到FLASH中
                        Ok = 0xFF; //设置为成功
                        break;   //并退出循环
                    }
                }
            }
        }
    }

    if(Ok == 0) //如果还是没找到，那么只好重新初始化了
    {
        NandFlash_BadBlocksCount = 0; //坏块数设置为0
        for(i = 0; i < NandFlash_BAD_BLOCKS_REMAP; i++)
        {
            //所有影射块都设置为好块
            NandFlash_RemapBlockStatu[i] = NandFlash_BLOCK_OK;
            //所有影射关系设置为-1
            NandFlash_BadBlockTable[0][i] = ~0;
            NandFlash_BadBlockTable[1][i] = ~0;
        }
        //设置好之后保存起来
        NandFlash_SaveBadBlockTable();
    }
    //设置当前访问过的地址为无效值
    NandFlash_LastAccessAddr = ~0;
}

/********************************************************************
函数功能：NAND FTL函数 - 管理交换区
入口参数：操作
返    回：下一个可以用的交换区地址
备    注：无
********************************************************************/
static uint32_t NandFlash_ManageSwapBlock(uint32_t Op)
{
    static uint32_t Current;
    static uint8_t FlashSwapBlockStatus[NandFlash_SWAP_BLOCKS];
    uint32_t i;

    switch(Op)
    {
    case 0:  //如果操作为1，表示初始化
        Current = 0;
        for(i = 0; i < NandFlash_SWAP_BLOCKS; i++)
        {
            FlashSwapBlockStatus[i] = 0; //初始化所有交换块为好的
        }
        break;

    case 1: //如果操作为1，表示获取下一个可用的交换区
        while(1)//一直尝试，如果交换区都用完（坏）了，那么就死循环了，
        {
            Current++;
            if(Current >= NandFlash_SWAP_BLOCKS)
            {
                Current = 0;
            }
            if(FlashSwapBlockStatus[Current] == 0)break; //如果该块标志为0，则说明未损坏
        }
        break;

    case 2: //如果操作为2，说明获取当前交换区地址
        break;

    case 3: //如果操作为3，设置当前交换块为坏块
        FlashSwapBlockStatus[Current] = NandFlash_BLOCK_BAD;
        break;

    default:
        break;
    }
    return NandFlash_SWAP_BLOCK_ADDR + Current * NandFlash_BLOCK_SIZE; //返回可用的交换块地址
}

/********************************************************************
函数功能：NAND FTL函数 - 更新坏块表
入口参数：旧地址 + 新映射地址
返    回：无
备    注：无
********************************************************************/
static void NandFlash_UpdateBadBlockTable(uint32_t OldAddr, uint32_t NewAddr)
{
    uint32_t i, j;
    OldAddr &= ~(NandFlash_BLOCK_SIZE - 1); //求得一块内起始地址
    NewAddr &= ~(NandFlash_BLOCK_SIZE - 1);
    if(OldAddr > NandFlash_MAX_SECTOR_ADDR) //如果比能够达到的最大地址还大，说明坏块地址本来就是被重新影射过的
    {
        //先要找到它原来影射的位置
        for(i = 0; i < NandFlash_BadBlocksCount; i++)
        {
            if(OldAddr == NandFlash_BadBlockTable[1][i]) //如果与某个地址吻合，则说明就是该地址了
            {
                NandFlash_BadBlockTable[1][i] = NewAddr; //重新影射到新的地址
                //并将原来的交换块设置为已损坏
                NandFlash_MarkRemapBlockBad(OldAddr);
                break;
            }
        }
    }
    else //说明坏块地址是没有被影射过的
    {
        //查找比它大的块地址，将它插入到前面，排好序，方便二分查表
        for(i = 0; i < NandFlash_BadBlocksCount; i++)
        {
            if(OldAddr < NandFlash_BadBlockTable[0][i]) //找到比它大的地址
            {
                break;
            }
        }
        for(j = NandFlash_BadBlocksCount; j > i; j--) //将上面的部分往上移动，腾出一个空位
        {
            NandFlash_BadBlockTable[0][j] = NandFlash_BadBlockTable[0][j - 1];
            NandFlash_BadBlockTable[1][j] = NandFlash_BadBlockTable[1][j - 1];
        }
        //将当前块的影射写入
        NandFlash_BadBlockTable[0][j] = OldAddr;
        NandFlash_BadBlockTable[1][j] = NewAddr;
        NandFlash_BadBlocksCount++; //增加一个坏块计数
    }
    NandFlash_SaveBadBlockTable(); //存储坏块表
    //修改当前访问过的地址为无效地址，这样下次操作时就会重新影射
    NandFlash_LastAccessAddr = ~0;
}

/********************************************************************
函数功能：NAND FTL函数 - 从FLASH中读出一扇区数据保存到缓冲区中。
入口参数：Addr: 字节地址；pBuf：保存数据的缓冲区；Remain：预计接下来还需要读多少扇区
返    回：读取的状态。0：成功。非0：失败。
备    注：当Remain不为0时，将保存当前地址以备后面的继续读当前页，当不为0时，
          设置当前读地址为无效，从而下次读时必须重新使用读命令将数据从flash中读入到页缓存。
********************************************************************/
static uint32_t NandFlash_DealBadBlock(uint32_t Addr, uint32_t Type)
{
    uint32_t i;
    uint32_t RemapBlockAddr;
    uint32_t SwapBlockAddr;
    while(1)
    {
        RemapBlockAddr = NandFlash_GetNewRemapBlock();
        if(RemapBlockAddr == ~0) //如果已经找不到新的可用的替换品，那只好直接返回了。
        {
            return Addr;
        }
        switch(Type)
        {
            //擦除时遇到的坏块，不需要将当前页缓冲区数据写回
            //只需要返回新的地址即可。地址统一在最后返回，这里不用处理
        case 1:
            goto Exit;
            //   break;

            //复制页时遇到的坏块，需要将该块中前面的页及当前页从交换区中重新复制
        case 2:
            //从交换区去复制前面页以及当前页的数据
            SwapBlockAddr = NandFlash_ManageSwapBlock(2); //获取当前所使用的交换块
            //复制前面以及当前页
            for(i = 0; i < (Addr & (NandFlash_BLOCK_SIZE - 1)) / NandFlash_PAGE_SIZE + 1; i++)
            {
                if(0x01 == (NandFlash_CopyPage(SwapBlockAddr + i * NandFlash_PAGE_SIZE, RemapBlockAddr + i * NandFlash_PAGE_SIZE) & 0x01))
                {
                    //如果复制失败，则说明该块有问题，需要找新的块
                    goto BadRemapBlock;
                }
            }
            //复制完毕，则退出循环
            goto Exit;
            //   break;

            //写数据时遇到的坏块，需要将该块中前面的页从交换区中重新复制，
            //还需要将当前页从交换区中复制并将缓冲区的输入写入到当前页中
            //这里无法再获取到缓冲区的数据了，只好直接从原来的页复制数据
        case 3:
            //从交换区去复制前面页数据
            SwapBlockAddr = NandFlash_ManageSwapBlock(2); //获取当前所使用的交换块
            //复制前面的页
            for(i = 0; i < (Addr & (NandFlash_BLOCK_SIZE - 1)) / NandFlash_PAGE_SIZE; i++)
            {
                if(0x01 == (NandFlash_CopyPage(SwapBlockAddr + i * NandFlash_PAGE_SIZE, RemapBlockAddr + i * NandFlash_PAGE_SIZE) & 0x01))
                {
                    //如果复制失败，则说明该块有问题，需要找新的块
                    goto BadRemapBlock;
                }
            }
            //对于当前页，只好从刚刚写入的错误地址去复制
            if(0x01 == (NandFlash_CopyPage(Addr, RemapBlockAddr + i * NandFlash_PAGE_SIZE) & 0x01))
            {
                //如果复制失败，则说明该块有问题，需要找新的块
                goto BadRemapBlock;
            }
            //复制完毕，则退出循环
            goto Exit;
            //   break;

        default:
            break;
        }
BadRemapBlock:
        //如果操作过程中失败，则要标志该块已经损坏
        NandFlash_MarkRemapBlockBad(RemapBlockAddr);
    }
Exit:
    //更新坏块表
    NandFlash_UpdateBadBlockTable(Addr, RemapBlockAddr);
    return RemapBlockAddr + (Addr & (NandFlash_BLOCK_SIZE - 1));
}

/********************************************************************
函数功能：NAND FTL函数 - 将一块数据复制到交换区。同时将原来的块删除，
          并将该块内Addr所在页前面的页面复制回原来的块。
入口参数：Addr：要复制出来的块地址。
返    回：原来块的地址。
备    注：如果在复制回去的过程中，出现错误，
          那么说明原来的块已经损坏，需要重新影射到一个好的块。
          这时返回的地址就是重新影射过后的地址。
********************************************************************/
static uint32_t NandFlash_CopyBlockToSwap(uint32_t Addr)
{
    uint32_t SwapAddr;
    uint32_t i;
    uint32_t BlockStartAddr;

    BlockStartAddr = (Addr) & (~(NandFlash_BLOCK_SIZE - 1)); //计算块起始地址

    while(1)
    {
        SwapAddr = NandFlash_ManageSwapBlock(1); //获取下一个交换区
        if(0x00 == (NandFlash_EraseBlock(SwapAddr) & 0x01)) //如果擦除成功
        {
            for(i = 0; i < NandFlash_BLOCK_SIZE / NandFlash_PAGE_SIZE; i++) //将对应块中所有页复制到交换区中
            {
                //复制一页
                if(0x01 & NandFlash_CopyPage(BlockStartAddr + i * NandFlash_PAGE_SIZE, SwapAddr + i * NandFlash_PAGE_SIZE))
                {
                    //如果复制失败，则说明该交换块已经损坏，查找下一个可用的交换块
                    goto BadSwapBlock;
                }
            }
            //全部复制完毕，则擦除掉原来的块
            if(0x01 == (NandFlash_EraseBlock(BlockStartAddr) & 0x01)) //如果擦除失败
            {
                Addr = NandFlash_DealBadBlock(Addr, 1); //处理擦除时遇到的坏块
                BlockStartAddr = (Addr) & (~(NandFlash_BLOCK_SIZE - 1)); //计算块起始地址
            }
            //将前面部分不会写到的页复制回去
            for(i = 0; i < (Addr - BlockStartAddr) / NandFlash_PAGE_SIZE; i++)
            {
                //复制一页
                if(0x01 & NandFlash_CopyPage(SwapAddr + i * NandFlash_PAGE_SIZE, BlockStartAddr + i * NandFlash_PAGE_SIZE))
                {
                    //如果复制失败，则处理该坏块
                    //注意FlashDealBadBlock返回的是当前正在操作的扇区地址，
                    //需要取出其块地址加上Addr原来的扇区地址合成新的扇区地址
                    Addr = (NandFlash_DealBadBlock(BlockStartAddr + i * NandFlash_PAGE_SIZE, 2) & (~(NandFlash_BLOCK_SIZE - 1)))
                           + (Addr & (NandFlash_BLOCK_SIZE - 1));
                    BlockStartAddr = (Addr) & (~(NandFlash_BLOCK_SIZE - 1)); //计算块起始地址
                }
            }
            return Addr; //复制完毕，返回
        }
        else //否则，擦除失败
        {
BadSwapBlock:
            //标志该块擦除时被损坏
            NandFlash_ManageSwapBlock(3);
        }
    }
    // return Addr;
}

/********************************************************************
函数功能：NAND API函数 - 往FLASH中写一个扇区（FLASH_SECTOR_SIZE字节）。
入口参数：Addr: 字节地址；pBuf：保存数据的缓冲区；Remain：预计接下来还需要写多少扇区
返    回：写入的状态。0：成功。非0：失败。
备    注：当Remain不为0时，当前页以及该块内剩余部分将不会回写！
          如果数据传输结束，应该将Remain置0，将数据写回。
********************************************************************/
uint32_t NandFlash_WriteOneSector(uint32_t Addr, uint8_t *pBuf, uint32_t Remain)
{
    uint32_t i;
    uint32_t SwapPageAddr;
    // printf("Addr = 0x%x\r\n",Addr);
    // printf("Remain = 0x%x\r\n",Remain);

    if(Addr > NandFlash_MAX_SECTOR_ADDR)return 1; //如果地址超出范围，则返回失败代码1，越界
    Addr = NandFlash_AddrRemap(Addr); //重新影射地址
    if((Addr & (~(NandFlash_PAGE_SIZE - 1))) != (NandFlash_CurrentWriteSectorAddr & (~(NandFlash_PAGE_SIZE - 1)))) //如果跨page
    {
        //  printf("跨页\r\n");
        if(NandFlash_NeedWriteBack) //如果前面写了数据，则需要将当前读出的page写回
        {
            if(NandFlash_WritePage() & 0x01) //写入失败
            {
                Addr = NandFlash_DealBadBlock(Addr - NandFlash_PAGE_SIZE, 3) + NandFlash_PAGE_SIZE; //坏块处理
            }
        }
        if((Addr & (~(NandFlash_BLOCK_SIZE - 1))) != (NandFlash_CurrentWriteSectorAddr & (~(NandFlash_BLOCK_SIZE - 1)))) //如果跨block，则需要擦除新的块，
        {
            //在擦除之前，要先将原来的块复制到交换区，并且将该块前面部分数据写回
            //该函数除了将整块数据复制到交换区以外，并且还将擦除掉原来的块，然后将前面部分复制回原来的块
            //   printf("跨块\r\n");
            Addr = NandFlash_CopyBlockToSwap(Addr);
        }
        //从交换区中读出对应的一页
        NandFlash_WriteCommand(0x00);
        NandFlash_WriteAddr4Byte(NandFlash_ManageSwapBlock(2) + (Addr & (NandFlash_BLOCK_SIZE - 1)));
        NandFlash_WriteCommand(0x35);
        NandFlash_Wait();
        //随机写
        NandFlash_WriteCommand(0x85);
        NandFlash_WriteAddr4Byte(Addr); //写4字节地址
        for(i = 0; i < NandFlash_SECTOR_SIZE; i++)
        {
            NandFlash_WriteDataByte(pBuf[i]);
        }
        NandFlash_NeedWriteBack = 1; //需要写回
    }

    else  //没有超过一页地址，则直接写数据
    {
        //随机写
        //  printf("直接写\r\n");
        NandFlash_WriteCommand(0x85);
        NandFlash_WriteAddr2Byte(Addr);
        for(i = 0; i < NandFlash_SECTOR_SIZE; i++)
        {
            NandFlash_WriteDataByte(pBuf[i]);
        }
        NandFlash_NeedWriteBack = 1; //需要写回
    }
    NandFlash_CurrentWriteSectorAddr = Addr; //保存本次地址
    if(Remain == 0) //剩余扇区数为0，不会再写了，需要写回
    {
        //  printf("写回\r\n");
        if(NandFlash_NeedWriteBack) //如果前面写了数据，则需要将当前读出的page写回
        {
            if(NandFlash_WritePage() & 0x01) //写入失败
            {
                Addr = NandFlash_DealBadBlock(Addr, 3); //坏块处理
            }
        }
        //计算剩余页数
        Remain = (((Addr + NandFlash_BLOCK_SIZE) & (~(NandFlash_BLOCK_SIZE - 1))) - (Addr & (~(NandFlash_PAGE_SIZE - 1)))) / NandFlash_PAGE_SIZE - 1;
        //计算在交换块中的起始页地址
        SwapPageAddr = NandFlash_ManageSwapBlock(2) + (Addr & (NandFlash_BLOCK_SIZE - 1));

        for(i = 0; i < Remain; i++) //将该块内保存在交换块中剩余部分页的数据复制回该块
        {
            Addr += NandFlash_PAGE_SIZE; //从下一页开始写
            SwapPageAddr += NandFlash_PAGE_SIZE;
            if(0x01 == (NandFlash_CopyPage(SwapPageAddr, Addr) & 0x01)) //如果复制失败
            {
                Addr = NandFlash_DealBadBlock(Addr, 2); //处理坏块
            }
        }
        NandFlash_NeedWriteBack = 0; //清除需要写回标志
        NandFlash_CurrentWriteSectorAddr = ~0;
    }
    return 0;
}

/********************************************************************
函数功能：NAND API函数 - 往FLASH中写一个扇区（FLASH_SECTOR_SIZE字节）。
入口参数：Addr: 字节地址；pBuf：保存数据的缓冲区；Remain：预计接下来还需要写多少扇区
返    回：写入的状态。0：成功。非0：失败。
备    注：当Remain不为0时，当前页以及该块内剩余部分将不会回写！
          如果数据传输结束，应该将Remain置0，将数据写回。
********************************************************************/
uint32_t NandFlash_ReadOneSector(uint32_t Addr, uint8_t  *pBuf, uint32_t Remain)
{
    uint32_t i;
    if(Addr > NandFlash_MAX_SECTOR_ADDR)return 1; //如果地址超出范围，则返回失败代码1，越界
    Addr = NandFlash_AddrRemap(Addr); //重新影射地址
    if((Addr & (~(NandFlash_PAGE_SIZE - 1)))
            != (NandFlash_CurrentReadSectorAddr & (~(NandFlash_PAGE_SIZE - 1)))) //如果跨page
    {
        //如果跨页的，则写读数据命令
        NandFlash_WriteCommand(0x00);
        NandFlash_WriteAddr4Byte(Addr);
        NandFlash_WriteCommand(0x30);
        NandFlash_Wait(); //等待数据读回
    }
    else
    {
        //如果没有跨页，则可以直接读
        NandFlash_WriteCommand(0x05);
        NandFlash_WriteAddr2Byte(Addr);
        NandFlash_WriteCommand(0xE0);
        NandFlash_Wait(); //等待数据读回
    }
    for(i = 0; i < NandFlash_SECTOR_SIZE; i++)
    {
        pBuf[i] = NandFlash_ReadDataByte(); //读一字节数据
    }
    NandFlash_CurrentReadSectorAddr = Addr; //保存当前操作的地址
    if(Remain == 0) //如果不会接着读，那么就设置当前读过的地址为无效值
    {
        NandFlash_CurrentReadSectorAddr = ~0;
    }

    return 0;
}

/********************************************************************
函数功能：NAND API函数 - 初始化NAND
入口参数：无
返    回：无
备    注：无
********************************************************************/
void NandFlash_Init(void)
{
    NandFlash_CurrentWriteSectorAddr = ~0; //初始化当前读、写地址为无效
    NandFlash_CurrentReadSectorAddr = ~0;
    NandFlash_NeedWriteBack = 0; //初始化不用回写
    NandFlash_ManageSwapBlock(0);  //初始化交换块
    NandFlash_LoadBadBlockTable(); //从FLASH中加载坏块表
}
