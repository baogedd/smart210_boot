#include "include/led.h"

//����EXT_INT_2_CON�ⲿ�жϼĴ��� ����EXIT16~EXIT9  �ֲ�p336
#define EXT_INT_2_CON       *((volatile unsigned int *)0xE0200E08) 
//�ⲿ�ж����μĴ���	�ֲ�p349
#define EXT_INT_2_MASK			*((volatile unsigned int *)0xE0200F08)

//�ж�ʹ�ܼĴ���
#define VIC0INTENABLE       *((volatile unsigned int *)0xF2000010)  
//�ж�������ַ�Ĵ���  �����жϷ�����ת�����ĺ�����ַ
#define VIC0_16VECTORADDR		*((volatile unsigned int *)0xF2000010 + 4*16)
//�жϷ�����־�Ĵ���	�ֲ�P351
#define EXT_INT_2_PEND      *((volatile unsigned int *)0xE0200F48) 
//���жϴ�������д��üĴ������������������
#define VIC0ADDRESS					*((volatile unsigned int *)0xE2000F00)	
/*
*		�ⲿ�ж�ʹ�õ�	
* 	GPH2_0,EINT16 GPH2_1,EINT17 GPH2_2,EINT18 GPH2_3,EINT19
*   
*/

void EINT16_31_Handler(void)
{
		volatile unsigned int key_code;
		//���Ĵ���r0~r12��lr�Ĵ��� �����ջ
		__asm__( 
    
    "sub lr, lr, #4\n"  
    "stmfd sp!, {r0-r12, lr}\n"       
    : 
    : 
   );
   
   key_code = EXT_INT_2_PEND & 0b11;		//�����Ӧ�жϷ���  ��Ӧλ��1
   
   switch(key_code) 
   {
   		case 1:		//key1�����ж�	ENT16
   			led_on();
   			break;
   		case 2:		//key2�����ж�	ENT17
   			led_off();
   			break;	
   		default:
   			break;
   }
   /*����ж�*/
   EXT_INT_2_PEND = ~0x0;		//Ϊʲôȫ����1 
   VIC0ADDRESS = 0;
   //�Ĵ�����ջ���ҷ���  ^��ʾ���һָ�CPSR
   __asm__( 
    "ldmfd sp!, {r0-r12, pc}^ \n"       
    : 
    : 
  );
}

void init_irq(void)
{
		//���ö�Ӧ���ⲿ�ж�Ϊ�½��ش���
		EXT_INT_2_CON = 0b010 | (0b010<<4);    /*�½��ش����ж�*/
		//���ⲿ�жϴ�
		EXT_INT_2_MASK = 0;                   /* ȡ�������ⲿ�ж�*/  
		//��ʼ���ж�
		VIC0INTENABLE = 0b1 << 16;						/* �ⲿ�ж�16~31���� */
		//�����ж�������ַ
		VIC0_16VECTORADDR = (int)EINT16_31_Handler;  /*����key1~key4����ͬһ�ж�Դ������ֻ������һ����ַ*/
		//��CPSR�Ķ�ӦIRQ�ж�λ�� C���asm		ARM���ֲ��ж���
		__asm__( 
    /*���ж�*/  
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"            
    : 
    : 
  );
}