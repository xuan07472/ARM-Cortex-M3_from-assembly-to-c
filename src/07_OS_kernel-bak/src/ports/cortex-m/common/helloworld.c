/*
 * \note	File format: UTF-8
 * \note	一些必要的Keil工程配置，可以关掉Keil的所有编译警告，
 *			但我这里一项项去掉，在配置页面的的C/C++(AC6)窗口中加入以下内容
 *			-Wno-padded，去掉结构体字节未对齐的警告
 *			-Wno-implicit-function-declaration找不到函数名警告，
 *			-Wno-implicit-int-conversion类型转换警告，
 *			-Wno-sign-conversion有符号转无符号警告，
 *			-Wno-self-assign自己给自己赋值的警告，
 *			-Wno-missing-noreturn void类型函数的警告，
 *			-Wno-comment双斜杠注释的警告
 *			-Wno-missing-variable-declarations设置静态变量
 *			-Wno-pointer-arith
 *			-Wno-unreachable-code-return死循环后有return
 *			-Wno-missing-prototypes允许函数名前面有下划线
 *			-Wno-cast-qual允许类型转换
 * \note	atomthreads下载地址：
 *			[ACleverDisguise / atomthreads](https://gitee.com/ACleverDisguise/atomthreads)
 *			代码是几年前的，最新版本是v1.3，因为代码量本来就小，所以也没有频繁更新
 * \note	libopencm3下载地址：
 *			[sunnybooy / libopencm3](https://gitee.com/sunnybooy/libopencm3)
 */

/*
 * Copyright (c) 2015, Tido Klaassen. All rights reserved.
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

#include <stdio.h>

#include <libopencm3/cm3/nvic.h>
#include "atom.h"
#include "atomport-private.h"
#include "atomtimer.h"

#define STACK_SIZE      1024
#define THREAD_PRIO     42

static ATOM_TCB main_tcb;

static uint8_t thread_stacks[2][STACK_SIZE];

static void main_thread_func(uint32_t data);

/**
 * Example for a stand-alone board application
 */
extern int board_setup(void);
int main(void)
{
    int8_t status;
    uint32_t loop;

    /**
     * Brief delay to give the debugger a chance to stop the core before we
     * muck around with the chip's configuration.
     */
    for(loop = 0;loop < 1000000;++loop){
        __asm__("nop");
    }

    board_setup();

    /**
     * Initialise OS and set up idle thread
     */
    status = atomOSInit(&thread_stacks[0][0], STACK_SIZE, FALSE);

    if(status == ATOM_OK){
        /* Set up main thread */
        status = atomThreadCreate(&main_tcb, THREAD_PRIO, main_thread_func, 0,
                                  &thread_stacks[1][0], STACK_SIZE, TRUE);

        if(status == ATOM_OK){
            atomOSStart();
        }
    }

    while(1)
        ;

    /* We will never get here */
    return 0;
}

extern void test_led_toggle(void);
static void main_thread_func(uint32_t data __maybe_unused)
{
    /* Print message */
    printf("Hello, world!\n");

    /* Loop forever and blink the LED */
    while(1){
        test_led_toggle();

        atomTimerDelay(SYSTEM_TICKS_PER_SEC);
    }

}
