[bits 32]
;若在相关的异常中CPU已经自动压入了错误码，为保持栈中格式同一，这里不做操作
%define ERROR_CODE nop

;若在相关的异常CPU中没有压入错误码，为统一栈中格式，就手工压入一个0
%define ZERO push 0

;idt_table是C中注册的中断处理程序数组
extern idt_table

section .data
global intr_entry_table
intr_entry_table:

%macro VECTOR 2
section .text
intr%1entry:
;每个中断处理程序都要压入中断向量号，所以一个中断类型一个中断处理程序
;自己知道自己的中断向量号多少
	%2
	;保存上下文环境
	push ds
	push es
	push fs
	push gs
	;压入32位寄存器
	pushad
	
	;如果是从片上进入的中断，除了向从片上发送EOI外，还要往主片上发送EOI
	;中断结束命令EOI
	mov al,0x20
	
	;向从片发送
	out 0xa0,al
	
	;向主片发送
	out 0x20,al

	;不管idt_table中的目标程序是否需要参数，都一律压入中断向量号，调式时很方便
	push %1
	;调用idt_table中的C版本中断处理函数
	call [idt_table + %1*4]
	jmp intr_exit

section .data
	;存储各个中断入口程序的地址,形成intr_entry_table数组
	dd intr%1entry
%endmacro

section .text
global intr_exit
intr_exit:
;以下是恢复上下文环境
	;跳过中断号
	add esp,4
	popad
	pop gs
	pop fs
	pop es
	pop ds
	add esp,4		;跳过error_code
	iretd

;33个中断处理程序，中断向量0~19为处理器内部固定的异常类型，20~31是Intel保留的
;可用的最低中断向量号为32，将来设置8259A，会把IR0的中断向量号设为32
VECTOR 0x00,ZERO
VECTOR 0x01,ZERO
VECTOR 0x02,ZERO
VECTOR 0x03,ZERO
VECTOR 0x04,ZERO
VECTOR 0x05,ZERO
VECTOR 0x06,ZERO
VECTOR 0x07,ZERO
VECTOR 0x08,ERROR_CODE
VECTOR 0x09,ZERO
VECTOR 0x0a,ERROR_CODE
VECTOR 0x0b,ERROR_CODE
VECTOR 0x0c,ZERO
VECTOR 0x0d,ERROR_CODE
VECTOR 0x0e,ERROR_CODE
VECTOR 0x0f,ZERO
VECTOR 0x10,ZERO
VECTOR 0x11,ERROR_CODE
VECTOR 0x12,ZERO
VECTOR 0x13,ZERO
VECTOR 0x14,ZERO
VECTOR 0x15,ZERO
VECTOR 0x16,ZERO
VECTOR 0x17,ZERO
VECTOR 0x18,ERROR_CODE
VECTOR 0x19,ZERO
VECTOR 0x1a,ERROR_CODE
VECTOR 0x1b,ERROR_CODE
VECTOR 0x1c,ZERO
VECTOR 0x1d,ERROR_CODE
VECTOR 0x1e,ERROR_CODE
VECTOR 0x1f,ZERO
VECTOR 0x20,ZERO	;时钟中断对应入口
VECTOR 0x21,ZERO	;键盘中断
VECTOR 0x22,ZERO	;级联用
VECTOR 0x23,ZERO	;串口2对应的入口
VECTOR 0x24,ZERO	;串口1
VECTOR 0x25,ZERO	;并口2
VECTOR 0x26,ZERO	;软盘
VECTOR 0x27,ZERO	;并口1
VECTOR 0x28,ZERO	;实时时钟
VECTOR 0x29,ZERO	;重定向
VECTOR 0x2a,ZERO	;保留
VECTOR 0x2b,ZERO	;保留
VECTOR 0x2c,ZERO	;ps/2鼠标
VECTOR 0x2d,ZERO	;fpu浮点单元异常
VECTOR 0x2e,ZERO	;硬盘
VECTOR 0x2f,ZERO	;保留

;;;	0x80号中断	;;
[bits 32]
extern syscall_table
section .text
global syscall_handler
syscall_handler:
;1保存上下文环境
	push 0
	push ds
	push es
	push fs
	push gs
	pushad
	push 0x80
;2为系统调用字功能传入参数
	push edx
	push ecx
	push ebx
;3调用字功能处理函数
	call [syscall_table + eax * 4]
	add esp,12
;将call调用返回值存入eax
	mov [esp + 8 * 4], eax
	jmp intr_exit
