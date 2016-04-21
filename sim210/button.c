#define GPH2CON *((volatile long *)0xE0200C40)

/*
*		K1对应GPH2_0   K2对应GPH2_1  K3对应GPH2_2  K4对应GPH2_3
*
*/

#define GPH0_int     (0xf<<(0*4))		//将GPH2_0定义为外部中断引脚
#define GPH1_int     (0xf<<(1*4))		//将GPH2_1定义为外部中断引脚
#define GPH2_int     (0xf<<(2*4))		//将GPH2_2定义为外部中断引脚
#define GPH4_int     (0xf<<(4*4))		//将GPH2_3定义为外部中断引脚

//用于清除
#define GPH0_msk    (0xf<<(0*4))
#define GPH1_msk    (0xf<<(1*4))
#define GPH2_msk    (0xf<<(2*4))
#define GPH4_msk    (0xf<<(4*4))


void button_init(void)
{
		//清除位
		GPH2CON &= ~(GPH0_msk | GPH1_msk | GPH2_msk | GPH4_msk);
		//设置为外部中断口
		GPH2CON |= GPH0_int | GPH1_int | GPH2_int | GPH4_int;
}
