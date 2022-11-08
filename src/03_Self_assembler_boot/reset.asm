;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; @brief	程序执行的第二行代码所在处
; @author	将狼才鲸
; @date		2022-11-06
; @note		File format: UTF-8
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;/*
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

				PRESERVE8	; 指定当前文件要求堆栈八字节对齐
				THUMB		; 使用THUMB指令集，不使用ARM指令集	; 头文件包含，里面只定义了一些宏定义

				AREA		|.text|, CODE, READONLY	; 定义.text代码段，只读

;;
; \brief	复位中断处理函数
; \details	程序执行的第一行代码是0地址的复位中断，然后才直接调用到这个函数，
;			中断向量表在startup.asm中定义
;;
Reset_Handler	PROC
				EXPORT	Reset_Handler	; 声明函数，声明后可以在别处调用
				;IMPORT	__hw_init		; 声明硬件初始化函数
				IMPORT	__main			; 声明c语言的main函数，c语言函数在汇编中会默认在前面加上两个下划线

				;BL		__hw_init		; 调用硬件初始化的函数
				BL		__main			; 跳转到C语言main()函数
				BL		Cpu_Stall		; 如果main函数中不是死循环，函数退出了，则直接在这里死循环

				ALIGN	; 取消8字节对齐，标号地址都是要4字节对齐的，否则程序会出错
				ENDP	; 函数结束

;;
; \brief	死循环函数
;;
Cpu_Stall		PROC
				EXPORT	Cpu_Stall
				B		.	; 跳转到本行地址，也就是死循环

				ENDP

				END			; 编译汇编时，文件尾要先写END，否则编译时从第一条语句就会报错，而且只会提示指令不支持
