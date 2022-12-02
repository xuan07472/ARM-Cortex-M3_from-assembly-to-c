/*
 * Copyright (c) 2010, Kelvin Lawson. All rights reserved.
 *
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
#ifndef __ATOM_QUEUE_H
#define __ATOM_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/* 队列头，里面有两个挂起相关的队列，一个消息环形缓存 */
typedef struct atom_queue
{
	/* 2个挂起相关的线程队列 */
	/* 等待发送消息的线程队列，有消息发送时就唤醒一个线程 */
    ATOM_TCB *  putSuspQ;       /* Queue of threads waiting to send */
	/* 等待接收消息的线程队列，有消息接收时就唤醒一个线程 */
    ATOM_TCB *  getSuspQ;       /* Queue of threads waiting to receive */
	/* 队列中的消息数据 */
    uint8_t *   buff_ptr;       /* Pointer to queue data area */
	/* 每条消息的大小 */
    uint32_t    unit_size;      /* Size of each message */
	/* 消息总数 */
    uint32_t    max_num_msgs;   /* Max number of storable messages */
	/* 队列插入、删除位置 */
    uint32_t    insert_index;   /* Next byte index to insert into */
    uint32_t    remove_index;   /* Next byte index to remove from */
	/* 已有消息数量 */
    uint32_t    num_msgs_stored;/* Number of messages stored */
} ATOM_QUEUE;

/* 初始化队列头 */
extern uint8_t atomQueueCreate (ATOM_QUEUE *qptr, uint8_t *buff_ptr, uint32_t unit_size, uint32_t max_num_msgs);
/* 删除队列，该队列中的所有线程都会被转成就绪状态 */
extern uint8_t atomQueueDelete (ATOM_QUEUE *qptr);
extern uint8_t atomQueueGet (ATOM_QUEUE *qptr, int32_t timeout, uint8_t *msgptr);
extern uint8_t atomQueuePut (ATOM_QUEUE *qptr, int32_t timeout, uint8_t *msgptr);

#ifdef __cplusplus
}
#endif

#endif /* __ATOM_QUEUE_H */
