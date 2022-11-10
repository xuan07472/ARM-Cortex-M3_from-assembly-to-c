;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; @brief	程序执行的第一行代码所在处，中断向量表
; @author	将狼才鲸
; @date		2022-11-06
; @note		File format: UTF-8
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

				INCLUDE	config.inc

				PRESERVE8	; 8字节对齐，函数标号所在处一定要4字节对齐才能运行正常
				THUMB		; 汇编使用thumb指令集

;;
; \brief	中断向量表
; \details	从地址0开始的数据段，0地址存放栈顶地址，4地址是复位中断，后面紧接着的是其它中断
;;
				AREA	RESET, DATA, READONLY	; 定义名为‘RESET’的数据段，只读

				; 声明本文件中定义的函数
				EXPORT	__Vectors		; 声明定义了第一个中断的入口地址
				EXPORT	__Vectors_End	; 声明定义了最后一个中断的入口地址
				EXPORT	__Vectors_Size	; 声明中断的地址总量
				EXPORT	Default_Interrupt_Handler	; 声明没有指定中断函数时的默认中断处理函数

				IMPORT	__Init_Sp		; 声明其它文件中定义的栈顶地址

;;
; \brief	在各个中断入口调用中断处理函数
; \details	__Vectors是整个中断入口的初始地址
;;
__Vectors		DCD	__Init_Sp		; 栈顶地址
				DCD	Reset_Handler	; 复位中断
				;TODO: 添加其它的所有中断入口
__Vectors_End

__Vectors_Size  EQU __Vectors_End - __Vectors

;;
; \brief	中断和异常处理的宏定义函数
; \details	这里定义的函数里面只有死循环，可以在别处重写这些中断处理函数
;;
				MACRO	; 宏定义函数
				Set_Default_Handler	$Handler_Name
$Handler_Name	PROC
				EXPORT	$Handler_Name [WEAK] ; 该函数可重定义，也可以不定义
				B		.	; 死循环
				ENDP
				MEND

;;
; \brief	声明默认的中断处理函数
; \details	该函数没有定义也可以
;;
				AREA     |.text|, CODE, READONLY	; 定义只读代码段
				Set_Default_Handler  Reset_Handler
				Set_Default_Handler  Default_Interrupt_Handler

				ALIGN	; 取消8字节对齐
				END		; 文件尾
