# ARM Cortex-M3从汇编到C，从Boot到应用的教程  

|作者|将狼才鲸|
|---|---|
|创建日期|2022-11-05|

Gitee工程和源码地址：[才鲸嵌入式 / ARM-Cortex-M3从汇编到C_从Boot到应用教程](https://gitee.com/langcai1943/ARM-Cortex-M3_from-assembly-to-c)  
CSDN文章阅读地址：[ARM Cortex-M3从汇编到C，从Boot到应用的教程](https://blog.csdn.net/qq582880551/article/details/127711475)  
Bilibili视频讲解地址（待完成）：[才鲸嵌入式](https://space.bilibili.com/106424039)  

* 其它相关仓库文档：  
[才鲸嵌入式 / 嵌入式知识图谱WiKi](https://gitee.com/langcai1943/embedded-knowledge-wiki)  
[才鲸嵌入式 / 8051_c51_单片机从汇编到C_从Boot到应用实践教程](https://gitee.com/langcai1943/8051-from-boot-to-application)  

---

## 一、前言

### 1）本仓库的目的

* 本仓库计划实现的内容：
  * 描述Cortex-M3的指令集和通用寄存器。
  * 针对M3内核，使用汇编从复位开始写boot引导C语言main()函数。
  * 不使用任何芯片厂商提供的开发包，自己写Boot，自行移植C语言库函数，自己写所有驱动和应用。
  * 移植一款RTOS操作系统。

* 本仓库面向的目标读者：
  * 使用M3的某一款芯片写过驱动或应用，但是对M3的boot过程和底层代码不熟悉的。
  * 没接触过ARM，对32位芯片的工作流程感兴趣的。
  * 对整个嵌入式裸机软件结构感兴趣的。

* 不适用本仓库的读者：
  * 想尽快用ARM芯片写出一个项目的（这时应该直接使用STM32，调用它丰富且使用简洁的库）。

### 2）M3介绍

* M3由ARM公司于2004年推出，至今仍是很多单片机芯片使用的内核，如STM32F10x系列。
* ST公司于2007年首次使用ARM公司的内核，产品是F1，随后凭借其简洁易用的软件开发包逐渐发展出著名的STM32系列，累计出货量近百亿颗。
* M3属于ARMv7架构，ARMv7是ARM11之后的版本。
* M3属于Cortex系列，该系列有三类：A、R和M，比如熟悉的Cortex-A9。
* M3内核仅33000门。
* M3不能使用ARM指令集，而是使用Thumb或Thumb-2指令集。
* M3使用Cortex微控制器软件接口标准 (CMSIS)作为硬件抽象层。

* *参考资料：*  

[MCU缺货涨价后的国产化浪潮（三）：全球 MCU 市场高度集中，多因素共振加速国产替代](https://www.bilibili.com/read/cv13508527/) 文章里也列出了全球和国内的MCU厂商和所有嵌入式的行业。  
[ARM CORTEX-M3简介](https://blog.csdn.net/changyourmind/article/details/51814844)  
[ARM Cortex-M3](https://wenku.baidu.com/view/8fc3c75d312b3169a451a41d.html)
[ARM发布适于高性能、低成本应用的Cortex-M3处理器](https://tech.hqew.com/xinpin_1765411)  

## 二、ARM-MDK IDE集成开发环境下载  
* MDK-arm软件社区版官方介绍（无代码大小限制，不能用于商用，需要先获取社区版许可证，也就是在官网注册账号后再下载）：[MDK-社区版概述](https://www.keil.com/pr/article/1299.htm)  
* 获取社区版许可证：[Log in with your Arm or Mbed account](https://www.keil.arm.com/login/?next=%2Fmdk-community%2F)  
* 下载地址主界面：[MDK Community Edition](https://www.keil.arm.com/mdk-community/)  
* 下载地址举例：https://www.keil.com/fid/comahow53j1j1wriguw1y56me9lv1dgw3o3fd1/files/eval/mdk536.exe  
* MKD-arm评估版软件官方下载地址（也就是不注册账号就下载，有32K代码限制）：[mdk536.exe](https://www.keil.com/fid/6upwf5w1y9wj1wt4huw1djsme93o1dgwmicqd1/files/eval/mdk536.exe)  
* MKD里面没有硬件模拟器，可以直接运行和调试程序，你也可以编译完生成可执行程序后在QEMU软件里面仿真运行。  

## 三、M3指令集和寄存器介绍

### 1）M3文档在线阅读及下载

* 芯片（IP核）手册 [ARM Cortex-M3 Processor Technical Reference Manual](https://developer.arm.com/documentation/100165/0201/)  
* ARMv7-M指令集手册 [ARMv7-M Architecture Reference Manual](https://developer.arm.com/documentation/ddi0403/ee)  

* 以上两个文档描述了M3内核的通用寄存器和指令集，其中：
  * 芯片手册的“Table 3-1 Cortex-M3 instruction set summary”有所有指令的汇总表格，在线阅读地址在[Processor instructions](https://developer.arm.com/documentation/100165/0201/Programmers-Model/Instruction-set-summary/Processor-instructions)。
  * 



### 2）其它ARM核指令集介绍

* ARM 可以用两套指令集：ARM指令集和Thumb指令集。  
* ARM核各个系列的介绍详见 ：  
*参考网址：*  
[关于ARM的内核架构](https://www.cnblogs.com/zhugeanran/p/8431127.html)  
[ARM汇编指令集汇总](https://blog.csdn.net/qq_40531974/article/details/83897559)  
[ARM指令集](https://blog.csdn.net/weixin_42110547/article/details/86672824)  
[关于ARM处理器指令集](https://blog.csdn.net/qq_45385706/article/details/108478867)  
[ARM指令集](https://www.diangon.com/m209371.html)  
[Arm Architecture Reference Manual for A-profile architecture](https://developer.arm.com/documentation/ddi0487/ha/?lang=en) ARM官方可以下载文档  

## 四、软件工程及源码

### 1）M3 boot代码编写  

```asm
; 使用Keil自动生成时，也可用纯C写Boot相关的配置

                INCLUDE YOUR_CONFIG.INC ; #include "YOUR_NAME.INC" 包含头文件
; 用户自定义宏定义
YOUR_CONFIG1    EQU 0x01    ; 类似于#define宏定义，用不同的配置选项配置程序
YOUR_CONFIG2    EQU 0x01

                PRESERVE8   ; 指定当前文件要求堆栈八字节对齐
                THUMB       ; 使用THUMB指令集，不使用ARM指令集

; 定义堆，堆是malloc主动分配内存的位置
Heap_Size       EQU 0       ; Heap_Size是MDK指定的堆空间长度名称；不用malloc分配的堆没什么用，所以长度设置为0
                IF Heap_Size != 0       ; IF ELSE ENDIF和同名宏定义的含义类似
                AREA     HEAP, NOINIT, READWRITE, ALIGN=3 ; 申明开辟名为HEAP的内存，不写入值初始化，可读可写，2^3 8字节对齐
                EXPORT   __heap_base    ; MDK指定的名称，堆起始地址位置
                EXPORT   __heap_limit   ; MDK指定的名称，堆结束地址位置
__heap_base
Heap_Mem        SPACE    Heap_Size      ; 开始分配指定长度的内存
__heap_limit
                ENDIF

; 定义栈，栈是为全局变量自动分配空间的位置
Stack_Size      EQU      (4096)         ; Stack_Size是MDK指定的栈空间长度名称

                AREA     STACK, NOINIT, READWRITE, ALIGN=3 ; 申明开辟名为STACK的内存，不写入值初始化，可读可写，2^3 8字节对齐
                EXPORT   __stack_limit  ; MDK指定的名称，栈起始地址位置
                EXPORT   __initial_sp   ; MDK指定的名称，栈结束地址位置

__stack_limit
Stack_Mem       SPACE    Stack_Size     ; 开始分配指定长度的一片连续的内存
__initial_sp

                AREA     RESET, DATA, READONLY      ; 定义数据段，名字为RESET；上电后首先运行的函数地址
                EXPORT   __Vectors                  ; 输出ARM CMSIS中需要用到的一些标号，__Vectors函数在后续定义
                EXPORT   __Vectors_End
                EXPORT   __Vectors_Size
                EXPORT   Default_Interrupt_Handler  ; 中断入口
                IMPORT   __initial_sp

; 申明异常和中断入口
__Vectors       DCD      __initial_sp               ;     Top of Stack
                DCD      Reset_Handler              ;     Reset Handler
                DCD      NMI_Handler                ; -14 NMI Handler
                DCD      HardFault_Handler          ; -13 Hard Fault Handler
                DCD      MemManage_Handler          ; -12 MPU Fault Handler
                DCD      BusFault_Handler           ; -11 Bus Fault Handler
                DCD      UsageFault_Handler         ; -10 Usage Fault Handler
                DCD      0                          ;     Reserved
                DCD      0                          ;     Reserved
                DCD      0                          ;     Reserved
                DCD      0                          ;     Reserved
                DCD      SVC_Handler                ;  -5 SVCall Handler
                DCD      DebugMon_Handler           ;  -4 Debug Monitor Handler
                DCD      0                          ;     Reserved
                DCD      PendSV_Handler             ;  -2 PendSV Handler
                DCD      SysTick_Handler            ;  -1 SysTick Handler

                ; Interrupts
                DCD      Interrupt_Handler_0
                ; …… 省略其它中断 ……
                DCD      Interrupt_Handler_45       ; BCD：分配存储单元
__Vectors_End
__Vectors_Size  EQU      __Vectors_End - __Vectors

                ; 类似于宏定义函数
                MACRO                               ; 宏定义函数的开始 
                Set_Default_Handler $Handler_Name   ; 前一个时宏定义函数的名字，后面是要操作的对象
$Handler_Name   PROC                                ; 定义子程序
                EXPORT   $Handler_Name [WEAK]       ; 输出函数名；[WEAK]虚函数，可定义可不定义
                B        .                          ; 死循环
                ENDP                                ; 子程序定义结束
                MEND                                ; 宏定义函数结束

                AREA     |.text|, CODE, READONLY    ; 定义.text代码段，只读

                ; 申明默认的异常和中断处理函数
                Set_Default_Handler  Reset_Handler
                Set_Default_Handler  NMI_Handler
                Set_Default_Handler  HardFault_Handler
                Set_Default_Handler  MemManage_Handler
                Set_Default_Handler  BusFault_Handler
                Set_Default_Handler  UsageFault_Handler
                Set_Default_Handler  SVC_Handler
                Set_Default_Handler  DebugMon_Handler
                Set_Default_Handler  PendSV_Handler
                Set_Default_Handler  SysTick_Handler
                Set_Default_Handler  Default_Interrupt_Handler
                Set_Default_Handler  Interrupt_Handler_0
                ; …… 省略其它中断 ……
                Set_Default_Handler  Interrupt_Handler_45
 
               ; 各种程序
                ; Reset_Handler是板子上电后首先执行的位置，它由异常中断的跳转来实现
Reset_Handler   PROC                    ; 程序名 PROC 程序内容 ENDP 程序结束
                EXPORT   Reset_Handler
                IMPORT	 __hardwareInit ; 自己编写的C程序，在里面初始化各种硬件配置
                IMPORT   __main         ; main()函数入口
                BL       __hardwareInit ; 调用初始化硬件的汇编函数
                BL       __main         ; 跳转到main()函数
                BL       cpuStall       ; 程序退出后一直死循环
                ALIGN
                ENDP

cpuStall        PROC
                EXPORT   cpuStall
                B        .              ; 死循环
                ENDP
                END                     ; 通知编译器已经到了该源文件的结尾了

__hardwareInit  PROC
                EXPORT   __hwInitialize
                PUSH     {R0,R1,R2,R3,LR} ; 压栈
                ; 配置GPIO输出
                ; 配置PLL系统主频，将主频从晶振原有的频率提高到实际的工作频率
                ; 初始化串口
                ; 初始化其它外设
                POP      {R0,R1,R2,R3,LR} ; 弹栈
                BX       LR               ; 跳转到LR寄存器里的地址执行，也就是跳转回被调用的地方
                ALIGN
                ENDP

                ; 其它的.inc汇编头文件中要做的事
                ; 定义各个硬件模块的地址
                ; 定义所有中断号
```

* *参考网址：*
[linux 汇编 preserve8,为什么ARM汇编程序前要加PRESERVE8](https://blog.csdn.net/weixin_42347422/article/details/117007357)  
[ARM/THUMB汇编(补丁开发类)基础教程](https://blog.csdn.net/diligentcatrich/article/details/6317647)  
[Thumb 指令集](https://blog.csdn.net/ctwei85/article/details/5356014)  
[keil 环境下 AREA SUN ,NOINIT, READONLY , ALIGN=3 分别表示什么意思](https://zhidao.baidu.com/question/625299405366134324.html)  
[【ARM汇编】SPACE和DCD指令的区别](https://blog.csdn.net/inurlcn/article/details/20691233)  
[STM32 stack_size和heap_size大小设置的意义](https://blog.csdn.net/zxm8513/article/details/108349536)  
[堆(heap)和栈(stack)有什么区别??](https://www.cnblogs.com/williamjie/p/11164022.html)  
[arm汇编指令-DCD](https://blog.csdn.net/weiwei_xiaoyu/article/details/20564759)  
[ARM 汇编 伪指令 MACRO及MEND](https://blog.csdn.net/huangyekan/article/details/6999967)  
[灵活使用ARM汇编的WEAK关键字](https://blog.csdn.net/ytfgff/article/details/84195418)  
[ARM汇编中B .的实际意义](https://blog.csdn.net/xhaochongchong/article/details/106145273)  
[【cm-3】汇编中的句子:AREA |.text|, CODE, READONLY, ALIGN=2详解](https://blog.csdn.net/dddxxxx/article/details/47129883)  
[ARM汇编：汇编中proc、endp、ret、near、far指令用法](https://blog.csdn.net/weibo1230123/article/details/84235296)  
[ARM Thumb指令集完整列表](https://blog.csdn.net/ASMARM/article/details/33306587)  

### 2）正在进行中……
