TI_GDT equ 0
RPL0 equ 0
SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0

section .data
put_int_buffer dq 0	;8字节缓冲区用于数字到字符的转换

[bits 32]
section .text
;;;put_str:通过put_char来打印以0字符结尾的字符串
global put_str
put_str:
	;只用到了ebx和ecx,从栈中获取待打印的字符串地址
	push ebx
	push ecx
	xor ecx,ecx
	mov ebx,[esp + 12]
   .goon:
	mov cl,[ebx]
	;遇到0则跳到结束处返回
	cmp cl,0
	jz .str_over
	;给put_char传参
	push ecx
	call put_char
	;回收参数所占栈空间,使ebx指向下一个字符
	add esp,4
	inc ebx
	jmp .goon
   .str_over:
	pop ecx
	pop ebx
	ret

[bits 32]
section .text
;;;put_char:把栈中的一个字符写入光标所在处
global put_char
put_char:
	;备份32位寄存器环境
	pushad
	
	;要保证gs为正确的视频段选择子，为保险，每次打印都为gs赋值
	mov ax,SELECTOR_VIDEO
	mov gs,ax

;获取当前光标位置
	;先获取高8位,dx索引寄存器,al提供光标位置的高8位
	mov dx,0x03d4
	mov al,0x0e
	out dx,al

	;通过读写数据端口0x3d5来获得或设置光标位置,al得到光标位置的高8位
	mov dx,0x03d5
	in al,dx
	mov ah,al

	;再获取低8位
	mov dx,0x03d4
	mov al,0x0f
	out dx,al
	mov dx,0x03d5
	in al,dx

	;将光标存入bx
	mov bx,ax

	;在栈中获取待打印的字符,pushad压入4*8=32字节
	;加上主调函数4字节返回地址,故esp+36字节
	mov ecx,[esp + 36]

	;CR(carriage_return 回车)是0x0d，LF是0x0a(LineFeed 换行)
	cmp cl,0xd
	jz .is_carriage_return
	cmp cl,0xa
	jz .is_line_feed

	;BS(backspace 退格)的ASCLL码是8
	cmp cl,0x8
	jz .is_backspace
	jmp .put_other

;;;退格，光标位置向前位移一个显存位置，添加空格或0字符来清除前一个位置的字符
.is_backspace:
	dec bx
	
	;光标左移一位(即*2),表示光标对应显存中的偏移字节
	shl bx,1

	;置0或空格
	mov byte [gs:bx],0x20
	inc bx
	mov byte [gs:bx],0x07
	shr bx,1
	jmp .set_cursor

.put_other:
	;光标值*2,表示对应显存中的偏移字节
	shl bx,1
	
	;ASCLL字符本身
	mov [gs:bx],cl
	inc bx
	
	;字符属性
	mov byte [gs:bx],0x07
	;恢复老的光标值
	shr bx,1
	;下一个光标值
	inc bx
	
	;大于2000表示超出屏幕大小，换行
	cmp bx,2000
	jl .set_cursor

;换行回车统一处理
.is_line_feed:
.is_carriage_return:
	;dx是被除数的高16位，清0;ax是被除数的低16位
	xor dx,dx
	mov ax,bx
	mov si,80
	div si
	
	;下一行行首,光标减去除80的余数便是取整
	sub bx,dx

.is_carriage_return_end:		;回车处理结束
	add bx,80
	cmp bx,2000
.is_line_feed_end:		;若是LF(\n),将光标位移+80便可
	jl .set_cursor

;屏幕行0-24，滚屏的原理是将1~24行搬到0~23行,将24行填满空格
.roll_screen:
	;用cld指令清除方向位
	cld
	;2000-80=1920,1920*2=3840字节,3840/4=960搬运次数	
	mov ecx,960

	;esi,第一行行首，edi第0行行首
	mov esi,0xc00b80a0
	mov edi,0xc00b8000
	rep movsd

	;最后一行填空白,偏移为3840
	mov ebx,3840
	mov ecx,80

.cls:
	mov word [gs:ebx],0x0720		;黑底白字
	add ebx,2
	loop .cls
	mov bx,1920			;光标重制为1920，最后一行的首字符

.set_cursor:
	;将光标设置为bx值
	;先设置高8位
	mov dx,0x03d4
	mov al,0x0e
	out dx,al
	mov dx,0x03d5
	mov al,bh
	out dx,al	

	;再设置低8位
	mov dx,0x03d4
	mov al,0x0f
	out dx,al
	mov dx,0x03d5
	mov al,bl
	out dx,al

   .put_char_done:
	popad
	ret

;将小端字节序的数字变成对应的ASCII后，倒置
;输入：栈中参数为待打印的字符 输出:打印16进制，不加0x
global put_int
put_int:
	pushad
	mov ebp,esp
	
	;call返回地址4字节,pushad8*4字节
	mov eax,[ebp + 4*9]
	mov edx,eax
	;指定put_int_buffer初始的偏移量
	mov edi,7
	;32位数字中，16进制数字的位数是8个
	mov ecx,8
	mov ebx,put_int_buffer
	
;将32位 数字按照16进制的形式从低位到高位逐个处理，共处理8个16进制数字
;每4位二进制是一位16进制数，遍历每一位16进制
.16based_4bits:
	;解析16进制每一位,用and保证只有低4位有效
	and edx,0x0000000F
	;0~9,a~f分别处理
	cmp edx,9
	jg .is_A2F
	;ascii码8位大小，add后，低8位有效
	add edx,'0'
	jmp .store
   
   ;a~f减10+a的ascii码即位对应的ascii码
   .is_A2F:
	sub edx,10
	add edx,'A'		

;转换后按大端字节的顺序存入put_int_buffer
.store:
	mov [ebx + edi],dl
	dec edi
	shr eax,4
	mov edx,eax
	loop .16based_4bits

;把高位连续字符去掉,000123->123
.ready_to_print:
	inc edi		;加1让-1变0
.skip_prefix_0:
	cmp edi,8	;表示全为0，已比较第9个字符了
	je .full0
;找0
.go_on_skip:
	mov cl,[put_int_buffer + edi]
	inc edi
	cmp cl,'0'
	je .skip_prefix_0
	dec edi			;若不是0，指向第一个非0的位置
	jmp .put_each_num

;全0则打印一个0
.full0:
	mov cl,'0'
.put_each_num:
	push ecx
	call put_char
	add esp,4
	inc edi
	mov cl,[put_int_buffer+edi]
	cmp edi,8
	jl .put_each_num
	popad 	
	ret

global set_cursor
set_cursor:
	pushad
	mov bx, [esp + 36]
	mov dx, 0x03d4
	mov al, 0x0e
	out dx, al
	mov dx, 0x03d5
	mov al, bh
	out dx, al

	mov dx, 0x03d4
	mov al, 0x0f
	out dx, al
	mov dx, 0x03d5
	mov al, bl
	out dx, al
	popad 
	ret
