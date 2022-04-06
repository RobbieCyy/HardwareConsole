# HardwareConsole

本软件旨在提供一个能够兼容所有离子阱量子计算实验硬件，并暴露统一的socket接口供上层软件使用的服务平台。



### 项目架构

- HardwareConsole： 直接支持socket通讯的硬件，同时作为统一的后台管理其它的硬件。
- CCDConsole： 支持Andor CCD的UI界面，并暴露socket接口供上层软件通讯。



### 开发环境

- QT版本 > 5.15



### 代码提交规范

- 请参考[这篇文档](https://www.conventionalcommits.org/zh-hans/v1.0.0/)




### Todo List

#### CCD

- [ ] 完善CCD界面设计，支持更多用户功能操作
  - [x] 开关shutter
  - [x] 设置曝光时间，调整增益
  - [ ] 增加非auto模式，能够调节image的lowCount和highCount
  - [ ] 等等，自己加
- [ ] 暴露socket接口供上层调用，从而能够进一步处理并判断离子状态
- [ ] 支持Kinetics模式以及外部触发，从而能够加入时序中
- [ ] 关于错误处理的相关事项

#### Hardware Console

- [ ] 支持socket功能，实现相关UI，并测试是否能得到toptica激光器的参数
- [ ] 编写python测试程序测试是否能通过python读取参数
- [ ] 展示一个锁频的功能demo（注意上层的软件不负责UI，但负责所有的逻辑，需要好好设计）

#### 波长计

- [ ] 实现UI界面，并暴露统一的socket接口，考虑WASM发布，这样可以支持多人同时查看UI