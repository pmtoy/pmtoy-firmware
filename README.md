# pmtoy-firmware
主控 STM32F070F6 固件， 编译工具 cmake,make,arm-gcc 

# 现有功能

+ USB 转虚拟串口, 电脑 USB 直接连接到传感器的串口
+ 按钮切换 LED 开关。

# TODO

+ 驱动 TFT 显示屏，显示空气质量
+ USB HID 功能与电脑通讯

# 编译方式(Linux)

+ 下载 [STM32CubeF0](http://www.st.com/en/embedded-software/stm32cubef0.html) 固件，放到平级目录
+ 下载 [stlink](https://github.com/texane/stlink) 烧录固件工具
+ 下载 [ARM GNU Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) 进行固件编译
+ 连接 st-link v2 到电脑，并连接小板
+ 切换到代码目录执行
+ `cmake -DBUILD_USB_VCP=ON .`
+ `make`
+ `make erase`
+ `make flash`
+ 固件烧录完成
