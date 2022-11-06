/*******************************************************************************
 * \brief	给Keil自带的boot加上注释
 * \details	Keil自带的boot文件在./RTE/Device/ARMCM3/目录下
 * \author	将狼才鲸
 * \date	2022-11-06
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
