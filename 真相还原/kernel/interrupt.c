#include "interrupt.h"
#include "stdint.h"
#include "global.h"
#include "io.h"
#include "print.h"

//可编程中断控制器8259A，主片控制端口0x20,数据端口0x21,从片控制端口0xa0,数据端口0xa1
#define PIC_M_CTRL 0x20
#define PIC_M_DATA 0x21
#define PIC_S_CTRL 0xa0
#define PIC_S_DATA 0xa1

//目前支持的中断数
#define IDT_DESC_CNT 0x81	

extern uint32_t syscall_handler(void);

//eflags寄存器中的if位为1
#define EFLAGS_IF 0x00000200
#define GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0" : "=g" (EFLAG_VAR))

//中断门描述符结构体
struct gate_desc{
	//中断处理程序在目标代码段内的偏移量的15~0位
	uint16_t func_offset_low_word;
	//中断处理程序目标代码段描述符选择子
	uint16_t selector;
	//双字计数字段，门描述符中的第4字节，固定值（书上应该是全0）
	uint8_t dcount;
	//属性值，8位，TYPE，S，P，DPL
	uint8_t attribute;
	//中断处理程序在目标段内的偏移量31~16位
	uint16_t func_offset_high_word;
};

//静态函数声明，非必须
static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function);
//idt是中断描述符表，本质数组
static struct gate_desc idt[IDT_DESC_CNT];

//用于保存异常的名字
char* intr_name[IDT_DESC_CNT];
//定义中断处理程序数组，在kernel.S中定义的intrXXentry，只是中断处理程序的入口，最终调用的是ide_table中的处理程序
intr_handler idt_table[IDT_DESC_CNT];
//声明引用定义在kernel.S中的中断处理函数入口
extern intr_handler intr_entry_table[IDT_DESC_CNT];

//初始化可编程中断控制器8259A
static void pic_init(void){
	//初始化主片
	outb(PIC_M_CTRL, 0x11);		//ICW1：边沿触发，级联8259，需要ICW4
	outb(PIC_M_DATA, 0x20);		//ICW2：起始中断向量号0x28，即IR[0~7]为0x20～0x27
	outb(PIC_M_DATA, 0x04);		//ICW3：IR2接从片
	outb(PIC_M_DATA, 0x01);		//ICW4：8086模式，正常EOI

	//初始化从片
	outb(PIC_S_CTRL, 0x11);		//ICW1：边沿触发，级联8259，需要ICW4
	outb(PIC_S_DATA, 0x28);		//ICW2：起始中断向量号0x28，即IR[0~7]为0x28～0x2F
	outb(PIC_S_DATA, 0x02);		//ICW3：设置从片连接到主片的IR2引脚
	outb(PIC_S_DATA, 0x01);		//ICW4：8086模式，正常EOI

	//打开主片上IR0，也就是目前只接受时钟产生的中断
	//outb(PIC_M_DATA, 0xfe);
	//outb(PIC_S_DATA, 0xff);

	outb(PIC_M_DATA, 0xfc);
	outb(PIC_S_DATA, 0xff);	

	put_str("    pic_init done.\n");

}

//创建中断门描述符
static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function){
	p_gdesc->func_offset_low_word = (uint32_t)function & 0x0000FFFF;
	p_gdesc->selector = SELECTOR_K_CODE;
	p_gdesc->dcount = 0;
	p_gdesc->attribute = attr;
	p_gdesc->func_offset_high_word = ((uint32_t)function & 0xFFFF0000) >> 16;
}

//初始化中断描述符表
static void idt_desc_init(void){
	int i, lastindex = IDT_DESC_CNT - 1;
	for(i = 0; i < IDT_DESC_CNT; i++){	
		make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0, intr_entry_table[i]);
	}
	//单独处理系统调用
	make_idt_desc(&idt[lastindex], IDT_DESC_ATTR_DPL3, syscall_handler);
	put_str("    idt_desc_init done.\n");
}

//通用的中断处理函数，一般在异常出现时的处理
static void general_intr_handler(uint8_t vec_nr){
	//IRQ7和IRQ15会产生伪中断(spurious interrupt)，无需处理
	//0x2f是从片8259A上的最后一个IRQ引脚，保留项
	if(vec_nr == 0x27 || vec_nr == 0x2f){
		return;
	}
	
	//重置光标为左上角
	set_cursor(0);	
	int cursor_pos = 0;
	while(cursor_pos < 320){
		put_char(' ');
		cursor_pos++;
	}
	set_cursor(0);
	put_str("!!!!!!!!!!!!!!!!	excetion message begin.		!!!!!!!!!\n");
	set_cursor(88);
	put_str(intr_name[vec_nr]);
	
	//若是缺页，打印缺失的地址
	if(vec_nr == 14){
		int page_fault_vaddr = 0;
		asm("movl %%cr2, %0" : "=r" (page_fault_vaddr));
		put_str("\npage fault addr is ");
		put_int(page_fault_vaddr);		
	}
	put_str("\n!!!!!!	excetion message end	!!!!!\n");
	while(1);
}

//完成一般中断处理函数注册及异常名称注册
static void exception_init(void){
	int i;
	//idt_table数组中的函数是在进入中断后根据中断向量号调用的
	//在kernel.S的call[idt_table + %1*4]
	for(i = 0; i < IDT_DESC_CNT; i++){
		//默认为general_intr_handler
		//之后由register_handler来注册具体处理函数
		idt_table[i] = general_intr_handler;		
		
		//先统一赋值为unknown
		intr_name[i] = "unknown";
	}
	intr_name[0] = "#DE Divide Error";     
	intr_name[1] = "#DB Debug Exception";       
	intr_name[2] = "#NMI Interrupt";
        intr_name[3] = "#BP Breakpoint Exception";        
        intr_name[4] = "#OF Overflow Exception";        
        intr_name[5] = "#BR BOUND Range Exceeded Exception";
        intr_name[6] = "#UD Invalid Opcode Exception";        
        intr_name[7] = "#NM Device Not Available Exception";        
        intr_name[8] = "#DF Double Fault Exception";
        intr_name[9] = "Coprocessor Segment Overrun";        
        intr_name[10] = "#TS Invalid TSS Exception";        
        intr_name[11] = "#NP Segment Not Present";
        intr_name[12] = "#SS Stack Fault Exception";        
        intr_name[13] = "#GP General Protection Exception";        
        intr_name[14] = "#PF Page-Fault Rxception";
	//intr_name[15]是intel保留项，未使用
        intr_name[16] = "#MF x87 FPU Floating-Point Error";        
        intr_name[17] = "#AC Alignment Check Exception";        
        intr_name[18] = "#MC Machine-Check Exception";
        intr_name[19] = "#XF SIMD Floating-Point Exception";        
}

void register_handler(uint8_t vector_no, intr_handler function){
	idt_table[vector_no] = function;
}

//开中断并返回开中断前的状态
enum intr_status intr_enable(){
	enum intr_status old_status;
	if(INTR_ON == intr_get_status()){
		old_status = INTR_ON;
		return old_status;
	}
	else{
		old_status = INTR_OFF;
		//开中断，sti指令将IF位置1
		asm volatile("sti");
		return old_status;
	}
}

//关中断，并且返回关中断前的状态
enum intr_status intr_disable(){
	enum intr_status old_status;
	if(INTR_ON == intr_get_status()){
		old_status = INTR_ON;
		//关中断，cli指令将if位置0
		asm volatile("cli" : : : "memory");
		return old_status;
	}
	else{
		old_status = INTR_OFF;
		return old_status;
	}
}

//将中断状态设置为status
enum intr_status intr_set_status(enum intr_status status){
	return status & INTR_ON ? intr_enable() : intr_disable();
}

//获取当前中断状态
enum intr_status intr_get_status(){
	uint32_t eflags = 0;
	GET_EFLAGS(eflags);
	return (EFLAGS_IF & eflags) ? INTR_ON : INTR_OFF;
}

//完成中断的所有初始化工作
void idt_init(){
	put_str("idt_init start.\n");
	idt_desc_init();
	exception_init();
	pic_init();

	//加载idt
	uint64_t idt_operand = ((sizeof(idt) - 1) | ((uint64_t)(uint32_t)idt << 16));
	asm volatile("lidt %0" : : "m" (idt_operand));
	put_str("idt_init done.\n");
}
