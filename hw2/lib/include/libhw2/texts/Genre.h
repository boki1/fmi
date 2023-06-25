#ifndef FMI_OOP_HW2_GENRE_H
#define FMI_OOP_HW2_GENRE_H

#include <cstdint>
#include <climits>

namespace libhw2 {

class Genre {
  constexpr static bool set_parity(uint16_t combined) {
    bool parity = combined & 1;
    for (unsigned i = 1; i < sizeof(combined) * CHAR_BIT; ++i) {
      parity ^= combined & (1 << i);
    }
    return parity;
  }

 public:
  enum class Characteristic { Fantasy, SciFi, Thriller, TrueCrime };
  enum class Type { Prose, Poetry, Drama };
  enum class TargetAudience { Above5, Above8, Above12, Above16, All, NerdsOnly };

 public:
  Genre(Characteristic characteristic, Type type, TargetAudience target)
      : m_characteristic{(uint8_t)characteristic},
        m_type{(uint8_t)type},
        m_target{(uint8_t)target} {
    m_parity = Genre::set_parity(m_combined);
  }

  Genre(const Genre&) = default;
  Genre& operator=(const Genre&) = default;

 public:
  [[nodiscard]] uint16_t characteristic() const { return m_characteristic; }

  [[nodiscard]] uint16_t type() const { return m_type; }

  [[nodiscard]] uint16_t target() const { return m_target; }

  [[nodiscard]] bool parity() const { return m_parity > 0; }

 private:
  union {
    struct {
      uint16_t m_characteristic : 7;
      uint16_t m_type : 4;
      uint16_t m_target : 4;
      uint16_t m_parity : 1;
    };

    uint16_t m_combined;
  };
};

}  // namespace libhw2

#endif  // FMI_OOP_HW2_GENRE_H
