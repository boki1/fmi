#include <iostream>

#include "oopcoin.hpp"
#include "cmd.hpp"
#include "error.hpp"

#include "mystd/exception.hpp"

#ifdef DEBUG
#define debug_log(msg) std::cout << "DEBUG: " << msg
#else
#define debug_log(msg)
#endif

//#define HACK_users_dat_poc
//oopcoin::user_description desc{.yser = oopcoin::user{.id = 4, .name = "asdf"}, .tombstone = oopcoin::user_description::status::active};
//{
//std::fstream file_stream{"users.dat", std::ios::in | std::ios::out | std::ios::binary};
//file_stream.write(reinterpret_cast<const char *>(&desc), sizeof(oopcoin::user_description));
//}
//
//{
//oopcoin::user_description load;
//std::fstream file_stream{"users.dat", std::ios::in | std::ios::out | std::ios::binary};
//file_stream.read(reinterpret_cast<char *>(&load), sizeof(oopcoin::user_description));
//std::cout << load.yser;
//}
//return 1;

int main() {
    using namespace oopcoin;
    cmdparser cmdp{std::cin};
    user_manager::the().refresh(); // Try loading from users.dat

    oopcoin::user_description desc{.yser = oopcoin::user{.id = 4, .name = "asdf"}, .tombstone = oopcoin::user_description::status::active};
    user_manager::the().m_storage.append(desc);
//    return -1;

    while (true) {
        try {
            std::cout << "> ";
            const auto &cmd_entry = cmdp.parse_next_cmd();
            cmd_entry.action();
        } catch (successful_exit &) {
            std::cout << "Bye-bye :)\n";
            break;
        } catch (mystd::explained_exception &explained_exception) {
            std::cerr << "Error occurred: " << explained_exception.what() << '\n';
        }
    }
    return 0;
}
