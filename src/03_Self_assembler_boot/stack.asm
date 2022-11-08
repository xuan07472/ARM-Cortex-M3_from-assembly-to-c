;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; @brief	定义栈地址，类似于宏定义，并不具体执行代码
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

				INCLUDE config.inc

				PRESERVE8
				THUMB

;;
; \brief	定义栈尺寸，必须8字节对齐
;;
Stack_Size		EQU		(CONFIG_STACK_K_SIZE * 1024)	; 4K

				; 申明开辟名为STACK的内存，不写入值初始化，可读可写，2^3 8字节对齐
				AREA	STACK, NOINIT, READWRITE, ALIGN=3
				EXPORT	__Stack_Limit	; 声明栈底地址变量
				EXPORT	__Init_Sp		; 声明栈顶地址变量

__Stack_Limit
Stack_Mem		SPACE	Stack_Size		; 开辟4K内存
__Init_Sp

				ALIGN	; 取消8字节对齐
				END		; 文件尾
