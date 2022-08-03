#include "exception.hpp"
#include "enumFile.hpp"
#include "winutils.hpp"
#include "log.hpp"
#include <toml++/toml.h>
#include <iostream>
#include <string>
#include <optional>

using namespace std;

int main(int argc, char* argv[])
{
	try {
		// 从配置文件中提取信息
		toml::table tlbs;
		optional<string> handle, tmpfile; // optional异常未捕获
		set<string> free_files;
		string log_file_path, logger_name, log_level;
		size_t max_log_file_size, max_log_files;

		try {
			tlbs = toml::parse_file(R"(.\config.toml)");
			handle = tlbs["handle"].value<string>();
			tmpfile = tlbs["tmpfile"].value<string>();
			const toml::array &freeFiles = *tlbs.get_as<toml::array>("freeFiles");
			for (int i = 0; i < freeFiles.size(); i++) {
				free_files.insert(freeFiles[i].value<string>().value());
			}
			log_file_path = tlbs["log"]["path"].value<string>().value();
			log_level = tlbs["log"]["level"].value<string>().value();
			max_log_file_size = tlbs["log"]["max_file_size"].value<int>().value();
			max_log_files = tlbs["log"]["max_files"].value<int>().value();
		} catch (const toml::parse_error& err) {
			cerr << "Parsing failed:\n" << err << "\n";
			return 1;
		}

		// 初始化日志配置
		Log::SPDLOG::getInstance().init(log_file_path, "default_logger", log_level, max_log_file_size * 1024 * 1024, max_log_files, false);
		Log::LOG_DEBUG("log has inited.");

		// kill占用文件进程
		freeFile::enumFile enumFile(handle.value(), tmpfile.value());
		enumFile.gen_files_info(free_files);
		enumFile.freeFiles();
	}
	catch (base_exception& e) {
		cerr << boost::diagnostic_information(e) << endl;
	}
	Log::LOG_DEBUG("freeFile finish");
	return 0;
}