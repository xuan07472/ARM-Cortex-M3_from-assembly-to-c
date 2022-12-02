#include <atomport.h>

uint32_t at_preempt_count;

// 需要在汇编中export archFirstThreadRestore、archContextSwitch、archThreadContextInit

/* 这3个函数中实现的功能就和task任务的运行与切换类似 */

/* 线程第一次运行时，初始化每个线程的堆栈 */
//void archThreadContextInit (ATOM_TCB *tcb_ptr, void *stack_top, void (*entry_point)(uint32_t), uint32_t entry_param)
void archThreadContextInit (void *tcb_ptr, void *stack_top, void (*entry_point)(uint32_t), uint32_t entry_param)
{

}

/* 线程堆栈切换 */
//void archContextSwitch (ATOM_TCB *old_tcb_ptr, ATOM_TCB *new_tcb_ptr)
void archContextSwitch (void *old_tcb_ptr, void *new_tcb_ptr)
{
    /* 里面使用汇编，对R0~R15等通用寄存器进行操作，设置CPU的栈指针sp */
}

//void archFirstThreadRestore(ATOM_TCB *new_tcb_ptr)
void archFirstThreadRestore(void *new_tcb_ptr)
{

}
