/* 
 * 用于段描述符的一些宏定义
 */ 
#define MMU_FULL_ACCESS     (3 << 10)   /* 访问权限 rw-rw- 全部可访问*/
#define MMU_DOMAIN          (0 << 5)    /* 属于哪个域 为什么是任何访问都会产生域错误？*/
#define MMU_SPECIAL         (1 << 4)    /* 必须是1 XN=0*/
#define MMU_CACHEABLE       (1 << 3)    /* cacheable 开启Cache*/
#define MMU_BUFFERABLE      (1 << 2)    /* bufferable 使能write buffer*/
#define MMU_SECTION         (2)         /* 表示这是段描述符 */
#define MMU_SECDESC         (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_SECTION)
#define MMU_SECDESC_WB      (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_CACHEABLE | MMU_BUFFERABLE | MMU_SECTION)


/*
* 建立页表
*/
void create_page_table(void)
{
	//TTB 页表初始地址
	unsigned long *ttb = (unsigned long *)0x20000000;
	unsigned long vaddr, paddr;  //vddr为虚拟地址   pddr为物理地址
	vaddr = 0xA0000000;
  paddr = 0xE0200000;
  //映射物理地址paddr 映射到vaddr  映射1MB   将外设地址映射到内存  外设不可使用Cache和write buffer
  *(ttb + (vaddr >> 20)) = (paddr & 0xFFF00000) | MMU_SECDESC;
  vaddr = 0x20000000;
  paddr = 0x20000000;
  //将其内存映射到内存  这里其实就是映射会原来的位置
  while (vaddr < 0x24000000)
  {
  	*(ttb + (vaddr >> 20)) = (paddr & 0xFFF00000) | MMU_SECDESC_WB;
  	vaddr += 0x100000;
  	paddr += 0x100000;
  }
}

void mmu_enable()
{
   __asm__(
    
    /*设置TTB*/
    "ldr    r0, =0x20000000\n"                  
    "mcr    p15, 0, r0, c2, c0, 0\n"    
    
    
    /*不进行权限检查*/
    "mvn    r0, #0\n"                   
    "mcr    p15, 0, r0, c3, c0, 0\n"    
    
    
   /*使能MMU*/
    "mrc    p15, 0, r0, c1, c0, 0\n"    
    "orr    r0, r0, #0x0001\n"          
    "mcr    p15, 0, r0, c1, c0, 0\n"    
    : 
    : 
  );
}

void mmu_init()
{
		//初始化  MMU页表
    create_page_table();
    //打开MMU
    mmu_enable();
}