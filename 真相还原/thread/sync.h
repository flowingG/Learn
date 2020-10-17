#ifndef __THREAD_SYNC_H
#define __THREAD_SYNC_H
#include "list.h"
#include "stdint.h"
#include "thread.h"

/***	信号量结构	***/
struct semaphore{
	uint8_t value;
	struct list waiters;
};

/***	锁结构	***/
struct lock{
	//锁的持有者
	struct task_struct* holder;
	//用二元信号量实现锁
	struct semaphore semaphore;
	//锁的持有者重复申请锁的次数
	uint32_t holder_repeat_nr;
};

void sema_init(struct semaphore* psema, uint8_t value);
void sema_down(struct semaphore* psema);
void sema_up(struct semaphore* psema);
void lock_init(struct lock* plock);
void lock_acquire(struct lock* plock);
void lock_release(struct lock* plock);
#endif
