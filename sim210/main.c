//#include "include/mmu.h"
#include "include/led.h"
//#include "include/interrupt.h"

int myboot_main()
{
	//��ʼ��MMU
	//mmu_init();
	//��ʼ��LED
	led_init();
	//��ʼ����ť
	//button_init();
	//led_on();
	//�жϳ�ʼ��
	//init_irq();
	//��ѭ��
	while(1);
	return 0;
}