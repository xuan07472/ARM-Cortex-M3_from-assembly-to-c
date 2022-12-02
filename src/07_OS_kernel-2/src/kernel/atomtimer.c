/*******************************************************************************
 * \brief	线程软定时器队列和系统滴答的源文件
 * \details	供挂起线程的时间片轮转，也供互斥和信号量的超时等待
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


/** 
 * \file
 * Timer and system tick library.
 *
 *
 * This module implements kernel system tick / clock functionality and timer
 * functionality for kernel and application code.
 *
 * \par Timer callbacks
 * Application and kernel code uses this module to request callbacks at a
 * specific number of system ticks in the future. atomTimerRegister() can be
 * called with a structure filled out requesting callbacks in a specific
 * number of ticks. When the timer expires the requested callback function is
 * called.
 *
 * \par Thread delays
 * Application threads can use atomTimerDelay() to request that the thread
 * delay for the specified number of system ticks. The thread will be put in
 * the timer list and taken off the ready queue. When the timer expires the
 * thread will be made ready-to-run again. This internally uses the same
 * atomTimerRegister() function that is used for registering all timers.
 *
 * \par System tick / Clock
 * This module also implements the system tick. At a predefined interval
 * (SYSTEM_TICKS_PER_SEC) architecture ports arrange for atomTimerTick() to be
 * called. The tick increments the system tick count, which can be queried by
 * application code using atomTimeGet(). On this tick, the registered timer
 * list is checked for any timers which have expired. Those which have expired
 * have their callback functions called. It is also on this system tick that
 * round-robin rescheduling time-slices occur. On exit from the tick interrupt
 * handler the kernel checks whether there are two or more threads 
 * ready-to-run at the same priority, and if so uses round-robin to schedule
 * in the next thread. This is in contrast to other (non-timer-tick)
 * interrupts which do not allow for round-robin rescheduling to occur, as
 * they should only occur on a new timer tick.
 *
 */


#include "atom.h"


/* Data types */

/* Delay callbacks data structure */
/* 硬件定时器中断中进行回调赋值的线程上下文中的时间结构 */
typedef struct delay_timer
{
    ATOM_TCB *tcb_ptr;  /* Thread which is suspended with timeout */

} DELAY_TIMER;


/* Global data */

/* Local data */

/** Pointer to the head of the outstanding timers queue */
/* 线程的软定时器队列，用来指示线程等待时间是否超时 */
static ATOM_TIMER *timer_queue = NULL;

/** Current system tick count */
/* 当前的系统时钟滴答（不是主频跳了多少次，比主频跳动要慢） */
static uint32_t system_ticks = 0;


/* Forward declarations */
static void atomTimerCallbacks (void);
static void atomTimerDelayCallback (POINTER cb_data);


/**
 * \b atomTimerRegister
 *
 * Register a timer callback.
 *
 * Callers should fill out and pass in a timer descriptor, containing
 * the number of system ticks until they would like a callback, together
 * with a callback function and optional parameter. The number of ticks
 * must be greater than zero.
 *
 * On the relevant system tick count, the callback function will be
 * called.
 *
 * These timers are used by some of the OS library routines, but they
 * can also be used by application code requiring timer facilities at
 * system tick resolution.
 *
 * This function can be called from interrupt context, but loops internally
 * through the time list, so the potential execution cycles cannot be
 * determined in advance.
 *
 * @param[in] timer_ptr Pointer to timer descriptor
 *
 * @retval ATOM_OK Success
 * @retval ATOM_ERR_PARAM Bad parameters
 */
/* 注册定时器回调 */
uint8_t atomTimerRegister (ATOM_TIMER *timer_ptr)
{
    uint8_t status;
    CRITICAL_STORE; /* 读取中断寄存器 */

    /* Parameter check */
    if ((timer_ptr == NULL) || (timer_ptr->cb_func == NULL)
        || (timer_ptr->cb_ticks == 0))
    {
        /* Return error */
        status = ATOM_ERR_PARAM;
    }
    else
    {
        /* Protect the list */
        CRITICAL_START (); /* 关闭中断 */

        /*
         * Enqueue in the list of timers.
         *
         * The list is not ordered, all timers are inserted at the start
         * of the list. On each system tick increment the list is walked
         * and the remaining ticks count for that timer is decremented.
         * Once the remaining ticks reaches zero, the timer callback is
         * made.
         */
        if (timer_queue == NULL)
        {
            /* List is empty, insert new head */
			/* 如果定时器队列为空，则将自己设为队列头 */
            timer_ptr->next_timer = NULL;
            timer_queue = timer_ptr;
        }
        else
        {
            /* List has at least one entry, enqueue new timer before */
			/* 如果定时器队列已存在，则将自己加到队列末尾 */
            timer_ptr->next_timer = timer_queue;
            timer_queue = timer_ptr;
        }

        /* End of list protection */
        CRITICAL_END (); /* 恢复中断 */

        /* Successful */
        status = ATOM_OK;
    }

    return (status);
}


/**
 * \b atomTimerCancel
 *
 * Cancel a timer callback previously registered using atomTimerRegister().
 *
 * This function can be called from interrupt context, but loops internally
 * through the time list, so the potential execution cycles cannot be
 * determined in advance.
 *
 * @param[in] timer_ptr Pointer to timer to cancel
 *
 * @retval ATOM_OK Success
 * @retval ATOM_ERR_PARAM Bad parameters
 * @retval ATOM_ERR_NOT_FOUND Timer registration was not found
 */
/* 取消之前注册的定时器回调，从定时器队列中移除 */
uint8_t atomTimerCancel (ATOM_TIMER *timer_ptr)
{
    uint8_t status = ATOM_ERR_NOT_FOUND;
    ATOM_TIMER *prev_ptr, *next_ptr;
    CRITICAL_STORE; /* 读中断状态 */

    /* Parameter check */
    if (timer_ptr == NULL)
    {
        /* Return error */
        status = ATOM_ERR_PARAM;
    }
    else
    {
        /* Protect the list */
        CRITICAL_START (); /* 关闭中断 */

        /* Walk the list to find the relevant timer */
        prev_ptr = next_ptr = timer_queue;
        while (next_ptr)
        {
            /* Is this entry the one we're looking for? */
            if (next_ptr == timer_ptr)
            {
                if (next_ptr == timer_queue)
                {
                    /* We're removing the list head */
                    timer_queue = next_ptr->next_timer;
                }
                else
                {
                    /* We're removing a mid or tail TCB */
                    prev_ptr->next_timer = next_ptr->next_timer;
                }

                /* Successful */
                status = ATOM_OK;
                break;
            }

            /* Move on to the next in the list */
            prev_ptr = next_ptr;
            next_ptr = next_ptr->next_timer;

        }

        /* End of list protection */
        CRITICAL_END (); /* 恢复中断 */
     }

    return (status);
}


/**
 * \b atomTimeGet
 *
 * Returns the current system tick time.
 *
 * This function can be called from interrupt context.
 *
 * @retval Current system tick count

 */
/* 获取系统时钟滴答 */
uint32_t atomTimeGet(void)
{
    return (system_ticks);
}


/**
 * \b atomTimeSet
 *
 * This is an internal function not for use by application code.
 *
 * Sets the current system tick time.
 *
 * Currently only required for automated test suite to test
 * clock behaviour.
 *
 * This function can be called from interrupt context.
 *
 * @param[in] new_time New system tick time value
 *
 * @return None
 */
/* 内部测试时使用的，修改系统滴答 */
void atomTimeSet(uint32_t new_time)
{
    system_ticks = new_time;
}


/**
 * \b atomTimerTick
 *
 * System tick handler.
 *
 * User ports are responsible for calling this routine once per system tick.
 *
 * On each system tick this routine is called to do the following:
 *  1. Increase the system tick count
 *  2. Call back to any registered timer callbacks
 *
 * @return None
 */
/* 系统移植时需要在硬件定时器中断处理中调用的第2个函数 */
/* 每次进入会增加操作系统的滴答，并会循环定时器队列中的所有软定时器，减去计数并判断是否到时 */
/* 可用于挂起的线程队列 */
void atomTimerTick (void)
{
    /* Only do anything if the OS is started */
    if (atomOSStarted)
    {
        /* Increment the system tick count */
        system_ticks++;

        /* Check for any callbacks that are due */
		/* 循环定时器队列中的所有软定时器，减去计数并判断是否到时 */
        atomTimerCallbacks ();
    }
}


/**
 * \b atomTimerDelay
 *
 * Suspend a thread for the given number of system ticks.
 *
 * Note that the wakeup time is the number of ticks from the current system
 * tick, therefore, for a one tick delay, the thread may be woken up at any
 * time between the atomTimerDelay() call and the next system tick. For
 * a minimum number of ticks, you should specify minimum number of ticks + 1.
 *
 * This function can only be called from thread context.
 *
 * @param[in] ticks Number of system ticks to delay (must be > 0)
 *
 * @retval ATOM_OK Successful delay
 * @retval ATOM_ERR_PARAM Bad parameter (ticks must be non-zero)
 * @retval ATOM_ERR_CONTEXT Not called from thread context
 */
/* 将线程挂起多少个系统滴答 */
uint8_t atomTimerDelay (uint32_t ticks)
{
    ATOM_TCB *curr_tcb_ptr;
    ATOM_TIMER timer_cb;
    DELAY_TIMER timer_data;
    CRITICAL_STORE; /* 读中断状态 */
    uint8_t status;

    /* Get the current TCB  */
    curr_tcb_ptr = atomCurrentContext();

    /* Parameter check */
    if (ticks == 0)
    {
        /* Return error */
        status = ATOM_ERR_PARAM;
    }

    /* Check we are actually in thread context */
    else if (curr_tcb_ptr == NULL)
    {
        /* Not currently in thread context, can't suspend */
        status = ATOM_ERR_CONTEXT;
    }

    /* Otherwise safe to proceed */
    else
    {
        /* Protect the system queues */
        CRITICAL_START (); /* 关中断 */

        /* Set suspended status for the current thread */
        curr_tcb_ptr->suspended = TRUE;

        /* Register the timer callback */

        /* Fill out the data needed by the callback to wake us up */
        timer_data.tcb_ptr = curr_tcb_ptr;

        /* Fill out the timer callback request structure */
        timer_cb.cb_func = atomTimerDelayCallback;
        timer_cb.cb_data = (POINTER)&timer_data;
        timer_cb.cb_ticks = ticks;

        /* Store the timeout callback details, though we don't use it */
        curr_tcb_ptr->suspend_timo_cb = &timer_cb;

        /* Register the callback */
		/* 将挂起的线程加入到软定时器队列 */
        if (atomTimerRegister (&timer_cb) != ATOM_OK)
        {
            /* Exit critical region */
            CRITICAL_END ();

            /* Timer registration didn't work, won't get a callback */
            status = ATOM_ERR_TIMER;
        }
        else
        {
            /* Exit critical region */
            CRITICAL_END (); /* 恢复中断 */

            /* Successful timer registration */
            status = ATOM_OK;

            /* Current thread should now block, schedule in another */
            atomSched (FALSE);
        }
    }

    return (status);
}


/**
 * \b atomTimerCallbacks
 *
 * This is an internal function not for use by application code.
 *
 * Find any callbacks that are due and call them up.
 *
 * @return None
 */
/* 遍历软定时器队列，减少里面的计数，找到队列中能可以退出的软定时器 */
static void atomTimerCallbacks (void)
{
    ATOM_TIMER *prev_ptr, *next_ptr, *saved_next_ptr;
    ATOM_TIMER *callback_list_tail = NULL, *callback_list_head = NULL;

    /*
     * Walk the list decrementing each timer's remaining ticks count and
     * looking for due callbacks.
     */
    prev_ptr = next_ptr = timer_queue;
    while (next_ptr)
    {
        /* Save the next timer in the list (we adjust next_ptr for callbacks) */
        saved_next_ptr = next_ptr->next_timer;
 
        /* Is this entry due? */
		/* 线程定时器是否到时 */
        if (--(next_ptr->cb_ticks) == 0)
        {
            /* Remove the entry from the timer list */
			/* 如果定时器已到，则移除该定时器 */
            if (next_ptr == timer_queue)
            {
                /* We're removing the list head */
                timer_queue = next_ptr->next_timer;
            }
            else
            {
                /* We're removing a mid or tail timer */
                prev_ptr->next_timer = next_ptr->next_timer;
            }

            /*
             * Add this timer to the list of callbacks to run later when
             * we've finished walking the list (we shouldn't call callbacks
             * now in case they want to register new timers and hence walk
             * the timer list.
             *
             * We reuse the ATOM_TIMER structure's next_ptr to maintain the
             * callback list.
             */
            if (callback_list_head == NULL)
            {
                /* First callback request in the list */ 
                callback_list_head = callback_list_tail = next_ptr;
            }
            else
            {
                /* Add callback request to the list tail */
                callback_list_tail->next_timer = next_ptr;
                callback_list_tail = callback_list_tail->next_timer;
            }

            /* Mark this timer as the end of the callback list */
			/* 退出循环 */
            next_ptr->next_timer = NULL;

            /* Do not update prev_ptr, we have just removed this one */

        }

        /* Entry is not due, leave it in there with its count decremented */
        else
        {
            /*
             * Update prev_ptr to this entry. We will need it if we want
             * to remove a mid or tail timer.
             */
            prev_ptr = next_ptr;
        }

        /* Move on to the next in the list */
		/* 循环下一个定时器 */
        next_ptr = saved_next_ptr;
    }

    /*
     * Check if any callbacks were due. We call them after we walk the list
     * in case they want to register new timers (and hence walk the list).
     */
    if (callback_list_head)
    {
        /* Walk the callback list */
        next_ptr = callback_list_head;
        while (next_ptr)
        {
            /*
             *  Save the next timer in the list (in case the callback
             *  modifies the list by registering again.
             */
            saved_next_ptr = next_ptr->next_timer;

            /* Call the registered callback */
			/* 定时器到时后执行对应的操作 */
            if (next_ptr->cb_func)
            {
                next_ptr->cb_func (next_ptr->cb_data);
            }

            /* Move on to the next callback in the list */
            next_ptr = saved_next_ptr;
        }
    }

}


/**
 * \b atomTimerDelayCallback
 *
 * This is an internal function not for use by application code.
 *
 * Callback for atomTimerDelay() calls. Wakes up the sleeping threads.
 *
 * @param[in] cb_data Callback parameter (DELAY_TIMER ptr for sleeping thread)
 *
 * @return None
 */
/* 唤醒挂起的线程 */
static void atomTimerDelayCallback (POINTER cb_data)
{
    DELAY_TIMER *timer_data_ptr;
    CRITICAL_STORE;

    /* Get the DELAY_TIMER structure pointer */
    timer_data_ptr = (DELAY_TIMER *)cb_data;

    /* Check parameter is valid */
    if (timer_data_ptr)
    {
        /* Enter critical region */
        CRITICAL_START (); /* 关闭中断 */

        /* Put the thread on the ready queue */
		/* 将挂起队列的线程加入到就绪队列 */
        (void)tcbEnqueuePriority (&tcbReadyQ, timer_data_ptr->tcb_ptr);

        /* Exit critical region */
        CRITICAL_END ();

        /**
         * Don't call the scheduler yet. The ISR exit routine will do this
         * in case there are other callbacks to be made, which may also make
         * threads ready.
         */
    }
}

