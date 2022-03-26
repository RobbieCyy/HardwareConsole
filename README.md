# HardwareConsole

本软件旨在提供一个能够兼容所有离子阱量子计算实验硬件，并暴露统一的websocket接口供上层软件使用的服务平台。

目前平台将首先兼容支持socket通讯的硬件。



### 开发环境

- QT版本 > 5.13



### 代码提交规范

- 请参考[这篇文档](https://www.conventionalcommits.org/zh-hans/v1.0.0/)



### Todo List

- [x] 调整Json文件结构，将具体设备和设备接口分开，并改写相应文件
- [ ] 支持socket功能，实现相关UI，并测试是否能得到toptica激光器的参数
- [ ] 支持websocket功能，并编写python测试程序测试是否能通过python读取参数
- [ ] 实现更多和激光器相关的操作
