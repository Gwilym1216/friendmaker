# Friend Maker Quick Start

本文档记录当前本地 fork 的最短运行流程。默认环境是 Windows + VS Code + PlatformIO 插件环境。

## 0. 先确认当前目录

在 VS Code 终端进入项目根目录：

```powershell
cd F:\vscode_projects\tomadachi\friendmaker
```

如果要检查 Git 是否在你的 fork 上：

```powershell
git remote -v
git status --short --branch
```

正常情况下：

- `origin` 应该是 `https://github.com/Gwilym1216/friendmaker.git`
- `upstream` 应该是 `https://github.com/zhouxiyu1997/friendmaker.git`

## 1. 安装或检查前端依赖

如果项目根目录已经有 `node_modules`，通常不用重新安装。

如果没有，或启动时报缺少依赖，再执行：

```powershell
npm install
```

注意：

- 这一步安装的是 Node.js 项目的依赖，主要用于网页 UI、串口通信、图片处理。
- 依赖会放在当前项目的 `node_modules/` 里，不会装进 Anaconda base。
- 如果 Electron 原生依赖编译失败，优先安装 Visual Studio Build Tools 的 C++ 桌面开发组件，或先只使用 `npm run ui:dev` 的网页开发模式。

## 2. 启动 Friend Maker UI

在项目根目录运行：

```powershell
npm run ui:dev
```

看到服务启动后，用浏览器打开：

```text
http://127.0.0.1:4307
```

网页里主要按这几个页面走：

1. `刷入固件`
2. `手柄测试`
3. `调试测速`
4. `脚本生成`

## 3. 推荐路线：ESP32-WROOM 蓝牙手柄

这是当前最稳定、最接近原项目的路线。

### 3.1 硬件连接

```text
电脑 USB
  -> ESP32-WROOM 开发板 USB/UART 口

Switch
  -> 不需要接线，走蓝牙连接
```

你的 WROOM 板子当前常见串口是 `COM11`，如果换了 USB 口，COM 号可能变化。

### 3.2 烧录固件

优先使用 VS Code PlatformIO 自带环境，不要使用 Anaconda base。

```powershell
cd F:\vscode_projects\tomadachi\friendmaker\firmware\esp32
C:\Users\86181\.platformio\penv\Scripts\pio.exe run -e esp32dev_wireless -t upload --upload-port COM11
```

如果你的板子明确是 `NodeMCU-32S`，可改用：

```powershell
C:\Users\86181\.platformio\penv\Scripts\pio.exe run -e nodemcu_32s_wireless -t upload --upload-port COM11
```

### 3.3 连接 Switch

1. Switch 打开 `手柄 -> 更改握法/顺序` 页面。
2. 网页 UI 进入 `手柄测试`。
3. 选择串口 `COM11`。
4. 点击 `连接手柄`。
5. 等状态到 `已就绪`。
6. 测试 `L+R 配对`、`A`、方向键。

当前稳定版支持记住最近 Switch 主机地址。通常选好串口后会很快重连，不需要频繁点 `重置手柄蓝牙`。

`重置手柄蓝牙` 的用途是重新启动开发板上的蓝牙 HID 协议栈，用来处理卡死、长期广播、断联后无法恢复等异常。正常已经能连上时，不要反复点击它。

## 4. 备用路线：ESP32-S3 USB 有线手柄

这条路线适合你的 ESP32-S3-N16R8 板子，用原生 USB 模拟 Switch 有线手柄。

### 4.1 硬件连接

```text
电脑
  -> ESP32-S3 左侧 UART Micro-USB 口

Switch Lite USB-C
  -> USB-C OTG 转接头或 Hub
  -> USB-A to Micro-USB 数据线
  -> ESP32-S3 右侧 USB Micro-USB 口
```

注意：

- 必须使用能传数据的线，不要只用充电线。
- S3 右侧原生 USB 口接 Switch，左侧 UART 口接电脑。
- 不需要手动接 GPIO19/GPIO20。

### 4.2 烧录固件

假设 S3 的 UART 串口是 `COM10`：

```powershell
cd F:\vscode_projects\tomadachi\friendmaker\firmware\esp32
C:\Users\86181\.platformio\penv\Scripts\pio.exe run -e esp32s3_usb_switch -t upload --upload-port COM10
```

### 4.3 Switch 设置

Switch Lite 上：

1. 打开 `设置 -> 手柄与感应器`。
2. 打开 `Pro 手柄有线通信`。
3. 关闭自动休眠，避免绘图中断。
4. 用 OTG 把 S3 右侧 USB 口接到 Switch Lite。
5. 在网页 `手柄测试` 页确认状态显示 USB 已启动，然后测试按键。

## 5. 开始正式绘图

在 `手柄测试` 已经确认可用后：

1. 进入游戏的画图界面。
2. 回到网页 `脚本生成`。
3. 导入图片。
4. 选择模式、画笔大小、颜色策略。
5. 检查预览和预计步数。
6. 点击开始绘制。

如果中途暂停或失败，先不要乱动 Switch 画布。优先使用网页里的恢复功能继续。

## 6. 常用检查命令

检查 TypeScript：

```powershell
npm run check
```

运行桌面端测试：

```powershell
npm run test:desktop
```

检查 WROOM 固件能否编译：

```powershell
cd F:\vscode_projects\tomadachi\friendmaker\firmware\esp32
C:\Users\86181\.platformio\penv\Scripts\pio.exe run -e esp32dev_wireless
```

检查 S3 USB 固件能否编译：

```powershell
cd F:\vscode_projects\tomadachi\friendmaker\firmware\esp32
C:\Users\86181\.platformio\penv\Scripts\pio.exe run -e esp32s3_usb_switch
```

## 7. 出问题时的最短处理

- 看不到串口：换数据线、换 USB 口、安装 CP210x 或 CH340 驱动。
- 蓝牙一直广播：Switch 停在 `更改握法/顺序`，重新点 `连接手柄`；仍不行再点一次 `重置手柄蓝牙`。
- 已连接但按键不响应：回到 `手柄测试`，确认状态是 `已就绪`，不是单纯 `已连接`。
- 画图错位：暂停，检查是否碰了摇杆/屏幕；必要时从恢复点继续。
- Python/PlatformIO 包冲突：不要改 Anaconda base，优先使用 `C:\Users\86181\.platformio\penv\Scripts\pio.exe`。

