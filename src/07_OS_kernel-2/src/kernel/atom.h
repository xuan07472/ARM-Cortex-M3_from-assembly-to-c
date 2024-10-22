/*
/*******************************************************************************
 * \brief	线程管理的头文件
 * \details	实现线程创建、挂起、切换、销毁等功能；一些线程管理的基础概念有：时间片
 *			轮转、线程优先级、线程死锁、优先级继承、优先级天花板、任务阻塞。
 *			输出的函数接口在atom.h中，创建线程要包含这个头文件。
 * \note	File format: UTF-8, 中文编码：UTF-8
 * \author	注释作者：将狼才鲸
 * \date	注释日期：2022-12-01
 *******************************************************************************
 * Copyright (c) 2010, Kelvin Lawson. All rights reserved.
 *
 * 以下都是版权声明：
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. No personal names or organizations' names associated with the
 *    Atomthreads project may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE ATOMTHREADS PROJECT AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ATOM_H
#define __ATOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "atomtimer.h"  /* 线程管理模块内部的头文件 */
#include "atomport.h"   /* 系统移植时需要提供的头文件 */

/* Data types */

/* Forward declaration */
struct atom_tcb;

/* 线程上下文，自身也是一个队列头，或者某个队列中的一个元素 */
typedef struct atom_tcb
{
    /*
     * Thread's current stack pointer. When a thread is scheduled
     * out the architecture port can save its stack pointer here.
     */
    POINTER sp_save_ptr;

    /* Thread's port specific private data */
#if defined(THREAD_PORT_PRIV)
    THREAD_PORT_PRIV;
#endif

    /* Thread priority (0-255) */
    /* 线程优先级 */
    uint8_t priority;

    /* Thread entry point and parameter */
    void (*entry_point)(uint32_t);
    uint32_t entry_param;

    /* Queue pointers */
    struct atom_tcb *prev_tcb;    /* Previous TCB in doubly-linked TCB list */
    struct atom_tcb *next_tcb;    /* Next TCB in doubly-linked list */

    /* Suspension data */
    /* 线程是否已暂停 */
    uint8_t suspended;            /* TRUE if task is currently suspended */
    uint8_t suspend_wake_status;  /* Status returned to woken suspend calls */
    ATOM_TIMER *suspend_timo_cb;  /* Callback registered for suspension timeouts */
    /* 线程是否已停止 */
    uint8_t terminated;           /* TRUE if task is being terminated (run to completion) */

    /* Details used if thread stack-checking is required */
#ifdef ATOM_STACK_CHECKING
    POINTER stack_bottom;         /* Pointer to bottom of stack allocation */
    uint32_t stack_size;          /* Size of stack allocation in bytes */
#endif

} ATOM_TCB;


/* Global data */
extern ATOM_TCB *tcbReadyQ; /* 会在整个线程管理模块中使用 */
extern uint8_t atomOSStarted;


/* Constants */
#define TRUE                    1
#define FALSE                   0

/* Error values */

#define ATOM_OK                 0
#define ATOM_ERROR              1
#define ATOM_TIMEOUT            2
#define ATOM_WOULDBLOCK         3
#define ATOM_ERR_CONTEXT        200
#define ATOM_ERR_PARAM          201
#define ATOM_ERR_DELETED        202
#define ATOM_ERR_OVF            203
#define ATOM_ERR_QUEUE          204
#define ATOM_ERR_TIMER          205
#define ATOM_ERR_NOT_FOUND      206
#define ATOM_ERR_OWNERSHIP      207

/* Idle thread priority (lowest) */
#define IDLE_THREAD_PRIORITY    255


/* Function prototypes */
extern uint8_t atomOSInit (void *idle_thread_stack_bottom, uint32_t idle_thread_stack_size, uint8_t idle_thread_stack_check);
extern void atomOSStart (void);

extern void atomSched (uint8_t timer_tick);

extern void atomIntEnter (void); /* 需要在操作系统移植时调用 */
extern void atomIntExit (uint8_t timer_tick); /* 需要在操作系统移植时调用 */

extern uint8_t tcbEnqueuePriority (ATOM_TCB **tcb_queue_ptr, ATOM_TCB *tcb_ptr);
extern ATOM_TCB *tcbDequeueHead (ATOM_TCB **tcb_queue_ptr);
extern ATOM_TCB *tcbDequeueEntry (ATOM_TCB **tcb_queue_ptr, ATOM_TCB *tcb_ptr);
extern ATOM_TCB *tcbDequeuePriority (ATOM_TCB **tcb_queue_ptr, uint8_t priority);

extern ATOM_TCB *atomCurrentContext (void);

extern uint8_t atomThreadCreate (ATOM_TCB *tcb_ptr, uint8_t priority, void (*entry_point)(uint32_t), uint32_t entry_param, void *stack_bottom, uint32_t stack_size, uint8_t stack_check);
extern uint8_t atomThreadStackCheck (ATOM_TCB *tcb_ptr, uint32_t *used_bytes, uint32_t *free_bytes);

/* 这3个函数需要系统移植的atomport.c/h文件中实现 */
extern void archContextSwitch (ATOM_TCB *old_tcb_ptr, ATOM_TCB *new_tcb_ptr);
extern void archThreadContextInit (ATOM_TCB *tcb_ptr, void *stack_top, void (*entry_point)(uint32_t), uint32_t entry_param);
extern void archFirstThreadRestore(ATOM_TCB *new_tcb_ptr);

extern void atomTimerTick (void); /* 需要在操作系统移植时调用 */

#ifdef __cplusplus
}
#endif

#endif /* __ATOM_H */
