#include "exception.hpp"
#include "enumFile.hpp"
#include "winutils.hpp"
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
		set<string> files;
		try {
			tlbs = toml::parse_file(R"(.\config.toml)");
			handle = tlbs["handle"].value<std::string>();
			tmpfile = tlbs["tmpfile"].value<std::string>();
			const toml::array &freeFiles = *tlbs.get_as<toml::array>("freeFiles");
			for (int i = 0; i < freeFiles.size(); i++) {
				files.insert(freeFiles[i].value<std::string>().value());
			}
		} catch (const toml::parse_error& err) {
			std::cerr << "Parsing failed:\n" << err << "\n";
			return 1;
		}

		// kill占用文件进程
		freeFile::enumFile enumFile(handle.value(), tmpfile.value());
		enumFile.gen_files_info(files);
		enumFile.freeFiles();
	}
	catch (base_exception& e) {
		std::cerr << boost::diagnostic_information(e) << std::endl;
	}
	return 0;
}