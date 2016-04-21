@****************************
@File:start.S
@
@S5PV210 bootloader
@****************************

.text
.global	_start
_start:
		b	reset						
		ldr	pc, _undefined_instruction	
		ldr	pc, _software_interrupt		
		ldr	pc, _prefetch_abort			
		ldr	pc, _data_abort				
		ldr	pc, _not_used				
		ldr	pc, _irq					
		ldr	pc, _fiq

_undefined_instruction: .word undefined_instruction
_software_interrupt:	.word software_interrupt
_prefetch_abort:	.word prefetch_abort
_data_abort:		.word data_abort
_not_used:		.word not_used
_irq:			.word irq
_fiq:			.word fiq					

undefined_instruction:
		nop

software_interrupt:
		nop

prefetch_abort:
		nop

data_abort:
		nop

not_used:
		nop

irq:
		nop

fiq:
		nop

reset:
		bl set_svc						@设置CPU位SVC模式
		bl disable_watchdog		@关闭看门狗
		bl disable_interrupt	@关闭中断
		bl disable_mmu				@关闭mmu
		bl clock_init					@初始化时钟
		bl mem_init						@内存初始化
		bl copy_to_ram				@目前只是虚拟将IRAM的代码拷贝到DRAM中运行  因为目前boot代码还没有超过96KB 可以全部放在IRAM
		bl init_stack					@栈初始化  搭建C语言运行环境
		bl clean_bss					@清除BSS段  BSS段是用来存放程序中未初始化的全局变量和静态变量
		@bl light_led	
		ldr pc,=myboot_main		@转跳到main.c文件的  myboot_main函数中
		@bl light_led					@点亮LED
set_svc:									@设置CPU为SVC模式  并且清除CPSR的I,F位
		mrs r0, cpsr					@读取cpsr值到r0
		bic r0, r0, #0x1f			@将r0对应位清0
		orr r0, r0, #0xd3			@将R0对应0xd3置1 0b1101 0011并且将irq,fiqg关闭
		msr cpsr, r0					@将r0的值保存到cpsr
		mov pc, lr
#define WTCON 0xE2700000	@WTCON 看门狗控制定时器
disable_watchdog:					@关闭看门狗
		ldr r0,=WTCON
		mov r1,#0x0						@将WTCON清0
		str r1,[r0]						@将R1值赋值给R0作为地址指向的存储单元  就是WTCON
		mov pc,lr							@函数返回

disable_interrupt:				@关闭中断
		mvn r0,#0							@取反赋值给r0全1
		ldr r1,=0xF2000014 	@VIC0INTENCLEAR寄存器
		str r0,[r1]
		
		ldr r1,=0xF2100014	  @VIC1INTENCLEAR寄存器
		str r0,[r1]
		
		ldr r1,=0xF2200014	  @VIC2INTENCLEAR寄存器
		str r0,[r1]
		
		ldr r1,=0xF2300014	  @VIC3INTENCLEAR寄存器
		str r0,[r1]
		
		mov pc,lr
disable_mmu:							@关闭MMU并且 刷新I/DCache 并且使能D Cache
		mcr p15,0,r0,c7,c5,0	@Invalidate Bothes Cache	
		mcr p15,0,r0,c7,c6,1
		mrc p15,0,r0,c1,c0,0	@读取CP15的C1寄存器的值到R0  ARM核手册上都有操作指令 C1寄存器为Control Register
		bic r0,r0,#0x00000007	@清除位
		mcr p15,0,r0,c1,c0,0	@将值都会CP15的C1寄存器 关闭MMU和D Cache
		mov pc,lr
#define CLK_DIV0	0xe0100300		@CLK_DIV0时钟分频器0
#define APLL_CON0	0xe0100100		@APLL_CON0寄存器
#define APLL_CON1 0xe0100104		@APLL_CON1寄存器
#define MPLL_CON	0xe0100108		@MPLL_CON寄存器
#define EPLL_CON0	0xe0100110		@EPLL_CON0寄存器
#define EPLL_CON1 0xe0100114		@EPLL_CON1寄存器
#define VPLL_CON  0xe0100120		@VPLL_CON寄存器
#define CLK_SRC0  0xe0100200		@CLK_SRC0 时钟选择寄存器

#define DIV_VAL ((0x0<<0)|(4<<4)|(4<<8)|(1<<12)|(3<<16)|(1<<20)|(4<<24)|(1<<28))
@ARMCLK = MOUT_MSYS   SCLKARM = SCLKAPLL/5  HCLK_MSYS = ARMCLK/5 
@PCLK_MSYS = HCLK_MSYS/2 HCLK_DSYS = MOUT_DSYS/4  PCLK_DSYS = HCLK_DSYS/2 
@HCLK_PSYS = MOUT_PSYS/5 PCLK_PSYS = HCLK_PSYS/2
#define APLL_VAL ((1<<31)|(3<<8)|(125<<16)|(1<<0))
@APLL使能 MDIV = 125 PDIV = 3 SDIV = 1  根据手册输出频率为1000MHz
#define MPLL_VAL ((1<<31)|(12<<8)|(667<<16)|(1<<0))
@MPLL是呢 MDIV = 667 PDIV = 12 SDIV = 1 根据手册输出频率为667MHz

clock_init:								@时钟初始化
		ldr r0,=CLK_DIV0			@设置CLK_DIV0
		ldr r1,=DIV_VAL
		str r1,[r0]
		
		ldr r0,=APLL_CON0			@设置APLL
		ldr r1,=APLL_VAL		
		str r1,[r0]
		
		ldr r0,=MPLL_CON			@设置MPLL
		ldr r1,=MPLL_VAL
		str r1,[r0]
		
		ldr r0, =CLK_SRC0
		ldr r1, =0x1111
		str r1, [r0]
		
		mov pc,lr

copy_to_ram:
		ldr r0, =0xd0020010		@拷贝源地址
		ldr r1, =0x20008000		@拷贝目的地址
		add r3, r0, #1024*4		@结束地址  就是拷贝的大小 4KB？　这里定义的位置是４字节对齐

copy_loop:
		ldr r2, [r0], #4			@将R0指向的存储区域的数据拷贝到R2  应该是4个字节
		str r2, [r1], #4			@将R1的数据拷贝到R1指向的内存 应该是4个字节
		cmp r0, r3						@如果达到1024*４　　说明拷贝结束　　
		bne copy_loop					@如果没有达到目的位置  继续循环拷贝		这里条件应该改成r0 >= r3
		mov pc, lr						@达到目的位置返回

init_stack:								@内存栈初始化  其实就是设置SP指针的位置  因为ARM的堆栈是向下增长的
		msr cpsr_c, #0xd2			@cpsr_c为CPSR寄存器的控制域只操作CPSR的低几位 设置为IRQ模式并且开启中断
		ldr sp, =0x23000000		@此处实际设置的是r13_irq 在IRQ模式下操作的是R13_irq
		msr cpsr_c, #0xd3			@设置系统为SVC模式
		ldr sp, =0x24000000		@设置SP的位置
		mov pc ,lr

clean_bss:
		ldr r0, =bss_start		@r0保存bss段起始地址bss_start定义在连接器脚步myboot.lds bss_start位置是设置了ALIGN(4)
		ldr r1, =bss_end			@r1保存bss段结束地址bss_end定义在连接器脚步myboot.lds
		cmp r0, r1						@如果r0 = r1说明bss中是没有存放数据的
		moveq pc, lr					@bss段为空直接返回
clean_loop:
		mov r2, #0						@设置r2为0  将整个bss段设置为0
		str r2, [r0], #4			@r0指向内存设置为0 r0 = r0 + 4地址加4	每次copy4字节
		cmp r0, r1						@检查r0是否达到r1位置		
		bne clean_loop				@如果r0==r1 说明清除完毕  否则继续清除 这里的条件应该改成r0 >= r1
		mov pc, lr

@GPIOJ2_0~GPIOJ3为led的IO口
#define GPJ2CON	0xE0200280
#define GPJ2DAT 0xE0200284
light_led:								@点亮led
		ldr r0,=GPJ2CON				@r0指向GPJ2CON
		ldr r1,=0x1111					
		str r1,[r0]						@将GPIOJ2_0~GPIOJ2_3设置为输出口
		ldr r0,=GPJ2DAT 
		mov r1,#0b0						@设置为低电平  点亮led
		str r1,[r0]
		mov pc,lr
