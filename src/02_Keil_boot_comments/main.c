/*******************************************************************************
 * \brief	使用Keil自带的boot代码创建默认工程，增加输出重定向，使用Keil模拟器
 *			运行，输出Hello world到调试终端中
 * \author	将狼才鲸
 * \date	2022-11-06
 * \note	参考网址：
 *			* [应用相关 新建一个基于ARM M3的工程]
 *			  (https://bbs.21ic.com/forum.php?mod=viewthread&tid=2818416) 选择CMSIS中的Core和Device中的Startup即可。
 *			* [Error: L6218E: Undefined symbol Image$$ARM_LIB_STACK$$ZI$$Limit Not enough information to list image]
 *			  (https://blog.csdn.net/qq_41200467/article/details/124958685)
 *			* 配置中的Debug页面勾选使用模拟器。
 *			* [Keil Debug(printf) Viewer的使用]
 *			  (https://blog.csdn.net/mygod2008ok/article/details/105234076)
 *			* [keil去除未使用的参数，变量，返回值的警告]
 *			  (https://blog.csdn.net/mygod2008ok/article/details/105234076)
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
