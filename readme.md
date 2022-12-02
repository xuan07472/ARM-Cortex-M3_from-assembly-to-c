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

|工程名称|作用|
|---|---|
|01_Hello_world|使用Keil的模拟器在虚拟终端输出Hello world|
|02_Keil_boot_comments|Keil自带汇编boot的注释|
|03_Self_assembler_boot|自行实现汇编boot|
|04_Uart_loopback|串口收发回环，使用Keil的虚拟终端窗口|
|05_Assembler_func|汇编函数的编写|
|~~06_Hardware_arch_code~~|~~将硬件相关的代码与系统逻辑代码分离~~|
|~~07_OS_kernel~~|~~移植操作系统线程管理模块~~|
|08_OS_memory|移植操作系统内存管理模块，实现malloc、free|
|09_OS_filesystem|移植操作系统文件系统模块|
|...|...|

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
  * 不想使用模拟器，而是想直接使用一款硬件运行程序的。

### 2）M3介绍

* M3由ARM公司于2004年推出，至今仍是很多单片机芯片使用的内核，如STM32F10x系列。
* ST公司于2007年首次使用ARM公司的内核，产品是F1，随后凭借其简洁易用的软件开发包逐渐发展出著名的STM32系列，累计出货量近百亿颗。
* M3属于ARMv7架构，ARMv7是ARM11之后的版本。
* M3属于Cortex系列，该系列有三类：A、R和M，比如熟悉的Cortex-A9。
* M3内核仅33000门。
* M3不能使用ARM指令集，而是使用Thumb或Thumb-2指令集。
  * M3使用Cortex微控制器软件接口标准 (CMSIS)作为硬件抽象层。
  * M3最大支持512M代码，512M SRAM，1G外部RAM，详见芯片文档的“3.4 Processor memory model”，芯片文档下载地址见本文档下面第三章。
  * M3的R0~R15通用寄存器介绍详见芯片文档的“3.8 Processor core register summary”。
  * M3的系统控制寄存器介绍详见芯片文档的“4.1 System control registers”，共有36个，如定时器、中断控制、系统状态、内存模式、指令集设置。

* *参考资料：*  

[MCU缺货涨价后的国产化浪潮（三）：全球 MCU 市场高度集中，多因素共振加速国产替代](https://www.bilibili.com/read/cv13508527/) 文章里也列出了全球和国内的MCU厂商和所有嵌入式的行业。  
[ARM CORTEX-M3简介](https://blog.csdn.net/changyourmind/article/details/51814844)  
[ARM Cortex-M3](https://wenku.baidu.com/view/8fc3c75d312b3169a451a41d.html)
[ARM发布适于高性能、低成本应用的Cortex-M3处理器](https://tech.hqew.com/xinpin_1765411)  

### 3）开发环境

* 本仓库所有工程均在Keil下创建，并使用Keil的模拟器运行，不使用具体的硬件开发板，Keil版本为MDK537.exe。
* Keil使用模拟器的方法：Keil配置的Debug栏要选择Use Simulator，不选择默认的ULINK2/ME Cortex Debugger。
* 如果有**报错**[Error: L6218E: Undefined symbol Image$$ARM_LIB_STACK$$ZI$$Limit Not enough information to list image](https://blog.csdn.net/qq_41200467/article/details/124958685)，则按网址中的描述配置一下链接器.sct文件路径。
* Keil的模拟器介绍：
  * [Keil 模拟 STM32F103 示例体验](https://os.iot.10086.cn/oneos-doc-2.0/quick_start/keilsimulator.html)

## 二、ARM-MDK IDE集成开发环境下载  
* MDK-arm软件社区版官方介绍（无代码大小限制，不能用于商用，需要先获取社区版许可证，也就是在官网注册账号后再下载）：[MDK-社区版概述](https://www.keil.com/pr/article/1299.htm)  
* 获取社区版许可证：[Log in with your Arm or Mbed account](https://www.keil.arm.com/login/?next=%2Fmdk-community%2F)  
* 下载地址主界面：[MDK Community Edition](https://www.keil.arm.com/mdk-community/)  
* 下载地址举例：https://www.keil.com/fid/comahow53j1j1wriguw1y56me9lv1dgw3o3fd1/files/eval/mdk536.exe  
* MKD-arm评估版软件官方下载地址（也就是不注册账号就下载，有32K代码限制）：[mdk536.exe](https://www.keil.com/fid/6upwf5w1y9wj1wt4huw1djsme93o1dgwmicqd1/files/eval/mdk536.exe)  
* 安装的时候会自动下载各种芯片包。  
* MKD里面有硬件模拟器，可以直接运行和调试程序，你也可以编译完生成可执行程序后在QEMU软件里面仿真运行。  
* Keil创建M3工程的流程可以网上自行搜索，创建时可以添加ARM官方提供的各个模块的代码，可以节省开发时间。

## 三、M3指令集和寄存器介绍

### 1）M3文档在线阅读及下载

* 芯片（IP核）手册 [ARM Cortex-M3 Processor Technical Reference Manual](https://developer.arm.com/documentation/100165/0201/)  
* ARMv7-M指令集手册 [ARMv7-M Architecture Reference Manual](https://developer.arm.com/documentation/ddi0403/ee)  
* Cortex-M3外设接口手册[Cortex-M3 Devices Generic User Guide](https://developer.arm.com/documentation/dui0552/latest/)
* CMSIS-Core Device Templates，在Keil的安装目录：file:///D:/Keil_v5/ARM/Packs/ARM/CMSIS/5.9.0/CMSIS/Documentation/Core/html/templates_pg.html

* 最前面两个两个文档描述了M3内核的通用寄存器和指令集，其中：
  * 芯片手册的“Table 3-1 Cortex-M3 instruction set summary”有所有指令的汇总表格，在线阅读地址在[Processor instructions](https://developer.arm.com/documentation/100165/0201/Programmers-Model/Instruction-set-summary/Processor-instructions)。
  *  指令集汇总的本地**子文档**：[01_ARM Cortex-M3指令集汇总.md](./doc/01_ARM Cortex-M3指令集汇总.md)

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

## 四、Keil汇编伪指令介绍

* 详见本仓库中**子文档**：[02_Keil_ARM汇编伪指令.md](./doc/02_Keil_ARM汇编伪指令.md)  

## 五、软件工程及源码

### 1）01_Hello_world

* 创建一个M3工程，使用Keil模拟器运行，在Keil软件的调试终端输出Hello world。
* 创建工程的过程参考以下网址：
  * [应用相关 新建一个基于ARM M3的工程](https://bbs.21ic.com/forum.php?mod=viewthread&tid=2818416) 选择CMSIS中的Core和Device中的Startup即可。
  * 如果有**报错**[Error: L6218E: Undefined symbol Image$$ARM_LIB_STACK$$ZI$$Limit Not enough information to list image](https://blog.csdn.net/qq_41200467/article/details/124958685)，则按网址中的描述配置一下链接器.sct文件路径。
  * 配置中的Debug页面勾选使用模拟器，Keil配置的Debug栏要选择Use Simulator，不能选择默认的ULINK2/ME Cortex Debugger。
* 进行printf输出重映射，将输出映射到Keil的Debug(printf) Viewer窗口
  * [Keil Debug(printf) Viewer的使用](https://blog.csdn.net/mygod2008ok/article/details/105234076)
  * [keil去除未使用的参数，变量，返回值的警告](https://blog.csdn.net/mygod2008ok/article/details/105234076)
* 工程和源码在本文档同级目录\src\01_Hello_world\下
* 第一打开工程时，需要自己点击软件上面窗口中带小串口的图标，打开Debug(printf) Viewer窗口
* 注意！文件使用了UTF-8编码，需要在Keil中进行设置才能正常显示中文，否则会显示乱码：Edit-->Configuation-->Encoding-->Encoding in UTF-8 without signature；因为GB2312在Git中会显示乱码，并且在Linux中使用GB2312会很不方便，从Linux和Windows中来回转时一不留神会用错误的编码报错导致中文丢失。

### 2）02_Keil_boot_comments

* 给Keil自带的boot加上注释
* 工程和源码在本文档同级目录\src\02_Keil_boot_comments\下
  * Keil自带的boot代码的汇编底层在Keil自带的工具包中，看不到，只注释能看到的C代码部分
  * Keil自带的boot代码的类汇编文件是.svt
  * Keil自带的头文件也在自己的工具包中，不能更改
  * 这个工程看不到boot的完整流程，下个工程会演示从第一行汇编代码引导到main.c的过程
  * 注意：ARMCM3_ac6.sct文件的注释使用了GB2312编码，已经配置了UTF-8的Keil中直接打开会显示乱码，其它的文件都是UTF-8格式

### 3）03_Self_assembler_boot

* 不使用官方自带的boot，自己用汇编写boot并引导C语言main()，成功打印输出，并编写注释，看到系统启动过程中的每一步。
* 工程和源码在本文档同级目录\src\03_Self_assembler_boot\下
  * 创建工程时，只选择选择CMSIS中的Core，不选择Device中的Startup。
  * ARM汇编的格式有两种：用Keil MDK ARM格式的汇编和GNU格式的汇编。
  * ARM格式的汇编关键字多为大写，编译使用armasm编译器和armclang编译器都可以；GNU格式关键字多为小写，要使用armclang 集成编译器；ARM的编译工具套件当前版本为ARM Compiler Version 6。
  * 我这里使用Keil MDK ARM格式的汇编，编译器让Keil自动识别文件自动选择，当前使用的是armclang。
  * Keil MDK ARM格式的汇编伪指令详见**子文档**[《02_Keil_ARM汇编伪指令.md》](./doc/02_Keil_ARM汇编伪指令.md)
  
* *参考网址：*  
  * [【方辉专栏】ARM嵌入式编译器（十） GNU 汇编和 armasm 汇编代码介绍](http://www.emdoor.cn/News/view/id/1732.html)
  * [零基础学ARM：MDK和GNU伪指令区别](https://www.elecfans.com/d/1434345.html)
  * [基于ARM编译器版本5的工程迁移与适配到ARM编译器版本6.12 后续1 - 汇编代码处理问题](https://blog.csdn.net/jackailson/article/details/103753951)
  * [从 Arm Compiler 5 迁移到 Arm Compiler 6](https://blog.csdn.net/zhzht19861011/article/details/109803651)

* 实际工程中将汇编分成了好几个文件，下面列出的源码先将汇编部分都放在一起，便于观看，实际使用的文件在工程的源码中列出：

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

### 4）04_Uart_loopback

* 使用Keil模拟器和Debug (printf) Viewer窗口实现scanf输入，并进行串口收发回环。
  * 工程和源码在本文档同级目录\src\04_Uart_loopback\下
  * Keil配置的Debug栏要选择Use Simulator，不能选择默认的ULINK2/ME Cortex Debugger
  * 创建工程方法：在Keil上新建一个工程，勾选CMSIS中的CORE，勾选Device中的Startup，一定要勾选Compiler--IO下的STDEER、STDIN、STDOUT，并将右侧的Breakpoint都改为ITM；不用重定向fgetc和fputc，直接使用scanf和printf即可。
  * 如果有**报错**[Error: L6218E: Undefined symbol Image$$ARM_LIB_STACK$$ZI$$Limit Not enough information to list image](https://blog.csdn.net/qq_41200467/article/details/124958685)，则按网址中的描述配置一下链接器.sct文件路径。
  * 在国内和国外的网站上都没找到介绍使用fgetc输入重定义，用scanf从Keil Debug (printf) Viewer窗口获取数据的方法，最终从Keil官网找到了。
  * 这是有效的方法：[µVision User's Guide - Debug (printf) Viewer](https://developer.arm.com/documentation/101407/0537/Debugging/Debug-Windows-and-Dialogs/Debug--printf--Viewer)
  * 在网上还找到了使用ITM_CheckChar()和ITM_ReceiveChar()来实现fgetc，但需要需要添加core_cm3.h头文件和stm32f1xx.h头文件，而我直接使用的是M3核，并没有stm32的头文件，所以这个方法失败。
  * MDK的Debug (printf) Viewer窗口不像C51的UART #1窗口，UART #1在网上能很容易的找到教程，通过VSD虚拟串口软件，将Keil C51的调试串口和电脑的虚拟串口相绑定，这样就能使用SSCOM或者PUTTY等串口软件收发二进制数据了；Debug (printf) Viewer窗口我还没找到绑定的方法，所以当前scanf不能获取到16进制和int型的数据，只能获取到字符和字符串，但是这对使用模拟器仿真程序来说够用了。

### 5）05_Assembler_func

* 如何写汇编函数，汇编宏定义函数
* 工程和源码在本文档同级目录\src\05_Assembler_func\下

### 6）06_Hardware_arch_code

* ==》该工程内容未编写，工程里只有没有的文件…………《==

* 将硬件arch和系统逻辑代码分离
* 仿照Linux kernel的文件结构，将系统底层硬件相关的文件独立出来，让后将相关代码放在名为arch的文件夹中
* 实现通用的操作系统底层硬件相关的接口，如中断控制、时钟基准、大小端转换、系统退出、输入输出重映射、延时函数
* 工程和源码在本文档同级目录\src\06_Hardware_arch_code\下
* 本仓库中的源码以Linux kernel中对应的文件为参考
* Linux kernel源码与相关文件的介绍
  * 1. 获取kernel源码，下载或clone的地址：[Gitee 极速下载 / Linux Kernel](https://gitee.com/mirrors/linux_old1)，源码总大小5.3G，.git历史数据的大小有3.8G，源码有1.3G左右；
  * 2. Windows下下载后解压会报错，Git clone后checkout .检出文件也会报错，因为有三个文件名aux.c和aux.h和Windows预留文件名重合，Windows只允许自己使用：'drivers/gpu/drm/nouveau/nvkm/subdev/i2c/aux.c'， 'drivers/gpu/drm/nouveau/nvkm/subdev/i2c/aux.h'，'include/soc/arc/aux.h'，类似的文件名还有COM1到COM9，LPT1到LPT9；可以将kernel仓库在Windows上下载或者clone后拷贝到Linux系统如Ubuntu中，然后checkout或者解压，然后切换到发布版本版本，git checkout 然后将上述三个文件重命名或者删除，将.git隐藏文件删除，然后拷贝回Windows系统，便于查看代码。
  * 如果你只是查看kernel代码，则可以删除.git文件夹，arch/下除了arm的其它文件夹，arch/arm/下match-开头的文件夹只保留一个你熟悉的板子型号，如match-stm32，这样创建工程后查看代码跳转时更方便。

### 7）07_OS_kernel

* ==》该工程内容未实现，工程编译不过…………《==
  * Atomthreads M3硬件相关的代码原本是适配Linux下的，移植到Keil中比较麻烦，已放弃

* 嵌入式常见的RTOS有好几个，很多都是线程管理、内存管理、驱动框架、文件系统框架等操作元素合在一起的，移植起来复杂一点，我需要更简单的演示；所以我这里选用Atomthreads，它纯粹就只有一个OS中的进程管理模块，总共也只有6个.c文件，内容简单，便于理解；可以熟悉移植线程管理模块需要修改哪些硬件相关的代码；它也可以移植到8位CPU上面。
  * 官方网址：[Atomthreads: Open Source RTOS](http://atomthreads.com/)

* 工程和源码在本文档同级目录\src\07_OS_kernel\下
* 更多的移植流程详见**子文档**[《03_ARM Cortex-M3 Atomthreads操作系统内核移植过程.md》](./doc/03_ARM Cortex-M3 Atomthreads操作系统内核移植过程.md)
* Keil模拟器的介绍详见**子文档**[《04_Keil模拟器介绍.md》](./doc/04_Keil模拟器介绍.md)

* 其它几个操作系统移植时需要的配置操作：
  * uCOS系统比较简单，配置没有图形界面或者字符界面，就是宏定义文件。
  * FreeRTOS配置也没有图形界面或者字符界面，就是宏定义文件。
  * RT-Thread系统配置在Windows下有图形界面，在Linux有Linux内核同款的menuconfig字符配置界面，配完后会生成一个有宏定义的头文件。
  * 自己写操作系统时，也可以用menuconfig模块作为你的配置界面。
  * eCos有自己的图形配置界面。
  * Linux使用menuconfig字符配置界面。

[嵌入式操作系统-ucos的移植（上）](https://zhuanlan.zhihu.com/p/385057352)  
[RT-Thread 之 PWM 设备驱动详细配置过程（血泪经验）](https://blog.csdn.net/hanhui22/article/details/107717884)  
[RTThread Studio开发STM32基本工程配置](https://blog.51cto.com/u_15060513/4039456)  
[rtthread 4.0 shell的裁剪](https://club.rt-thread.org/ask/question/da39bbf94abde0fe.html)  
[使用eCos图形化配置工具管理eCos应用程序](https://blog.csdn.net/zoomdy/article/details/12908559)  
[uCOSII、eCos、FreeRTOS和djyos操作系统的特点及不足](https://www.elecfans.com/emb/20201001785961.html)  
[关于ucosII系统的软件系统裁剪性](https://www.cnblogs.com/bajiankeji/p/4966775.html)  
[FreeRTOS（1）---FreeRTOS 内核配置说明](https://blog.csdn.net/qq_27114397/article/details/82996622)  

* Keil选择设备时，不能再选择ARM中的Cortex-M3，而要选择一款实际的芯片了，因为这样才能使用中断和定时器的头文件定义；我这里选用STM32F103ZG
  * 首先点击软件上面绿色和黄色方框的按钮，需要先在Keil中的Pack Installer窗口中左侧选中STMicroelectronics--STM32F1 Series--STM32F103--STM32F103ZG，再在右侧安装STM32F1xx_DFP；如果软件下载速度很慢，可以自行去网上下载keil.STM32F1xx_DFP.2.4.0.pack；一个很快的下载地址是[Keil.STM32F1xx_DFP.2.3.0.pack (47.84MB)](https://download.armfly.com/armbbs/bbs/96992/Keil.STM32F1xx_DFP.2.3.0.pack)
  * [MDK5.29，5.30，5.31，5.32，5.33, 5.34，5.35, 5.36, 5.37和各种pack软件包镜像下载（2022-05-04）](https://blog.csdn.net/Simon223/article/details/105090189)
  * 右侧还自带了TencentOS，RT-Thread，FreeRTOS操作系统，可以开盒即用，很方便；因为我这里是演示操作系统移植的原理，所以不使用这些现成的。

* 在软件配置的Debug页面，选择模拟器Use Simulator，并且将Cortex-M3的模拟器库改成STM32F1xx的；将DCM.DLL和-pCM3改为DARMSTM.DLL和-pSTM32F103ZG

* *遇到的问题及解决办法*
  * [Keil环境配置及stm32程序的仿真调试](https://blog.csdn.net/qq_41675500/article/details/120517476)
  * [“error 65: access violation at 0x40021000 : no 'read' permission”错误的解决](https://blog.csdn.net/u012075442/article/details/78722593)
  * [【09】keil软件仿真STM32，电脑上虚拟串口收发，无需硬件](https://www.bilibili.com/read/cv18123376/)

---

* Keil创建STM32F103ZG工程
* 创建工程时选择CMSIS--CORE，Compiler--I/O--STDERR/STDIN/STDOUT--ITM，Device--Startup
* 在软件配置的Debug页面，选择模拟器Use Simulator，并且将Cortex-M3的模拟器库改成STM32F1xx的，步骤未：将DCM.DLL和-pCM3改为DARMSTM.DLL和-pSTM32F103ZG
* 防止仿真时出错：新建一个debug_map.ini文件，里面写上map 0x40000000,0x400FFFFF read write保存；在魔术棒的设置图标Options->Debug->Use_Simulator->Initialization_File，选中刚刚的文件。
* 工程里新建一个main.c，写上main()函数，里面写printf语句，编译，debug调试，点开Debug(printf) Viewer窗口中能看到你的输出语句。
* 加入Atomthreads kernel部分的源码，设置窗口--C/C++(A6)--Include Paths中加入该源码的文件夹路径，用于加载头文件。
* 自己写硬件相关的部分（也就是移植操作系统kernel），给kernel提供atomport.h头文件。
* 先熟悉整个kernel，看看源码中需要哪些底层支撑，顺便给kernel源码加上注释。
* 写个atomport.h文件，里面加上一些宏定义；写个atomport.c，里面实现3个空函数；先保证整个工程编译通过，然后再看具体要实现什么东西；具体弄清楚要实现的东西，也就能对内核需要哪些硬件支撑有了解了。
* 去掉keil自带的一些警告输出。
* 操作系统移植的工作：
  * 开启一个硬件定时器，在定时器的中断函数中调用atomIntEnter、atomIntExit、函数
  * 新建atomport.c/h文件，在里面实现archFirstThreadRestore、archContextSwitch、archThreadContextInit函数，实现读取中断状态寄存器、关中断、恢复中断；用汇编语言，使用CPU的R0~R15状态寄存器和其它寄存器进行任务切换。
  * 底层还能再实现一个task任务模块。

### 8）08_OS_memory

* ==》该工程内容未编写…………《==
* 工程和源码在本文档同级目录\src\08_OS_memory\下
* 内存操作实际上就是对Heap堆的操作。
* Keil也自带了堆操作的库，已经实现了malloc和free，直接在工程里勾选微库MicroLI即可，微库内部位置一个堆管理模块。
  * [Keil的堆设置](https://blog.csdn.net/fuyunyouziyi/article/details/51490787)
* 使用开源的dlmalloc可以实现操作系统中的内存管理模块，只有一个.c和一个.h就可实现。
  * 第一个下载地址是[mirrors_android_source / dlmalloc](https://gitee.com/mirrors_android_source/dlmalloc)，但是不用这里面的代码
  * 上面下载的版本里面有安卓加的少量修改，但是文件的注释里面有没修改的原始地址，是ftp的方式：ftp://gee.cs.oswego.edu/pub/misc/malloc.c 和ftp://gee.cs.oswego.edu/pub/misc/malloc-2.8.6.h ，如果你不会ftp下载，可以直接网页访问https://gee.cs.oswego.edu/pub/misc/ 复制里面的malloc-2.8.6.c和malloc-2.8.6.h，将其改名为dlmalloc.c和dlmalloc.h。
  * 在.c源码里加入dlmalloc_init和dlmalloc_sbrk函数，传入你给内存分配器分配的总内存。
  * 然后在.h头文件里加入MALLOC_ALIGNMENT、malloc_getpagesize等一系列配置宏定义。
  * 然后在程序中调用dlmalloc_init函数初始化后，你就可以使用malloc和free了

* *参考网址：*
* [dlmalloc（一）](https://blog.csdn.net/ycnian/article/details/12971863)

### 9）09_OS_filesystem

* ==》该工程内容未编写…………《==
* 工程和源码在本文档同级目录\src\09_OS_filesystem\下
* 使用FatFs开源嵌入式文件系统，里面只有7个源文件，支持exFAT和FAT32格式的U盘、SD卡等，支持Unicode中文和ANSI/OEM GB2312中文。
  * 源码中已适配uC/OS-II、FreeRTOS和Keil CMSIS-RTOS操作系统。
  * 官方下载地址[FatFs](http://elm-chan.org/fsw/ff/archives.html)，下载地址比较慢。
  * 添加头文件实现DWORD、QWORD、UINT、BYTE、WORD、WCHAR、TCHAR等数据类型定义。
  * 将ffconf.h中的配置宏定义和你自己系统中的宏定义结合统一起来。
  * 实现diskio.c中要调用的底层USB、SD卡等驱动的文件读写接口。 

* *参考网址：*
* [Fatfs（文件系统的移植）](https://blog.csdn.net/qq_45531642/article/details/121035429)
* [FatFs（通用FAT文件系统模块）下载与介绍](https://blog.csdn.net/qq_38318207/article/details/108566392)
