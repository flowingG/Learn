#ifndef __KERNEL_MEMORY_H
#define __KERNEL_MEMORY_H
#include "stdint.h"
#include "bitmap.h"
#include "list.h"

/***内存池标记，用于判断是哪个内存池***/
enum pool_flags{
        PF_KERNEL = 1,
        PF_USER = 2
};

#define PG_P_1  1       //页表项或页目录项存在属性位
#define PG_P_0  0       
#define PG_RW_R 0       //RW属性位值，读/执行
#define PG_RW_W 2       //RW属性位值，读/写/执行
#define PG_US_S 0       //U/S属性位值，系统级
#define PG_US_U 4       //U/S属性位值，用户级

/***虚拟地址池，用于虚拟地址管理***/
struct virtual_addr{
	struct bitmap vaddr_bitmap;
	uint32_t vaddr_start;
};

//内存块
struct mem_block{
	struct list_elem free_elem;
};

//内存块描述符
struct mem_block_desc{
	uint32_t block_size;
	uint32_t blocks_per_arena;
	struct list free_list;
};

#define DESC_CNT 7

extern struct pool kernel_pool, user_pool;
void mem_init(void);
void* get_kernel_pages(uint32_t pg_cnt);
void* malloc_page(enum pool_flags pf, uint32_t pg_cnt);
void malloc_init(void);
uint32_t* pte_ptr(uint32_t vaddr);
uint32_t* pde_ptr(uint32_t vaddr);
uint32_t addr_v2p(uint32_t vaddr);
void *get_a_page(enum pool_flags pf, uint32_t vaddr);
void *get_user_pages(uint32_t pg_cnt);
#endif
