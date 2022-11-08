/******************************************************************************
 * \brief	程序除boot外执行的第一个C语言函数
 * \details	main函数里什么都没有，这个工程主要是自己用汇编写了M3的boot代码，
 *			并成功用汇编引导了C语言main函数
 * \author	将狼才鲸
 * \date	2022-11-08
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
