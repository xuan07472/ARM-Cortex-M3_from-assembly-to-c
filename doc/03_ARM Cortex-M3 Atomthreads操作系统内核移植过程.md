### ARM Cortex-M3 Atomthreads操作系统内核移植过程

|作者|将狼才鲸|
|---|---|
|创建日期|2022-11-12

#### 一、RTOS 操作系统介绍

* 嵌入式常见的RTOS有好几个，很多都是线程管理、内存管理、驱动框架、文件系统框架等操作元素合在一起的，移植起来复杂一点，我需要更简单的演示；所以我这里选用Atomthreads，它纯粹就只有一个OS中的进程管理模块，总共也只有6个.c文件，内容简单，便于理解；可以熟悉移植线程管理模块需要修改哪些硬件相关的代码；它也可以移植到8位CPU上面。
  * 官方网址：[Atomthreads: Open Source RTOS](http://atomthreads.com/)

* 工程和源码在本文档同级目录\src\07_OS_kernel\下
* 其它几个操作系统移植时需要的配置操作：
  * uCOS系统比较简单，配置没有图形界面或者字符界面，就是宏定义文件。
  * FreeRTOS配置也没有图形界面或者字符界面，就是宏定义文件。
  * RT-Thread系统配置在Windows下有图形界面，在Linux有Linux内核同款的menuconfig字符配置界面，配完后会生成一个有宏定义的头文件。
  * eCos有自己的图形配置界面。
  * Linux使用menuconfig字符配置界面。
  * 自己写操作系统时，也可以用menuconfig模块作为你的配置界面。

#### 二、Atomthreads系统移植

##### 1）atomthreads下载
  * 下载地址：[ACleverDisguise / atomthreads](https://gitee.com/ACleverDisguise/atomthreads)，代码是几年前的，最新版本是v1.3，因为代码量本来就小，所以也没有频繁更新
  * 源码文件目录：

```shell
jim@DESKTOP-SVP3BEM MINGW64 /d/1_git/atomthreads (master)
$ tree
.
|-- README.md
|-- kernel	/* atomthreads内核需要的文件很少，只实现线程同步和互斥的功能 */
|   |-- Doxyfile
|   |-- README
|   |-- atom.h
|   |-- atomkernel.c
|   |-- atommutex.c
|   |-- atommutex.h
|   |-- atomport-template.h
|   |-- atomqueue.c
|   |-- atomqueue.h
|   |-- atomsem.c
|   |-- atomsem.h
|   |-- atomtimer.c
|   `-- atomtimer.h
|-- ports	/* 系统移植相关 */
|   |-- arm	/* ARMv5 and ARMv7核通用 */
|   |   `-- ......
|   |-- armv7a	/* ARMV7A核通用 */
|   |   `-- ......
|   |-- avr
|   |   `-- ......
|   |-- cortex-m
|   |   |-- Makefile
|   |   |-- README.md
|   |   |-- asm_offsets.h
|   |   |-- atomport-asm.S
|   |   |-- atomport-private.h
|   |   |-- atomport-tests.h
|   |   |-- atomport.c
|   |   |-- atomport.h
|   |   |-- boards
|   |   |   |-- ek-lm4f120xl	/* ARM EK-LM4F120XL 评估板 */
|   |   |   |   |-- Makefile.include
|   |   |   |   `-- board_setup.c
|   |   |   |-- nucleo-f072rb	/* stm32f072 */
|   |   |   |   |-- Makefile.include
|   |   |   |   `-- board_setup.c
|   |   |   |-- nucleo-f103rb	/* stm32f103 */
|   |   |   |   |-- Makefile.include
|   |   |   |   `-- board_setup.c
|   |   |   |-- nucleo-f411re	/* stm32f411 */
|   |   |   |   |-- Makefile.include
|   |   |   |   `-- board_setup.c
|   |   |   `-- qemu		/* QEMU模拟器 */
|   |   |       |-- Makefile.include
|   |   |       |-- README.md
|   |   |       |-- board_setup.c
|   |   |       `-- run_test.exp
|   |   |-- common
|   |   |   |-- helloworld.c
|   |   |   |-- lm4f_con.c
|   |   |   |-- stm32_con.c
|   |   |   `-- stubs.c
|   |   |-- libopencm3
|   |   |-- linker
|   |   |   |-- lm4f120xl.ld
|   |   |   |-- stm32f072rb.ld
|   |   |   |-- stm32f103rb.ld
|   |   |   `-- stm32f411re.ld
|   |   `-- tests-main.c
|   |-- mips
|   |   `-- ......
|   `-- stm8
|       `-- ......
`-- tests /* 系统功能测试用的文件 */
    |-- README
    |-- commit-tests-arm.sh
    |-- commit-tests-avr.sh
    |-- commit-tests-cortex-m.sh
    |-- kern1.c
    |-- ......
    `-- timer8.c
23 directories, 233 files
```

##### 2）ARM Cortex-M3移植过程

1. 删除源码中用不到的文件夹：
  * 删除atomthreads\tests文件夹，这是测试内核用的
  * 删除atomthreads\ports下的arm、armv7a、avr、mips、stm8文件夹，只保留cortex-m，还有保留arm文件夹下的README文件，这里描述的系统移植步骤比较明了
  * 删除atomthreads\ports\cortex-m\boards下的ek-lm4f120xl、nucleo-f072rb、nucleo-f411re、qemu文件夹，只保留nucleo-f103rb这个stm32f103芯片的文件夹

2. 查看kernel移植说明
  * 参考阅读atomthreads\ports\arm\README，这里说了内核移植要改哪些东西
  * 阅读atomthreads\ports\cortex-m\README.md文档的后半部分，这里说了M3移植要注意的东西
  * 内核代码中只需要包含ports中硬件相关的一个atomport.h文件即可
  * 整个系统开始运行的文件是atomthreads\ports\cortex-m\tests-main.c

3. 修改。。。

* *参考网址：*
  * 官方网址：[Atomthreads: Open Source RTOS](http://atomthreads.com/)
  * [stm8使用atomthreads项目](https://blog.csdn.net/yoeksome/article/details/51724778)
  * [AtomThreads RTOS移植到低RAM(4K)的芯片](https://www.pudn.com/news/6228e2ec9ddf223e1ad3271c.html)
  * [8位单片机轻松跑RTOS操作系统](https://blog.csdn.net/ybhuangfugui/article/details/120714748)
  * [STM8L最小系统板移植Atomthreads成功](https://blog.csdn.net/cpuwolf/article/details/13289351)