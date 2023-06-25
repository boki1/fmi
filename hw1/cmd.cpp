#include <mystd/string.hpp>

#include "error.hpp"
#include "oopcoin.hpp"
#include "cmd.hpp"

namespace oopcoin {

    namespace cmdimpl {
        void create_user() {
            mystd::string name;
            std::cin >> name;
            // FIXME:
            // The predefined user structure does not allow for more.
            if (name.size() >= sizeof(user::name) - 1)
                name[name.size() - 1] = '\0';

            user u;
            std::strncpy(u.name, name.c_str(), sizeof(u.name));

            // Handles the rest
            user_manager::the().add(u);
        }

        void remove_user() {
            mystd::string name;
            std::cin >> name;

            user_manager::the().remove_by_name(name);
        }

        void send_coins() {}

        void verify_transactions() {}

        void wealthiest_users() {
            std::uint32_t n;
            std::cin >> n;

            int i = 0;
            for (const auto &user: user_manager::the().users()) {
                std::cout << user << '\n';
                // TODO: Get money of user and print it also:
                // std::cout << "\tCoins of user" << user.id << ": " << coins_of(user) << '\n';
                if (++i >= n)
                    break;
            }
        }

        void biggest_blocks() {}

        void quit() {
            throw successful_exit{};
        }
    }

    cmdparser::cmdparser(std::istream &input_stream)
            : m_entries{{.name = "create-user", .action = cmdimpl::create_user},
                        {.name = "remove-user", .action = cmdimpl::remove_user},
                        {.name = "send-coins", .action = cmdimpl::send_coins},
                        {.name = "verify-transactions", .action = cmdimpl::verify_transactions},
                        {.name = "wealthiest-users", .action = cmdimpl::wealthiest_users},
                        {.name = "biggest-blocks", .action = cmdimpl::biggest_blocks},
                        {.name = "exit", .action = cmdimpl::quit},
                        {.name = "dummy", .action = [](){}}
                        }
                        , m_input_stream{input_stream} {}

    const cmdparser::cmdentry &cmdparser::parse_next_cmd() {
        mystd::string cmd;
        std::cin >> cmd;

        // FIXME: HACK
        if (cmd.size() == 0) return m_entries[TOTAL_COUNT_CMDS - 1];

        const auto foundp = std::find_if(std::begin(m_entries), std::end(m_entries),
                                         [&cmd](const cmdentry &candidate) {
                                             return candidate.name == cmd;
                                         });

        if (foundp != std::end(m_entries))
            return *foundp;
        throw bad_cmd{"Entered bad command."};
    }
}