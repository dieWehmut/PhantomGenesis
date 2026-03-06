# PhantomGenesis

## 目录

- [PhantomGenesis](#phantomgenesis)
  - [目录](#目录)
  - [项目简介](#项目简介)
  - [技术栈](#技术栈)
  - [构建与运行](#构建与运行)
    - [环境要求](#环境要求)
    - [编译步骤](#编译步骤)
  - [项目结构](#项目结构)
  - [玩法说明](#玩法说明)
    - [基础操作](#基础操作)
    - [地图与场景机制](#地图与场景机制)
    - [角色与单位](#角色与单位)
      - [基础类](#基础类)
      - [玩家](#玩家)
      - [敌人与衍生单位](#敌人与衍生单位)
      - [攻击单位](#攻击单位)
  - [系统设计](#系统设计)
    - [界面与交互](#界面与交互)
    - [存档与读档](#存档与读档)
    - [掉落与强化](#掉落与强化)
    - [阶段推进](#阶段推进)
  - [开发过程](#开发过程)
  - [代码特点](#代码特点)
  - [结局与提示](#结局与提示)

## 项目简介

PhantomGenesis 是一个基于 C++ 与 Qt 6 开发的动作冒险游戏。整体玩法从类幸存者游戏出发，结合地图探索、敌人战斗、掉落强化、情报收集与多结局设计，玩家将扮演幽灵在地图中生存并推进剧情。

项目当前以单张主地图为核心，围绕可活动单位、波类攻击、传送机制、视野变化和存档系统构建主要游戏流程。

## 技术栈

- 编程语言：C++
- UI 框架：Qt Widgets
- 音频模块：Qt Multimedia
- 核心模块：Qt Core
- 构建系统：CMake
- 当前工程名：PhantomGenesis

## 构建与运行

### 环境要求

- CMake 3.19 或更高版本
- Qt 6.5 或更高版本
- 支持 C++ 的编译器
	- Windows 下可使用 MSVC
	- 也可使用支持 Qt 6 的 MinGW 或 Clang 工具链

### 编译步骤

1. 克隆仓库

```bash
git clone <your-repo-url>
cd mygame0
```

2. 配置工程

```bash
cmake -S . -B build
```

3. 编译项目

```bash
cmake --build build
```

4. 运行程序

- 若使用 Qt Creator，可直接打开 CMakeLists.txt 并选择合适的 Kit 构建运行。
- 若使用命令行构建，生成的可执行文件位于 build 目录对应的构建输出中。

## 项目结构

源代码位于 `src/` 子目录，结构与功能如下：

```text
mygame0/
├── .gitignore
├── README.md
├── SPOILER.md
└── src/
    ├── ActiveObject.cpp
    ├── ActiveObject.h
    ├── CMakeLists.txt
    ├── CMakeLists.txt.user
    ├── DropItem.cpp
    ├── DropItem.h
    ├── FlamePhantom.cpp
    ├── FlamePhantom.h
    ├── FlameWave.cpp
    ├── FlameWave.h
    ├── GameManager.cpp
    ├── GameManager.h
    ├── King.cpp
    ├── King.h
    ├── KingWave.cpp
    ├── KingWave.h
    ├── LurkPhantom.cpp
    ├── LurkPhantom.h
    ├── LurkWave.cpp
    ├── LurkWave.h
    ├── main.cpp               # 程序入口
    ├── mainwindow.cpp
    ├── mainwindow.h
    ├── mainwindow.ui          # Qt 界面定义
    ├── Map.cpp
    ├── Map.h
    ├── PhantomBase.cpp
    ├── PhantomBase.h
    ├── Player.cpp
    ├── Player.h
    ├── PlayerWave.cpp
    ├── PlayerWave.h
    ├── resources.qrc          # Qt 资源列表
    ├── VisionMaskItem.cpp
    ├── VisionMaskItem.h
    ├── Wave.cpp
    ├── Wave.h
    └── Resource/              # 资源文件（图片、音频等）
```

## 玩法说明

### 基础操作

- W / A / S / D：控制玩家移动
- Space：释放波类攻击
- 攻击方向：玩家最后一次移动的方向

游戏支持存档与读档，且允许在外部修改部分存档内容后重新读入。需要注意，若数值被改得过大，例如速度异常升高，程序可能出现非预期行为甚至崩溃。

### 地图与场景机制

- 当前主要实现了一张地图，规模约为 21 × 25
- 单格尺寸为 1024 × 1024
- 玩家可视区域边界会随流程实时拓展
- 地图内包含多种功能格
	- xxPhantomBase：对应敌人生成点
	- portalStart / portalEnd：传送门起点与终点
	- mazeExit：迷宫出口

地图、活动对象与整体流程由 GameManager 统一管理。

### 角色与单位

#### 基础类

- ActiveObject：活动单位基类，包含速度、范围指示器、碰撞等通用能力
- Wave：所有波类攻击的基类，包含飞行方向、射程等属性

#### 玩家

- Player：继承自 ActiveObject
- 具备多种状态
- 包含传送次数、护盾等机制

#### 敌人与衍生单位

- PhantomBase：敌人基类，前期随机游走，后期会锁定玩家
- FlamePhantom：近战击退玩家，并附带范围减速效果
- LurkPhantom：拥有护盾，并具备闪现能力
- King：由多个 PhantomBase 聚集生成，可对非己方单位造成影响

#### 攻击单位

- PlayerWave：玩家释放的波，默认 3 发，可强化，可具备击退、穿透与群伤能力
- FlameWave：火焰敌人的波，默认 1 发，可使玩家燃烧 3 秒
- LurkWave：潜伏敌人的波，默认 5 发
- KingWave：王类敌人的波，八方向释放，仅击退玩家

## 系统设计

### 界面与交互

主窗口通过多页面切换承载不同界面，当前包含开始界面、游戏界面、存档界面、情报界面等内容。设置界面在原始设计中预留，但暂未完整实现。

存档界面提供多个存档槽位，支持覆盖保存。游戏中的碰撞、传送、拾取强化等关键行为也配有音效反馈。

### 存档与读档

- 包含 1 个上次游戏文件
- 包含 8 个可覆盖的存档槽位
- 存档内容可序列化保存倒计时秒数、DropItem、全部 ActiveObject 等状态

这部分设计使得游戏状态可以较完整地恢复，也为调试与外部修改提供了空间。

### 掉落与强化

敌人掉落的 DropItem 会为玩家提供不同强化。

- flamePhantom 的掉落可按既定顺序强化攻击、视野范围、范围伤害半径、攻击距离等属性
- lurkPhantom 的掉落提供另一套强化收益
- 拾取强化后，玩家可获得例如视野扩大等效果

此外，FlamePhantom 的范围减速、玩家波类的伤害范围增强等内容也构成了游戏中后期的重要成长体验。

### 阶段推进

游戏流程中存在两次倒计时。

- 第一次倒计时结束后，部分 PhantomBase 会得到强化
- 第二阶段推进后，战场压力显著增加
- 不同阶段的存活、探索与路线选择会影响最终结局

## 开发过程

项目开发大致经历了以下阶段：

1. 第一阶段：构思玩法，创建地图和可活动单位，完成基本界面。
2. 第二阶段：完善各类单位功能，补充多个界面与按钮，并加入背景音乐。
3. 第三阶段：加入情报系统，实现存档与读档，完成主要设计与数值调整。
4. 第四阶段：进行最终整合、打包与可移植性测试。

## 代码特点

- 使用面向对象方式组织核心游戏逻辑
- 头文件与实现文件数量较多，职责划分明确
- 变量名主要采用小驼峰命名
- 类名与文件名主要采用大驼峰命名
- 地图、角色、波类、掉落物与管理器均采用独立类建模

## 结局与提示

游戏包含多结局设计，部分情报会随着结局达成数量逐步开放。真结局需要结合情报信息、地图结构和阶段机制进行探索。

更多提示与剧透内容请查看 [SPOILER.md](SPOILER.md)。
