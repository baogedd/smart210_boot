#include "include/led.h"

//设置EXT_INT_2_CON外部中断寄存器 设置EXIT16~EXIT9  手册p336
#define EXT_INT_2_CON       *((volatile unsigned int *)0xE0200E08) 
//外部中断屏蔽寄存器	手册p349
#define EXT_INT_2_MASK			*((volatile unsigned int *)0xE0200F08)

//中断使能寄存器
#define VIC0INTENABLE       *((volatile unsigned int *)0xF2000010)  
//中断向量地址寄存器  设置中断发生后转跳到的函数地址
#define VIC0_16VECTORADDR		*((volatile unsigned int *)0xF2000010 + 4*16)
//中断发生标志寄存器	手册P351
#define EXT_INT_2_PEND      *((volatile unsigned int *)0xE0200F48) 
//在中断处理函数里写入该寄存器可以用于清除函数
#define VIC0ADDRESS					*((volatile unsigned int *)0xE2000F00)	
/*
*		外部中断使用的	
* 	GPH2_0,EINT16 GPH2_1,EINT17 GPH2_2,EINT18 GPH2_3,EINT19
*   
*/

void EINT16_31_Handler(void)
{
		volatile unsigned int key_code;
		//将寄存器r0~r12和lr寄存器 保存进栈
		__asm__( 
    
    "sub lr, lr, #4\n"  
    "stmfd sp!, {r0-r12, lr}\n"       
    : 
    : 
   );
   
   key_code = EXT_INT_2_PEND & 0b11;		//如果对应中断发生  对应位置1
   
   switch(key_code) 
   {
   		case 1:		//key1发生中断	ENT16
   			led_on();
   			break;
   		case 2:		//key2发生中断	ENT17
   			led_off();
   			break;	
   		default:
   			break;
   }
   /*清除中断*/
   EXT_INT_2_PEND = ~0x0;		//为什么全部置1 
   VIC0ADDRESS = 0;
   //寄存器出栈并且返回  ^表示并且恢复CPSR
   __asm__( 
    "ldmfd sp!, {r0-r12, pc}^ \n"       
    : 
    : 
  );
}

void init_irq(void)
{
		//设置对应的外部中断为下降沿触发
		EXT_INT_2_CON = 0b010 | (0b010<<4);    /*下降沿触发中断*/
		//将外部中断打开
		EXT_INT_2_MASK = 0;                   /* 取消屏蔽外部中断*/  
		//初始化中断
		VIC0INTENABLE = 0b1 << 16;						/* 外部中断16~31开启 */
		//设置中断向量地址
		VIC0_16VECTORADDR = (int)EINT16_31_Handler;  /*由于key1~key4属于同一中断源，所以只需设置一个地址*/
		//将CPSR的对应IRQ中断位打开 C混编asm		ARM核手册有定义
		__asm__( 
    /*开中断*/  
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"            
    : 
    : 
  );
}