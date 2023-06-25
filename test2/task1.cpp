#include <iostream>
#include <cstring>

#include "mystd.h"

int main() {
    mystd::string many_files;
    std::cin >> many_files;
    mystd::string other_file;
    std::cin >> other_file;

    if (many_files.check_substr(other_file))
        other_file.append_str(mystd::string{"_2"}, false);
    many_files.append_str(other_file);

    std::cout << many_files;

    return 0;
}