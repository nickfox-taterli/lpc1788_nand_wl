#include "LPC177x_8x.h"
#include "arm_math.h"

#include "EMC.h"
#include "ff.h"

FATFS fs;
FRESULT res;

void RTC_Init(void)
{
    if(!LPC_RTC->CCR.CLKEN)
    {
        LPC_RTC->CCR.CLKEN = 1;
        LPC_RTC->SEC = 0;
        LPC_RTC->MIN = 0;
        LPC_RTC->HOUR = 10;

        LPC_RTC->DOW = 6;
        LPC_RTC->DOM = 1;
        LPC_RTC->DOY = 1;

        LPC_RTC->MONTH = 1;
        LPC_RTC->YEAR = 2017;

        LPC_RTC->CALIBRATION = 4;
        LPC_RTC->CCR.CCALEN = 0;
    }
}

int32_t main()
{

    SEGGER_RTT_Init();

    NORRAM_Init();
    RTC_Init();

    res = f_mount(&fs, "0:", 1);

    while(1)
    {

    }
}
