# Erdtree

基于C++开发的daemon框架。名字来源于由From Software开发的Elden Ring。

## ✨ **核心特性**

- **可选 daemon 模式**，无侵入接入已有可执行程序
- **Fork + Exec 启动目标程序**
- **进程异常退出自动重启**
- **支持配置文件灵活定制 (YAML)**
- **支持 POSIX 共享内存，进程间通信简单易用**
- **提供 Hook 生命周期钩子 (prefork, postfork, whenready)**
- **信号管理 (默认 SIGCHLD 监听，daemon 退出时一并清理子进程)**
- **无日志依赖，预留日志接口，适配任意日志方案**
- **轻量纯 C++20 实现，跨平台 (Linux/macOS)**
- **Conan 包管理 + CMake 构建，易集成**

## how to build

~~~
conan build . --build=missing
~~~

## 配置文件说明

`config.yaml` 示例：

```yaml
target_path: "/path/to/your_app"
target_args: ["--option1", "value1"]
shm_key: "/erdtree_shm"
shm_size: 4096
max_restarts: 3
hooks:
  prefork: "/path/to/prefork_script.sh"
  postfork: "/path/to/postfork_script.sh"
  whenready: "/path/to/ready_script.sh"
```

| 参数        | 说明 |
|------------|---------------------------------------------|
| `target_path` | 需要被守护启动的可执行文件路径 |
| `target_args` | 启动参数 |
| `shm_key` | 共享内存 key，需以 `/` 开头 |
| `shm_size` | 共享内存大小，至少为页面大小 (通常 4096) |
| `max_restarts` | 最大自动重启次数 |
| `hooks` | 各生命周期钩子 (可选) |

## 🧩 模块

模块 | 功能
---|---
`DaemonController` | 总控，协调各子模块
`ProcessManager` | 管理子进程生命周期，自动重启
`SignalManager` | 信号捕获处理
`SharedMemoryManager` | 简单 POSIX 共享内存封装
`HookManager` | 生命周期 Hook 触发
`ConfigManager` | YAML 配置加载

## 🐳 Docker 集成建议

Daemon 可作为 **独立可执行文件** 运行在容器中，也可直接通过 **Docker Entrypoint** 选择是否启用：

```dockerfile
COPY ./build/ErdtreeDaemon /usr/local/bin/erdtree
CMD ["erdtree", "--config", "/app/config.yaml"]
```