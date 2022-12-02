/*******************************************************************************
 * \brief	系统移植时要提供的头文件
 * \details	要提供读中断状态寄存器、关中断、恢复中断，在硬件定时器中调用线程管理模块
 *			暴露出来的函数接口
 * \author	将狼才鲸
 * \date	2022-12-01
 ******************************************************************************/

/* based on atomthreads\ports\mips\atomport.h */

#ifndef __ATOM_PORT_H
#define __ATOM_PORT_H


/* Required number of system ticks per second (normally 100 for 10ms tick) */
/* 每秒有多少次系统tick，时间片的大小 */
#define SYSTEM_TICKS_PER_SEC            100

/**
 * Definition of NULL. stddef.h not available on this platform.
 */
#define NULL ((void *)(0))

/* Size of each stack entry / stack alignment size (32 bits on MIPS) */
/* 栈对齐的字节数 */
#define STACK_ALIGN_SIZE                sizeof(uint32_t)

/**
 * Architecture-specific types.
 * Provide stdint.h style types.
 */
#define uint8_t   unsigned char
#define uint16_t  unsigned short
#define uint32_t  unsigned int
#define uint64_t  unsigned long int
#define int8_t    char
#define int16_t   short
#define int32_t   int
#define int64_t   long int
#define size_t    unsigned int
#define POINTER   void *
#define UINT32    uint32_t

extern uint32_t at_preempt_count;

/* 用于保护线程调度的临界区、原子操作 */
#define CRITICAL_STORE	    uint32_t status_reg /* 读中断状态 */
/* 关中断 */
#define CRITICAL_START()					\
	do {							\
	}while(0)

/* 恢复中断 */
#define CRITICAL_END()							\
	do {								\
	}while(0)

#endif /* __ATOM_PORT_H */
