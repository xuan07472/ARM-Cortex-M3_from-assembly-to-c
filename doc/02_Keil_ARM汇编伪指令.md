# Keil ARM汇编伪指令

|作者|将狼才鲸|
|---|---|
|创建日期|2022-11-06|

* 本文档描述Keil MDK ARM格式的汇编，而不是GNU格式的汇编。
* ARM格式的汇编使用armasm编译器，伪指令多为大写；GNU格式使用armclang 集成编译器，伪指令多为小写。
* Keil MDK针对ARM 32位CPU的汇编伪指令和Keil C51中针对8051的伪指令不同。

* *参考网址：*  
  * [【方辉专栏】ARM嵌入式编译器（十） GNU 汇编和 armasm 汇编代码介绍](http://www.emdoor.cn/News/view/id/1732.html)
  * [零基础学ARM：MDK和GNU伪指令区别](https://www.elecfans.com/d/1434345.html)
  * [基于ARM编译器版本5的工程迁移与适配到ARM编译器版本6.12 后续1 - 汇编代码处理问题](https://blog.csdn.net/jackailson/article/details/103753951)
  * [从 Arm Compiler 5 迁移到 Arm Compiler 6](https://blog.csdn.net/zhzht19861011/article/details/109803651)

---

<center>表1 Keil ARM汇编伪指令</center>

|伪指令|作用|举例|
|---|---|---|
|END|通知编译器已经到了该源文件的结尾了，编译汇编时，文件尾要先写END，否则编译时从第一条语句就会报错，而且只会提示指令不支持||
|INCLUDE|包含头文件|INCLUDE conf.inc|
|PRESERVE8|指定当前文件要求堆栈八字节对齐||
|THUMB|使用THUMB指令集，不使用ARM指令集||
|AREA|定义段|AREA .text, CODE, READONLY|
|CODE|定义代码段||
|READONLY|只读||
|PROC|汇编函数|MyFunc PROC 指令|
|ENDP|汇编函数结束|MyFunc PROC 指令 ENDP|
|EXPORT|输出函数声明，然后该函数在别处可用|EXPORT MyFunc|
|IMPORT|进行函数声明，可以使用其它文件的函数|EXPORT YourFunc|
|ALIGN|取消字节对齐||
|DCD|数据定义并分配存储单元||
|EQU|宏定义|Size EQU 4|
|MACRO|宏定义函数||
|MEND|宏定义函数结束||
|$|定义变量名|$Name|
|STACK|定义栈|AREA	STACK, NOINIT, READWRITE, ALIGN=3|
|NOINIT|内存不初始化||
|READWRITE|可读可写||
|SPACE|分配指定长度的一片连续的内存|SPACE 1024|
|IF|判断||
|ENDIF|IF结束||

|...|...|...|
