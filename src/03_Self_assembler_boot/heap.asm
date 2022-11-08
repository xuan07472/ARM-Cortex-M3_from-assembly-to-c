;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; @brief	定义堆，类似于宏定义，并不具体执行代码
; @details	这个文件里不写东西也能编译通过，并没有分配堆
; @note		File format: UTF-8
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

				INCLUDE	config.inc

				PRESERVE8
				THUMB

;;
; \brief	堆地址与堆空间定义
;;
Heap_Size		EQU		(CONFIG_HEAP_K_SIZE * 1024)	; 长度为0，没有开辟堆

				IF		Heap_Size != 0	; 如果需要开辟堆

				AREA	HEAP, NOINIT, READWRITE, ALIGN=3
				EXPORT	__Heap_Base
				EXPORT	__Heap_Limit

__Heap_Base
Heap_Mem		SPACE	Heap_Size
__Heap_Limit

				ENDIF

				END
