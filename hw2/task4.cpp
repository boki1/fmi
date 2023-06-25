#include <iostream>

#include "mystd.h"

mystd::string decipher(mystd::string &str, int i = 0) {
    int curr_multiplier = 0;
    mystd::string new_str;
    for (; i < str.size; ++i) {
        if (mystd::is_alpha(str.ptr[i])) {
            new_str.append(str.ptr[i]);
            continue;
        }
        if (str.ptr[i] == ')')
            break;
        while (i + 1 < str.size && str.ptr[i + 1] != ')' && mystd::is_num(str.ptr[i])) {
            curr_multiplier *= 10;
            curr_multiplier += static_cast<int>(str.ptr[i++] - '0');
        }
        mystd::assert(str.ptr[i] == '(', "Parsing error: Incorrect input - mismatched ')'!");
        const mystd::string inner = decipher(str, ++i);
        for (int j = 0; j < curr_multiplier; ++j)
            new_str.append_str(inner);
        curr_multiplier = 0;
        i += inner.size;
    }

    return new_str;
}

int main() {
    for (int i = 0; i < 3; ++i) {
        mystd::string inp;
        std::cin >> inp;
        std::cout << decipher(inp) << '\n';
    }
    return 0;
}