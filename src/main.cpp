#include "exception.hpp"
#include "enumFile.hpp"
#include "winutils.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv)
{
	try {
		set<string> files;
		files.insert("D:\\cmake");
		files.insert("D:\\git\\Git\\LICENSE.txt");
		freeFile::enumFile enumFile("./handle.exe", "output.txt");
		enumFile.gen_files_info(files);
		enumFile.freeFiles();
	}
	catch (base_exception& e) {
		std::cerr << boost::diagnostic_information(e) << std::endl;
	}
	return 0;
}