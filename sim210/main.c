//#include "include/mmu.h"
#include "include/led.h"
//#include "include/interrupt.h"

int myboot_main()
{
	//初始化MMU
	//mmu_init();
	//初始化LED
	led_init();
	//初始化按钮
	//button_init();
	//led_on();
	//中断初始化
	//init_irq();
	//死循环
	while(1);
	return 0;
}