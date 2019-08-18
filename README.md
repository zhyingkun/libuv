# libuv

[![Build Status](https://travis-ci.com/zhyingkun/libuv.svg)](https://travis-ci.com/zhyingkun/libuv)

----

## 基本介绍

1. 本工程是libuv及其C扩展的构建工程，用于运行调试
2. 所有libuv源码来自libuv官方[https://libuv.org/]
3. 仅保留Mac/Linux/Win三个平台的源码，去除了其他平台特定代码（为了足够简单）
4. 增加了源码注释，支持Debug/Release编译模式
5. 整个工程PC端编译构建采用cmake来管理，支持跨平台（可以在树莓派上正常cmake+make）

----

## 如何编译

1. 在Mac上采用Xcode编译
```bash
cd libuv/
mkdir buildXcode && cd buildXcode
cmake -DCMAKE_INSTALL_PREFIX=./install -G "Xcode" ..
# cmake -DCMAKE_INSTALL_PREFIX=/usr/local/zyk -G "Xcode" ..
```
此时已经在buildXcode文件夹下生成了Xcode工程，直接打开并编译即可

2. 直接命令行编译（支持Mac和Linux）
```bash
cd libuv/
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=./install .. # default is Debug
# for Debug: cmake -DCMAKE_BUILD_TYPE=Debug ..
# for Release: cmake -DCMAKE_BUILD_TYPE=Release ..
make
# for more details: make VERBOSE=1
make install
```
make命令会自动编译好各个模块

3. 在Windows上使用Cygwin + Visual Studio 2017进行编译
```bash
cd libuv/
mkdir buildVS && cd buildVS
cmake -DCMAKE_INSTALL_PREFIX=./install -G "Visual Studio 15 2017 Win64" ..
```
此时已经在buildVS文件夹下生成了Visual Studio工程，双击打开并编译即可

----

## 文件夹说明

1. demo：libuv源码中相关组件的用法示例，以及其他应用构建
2. example：libuv官方源码中附带的用法示例，做了一些小修改
3. libuv：libuv官方源码，加了注释，编译出libuv动态链接库
