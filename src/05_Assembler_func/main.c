/******************************************************************************
 * \brief	main函数里什么都没有，这个工程主要是演示func.asm中的汇编函数的写法
 * \author	将狼才鲸
 * \date	2022-11-10
 * \note	File format: UTF-8
 ******************************************************************************/

/************************************ 头文件 **********************************/
#include <stdio.h>

/************************************ 宏定义 **********************************/
#define UNUSED_VARIABLE(X)	((void)(X))
#define ITM_PORT8(n)		(*(volatile unsigned char *)(0xe0000000 + 4*(n)))
#define ITM_PORT16(n)		(*(volatile unsigned short *)(0xe0000000 + 4*(n)))
#define ITM_PORT32(n)		(*(volatile unsigned long *)(0xe0000000 + 4*(n)))
#define DEMCR				(*(volatile unsigned long *)(0xE000EDFC))
#define TRCENA				0X01000000

/*********************************** 接口函数 *********************************/
/**
 * \brief	printf输出重定向到Keil的Debug(printf) Viewer窗口中
 * \details	Keil仿真时使用Keil模拟器，并在Keil Debug(printf) Viewer窗口输出数据
 */
int fputc(int ch, FILE *f)
{
	UNUSED_VARIABLE(f);

	if(DEMCR & TRCENA) {
		while(ITM_PORT32(0) == 0);
		ITM_PORT8(0) = (char)ch;
	}
	return ch;
}

int main()
{
	printf("Hello world!\n");

	return 0;
}
