/******************************************************************************
 * \brief	使用Keil自带的调试窗口Debug(printf) Viewer实现串口输入输出
 * \author	将狼才鲸
 * \date	2022-11-10
 * \note	File format: UTF-8
 * \note	配置中的Debug页面勾选使用模拟器。
 *			MDK的Debug (printf) Viewer窗口不像C51的UART #1窗口，UART #1在网上能
 *			很容易的找到教程，通过VSD虚拟串口软件，将Keil C51的调试串口和电脑的
 *			虚拟串口相绑定，这样就能使用SSCOM或者PUTTY等串口软件收发二进制数据了；
 *			Debug (printf) Viewer窗口我还没找到绑定的方法，所以当前scanf不能
 *			获取到16进制和int型的数据，只能获取到字符和字符串，但是这对使用
 *			模拟器仿真程序来说够用了。
 * \note	参考网址：
 *			* 报错解决[Error: L6218E: Undefined symbol Image$$ARM_LIB_STACK$$ZI$$Limit Not enough information to list image]
 *			  (https://blog.csdn.net/qq_41200467/article/details/124958685)
 *			* 如何配置工程[µVision User's Guide - Debug (printf) Viewer]
 *			  (https://developer.arm.com/documentation/101407/0537/Debugging/Debug-Windows-and-Dialogs/Debug--printf--Viewer)
 * \warning	Keil Debug(printf) Viewer获取单个字符时有个bug，第一次输入的值获取不到，
 *			以后每次获取到的字符都是你前一次按下的字符，推测因为软件模拟的中断收发
 *			实际上都是用的键盘键值，一个键由好几个字符组成，Keil TIM工具再将键值转成
 *			ASCII的单个字符，中间有点bug；但是字符串输入是正常的，不会丢数据。
 ******************************************************************************/

/********************************** 头文件 ************************************/
#include <stdio.h>

/********************************** 宏定义 ************************************/
//#define CHAR_LOOPBACK		/* 收到一个字符就返回一个字符 */
#define STRING_LOOPBACK	/* 收到一行后再返回一行，一行以回车结尾 */
#define BUF_MAX_SIZE 256	/* 设置长一点，防止按回车键前数据溢出 */
/* Keil使用自带的模拟器运行时，输入的换行是\r，但输出换行却是\r\n */
#define FIX_KEIL_EMULATOR_AND_SSCOM32_UART_LINE_FEED_ISSUE

/********************************* 全局变量 ***********************************/
static char rcv_buf[BUF_MAX_SIZE] = {0};	/* 完整的一行接收缓存，输入时以回车键结尾，值为\r\n的组合 */

/********************************* 接口函数 ***********************************/
/**
 * \brief	入口函数
 */
int main()
{
	/* 1. 打印提示信息 */
	printf("====\r\n");
	printf("| Check your terminal line end format.\r\n");
	printf("| \r \\r \r");
	printf("| \n \\n \n");
	printf("| \n\r \\n\\r \n\r");
	printf("| \r\n \\r\\n \r\n");
	printf("====\r\n\r\n");

	printf("====\r\n");
#ifdef STRING_LOOPBACK
	printf("Now is string loopback mode\r\n");
	printf("Please input string, and end with 'Enter' key(value is\\r or \\n).\r\n");
#else
	printf("Now is char loopback mode\r\n");
	printf("Please input char\r\n");
#endif

	/* 3. 打印从串口中断收到的信息 */
	while (1) {
#ifdef STRING_LOOPBACK
		scanf("%s", rcv_buf);
		printf("Your Input: ");
		printf(rcv_buf);  /* 把接收到的字符串发送回去 */
#	ifdef FIX_KEIL_EMULATOR_AND_SSCOM32_UART_LINE_FEED_ISSUE
		printf("\n");
#	endif
#else
		scanf("%c", &rcv_buf[0]);
		printf(">"); /* 防止有人不知道串口中断输入是没有回显的，原样输出会以为只是自己的输入 */
		printf("%c", rcv_buf[0]);
#endif
	}

	return 0;
}
