/* based on atomthreads\ports\mips\atomport.h */

#ifndef __ATOM_PORT_H
#define __ATOM_PORT_H


/* Required number of system ticks per second (normally 100 for 10ms tick) */
#define SYSTEM_TICKS_PER_SEC            100

/**
 * Definition of NULL. stddef.h not available on this platform.
 */
#define NULL ((void *)(0))

/* Size of each stack entry / stack alignment size (32 bits on MIPS) */
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
#define CRITICAL_STORE	    uint32_t status_reg
#define CRITICAL_START()					\
	do {							\
	}while(0)

#define CRITICAL_END()							\
	do {								\
	}while(0)

#endif /* __ATOM_PORT_H */
