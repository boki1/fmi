#include <algorithm>

#include "oopcoin.hpp"
#include "error.hpp"

std::ostream &operator<<(std::ostream &os, const oopcoin::user &user) {
    os << "user { id: " << user.id << ", name: " << user.name << " };";
    return os;
}

bool oopcoin::wealthiest_user_comparator(const oopcoin::user &lhs, const oopcoin::user &rhs) {
    // FIXME: Implement coins_of().
    // return coins_of(lhs) > coins_of(rhs);
    return true;
}

// Adds a user. Keeps them in order, sorted by amount of money
// in order to speed up eventual access to the full list.
void oopcoin::user_manager::add(user &new_user) {
    new_user.id = id_next++;

    // NB: In the task description it is not explicitly mentioned what to do when
    //     duplicate names are entered. Since we control the users that exist using
    //     the name and not the actual unique identifier that is assigned on creation
    //     I am going to refuse users with the same name.

    const auto *userp = std::find_if(std::begin(m_users), std::end(m_users), [&new_user](const user &other) {
        return mystd::string{new_user.name} == other.name; // Brutal memory consumption...
    });

    if (userp != std::end(m_users))
        throw duplicate_user{};

    m_users.push_back(new_user);
    m_users_dirty = true;

    std::sort(std::begin(m_users), std::end(m_users), default_user_comparator);
}

void oopcoin::user_manager::refresh() {
    mystd::vector<user_description> descs = m_storage.read_all();
    mystd::vector<user> new_users;
    for (const auto &desc: descs)
        new_users.push_back(user_serializer::description_to_obj(desc));
    m_users = new_users;
    m_users_dirty = false;
}

void oopcoin::user_manager::sync() {
    mystd::vector<user_description> descs;
    for (const auto &user: m_users)
        descs.push_back(user_serializer::obj_to_description(user));
    m_storage.append_all(descs);
}

void oopcoin::user_manager::remove_by_name(const mystd::string &name) {
    const auto *userp = std::find_if(std::begin(m_users), std::end(m_users),
                                     [&name](const auto &user) {
                                         return user.name == name;
                                     });
    if (userp == std::end(m_users))
        throw unknown_user{};

    m_storage.set_status(userp->id, user_description::status::removed);
    m_users.erase(userp);
    m_users_dirty = true;
}

namespace predefined {
    unsigned computeHash(const unsigned char *memory, int length) {
        unsigned hash = 0xbeaf;

        for (int c = 0; c < length; c++) {
            hash += memory[c];
            hash += hash << 10;
            hash ^= hash >> 6;
        }

        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;
        return hash;
    }
}

void oopcoin::user_serializer::set_status(unsigned int id, oopcoin::user_description::status status) {
    user_description desc;

    while (m_file_stream && m_file_stream.read(reinterpret_cast<char *>(&desc), sizeof(desc))) {
        if (desc.yser.id != id) {
            continue;
        }

        m_file_stream.seekp(m_file_stream.tellg() - std::streamoff{sizeof(desc.tombstone)}, std::ios::beg);
        m_file_stream.write(reinterpret_cast<const char *>(&status), sizeof(status));
        return;
    }

    throw unknown_user{};
}

void oopcoin::user_serializer::append(oopcoin::user_description desc) {
    m_file_stream.seekg(0, std::ios::end);
    m_file_stream.write(reinterpret_cast<const char *>(&desc), sizeof(user_description));
    // HACK: Remove this
    m_file_stream.flush();
}

oopcoin::user_description oopcoin::user_serializer::read(unsigned int id) {
    user_description desc;

    while (m_file_stream && m_file_stream.read(reinterpret_cast<char *>(&desc), sizeof(desc))) {
        if (desc.yser.id == id) {
            return desc;
        }
    }

    throw unknown_user{};
}

oopcoin::user_description oopcoin::user_serializer::obj_to_description(const oopcoin::user &user) {
    return user_description{
            .yser = user,
            .tombstone = user_description::status::active
    };
}

oopcoin::user oopcoin::user_serializer::description_to_obj(const oopcoin::user_description &desc) {
    return desc.yser;
}

oopcoin::user_description oopcoin::user_serializer::read_current() {
    user_description desc;
    m_file_stream.read(reinterpret_cast<char *>(&desc.yser.id), sizeof(desc.yser.id));
    m_file_stream.read(desc.yser.name, sizeof(desc.yser.name));
    m_file_stream.read(reinterpret_cast<char *>(&desc.tombstone), sizeof(desc.tombstone));
    return desc;
}

mystd::vector<oopcoin::user_description> oopcoin::user_serializer::read_all() {
    mystd::vector<user_description> loaded;

    // TODO: Load directly into the vector.
    user_description desc;
    while (m_file_stream && m_file_stream.read(reinterpret_cast<char *>(&desc), sizeof(desc))) {
        loaded.push_back(desc);
    }

    return loaded;
}

void oopcoin::user_serializer::append_all(const mystd::vector<user_description> &user_descriptions) {
    user_description tmp_desc;

    for (const auto &desc: user_descriptions) {
        bool met = false;
        m_file_stream.seekg(0, std::ios::beg);
        while (m_file_stream && m_file_stream.read(reinterpret_cast<char *>(&tmp_desc), sizeof(tmp_desc))) {
            if (tmp_desc.yser.id == desc.yser.id) {
                met = true;
                break;
            }
        }
        if (!met) {
            append(desc);
        }
    }
}
