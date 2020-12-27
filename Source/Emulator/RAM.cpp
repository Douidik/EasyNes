#include "RAM.hpp"

namespace EasyNes {

RAM::RAM() { Wipe(); }

RAM::~RAM() {}

void RAM::Wipe() {
  for (u8 &data : m_Data) {
    data = 0;
  }
}

u8 &RAM::operator[](u16 address) {
  // We don't check the address to be within the memory boundaries because we
  // cannot represent an out of range address with only 16 bits
  return m_Data[address];
}

}  // namespace EasyNes
