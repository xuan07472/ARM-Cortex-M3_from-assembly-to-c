# Keil模拟器介绍

|作者|将狼才鲸|
|---|---|
|创建日期|2022-11-12|

---

## 一、模拟器介绍

* Keil的模拟器可以模拟CPU和通用的外设，也可以在模拟器上直接跑操作系统；关于模拟器的介绍如下：
  * [Home  /  MDK  /  Simulation](https://www2.keil.com/mdk5/simulation/)
  * [µVision User's Guide - Simulation](https://developer.arm.com/documentation/101407/0537/Simulation)
  * [MDK-Middleware](https://www2.keil.com/mdk5/middleware)
  * [UVISION DEBUGGER: Simulation of Cortex-M Devices](https://developer.arm.com/documentation/ka002225/latest)

* Keil模拟器能够模拟文件系统、图形库和GUI、网络、USB
* 只仿真程序的逻辑运算、输入输出，可以在工程中只选择Cortex-M3核
* 要想仿真操作系统，则需要在工程中选中一款实际芯片，并指定模拟器库，如DARMSTM.DLL，-pSTM32F103T8；而M3核心的模拟器库是SARMCM3.DLL，-MPU。
* 模拟器支持的芯片完整列表如下：[UVISION DEBUGGER: Simulation of Cortex-M Devices](https://developer.arm.com/documentation/ka002225/latest)
* 只选择M3核心的话，一些头文件如core_m3.h里面很多和设备相关的定义，如中断，都是空的，需要芯片制造商添加，详见[CMSIS-Core Device Templates](https://arm-software.github.io/AVH/main/overview/html/index.html)
* Keil软件的界面中可以显示串口、GPIO、CAN、I2C、SPI的数据，详见软件安装文件夹的D:\Keil_v5\ARM\Hlp\uv4.chm文件的Simulation章节。
