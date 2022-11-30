#include <atomport.h>

uint32_t at_preempt_count;

// 需要在汇编中export archFirstThreadRestore、archContextSwitch、archThreadContextInit
//void archContextSwitch (ATOM_TCB *old_tcb_ptr, ATOM_TCB *new_tcb_ptr)
void archContextSwitch (void *old_tcb_ptr, void *new_tcb_ptr)
{

}

//void archThreadContextInit (ATOM_TCB *tcb_ptr, void *stack_top, void (*entry_point)(uint32_t), uint32_t entry_param)
void archThreadContextInit (void *tcb_ptr, void *stack_top, void (*entry_point)(uint32_t), uint32_t entry_param)
{

}

//void archFirstThreadRestore(ATOM_TCB *new_tcb_ptr)
void archFirstThreadRestore(void *new_tcb_ptr)
{

}
