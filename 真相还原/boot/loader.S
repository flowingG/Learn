%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR

;构建GDT及其内部的描述符
GDT_BASE:	dd	0x00000000
		dd	0x00000000

CODE_DESC:	dd	0x0000FFFF
		dd	DESC_CODE_HIGH4

DATA_STACK_DESC:dd	0x0000FFFF
		dd	DESC_DATA_HIGH4

;limit=(0xbffff-0xb8000)/4k=0x7
VIDEO_DESC:	dd	0x80000007
		dd	DESC_VIDEO_HIGH4

GDT_SIZE	equ	$ - GDT_BASE
GDT_LIMIT	equ	GDT_SIZE - 1

;预留60个描述符空位
times 60 dq 0

;相当于(CODE_DESC - GDT_BASE)/8 + TI_GDT + RPL0
SELECTOR_CODE	equ	(0x0001 << 3) + TI_GDT + RPL0
SELECTOR_DATA	equ	(0x0002 << 3) + TI_GDT + RPL0
SELECTOR_VIDEO	equ	(0x0003 << 3) + TI_GDT + RPL0

;total_mem_bytes用来保存内存容量，以字节为单位
;当前偏移loader.bin文件头0x200字节
;loader.bin的加载地址0x900,故total_mem_bytes内存中地址为0xb00
;内核会引用该地址
total_mem_bytes dd 0

;GDT指针，前2字节是GDT界限，后4字节是GDT起始地址
gdt_ptr		dw	GDT_LIMIT
		dd	GDT_BASE

;人工对齐：total_mem_byte4+gdt_ptr6+ards_buf244+ards_nr2,共512字节
ards_buf times 244 db 0
;用来记录结构体ARDS数量
ards_nr dw 0

loader_start:
	;int 15h eax=0000E820h,edx=534D150h('SMAP')获取内存布局
	xor ebx,ebx		;第一次调用时,ebx要为0
	mov edx,0x534d4150	
	mov di,ards_buf		;ARDS结构缓冲区
    .e820_mem_get_loop:		;循化获取每个ARDS内存范围描述结构
	mov eax,0x0000e820	;执行int 0x15后,eax值变为0x534d4150,故执行int前要更新为子功能号
	mov ecx,20		;ARDS地址描述符结构大小20字节
	int 0x15
	jc .e820_failed_so_try_e801	;若cf位为1则有错误发生,尝试0xe801子功能
	add di,cx		;使di增加20字节指向缓冲区新ARDS结构位置
	inc word [ards_nr]	;记录ARDS数量
	cmp ebx,0		;若ebx为0且cf不为1，说明ards全部返回
	jnz .e820_mem_get_loop	;当前已是最后一个

	;在所有ards结构中，找出(base_add_low + length_low)的最大值，即内存容量
	mov cx,[ards_nr]	;遍历每个ARDS，循环次数为ARDS数量
	mov ebx,ards_buf		
	xor edx,edx		;edx为最大的内存容量,先清零
    .find_max_mem_area:		;无需判断ARDS中的Type字段，因为最大内存块一定可使用
	mov eax,[ebx]		;base_add_low
	add eax,[ebx+8]		;length_low
	add ebx,20		;指向下一个ARDS	
	cmp edx,eax
	;冒泡排序，找出最大，edx始终是最大内存容量
	jge .next_ards
	mov edx,eax
     .next_ards:
	loop .find_max_mem_area
	jmp .mem_get_ok

;int 15h ax=E801h 获取内存大小，方式2，最大支持4G
;返回后ax,cx值一样，单位1KB，bx dx值一样，64KB
;ax cx中为低16MB，bx dx中为16MB到4GB
.e820_failed_so_try_e801:
	mov ax,0xe801
	int 0x15
	;若当前e801方法失败，尝试0x88
	jc .e801_failed_so_try88
	
	;算出低15MB内存，ax cx以KB为单位，转换为以byte为单位
	mov cx,0x400
	mul cx
	shl edx,16
	and eax,0x0000FFFF
	or edx,eax
	;ax只是15MB，要加1MB
	add edx,0x100000
	;先把低15MB内存容量存入esi
	mov esi,edx
	
	;将16MB以上内存转换为byte为单位，bx dx中以64KB为单位
	xor eax,eax
	mov ax,bx
	mov ecx,0x10000
	mul ecx
	add esi,eax
	mov edx,esi
	jmp .mem_get_ok

;int 15h ah=0x88 只能获取64MB以内
.e801_failed_so_try88:
	mov ah,0x88
	int 0x15
	jc .error_hlt
	and eax,0x0000FFFF
	mov cx,0x400
	mul cx
	shl edx,16
	or edx,eax
	add edx,0x100000

.mem_get_ok:
	mov [total_mem_bytes],edx

;准备进入保护模式
;1.打开A20 2.加载GDT 3.将cr0的pe位置1
	;打开A20
	in al,0x92
	or al,0000_0010B
	out 0x92,al

	;加载GDT
	lgdt [gdt_ptr]

	;cr0PE位置1
	mov eax,cr0
	or eax,0x00000001
	mov cr0,eax
	;刷新流水线
	jmp dword SELECTOR_CODE:p_mode_start
	
	;出错则挂起
    .error_hlt:
	hlt
[bits 32]
p_mode_start:
	mov ax,SELECTOR_DATA
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov esp,LOADER_STACK_TOP
	mov ax,SELECTOR_VIDEO
	mov gs,ax

;加载kernel
	;kernel所在扇区号eax
	;从磁盘读出后，写入到ebx指定的地址
	;读入的扇区数ecx
	mov eax, KERNEL_START_SECTOR
	mov ebx, KERNEL_BIN_BASE_ADDR
	mov ecx, 200
	call rd_disk_m_32
	
	;创建页目录及页表并初始化页内存位图
	call setup_page

	;将描述符表地址及偏移量写入内存gdt_ptr,一会用新地址重新加载
	;存储到原来gdt所有的位置
	sgdt [gdt_ptr]
	
	;将gdt描述符中视频段描述符中的段基址+0xc000_0000
	;视频段是第3个段描述符，每个描述符是8字节，故0x18
	;段描述符的高4字节的最高位是段基址的第31到24位
	mov ebx,[gdt_ptr + 2]
	or dword [ebx + 0x18 + 4],0xc000_0000

	;将gdt的基址加上0xc0000000使其成为内核所在的高地址
	add dword [gdt_ptr + 2],0xc000_0000

	;将栈指针同样映射到内核地址
	add esp,0xc000_0000

	;把页目录地址赋给cr3
	mov eax,PAGE_DIR_TABLE_POS
	mov cr3,eax

	;打开cr0的pg位（第31位）
	mov eax,cr0
	or eax,0x8000_0000
	mov cr0,eax

	;开启分页后，用gdt新的地址重新加载
	lgdt [gdt_ptr]

	;为以防万一，刷新流水线
	jmp SELECTOR_CODE:enter_kernel
enter_kernel:
	call kernel_init
	mov esp,0xc009f000
	jmp KERNEL_ENTRY_POINT

;将kernel.bin中的segment拷贝到编译的地址
kernel_init:
	;ebx记录程序头表地址，cx记录头表数量，dx记录头表尺寸
	xor eax,eax
	xor ebx,ebx
	xor ecx,ecx
	xor edx,edx
	
	;偏移42处为e_phentsize,program header大小
	;28为e_phoff,第一个program header在文件中的偏移量
	mov dx,[KERNEL_BIN_BASE_ADDR + 42]
	mov ebx,[KERNEL_BIN_BASE_ADDR + 28]
	
	;偏移文件开始部分44字节,e_phnum,program header数量
	add ebx,KERNEL_BIN_BASE_ADDR
	mov cx,[KERNEL_BIN_BASE_ADDR + 44]

.each_segment:
	;若相等，说明该program header未使用
	cmp byte [ebx + 0],PT_NULL
	je .PTNULL

	;为函数memcpy压入参数，从右往左，类似memcpy(dst,src,size)
	;偏移16字节为p_filesz，压入memcpy第三个参数size
	push dword [ebx + 16]
	
	;4字节是p_offset，加上kernel.bin被加载到的物理地址，eax为该段的物理地址
	;压入memcpy的第二个参数：源地址
	mov eax,[ebx + 4]
	add eax,KERNEL_BIN_BASE_ADDR
	push eax
	;压入第一个参数：目的地址，8字节为p_vaddr
	push dword [ebx + 8]
	call mem_cpy
	;清理栈
	add esp,12

.PTNULL:
	;ebx指向下一个program header，edx为e_phentsize
	add ebx,edx
	loop .each_segment
	ret

;逐字节拷贝，输入：栈中三个参数
mem_cpy:
	cld
	push ebp
	mov ebp,esp
	push ecx
	mov edi,[ebp + 8]	;dst
	mov esi,[ebp + 12]	;src
	mov ecx,[ebp + 16]	;size
	rep movsb	;逐字节拷贝
	
	pop ecx
	pop ebp
	ret

;创建页目录和页表
setup_page:
	;先把页目录占用的空间逐字节清零
	mov ecx,4096
	mov esi,0
   .clear_page_dir:
	mov byte [PAGE_DIR_TABLE_POS + esi],0
	inc esi
	loop .clear_page_dir

;开始创建页目录表项(PDE)
.create_pde:		;创建Page Directory Entry
	mov eax,PAGE_DIR_TABLE_POS
	add eax,0x1000		;此时eax为第一个页表的位置及属性
	
	;为ebx赋值,为.create_pde做准备,ebx为基址
	mov ebx,eax	

	;下面将页目项0和0xc00都存为第一个页表的地址,每个页表表示4MB内存
	;0xc03f_ffff以下的地址和0x003f_ffff以下的地址指向相同的页表
	;页目录项的RW和P位为1，US为1，表示用户属性
	or eax,PG_US_U | PG_RW_W | PG_P	

	;第一个目录项,在页目录表的第1个目录项写入第一个页表的位置0x101000及属性(7)
	mov [PAGE_DIR_TABLE_POS + 0x0],eax
	
	;0xc00表示第768个页表占用的目录项，0xc00以上的目录项用于内核空间
	;即0xc000_0000到0xffff_ffff共1G属于内核
	;0x0到0xbfff_ffff共3G属于用户进程
	mov [PAGE_DIR_TABLE_POS + 0xc00],eax	

	;使最后一个目录项指向页目录表自己的地址
	sub eax,0x1000
	mov [PAGE_DIR_TABLE_POS + 4092],eax

;创建页表项(PTE)
	;1M低端内存/每页大小4K=256
	mov ecx,256
	mov esi,0
	
	;属性为(7),US,RW,P为1
	mov edx,PG_US_U | PG_RW_W | PG_P

.create_pte:	;创建Page Table Entry
	;ebx已经是0x101000，第一个页表的位置
	mov [ebx + esi*4],edx

	add edx,4096
	inc esi
	loop .create_pte

;创建内核和其他页表的PDE
	mov eax,PAGE_DIR_TABLE_POS
	;第二个页表的位置
	add eax,0x2000
	or eax,PG_US_U | PG_RW_W | PG_P
	mov ebx,PAGE_DIR_TABLE_POS
	
	;范围为769到1022的所有目录项数量
	mov ecx,254
	mov esi,769
.create_kernel_pde:
	mov [ebx+esi*4],eax
	inc esi
	add eax,0x1000
	loop .create_kernel_pde
	ret

;读取硬盘n个扇区
rd_disk_m_32:
	mov esi,eax
	mov di,cx

	mov dx,0x1f2
	mov al,cl
	out dx,al
	
	mov eax,esi

	mov dx,0x1f3
	out dx,al

	mov cl,8
	shr eax,cl
	mov dx,0x1f4
	out dx,al
	
	shr eax,cl
	mov dx,0x1f5
	out dx,al

	shr eax,cl
	and al,0x0f
	or al,0xe0
	mov dx,0x1f6
	out dx,al

	mov dx,0x1f7
	mov al,0x20
	out dx,al

  .not_ready:
	nop
	in al,dx
	and al,0x88
	cmp al,0x08
	jnz .not_ready

	mov ax,di
	mov dx,256
	mul dx
	mov cx,ax
	mov dx,0x1f0
 
  .go_on_read:
	in ax,dx
	mov [ebx],ax
	add ebx,2
	loop .go_on_read
	ret
