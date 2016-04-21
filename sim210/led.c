#define GPJ2CON (volatile unsigned long*)0xE0200280
#define GPJ2DAT (volatile unsigned long*)0xE0200284

#define GPJ2CON_MAP	(volatile unsigned long*)0xA0200280
#define GPJ2DAT_MAP (volatile unsigned long*)0xA0200284

/*
*		LED��ڳ�ʼ��
*/

void led_init(void)
{
		*GPJ2CON = (unsigned long)0x1111;		//��GPIOJ2_0~GPIOJ2_3����Ϊ�����
		*GPJ2DAT = 0x0;
}

void led_light(unsigned long data)
{
		*GPJ2DAT = (unsigned long)data;			//����ȫ��LED			����ʹ��MMUӳ��ĵ�ַ
}
//��LED
void led_on(void)
{
		led_light(0);
}
//�ر�LED
void led_off(void)
{
		led_light(~0x0);
}