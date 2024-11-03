# awtk-harmonyos-next

AWTK [鸿蒙系统 (harmonyos-next)](https://www.harmonyos.com/) 移植

![](docs/images/awtk_button.png)

## 安装开发环境

* [DevEco Studio](https://developer.harmonyos.com/cn/develop/deveco-studio) HarmonyOS 的开发工具。

* [Python](https://www.python.org/downloads/) 运行环境。

* [git](https://git-scm.com/downloads) 源码管理工具。

## 下载 awtk 和 awtk-harmonyos-next 源码

```
git clone https://github.com/zlgopen/awtk.git
git clone https://github.com/zlgopen/awtk-harmonyos-next.git
```

## 创建并编译 demoui 的工程

* 进入 awtk-harmonyos-next

```
cd awtk-harmonyos-next 
```

* 创建 DevEco Studio 工程

```
python create_project.py ../awtk/build.json
```

* DevEco Studio 打开 build/demoui

## 创建并编译自己的工程

创建并编译自己的工程需要先写一个配置文件，请参考 [平台编译配置](https://github.com/zlgopen/awtk/blob/master/docs/build_config.md)

后续过程和 demoui 完全一样。

## 注意

* 默认模拟器的 HDPI 为 640, 所以需要准备 x4 的图片。