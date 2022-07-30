#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>

// 不考虑路径或者参数可能存在中文的情况

std::string get_temp_path(void)
{
	std::string path;
	char buffer[100] = {0};
	char* tmpbuf = NULL;
	do {
		int n = GetTempPath(100, buffer);
		if (n > 100) {
			tmpbuf = (char*)malloc(n + 1);
			int n = GetTempPath(n+1, tmpbuf);
		}
	} while (0);
	
	if (tmpbuf == NULL) {
		path = std::string(buffer);
	} else {
		std::string(tmpbuf);
	}

	free(tmpbuf);
	return path;
}

int waitExecuteToFile(const std::string& executor, std::string arg, std::string outfile)
{
	// 等待executor arg 执行结束; 更好的写法是，将执行和等待分离；
	// 分离的写法，意味要传递PROCESS_INFORMATION。如果不想直接使用这个不熟悉的类型，可以把他们封装

	// 组合参数
	std::ostringstream oss;
	oss << executor << ' ' <<  arg;

	//创建文件，用于保存重定向输出
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	HANDLE outfile_handle = CreateFile(outfile.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_READ,
		&sa,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	STARTUPINFO startup_info;
	ZeroMemory(&startup_info, sizeof(startup_info));
	PROCESS_INFORMATION process_info; 
	ZeroMemory(&process_info, sizeof(process_info));
	//将文件句柄传递给子进程，作为子进程的标准输出，标准错误设置
	startup_info.cb = sizeof(STARTUPINFOA);
	startup_info.dwFlags |= STARTF_USESTDHANDLES;
	startup_info.hStdInput = NULL;
	startup_info.hStdError = outfile_handle;
	startup_info.hStdOutput = outfile_handle;

	// 执行程序
	std::cout << "execute cmd is: " << oss.str() << std::endl;
	int ret = CreateProcess(NULL, const_cast<LPSTR>(oss.str().c_str()), NULL, NULL, TRUE, NULL, NULL, NULL, &startup_info, &process_info);
	if (ret == 0) {
		// 获取错误消息
		return 0;
	}

	// 等待执行结束
	if (ret) {
		WaitForSingleObject(process_info.hProcess, INFINITE);
		CloseHandle(process_info.hProcess);
		CloseHandle(process_info.hThread);
	}
	return 1;
}

void close_process(int pid)
{
	// come from: https://perrohunter.com/how-to-kill-a-process-by-its-pid-on-windows/
	HANDLE pid_handle = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, TRUE, pid);
	TerminateProcess(pid_handle, 0);
}

void path_convert(std::string& path)
{
	// code come from: https://blog.csdn.net/qq_29567701/article/details/83446691
	std::string::size_type pos = 0;
	while ((pos = path.find('\\', pos)) != std::string::npos) {
		path.insert(pos, "\\");
		pos = pos + 2;
	}
}
