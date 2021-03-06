;loader和kernel
LOADER_BASE_ADDR equ 0x900
LOADER_STACK_TOP equ LOADER_BASE_ADDR
LOADER_START_SECTOR equ 0x2

KERNEL_BIN_BASE_ADDR equ 0x70000
KERNEL_START_SECTOR equ 0x9
KERNEL_ENTRY_POINT equ 0xc0001500

;页表配置
PAGE_DIR_TABLE_POS equ 0x100000

;GDT描述符属性
;粒度，4K
DESC_G_4K	equ	1_0000_0000_0000_0000_0000_000b
;D/B字段，表示32位
DESC_D_32	equ	1_0000_0000_0000_0000_0000_00b
;64位标记，置0表32即可
DESC_L		equ	0_0000_0000_0000_0000_0000_0b
;AVL字段
DESC_AVL	equ	0_0000_0000_0000_0000_0000b
;
DESC_LIMIT_CODE2	equ	1111_0000_0000_0000_0000b
;
DESC_LIMIT_DATA2	equ	DESC_LIMIT_CODE2
;
DESC_LIMIT_VIDEO2	equ	0000_0000_0000_0000_000b
;P字段，段是否存在
DESC_P		equ	1_0000_0000_0000_000b
;DPL字段，描述符特权级，以下是0123级
DESC_DPL_0	equ	00_0000_0000_0000_0b
DESC_DPL_1	equ	01_0000_0000_0000_0b
DESC_DPL_2	equ	10_0000_0000_0000_0b
DESC_DPL_3	equ	11_0000_0000_0000_0b
;S字段，指示系统段和非系统段
DESC_S_CODE	equ	1_0000_0000_0000b
DESC_S_DATA	equ	DESC_S_CODE
DESC_S_sys	equ	0_0000_0000_0000b
;TYPE字段
;xcra=1000,可执行，非一致性，不可读，已访问位a清0，代码段
DESC_TYPE_CODE	equ	1000_0000_0000b
;xewa=0010,不可执行，向上扩展，可写，a清0，数据段
DESC_TYPE_DATA	equ	0010_0000_0000b
;定义高4字节
DESC_CODE_HIGH4	equ	(0X00 << 24) + DESC_G_4K + DESC_D_32 + DESC_L + DESC_AVL + DESC_LIMIT_CODE2 + DESC_P + DESC_DPL_0 + DESC_S_CODE + DESC_TYPE_CODE + 0x00
DESC_DATA_HIGH4	equ	(0X00 << 24) + DESC_G_4K + DESC_D_32 + DESC_L + DESC_AVL + DESC_LIMIT_DATA2 + DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x00
DESC_VIDEO_HIGH4 equ	(0x00 << 24) + DESC_G_4K + DESC_D_32 + DESC_L + DESC_AVL + DESC_LIMIT_VIDEO2 + DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x0b

;选择子属性
RPL0	equ	00b
RPL1	equ	01b
RPL2	equ	10b
RPL3	equ	11b
TI_GDT	equ	000b
TI_LDT	equ	100b

;页表相关属性
PG_P	equ 	1b
PG_RW_R equ 	00b
PG_RW_W equ 	10b
PG_US_S equ	000b
PG_US_U	equ	100b

;program type定义
PT_NULL equ 0
