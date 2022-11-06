;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; @brief	程序执行的第一行代码
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
			END			; 编译汇编时，文件尾要先写END，否则编译时从第一条语句就会报错，而且只会提示指令不支持
