# Nand 磨损平衡算法

------

这个工程演示一个Nand磨损平衡算法,基于微雪的LPC1788核心板.首先他的硬件配置是这样的.

> * LPC1788FBD208
> * Nand Flash : k9F1G08U0D
> * Nor Flash : SST39VF3201B [工程未使用]
> * SDRAM : HY57V2562GTR * 2  [工程未使用]

系统配置状态:

> * PLL0 = 120MHz = SYSCLK
> * EMCCLK = 120MHz / 2 = 60MHz
> * PLL1 = 48MHz

NAND配置状态:

> * 交换Block : 10个
> * 重映射Block : 50个
> * 坏块表Block : 3个

工程基于Keil UV5.在LPCXpresso IDE上没编译成功.具体查看:https://community.nxp.com/message/903811#
