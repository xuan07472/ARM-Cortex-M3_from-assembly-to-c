;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; @brief	演示ARM M3的汇编宏定义函数
; @author	将狼才鲸
; @date		2022-11-10
; @note		File format: UTF-8
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;; 私有宏定义函数，本模块内部使用 ;;;;;;;;
;;
; \brief		将通用寄存器里事先存好的值写到地址中去
; \param[in]	$reg, 只能是一个通用寄存器如R0等，里面存放要写的值
; \param[in]	$addr, 立即数地址
;;
				MACRO	; 宏定义函数
				ADDR_W_REG $reg, $addr
				; 将地址存在R1中
				MOVW R1, #($addr & 0xffff)	; movw命令代表将值放在寄存器的低16位，清零高16位
				MOVT R1, #($addr >> 16)		; movt命令代表将值放在寄存器的高16位，不影响低16位
				STR $reg, [R1, #0x00]		; 将前面寄存器里的值放到后面R1+0的地址中，
				MEND	; 宏定义函数结束

;;
; \brief		将地址里的值读到寄存器中
; \param[in]	$reg, 目的寄存器
; \param[in]	$addr, 要读的地址
;;
				MACRO
				ADDR_R_REG $reg, $addr
				MOVW R1, #($addr & 0xffff)
				MOVT R1, #(&addr >> 16)
				LDR $reg, [R1, #0x00]	; *reg = *(R1 + 0)
				MEND

;;;;;;;; 接口宏定义函数 ;;;;;;;;
;;
; \brief	将一个立即数写入到地址中
; \param[in]	$v, 要写入的值
; \param[in]	$addr, 要写的地址
;;
				MACRO
				ADDR_WRITE	$v, $addr
				MOVW R0, #($v & 0xffff) ; 将要写入的值低16位转到寄存器
				MOVT R0, #($v >> 16)	; 转存高16位
				ADDR_W_REG R0, $addr	; 调用函数将R0写入到addr
				MEND

;;
; \brief	读一个地址的数据，值放到R0中
; \param[in]	$addr, 要读的地址
; \param[out]	R0, 返回的值
;;
				MACRO
				ADDR_READ $addr
				ADDR_R_REG R0, $addr	; 调用函数将addr中的值读到R0中
				MEND

				END