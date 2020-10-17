#include "memory.h"
#include "bitmap.h"
#include "stdint.h"
#include "global.h"
#include "debug.h"
#include "print.h"
#include "string.h"
#include "sync.h"
#include "interrupt.h"

/***	位图地址	***
 * 0xc009f000是内核主线程栈顶，0xc009e000是内核主线程的pcb
 * 一个页框大小的位图可表示128M内存，位图安排在地址0xc009a000
 * 这样本系统最大支持4个页框的位图，512MB */
#define MEM_BITMAP_BASE 0xc009a000

/* 0xc0000000是内核从虚拟地址3G起
 * 0x100000指跨过低端1MB内存，使虚拟地址在逻辑上连续 */
#define K_HEAP_START 0xc0100000

#define PDE_IDX(addr) ((addr & 0xffc00000) >> 22)
#define PTE_IDX(addr) ((addr & 0x003ff000) >> 12)

/***	内存池结构，生成两个实例用于管理内核内存池和用户内存池	***/
struct pool{
	//本内存池用到的位图结构，管理物理内存
	struct bitmap pool_bitmap;
	//本内存池所管理的物理内存的起始地址
	uint32_t phy_addr_start;
	//本内存池字节容量
	uint32_t pool_size;
	struct lock lock;
};

//内存仓库arena元信息
struct arena{
	struct mem_block_desc* desc;
	uint32_t cnt;
	bool large;
};

struct mem_block_desc k_block_descs[DESC_CNT];
struct pool kernel_pool, user_pool;
struct virtual_addr kernel_vaddr;

/***	在pf中表示的虚拟内存池中申请pg_cnt个虚拟页
 * 成功则返回虚拟页的起始地址，失败则返回NULL	***/
static void* vaddr_get(enum pool_flags pf, uint32_t pg_cnt){
	int vaddr_start = 0, bit_idx_start = -1;
	uint32_t cnt = 0;
	if(pf == PF_KERNEL){
		bit_idx_start = bitmap_scan(&kernel_vaddr.vaddr_bitmap, pg_cnt);
		if(bit_idx_start == -1){
			return NULL;
		}
		while(cnt < pg_cnt){
			bitmap_set(&kernel_vaddr.vaddr_bitmap, bit_idx_start + cnt++, 1);
		}
		vaddr_start = kernel_vaddr.vaddr_start + bit_idx_start * PG_SIZE;
	}
	//用户内存池
	else{
		struct task_struct *cur = running_thread();
		bit_idx_start = bitmap_scan(&cur->userprog_vaddr.vaddr_bitmap, pg_cnt);
		if(bit_idx_start == -1){
			return NULL;
		}
		while(cnt < pg_cnt){
			bitmap_set(&cur->userprog_vaddr.vaddr_bitmap, bit_idx_start + cnt++, 1);
		}
		vaddr_start = cur->userprog_vaddr.vaddr_start + bit_idx_start * PG_SIZE;
		ASSERT((uint32_t)vaddr_start < (0xc0000000 - PG_SIZE));
	}
	return (void*)vaddr_start;
}

/***	得到虚拟地址vaddr对应的pte指针	***/
uint32_t* pte_ptr(uint32_t vaddr){
	/* 先访问页表自己 + 再用页目录项pde(页目录内页表的索引)作为pte的索引访问到页表 +
 	* 再用pte的索引作为页内偏移 */
	uint32_t* pte = (uint32_t*)(0xffc00000 + ((vaddr & 0xffc00000) >> 10) + \
		PTE_IDX(vaddr) * 4);
	return pte; 
}

/***	得到虚拟地址vaddr对应的pde指针	***/
uint32_t* pde_ptr(uint32_t vaddr){
	/* 0xfffff用来访问到页表本身所在的地址 */
	uint32_t* pde = (uint32_t*)(0xfffff000 + PDE_IDX(vaddr) * 4);
	return pde; 
} 

/***	在m_pool指向的物理内存池中分配一个物理页，
 * 成功则返回页框的物理地址，失败则返回NULL	***/
static void* palloc(struct pool* m_pool){
	/* 扫描或设置位图要保证原子操作 */
	int bit_idx = bitmap_scan(&m_pool->pool_bitmap, 1);	//找一个物理页面
	if(bit_idx == -1){
		return NULL;
	}
	bitmap_set(&m_pool->pool_bitmap, bit_idx, 1);
	uint32_t page_phyaddr = ((bit_idx * PG_SIZE) + m_pool->phy_addr_start);
	return (void*)page_phyaddr;
}

/***	页表中添加虚拟地址_vaddr与物理地址_page_phyaddr的映射	***/
static void page_table_add(void* _vaddr, void* _page_phyaddr){
	uint32_t vaddr = (uint32_t)_vaddr, page_phyaddr = (uint32_t)_page_phyaddr;
	uint32_t* pde = pde_ptr(vaddr);
	uint32_t* pte = pte_ptr(vaddr);

	/* 执行*pte会访问到空的pde，故要确保pde创建完成后才能执行*pte
 	 * 否则会触发page_fault,因此在*pde为0后，*pte只能出现在下面else语句块中的*pde后 */
	//先在页目录判断页目录项的P位，为1表示该表已存在
	//页目录项和页表项的第0位为P，此处判断目录项是否存在
	if(*pde & 0x00000001){
		ASSERT(!(*pte & 0x00000001));

		//多判断一下保证可靠
		if(!(*pte & 0x00000001)){
			//US=1,RW=1,P=1
			*pte = (page_phyaddr | PG_US_U | PG_RW_W | PG_P_1);
		}
		//ASSERT判断过，应该不会执行到这里
		else{
			PANIC("pte repeat!\n");
		}
	}
	//页目录项不存在，所以要先创建页目录再创建页表项
	//页表中用到的页框一律从内核空间分配
	else{
		uint32_t pde_phyaddr = (uint32_t)palloc(&kernel_pool);
		*pde = (pde_phyaddr | PG_US_U | PG_RW_W | PG_P_1);	
		
		memset((void*)((int)pte & 0xfffff000), 0, PG_SIZE);

		ASSERT(!(*pte & 0x00000001));
		*pte = (page_phyaddr | PG_US_U | PG_RW_W | PG_P_1);
	}
}

/***	分配pg_cnt个页空间，成功时返回起始虚拟地址，失败返回NULL	***/
void* malloc_page(enum pool_flags pf, uint32_t pg_cnt){
	ASSERT(pg_cnt > 0 && pg_cnt < 3840);
	
	/* 通过vaddr_get在虚拟内存池中申请虚拟地址
 	 * 通过palloc在物理内存池中申请物理页
 	 * 通过page_table_add将以上得到的虚拟地址和物理地址在页表中完成映射 */
	void* vaddr_start = vaddr_get(pf, pg_cnt);
	if(vaddr_start == NULL){
		return NULL;
	}

	uint32_t vaddr = (uint32_t)vaddr_start, cnt = pg_cnt;
	struct pool* mem_pool = pf & PF_KERNEL ? &kernel_pool : &user_pool;

	//虚拟地址连续但物理地址不连续，故逐个映射
	while(cnt-- > 0){
		void* page_phyaddr = palloc(mem_pool);
		
		//失败则将已申请的虚拟地址和物理页全部回滚，在将来完成内存回收时再补充
		if(page_phyaddr == NULL){
			return NULL;
		}

		//在页表中做映射
		page_table_add((void*)vaddr, page_phyaddr);
		//下一个虚拟页
		vaddr += PG_SIZE;
	}
	return vaddr_start;
}

/***	从内核物理内存池中申请1页内存，成功则返回其虚拟地址，失败返回NULL	***/
void* get_kernel_pages(uint32_t pg_cnt){
	lock_acquire(&kernel_pool.lock);
	void* vaddr = malloc_page(PF_KERNEL, pg_cnt);
	if(vaddr != NULL){
		memset(vaddr, 0, pg_cnt * PG_SIZE);
	}
	lock_release(&kernel_pool.lock);
	return vaddr;
}

/* 在用户空间申请4K内存,并返回其虚拟地址 */
void *get_user_pages(uint32_t pg_cnt){
	lock_acquire(&user_pool.lock);
	void *vaddr = malloc_page(PF_USER, pg_cnt);
	memset(vaddr, 0, pg_cnt * PG_SIZE);
	lock_release(&kernel_pool.lock);
	return vaddr;
}

/* 将地址vaddr与pf池中的物理地址相关联,仅支持一页空间分配 */
void *get_a_page(enum pool_flags pf, uint32_t vaddr){
	struct pool *mem_pool = pf & PF_KERNEL ? &kernel_pool : &user_pool;
	lock_acquire(&mem_pool->lock);
	struct task_struct *cur = running_thread();
	int32_t bit_idx = -1;
	
	if(cur->pgdir != NULL && pf == PF_USER){
		bit_idx = (vaddr - cur->userprog_vaddr.vaddr_start) / PG_SIZE;
		ASSERT(bit_idx > 0);
		bitmap_set(&cur->userprog_vaddr.vaddr_bitmap, bit_idx, 1);
	}else if(cur->pgdir == NULL && pf == PF_KERNEL){
		bit_idx = (vaddr - kernel_vaddr.vaddr_start) / PG_SIZE;
		ASSERT(bit_idx > 0);
		bitmap_set(&kernel_vaddr.vaddr_bitmap, bit_idx, 1);
	}else{
		PANIC("get_a_page:not allow kernel alloc userspace or user alloc kernelspace by get_a_page");
	}
	void *page_phyaddr = palloc(mem_pool);
	if(page_phyaddr == NULL){
		return NULL;
	}
	page_table_add((void*)vaddr, page_phyaddr);
	lock_release(&mem_pool->lock);
	return (void*)vaddr;
}

/* 得到虚拟地址映射到的物理地址 */
uint32_t addr_v2p(uint32_t vaddr){
	uint32_t *pte = pte_ptr(vaddr);
	return ((*pte & 0xfffff000) + (vaddr & 0x00000fff));
}

/***	初始化内存池	***/
static void mem_pool_init(uint32_t all_mem){
	put_str("   mem_pool_init start.\n");
	
	//页表大小=1页的页目录表+第0和第768个页目录项指向同一个页表+
	//769~1022个页目录项共指向254个页表，共256个页框
	uint32_t page_table_size = PG_SIZE * 256;
	
	uint32_t used_mem = page_table_size + 0x100000;
	uint32_t free_mem = all_mem - used_mem;
	uint16_t all_free_pages = free_mem / PG_SIZE;
	uint16_t kernel_free_pages = all_free_pages / 2;
	uint16_t user_free_pages = all_free_pages - kernel_free_pages;

	//简化位图操作，余数不处理，坏处是丢失内存
	//好处是不用作内存的越界检查，因为位图表示的内存少于实际物理内存
	//Kernel BitMap的长度，位图中的一位表示一页，以字节为单位
	uint32_t kbm_length = kernel_free_pages / 8;
	
	//User BitMap的长度
	uint32_t ubm_length = user_free_pages / 8;

	//内存内核池起始地址
	uint32_t kp_start = used_mem;

	//用户内存池起始地址
	uint32_t up_start = kp_start + kernel_free_pages * PG_SIZE;

	kernel_pool.phy_addr_start = kp_start;
	kernel_pool.pool_size = kernel_free_pages * PG_SIZE;
	kernel_pool.pool_bitmap.btmp_bytes_len = kbm_length;

	user_pool.phy_addr_start = up_start;
	user_pool.pool_size = user_free_pages * PG_SIZE;
	user_pool.pool_bitmap.btmp_bytes_len = ubm_length;

/***	内核内存池和用户内存池位图	***
 * 位图是全局的数据，长度不固定，全局或静态的数组需要在编译时知道其长度
 * 需要根据总内存大小算出需要多少字节，所以改为指定一块内存来生成位图 */
	//内核使用的最高地址是0xc009f000,这是主线程的栈地址
	//内核的大小大概为70KB左右，32MB内存占用的位图是2KB
	//内核内存池的位图先定在MEM_BITMAP_BASE(0xc009a000)处
	kernel_pool.pool_bitmap.bits = (void*)MEM_BITMAP_BASE;

	//用户内存池的位图紧跟在内核内存池位图后
	user_pool.pool_bitmap.bits = (void*)(MEM_BITMAP_BASE + kbm_length);

/***	输出内存池信息	***/
	put_str("   kernel_pool_bitmap_start:");
	put_int((int)kernel_pool.pool_bitmap.bits);
	put_str("   kernel_pool_phy_addr_start:");
	put_int(kernel_pool.phy_addr_start);
	put_str(".\n");
	put_str("   user_pool_bitmap_start:");
	put_int((int)user_pool.pool_bitmap.bits);
	put_str("    user_pool_phy_addr_start:");
	put_int(user_pool.phy_addr_start);
	put_str(".\n");

	//将位图置0
	bitmap_init(&kernel_pool.pool_bitmap);
	bitmap_init(&user_pool.pool_bitmap);

	lock_init(&kernel_pool.lock);
	lock_init(&user_pool.lock);

	//初始化内核虚拟地址的位图，按实际物理内存大小生成数组
	//用于维护内核堆的虚拟地址，要和内核内存池大小一致
	kernel_vaddr.vaddr_bitmap.btmp_bytes_len = kbm_length;

	//位图的数组指向一块未使用的内存
	//目前定位在内核内存池和用户内存池值之外
	kernel_vaddr.vaddr_bitmap.bits = (void*)(MEM_BITMAP_BASE + kbm_length + ubm_length);

	kernel_vaddr.vaddr_start = K_HEAP_START;
	bitmap_init(&kernel_vaddr.vaddr_bitmap);
	put_str("   mem_pool_init done!!!\n");
}

void block_desc_init(struct mem_block_desc *desc_array){
	uint16_t desc_idx, block_size = 16;
	for(desc_idx = 0; desc_idx < DESC_CNT; desc_idx++){
		desc_array[desc_idx].block_size = block_size;
		desc_array[desc_idx].blocks_per_arena = (PG_SIZE - sizeof(struct arena)) / block_size;
		list_init(&desc_array[desc_idx].free_list);
		block_size *= 2;
	}
}

static struct mem_block* arena2block(struct arena* a, uint32_t idx){
	return (struct mem_block*)((uint32_t)a + sizeof(struct arena) + idx * a->desc->block_size);
}

static struct arena* block2arena(struct mem_block* b){
	return (struct arena*)((uint32_t)b & 0xfffff000);
}

void *sys_malloc(uint32_t size){
	enum pool_flags PF;
	struct pool* mem_pool;
	uint32_t pool_size;
	struct mem_block_desc* descs;
	struct task_struct* cur_thread = running_thread();
	//判断内存池
	if(cur_thread->pgdir == NULL){
		PF = PF_KERNEL;
		pool_size = kernel_pool.pool_size;
		mem_pool = &kernel_pool;
		descs = k_block_descs;
	}else{
		PF = PF_USER;
		pool_size = user_pool.pool_size;
		mem_pool = &user_pool;
		descs = cur_thread->u_block_desc;
	}
	
	if(!(size > 0 && size < pool_size))
		return NULL;
	struct arena* a;
	struct mem_block* b;
	lock_acquire(&mem_pool->lock);

	if(size > 1024){
		uint32_t page_cnt = DIV_ROUND_UP(size + sizeof(struct arena), PG_SIZE);
		a = malloc_page(PF, page_cnt);
		if(a != NULL){
			memset(a, 0, page_cnt * PG_SIZE);
			a->desc = NULL;
			a->cnt = page_cnt;
			a->large = true;
			lock_release(&mem_pool->lock);
			return (void*)(a + 1);
		}else{
			lock_release(&mem_pool->lock);
			return NULL;
		}
	}else{
		uint8_t desc_idx;
		for(desc_idx = 0; desc_idx < DESC_CNT; desc_idx++){
			if(size <= descs[desc_idx].block_size){
				break;
			}
		}

		if(list_empty(&descs[desc_idx].free_list)){
			a = malloc_page(PF, 1);
			if(a == NULL){
				lock_release(&mem_pool->lock);
				return NULL;
			}
			memset(a, 0, PG_SIZE);
			a->desc = &descs[desc_idx];
			a->large = false;
			a->cnt = descs[desc_idx].blocks_per_arena;
			uint32_t block_idx;
			
			enum intr_status old_status = intr_disable();

			for(block_idx = 0; block_idx < descs[desc_idx].blocks_per_arena; block_idx++){
				b = arena2block(a, block_idx);
				ASSERT(!elem_find(&a->desc->free_list, &b->free_elem));
				list_append(&a->desc->free_list, &b->free_elem);
			}
			intr_set_status(old_status);
		}
		b = elem2entry(struct mem_block, free_elem, list_pop(&(descs[desc_idx].free_list)));
		memset(b, 0, descs[desc_idx].block_size);
		a = block2arena(b);
		a->cnt--;
		lock_release(&mem_pool->lock);
		return (void*)b;
	}
}

/***	内存管理部分初始化入口	***/
void mem_init(){
	put_str("mem_init start\n");
	uint32_t mem_bytes_total = (*(uint32_t*)(0xb00));
	mem_pool_init(mem_bytes_total);
	block_desc_init(k_block_descs);
	put_str("mem_init done\n");
}
