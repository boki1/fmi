#include <cstdint>
#include <algorithm>

#include "mystd/find.h"
#include "mystd/list.h"

#include "bar33factorial.h"

bar33factorial::bar33factorial(std::uint64_t capacity,
                               std::uint64_t max_group_size)
        : m_capacity{capacity}, m_max_group_size{max_group_size} {}

[[nodiscard]] bool bar33factorial::tick_simulation(std::ostream &os) {
    // Kick out all students with no enthusiasm left.
    for (auto it = m_partying.begin(); it != m_partying.end();) {
        if (enthusiasm(*it) == 0) {
            os << m_now << " " << it->m_id << " exit\n";
            it = m_partying.erase(it);
        } else ++it;
    }

    // Add groups for which there is enough space. Look through them in order
    // of arrival.
    for (auto it = m_groups_waiting.begin(); it != m_groups_waiting.end();) {
        assert(it->size() > 0);
        if (!has_room_for_more(it->size())) {
            ++it;
            continue;
        }
        for (auto &st : *it) {
            os << m_now << " " << st.m_id << " enter\n";
            st.m_enter = m_now;
        }
        m_partying.merge(std::move(*it), [&](const student &a, const student &b) {
            return enthusiasm(a) >= enthusiasm(b);
        });
        it = m_groups_waiting.erase(it);
    }

    // Make time tick-tack.
    ++m_now;

    return !(m_groups_waiting.empty() && m_partying.empty());
}

void bar33factorial::add_waiting_student(const student &st) {
    auto &group = [&]() -> mystd::list<student> & {
        auto it = mystd::find_if(m_groups_waiting.begin(), m_groups_waiting.end(),
                                 [&](const auto &group) {
                                     return group.size() + 1 <= m_max_group_size &&
                                            group.front().m_program == st.m_program;
                                 });
        if (it != m_groups_waiting.end())
            return *it;
        m_groups_waiting.push_back({});
        return m_groups_waiting.back();
    }();

    auto insert_it =
            mystd::find_if(group.begin(), group.end(), [&](const student &w) {
                return w.m_arrival > st.m_arrival;
            });

    group.insert(insert_it, st);
}

std::uint64_t bar33factorial::now() const noexcept { return m_now; }

std::uint64_t bar33factorial::capacity() const noexcept { return m_capacity; }

///
/// Helpers
///

bool bar33factorial::has_room_for_more(std::uint64_t more) const noexcept {
    return m_capacity >= m_partying.size() + more;
}

std::uint64_t bar33factorial::enthusiasm(const student &st) const noexcept {
    return st.m_enter + st.m_stay - m_now;
};
