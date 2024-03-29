## 工具介绍
windows下，杀死占用文件的进程。一些更详细的文档，可以参考[doc](doc)目录。

## 编译前的准备工作
* 代码依赖[handle.exe](https://docs.microsoft.com/en-us/sysinternals/downloads/handle)来查找占用文件进程的pid。本仓库采用MIT协议。由于协议原因，需要用户手动下载解压handle.exe到bin目录。
* 代码依赖[Boost Library](https://www.boost.org/)。

## 工具使用

编译完成后，进入`bin`目录。根据需要，在配置文件中写入被进程占用的文件，运行`freeFile.exe`。

## 进度

- [x] 基本功能：查找占用文件进程的pid；通过pid杀死进程；
- [x] 通过配置文件读取被占用文件路径。
- [x] 日志记录
- [ ] 不实现：整体的优化。
    * 查找的过程遍历。目前每次查找一个文件遍历一次。应当查找所有的文件只需要遍历一次。handle.exe的源码没有公开，不好借鉴。
    * 在完善命令行代码之后，添加图形化界面，可以搜索一个目录，列出被占用的文件，与占用文件进程的信息，并可选的关闭这些进程。
    * 在关闭占用文件进程的基础上，很容易拓展强制删除功能。先杀死占用文件的进程，再删除文件。