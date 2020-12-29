#ifndef EASYNES_RAM_HPP
#define EASYNES_RAM_HPP

#include <array>

#include "Types.hpp"

namespace EasyNes {

constexpr std::size_t RAM_SIZE = 256 * 256;

class RAM {
 public:
  RAM();
  ~RAM();

  void Wipe();

  u8 &operator[](u16 address);

 private:
  std::array<u8, RAM_SIZE> m_Data;
};

}  // namespace EasyNes

#endif  // EASYNES_RAM_HPP
