// deleteFile.cpp: 定义应用程序的入口点。
//

#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

using namespace boost::filesystem;
using namespace std;

int main(void)
{
    const path file_name = path("./test.txt");
    std::fstream file_stream(file_name.string(), std::ios_base::app);

    string s;
    while (cin >> s) {
        file_stream << s;
        file_stream.flush();
    }
     
    file_stream.close();
    return 0;
}
