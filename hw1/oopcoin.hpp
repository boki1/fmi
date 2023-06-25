#ifndef OOPCOIN_OOPCOIN_H_INCLUDED_
#define OOPCOIN_OOPCOIN_H_INCLUDED_

#include <fstream>

#include "mystd/vector.hpp"
#include "mystd/string.hpp"

namespace oopcoin {

    namespace predefined {
        unsigned computeHash(const unsigned char *memory, int length);

        struct User {
            unsigned id;
            char name[128];
        };
        struct Transaction {
            unsigned sender;
            unsigned receiver;
            double coins;
            long long time;
        };
        struct TransactionBlock {
            unsigned id;
            unsigned prevBlockId;
            unsigned prevBlockHash;
            int validTransactions;
            Transaction transactions[16];
        };
    }

    // Conform with my naming convention :)
    using transaction_block = predefined::TransactionBlock;
    using transaction = predefined::Transaction;
    using user = predefined::User;
    // FIXME:
    // auto *const hash_mem = predefined::computeHash;

    bool wealthiest_user_comparator(const user &lhs, const user &rhs);

    auto *const default_user_comparator = wealthiest_user_comparator;

    struct user_description {
        enum class status : uint8_t {
            removed, active
        };

        user yser;
        status tombstone;
    };

    class user_serializer {
        static inline const char *file_name = "users.dat";

        user_description read_current();

    public:
        user_serializer()
                : m_file_stream{file_name, std::ios::in | std::ios::out | std::ios::binary} {
            if (!m_file_stream.is_open()) {
                { std::ofstream tmp{file_name, std::ios::binary}; }
                m_file_stream.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            }
        }

        ~user_serializer() {
            // FIXME: Use RAII
            m_file_stream.sync();
            m_file_stream.close();
        }

        static user description_to_obj(const user_description &desc);

        static user_description obj_to_description(const user &user);

        user_description read(unsigned id);

        void append(user_description desc);

        void set_status(unsigned id, user_description::status status);

        mystd::vector<user_description> read_all();

        void append_all(const mystd::vector<user_description> &);

    private:
        std::fstream m_file_stream;
    };

    class user_manager {
        inline static std::uint64_t id_next = 0;

        user_manager() = default;

    public:
        ~user_manager() {
            sync();
        }

        [[nodiscard]] static user_manager &the() {
            static user_manager instance;
            return instance;
        }

        // Writes users as stored in mem.
        void sync();

        // Reads users as stored in file.
        void refresh();

        void add(user &new_user);

        void remove_by_name(const mystd::string &name);

        [[nodiscard]] const mystd::vector<user> &users() const { return m_users; }

        user_serializer m_storage;
    private:
        mystd::vector<user> m_users;
        bool m_users_dirty{false};
    };

} // namespace oopcoin

std::ostream &operator<<(std::ostream &os, const oopcoin::user &user);

#endif // OOPCOIN_OOPCOIN_H_INCLUDED_