#pragma once

#include "exception.hpp"
#include "winutils.hpp"
#include <boost/filesystem.hpp>
#include <string>
#include <map>
#include <tuple>
#include <fstream>
#include <algorithm>
#include <set>

namespace freeFile {
using namespace std;
using namespace boost::filesystem;

struct enum_execption : virtual base_exception {};;

class enumFile {
public:
	enumFile(const string &exe, const string &tmpfile = string());
	void gen_files_info(set<string>& files);
	void print_files_info();
	void freeFiles();
	void clear_files_info();

private:
	void enumFile::gen_file_info(const string& file);
	bool isExeTail(string file_name);
	void get_files_pid();

private:
	string exefile_;
	string tmpfile_;
	vector<tuple<string, int, HANDLE, string>> files_info_; // 占用文件的程序名，程序的pid, 文件的句柄，文件名
	set<int> files_pid_;
};

enumFile::enumFile(const string& exe, const string& tmpfile)
{
	path exe_path(exe);
	if (!exists(exe_path)) {
		BOOST_THROW_EXCEPTION(enum_execption() 
			<< err_str("handle.exe file not exist: " + exe_path.string()));
	}
	exefile_ = exe;

	if (tmpfile.empty()) {
		path tmpfile_path = get_temp_path() / "freeFile.txt";
		tmpfile_ = tmpfile_path.string();
	} else {
		tmpfile_ = tmpfile;
	}
}

void enumFile::gen_file_info(const string& file)
{
	// 提取格式：handle.exe   pid: 22432  type: File       A0: E:\code\FreeFile\bin\output.txt
	waitExecuteToFile(exefile_, file, tmpfile_);
	ifstream input_file(tmpfile_);
	string info;
	string pro_name;
	int pid;
	HANDLE file_handle;
	string row_file_name;
	while (getline(input_file, info)) {
		stringstream ss(info);
		ss >> pro_name;
		if (!isExeTail(pro_name)) {
			continue; // 这个检查存在缺陷，最好遍历查看下有没有这个进程
		}
		string flag_str;
		while (ss >> flag_str) {
			if (flag_str == "pid:") {
				ss >> pid;
			}
			if (flag_str == "File") {
				ss >> file_handle;
				ss >> flag_str;
				ss >> row_file_name;
			}
		}
		files_info_.emplace_back(make_tuple(pro_name, pid, file_handle, row_file_name));
	}
}

void enumFile::gen_files_info(set<string>&files)
{
	for (auto file : files) {
		gen_file_info(file);
	}
}

void enumFile::print_files_info()
{
	for (int i = 0; i < files_info_.size(); i++) {
		for (auto& info : files_info_) {
			cout << get<0>(info) << "    pid: " << get<1>(info) << "    " << get<2>(info) << ": " << get<3>(info) << endl;
		}
	}
}

bool enumFile::isExeTail(string file_name)
{
	bool retval = true;
	string tail_format = ".exe";
	if (file_name.size() < tail_format.size()) {
		return false;
	}
	transform(file_name.begin(), file_name.end(), file_name.begin(), tolower);
	for (int loop = 3, i = file_name.size() - 1; loop >= 0; loop--, i--) {
		if (file_name[i] != tail_format[loop]) {
			retval = false;
			break;
		}
	}
	return retval;
}

void enumFile::get_files_pid()
{
	for (auto& info : files_info_) {
		files_pid_.insert(get<1>(info));
	}
}

void enumFile::freeFiles()
{
	get_files_pid();
	for (int pid : files_pid_) {
		close_process(pid);
	}
}

void enumFile::clear_files_info()
{
	files_info_.clear();
	files_pid_.clear();
}

}// freeFile namespace