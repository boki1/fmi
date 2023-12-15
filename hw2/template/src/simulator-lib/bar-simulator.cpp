#include <limits>
#include <stdexcept>
#include <cassert>
#include <algorithm>    // for std::is_sorted

#include "mystd/list.h"

#include "bar-simulator.h"
#include "bar33factorial.h"
#include "student.h"

///
/// @brief Simulate the student bar problem
///
/// @param input
///   The stream, which contains the simulation commands
/// @param output
///   The stream, where the steps of simulation have to be printed.
///
void simulate_bar(std::istream &input, std::ostream &output) {
    bar33factorial bar = [&input] {
        std::uint64_t bar_capacity;
        std::uint64_t max_group_size;
        input >> bar_capacity >> max_group_size;
        return bar33factorial{bar_capacity, max_group_size};
    }();

    auto waiting_outside_simulation = [&]() {
        std::uint64_t num_students;
        input >> num_students;
        if (num_students > 0 && bar.capacity() == 0)
            throw incorrect_simulation{"There are students who won't ever party!"};
        mystd::list<student> waiting_out;
        for (std::uint64_t i = 0; i < num_students; ++i) {
            auto smart_st = smart_student::read_from(input);
            waiting_out.push_back(*smart_st);
        }
        if (!std::is_sorted(waiting_out.begin(), waiting_out.end(), [](const student &a, const student &b) {
            return a.m_arrival < b.m_arrival;
        }))
            throw incorrect_simulation{"The order of incoming party students is invalid!"};
        return waiting_out;
    }();

    for (;;) {
        // Add any students whose time has come are not in the simulation yet.
        for (auto it = waiting_outside_simulation.begin(); it != waiting_outside_simulation.end();) {
            if (it->m_arrival > bar.now())
                break;
            // Safety: The list is sorted.
            assert(it->m_arrival == bar.now());
            bar.add_waiting_student(*it);
            waiting_outside_simulation.erase(it++);
        }

        // Make the simulation tick once.
        const auto simulation_has_more = bar.tick_simulation(output);
        if (!simulation_has_more && waiting_outside_simulation.empty())
            break;

        if (bar.now() == std::numeric_limits<std::uint64_t>::max())
            throw std::runtime_error{"All the simulation time has passed but there "
                                     "are still students who haven't got to party!"};
    }
}
