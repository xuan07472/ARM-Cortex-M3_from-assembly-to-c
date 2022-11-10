;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; @brief	演示ARM M3汇编函数的编写，压栈弹栈，汇编宏定义函数等
; @author	将狼才鲸
; @date		2022-11-10
; @note		File format: UTF-8
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

				INCLUDE	func.inc	; 包含头文件，头文件没有规避递归包含，注意不要多次包含同一个文件

				PRESERVE8
				THUMB

				AREA	|.text|, CODE, READONLY

;;
; \brief	汇编写的系统、硬件初始化函数，在main函数执行前先把DDR、PLL等准备好
;;
__hw_init		PROC
				EXPORT	__hw_init
				PUSH	{R0, R1, R2, R3, LR}	; 先压栈
				BL		__hw_device_init		; 调用没有参数的函数
				POP		{R0, R1, R2, R3, LR}	; 最后再弹栈，不需要像51那样和压栈时顺序相反
				BX		LR	; 返回调用的函数
				ALIGN		; 取消字节对齐
				ENDP		; 函数结束

__hw_device_init	PROC
				EXPORT	__hw_device_init
				PUSH	{R0, R1, R2, R3, LR}
				ADDR_WRITE 0x00, 0x40004100
				;TODO 配置必要的寄存器
				POP		{R0, R1, R2, R3, LR}
				BX		LR
				ALIGN
				ENDP
	
				END
