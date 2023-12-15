#pragma once

#include <cstdint>

#include "mystd/list.h"
#include "student.h"

class bar33factorial {
public:
  bar33factorial(std::uint64_t capacity, std::uint64_t max_group_size);

  [[nodiscard]] bool tick_simulation(std::ostream &);
  void add_waiting_student(const student &);

  [[nodiscard]] std::uint64_t now() const noexcept;
  [[nodiscard]] std::uint64_t capacity() const noexcept;

private:
  [[nodiscard]] bool has_room_for_more(std::uint64_t) const noexcept;
  [[nodiscard]] std::uint64_t enthusiasm(const student &st) const noexcept;

private:
  // The m_partying list is kept in sorted order where students with less
  // enthusiasm left are stored nearby the beginning and students with more
  // enthusiasm are towards the beginning.
  mystd::list<student> m_partying;

  mystd::list<mystd::list<student>> m_groups_waiting;

  std::uint64_t m_capacity;
  std::uint64_t m_max_group_size;

  std::uint64_t m_now{0};
};
