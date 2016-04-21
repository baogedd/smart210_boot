#define GPH2CON *((volatile long *)0xE0200C40)

/*
*		K1��ӦGPH2_0   K2��ӦGPH2_1  K3��ӦGPH2_2  K4��ӦGPH2_3
*
*/

#define GPH0_int     (0xf<<(0*4))		//��GPH2_0����Ϊ�ⲿ�ж�����
#define GPH1_int     (0xf<<(1*4))		//��GPH2_1����Ϊ�ⲿ�ж�����
#define GPH2_int     (0xf<<(2*4))		//��GPH2_2����Ϊ�ⲿ�ж�����
#define GPH4_int     (0xf<<(4*4))		//��GPH2_3����Ϊ�ⲿ�ж�����

//�������
#define GPH0_msk    (0xf<<(0*4))
#define GPH1_msk    (0xf<<(1*4))
#define GPH2_msk    (0xf<<(2*4))
#define GPH4_msk    (0xf<<(4*4))


void button_init(void)
{
		//���λ
		GPH2CON &= ~(GPH0_msk | GPH1_msk | GPH2_msk | GPH4_msk);
		//����Ϊ�ⲿ�жϿ�
		GPH2CON |= GPH0_int | GPH1_int | GPH2_int | GPH4_int;
}
