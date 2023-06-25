#ifndef OOPCOIN_CMDPARSER_H_INCLUDED_
#define OOPCOIN_CMDPARSER_H_INCLUDED_

#include <iostream>
#include <functional>
#include <string_view>

namespace oopcoin {

    /// In this namespace reside the implementations of all supported commands.
    namespace cmdimpl {
        using cmd_type = void(*)();

        void create_user();
        void remove_user();
        void send_coins();
        void verify_transactions();
        void wealthiest_users();
        void biggest_blocks();
        void quit(); // Do not conflict with std::exit().

        // FIXME:
        // static_assert(std::is_same_v<decltype(create_user), std::decay<cmd_type>()>);
    }

    class cmdparser {
    public:
        // FIXME: What if the reference goes out of scope? BAAD. I want to copy?
        explicit cmdparser(std::istream &input_stream);

        using action_type = cmdimpl::cmd_type;
        struct cmdentry {
            const char *name;
            action_type action;
        };

        const cmdparser::cmdentry &parse_next_cmd();

        void perform(cmdentry &);

    private:
        static const std::size_t TOTAL_COUNT_CMDS = 8;

        // TODO: Replace with std::array<>
        const cmdentry m_entries[TOTAL_COUNT_CMDS];
        std::istream &m_input_stream;
    };

}

#endif // OOPCOIN_CMDPARSER_H_INCLUDED_