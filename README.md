# WhxChat

一个基于 Qt 的跨平台即时通讯应用，采用分布式微服务架构。

## 技术栈

| 模块 | 技术选型 |
|------|----------|
| 客户端界面 | Qt 5.14+ |
| HTTP网关 | Boost.Beast |
| 异步服务器 | Boost.Asio |
| 验证服务 | Node.js |
| 服务间通信 | gRPC |
| 缓存 | Redis |
| 数据库 | MySQL |

#### 1. Qt客户端 (QtPanel)
- 使用 Qt 5.14+ 开发的跨平台客户端
- 实现用户注册、登录等基础功能
- 通过 HTTP 协议与网关服务通信

#### 2. 网关服务 (GateServer)
- 基于 Boost.Beast 实现的 HTTP 服务器
- 使用 Boost.Asio 处理异步网络请求
- 负责请求的转发和负载均衡
- 通过 gRPC 与验证服务通信

#### 3. 验证服务 (VerifyServer)
- 使用 Node.js 开发的验证服务
- 提供邮箱验证码、用户注册等功能
- 使用 Redis 缓存验证码
- 使用 MySQL 存储用户数据

## 开发环境要求

- cpp 20
- Qt 5.14+
- Boost 1.74+
- Node.js 12+
- CMake 3.24+
- gRPC
- Redis
- MySQL

## 如何使用

### 1. 克隆仓库
```bash
git clone https://github.com/yourusername/WhxChat.git
cd WhxChat
```

### 2. 配置环境

#### 建议手动配置，只是给一个简易的，先读一下这个脚本的声明
```bash
./scripts/ConfigAllEnvir.sh
```
#### 修改配置文件
1. 编辑 ServerPanel/VarifyServer/config.json 配置邮箱和数据库信息
2. 编辑 QtPanel/resources/config.ini 配置服务器地址

### 3. 编译
```bash
# 编译验证服务
./scripts/VarifyServerCompile.sh
# 编译网关服务
./scripts/GateServerCompile.sh
# 编译客户端
./scripts/ClientCompile.sh
```

### 4. 启动服务

按以下顺序启动各个服务：
```bash
# 启动验证服务
./scripts/VarifyStart.sh
# 启动网关服务
./scripts/GateServerStart.sh
# 启动客户端(可以多开)
./scripts/ClientStart.sh
```

## 项目结构
QtPanel
- Qt客户端程序

ServerPanel
- GateServer: HTTP网关服务
- VarifyServer: 验证服务

scripts
- 脚本工具集合


## 一些其他的备注

| 服务 | 端口 | 说明 |
|------|------|------|
| GateServer | 14789 | HTTP网关服务端口 |
| VerifyServer | 50051 | gRPC验证服务端口 |

#### 获取验证码
- 路径: `/get_varifycode`
- 方法: POST

### gRPC接口

`ServerPanel/GateServer/proto/message.proto`

### 清理构建
```bash
# 清理网关服务
./scripts/GateServerClean.sh
# 清理客户端
./scripts/ClientClean.sh
```

## 贡献指南

1. Fork 本仓库
2. 创建您的特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交您的更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开一个 Pull Request

## 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件

## 联系方式

- 作者：KBchulan
- 邮箱：18737519552@163.com
