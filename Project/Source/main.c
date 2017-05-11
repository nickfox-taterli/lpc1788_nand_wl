#include "LPC177x_8x.h"
#include "arm_math.h"

#include "EMC.h"

#define BUFFER_SIZE         512
uint8_t TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];  //发送和接收数据缓冲区

void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
    uint16_t IndexTmp = 0;

    /* Put in global buffer same values */
    for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
    {
        pBuffer[IndexTmp] = IndexTmp + Offset;
    }
}

int32_t main()
{

    NORRAM_Init();

   NandFlash_Init();
    Fill_Buffer(TxBuffer, BUFFER_SIZE , 0xA5);
    NandFlash_WriteOneSector(0, TxBuffer, 0); /* bug in here */
    NandFlash_ReadOneSector(0, RxBuffer, 0);

    while(1)
    {

    }
}
