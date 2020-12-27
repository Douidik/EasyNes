//
// Created by douidik on 26/12/2020.
//

#ifndef EASYNES_CORE_HPP
#define EASYNES_CORE_HPP

#include "CPU.hpp"
#include "RAM.hpp"

namespace EasyNes {

struct Core {
  CPU cpu{this};
  RAM ram;
};

}  // namespace EasyNes

#endif  // EASYNES_CORE_HPP
