#ifndef EASYNES_INSTRUCTIONS_HPP
#define EASYNES_INSTRUCTIONS_HPP

#include <array>

#include "CPU.hpp"

namespace EasyNes {
  
struct Instruction {
  u8 cycles = 0;
  u8* (CPU::*addressing)(void) = &CPU::IMP;
  void (CPU::*operation)(u8*) = &CPU::ILL;
};

// clang-format off
  
constexpr Instruction INSTRUCTION_SET[256] = {
    [0x69] = {2, &CPU::IMM, &CPU::ADC}, [0x65] = {3, &CPU::ZER, &CPU::ADC},
    [0x75] = {4, &CPU::ZPX, &CPU::ADC}, [0x6D] = {4, &CPU::ABS, &CPU::ADC},
    [0x7D] = {4, &CPU::ABX, &CPU::ADC}, [0x79] = {4, &CPU::ABY, &CPU::ADC},
    [0x61] = {6, &CPU::IDX, &CPU::ADC}, [0x71] = {5, &CPU::IDY, &CPU::ADC},

    [0x29] = {2, &CPU::IMM, &CPU::AND}, [0x25] = {3, &CPU::ZER, &CPU::AND},
    [0x35] = {4, &CPU::ZPX, &CPU::AND}, [0x2D] = {4, &CPU::ABS, &CPU::AND},
    [0x3D] = {4, &CPU::ABX, &CPU::AND}, [0x39] = {4, &CPU::ABY, &CPU::AND},
    [0x21] = {6, &CPU::IDX, &CPU::AND}, [0x31] = {5, &CPU::IDY, &CPU::AND},

    [0x0A] = {2, &CPU::IMP, &CPU::ASL}, [0x06] = {5, &CPU::ZER, &CPU::ASL},
    [0x16] = {6, &CPU::ZPX, &CPU::ASL}, [0x0E] = {6, &CPU::ABS, &CPU::ASL},
    [0x1E] = {7, &CPU::ABX, &CPU::ASL},

    [0x90] = {2, &CPU::REL, &CPU::BCC}, [0xB0] = {2, &CPU::REL, &CPU::BCS},
    [0xF0] = {2, &CPU::REL, &CPU::BEQ},

    [0x24] = {3, &CPU::ZER, &CPU::BIT}, [0x2C] = {4, &CPU::ABS, &CPU::BIT},

    [0xD0] = {2, &CPU::REL, &CPU::BNE}, [0x10] = {2, &CPU::REL, &CPU::BPL},

    [0x00] = {7, &CPU::IMP, &CPU::BRK},

    [0x50] = {2, &CPU::REL, &CPU::BVC}, [0x70] = {2, &CPU::REL, &CPU::BVS},

    [0x18] = {2, &CPU::IMP, &CPU::CLC}, [0xD8] = {2, &CPU::IMP, &CPU::CLD},
    [0x58] = {2, &CPU::IMP, &CPU::CLI}, [0xB8] = {2, &CPU::IMP, &CPU::CLV},

    [0xC9] = {2, &CPU::IMM, &CPU::CMP}, [0xC5] = {3, &CPU::ZER, &CPU::CMP},
    [0xD5] = {4, &CPU::ZPX, &CPU::CMP}, [0xCD] = {4, &CPU::ABS, &CPU::CMP},
    [0xDD] = {4, &CPU::ABX, &CPU::CMP}, [0xD9] = {4, &CPU::ABY, &CPU::CMP},
    [0xC1] = {6, &CPU::IDX, &CPU::CMP}, [0xD1] = {5, &CPU::IDY, &CPU::CMP},

    [0xE0] = {2, &CPU::IMM, &CPU::CPX}, [0xE4] = {3, &CPU::ZER, &CPU::CPX},
    [0xEC] = {4, &CPU::ABS, &CPU::CPX},

    [0xC0] = {2, &CPU::IMM, &CPU::CPY}, [0xC4] = {3, &CPU::ZER, &CPU::CPY},
    [0xCC] = {4, &CPU::ABS, &CPU::CPY},

    [0xC6] = {5, &CPU::ZER, &CPU::DEC}, [0xD6] = {6, &CPU::ZPX, &CPU::DEC},
    [0xCE] = {6, &CPU::ABS, &CPU::DEC}, [0xDE] = {7, &CPU::ABX, &CPU::DEC},

    [0xCA] = {2, &CPU::IMP, &CPU::DEX}, [0x88] = {2, &CPU::IMP, &CPU::DEY},

    [0x49] = {2, &CPU::IMM, &CPU::EOR}, [0x45] = {3, &CPU::ZER, &CPU::EOR},
    [0x55] = {4, &CPU::ZPX, &CPU::EOR}, [0x4D] = {4, &CPU::ABS, &CPU::EOR},
    [0x5D] = {4, &CPU::ABX, &CPU::EOR}, [0x59] = {4, &CPU::ABY, &CPU::EOR},
    [0x41] = {6, &CPU::IDX, &CPU::EOR}, [0x51] = {5, &CPU::IDY, &CPU::EOR},

    [0xE6] = {5, &CPU::ZER, &CPU::INC}, [0xF6] = {6, &CPU::ZPX, &CPU::INC},
    [0xEE] = {6, &CPU::ABS, &CPU::INC}, [0xFE] = {7, &CPU::ABX, &CPU::INC},

    [0xE8] = {2, &CPU::IMP, &CPU::INX}, [0xC8] = {2, &CPU::IMP, &CPU::INY},

    [0x4C] = {3, &CPU::ABS, &CPU::JMP}, [0x6C] = {5, &CPU::IND, &CPU::JMP},

    [0x20] = {6, &CPU::ABS, &CPU::JSR},

    [0xA9] = {2, &CPU::IMM, &CPU::LDA}, [0xA5] = {3, &CPU::ZER, &CPU::LDA},
    [0xB5] = {4, &CPU::ZPX, &CPU::LDA}, [0xAD] = {4, &CPU::ABS, &CPU::LDA},
    [0xBD] = {4, &CPU::ABX, &CPU::LDA}, [0xB9] = {4, &CPU::ABY, &CPU::LDA},
    [0xA1] = {6, &CPU::IDX, &CPU::LDA}, [0xB1] = {5, &CPU::IDY, &CPU::LDA},

    [0xA2] = {2, &CPU::IMM, &CPU::LDX}, [0xA6] = {3, &CPU::ZER, &CPU::LDX},
    [0xB6] = {4, &CPU::ZPY, &CPU::LDX}, [0xAE] = {4, &CPU::ABS, &CPU::LDX},
    [0xBE] = {4, &CPU::ABY, &CPU::LDX},

    [0xA0] = {2, &CPU::IMM, &CPU::LDY}, [0xA4] = {3, &CPU::ZER, &CPU::LDY},
    [0xB4] = {4, &CPU::ZPX, &CPU::LDY}, [0xAC] = {4, &CPU::ABS, &CPU::LDY},
    [0xBC] = {4, &CPU::ABX, &CPU::LDY},

    [0x4A] = {2, &CPU::IMM, &CPU::LSR}, [0x46] = {5, &CPU::ZER, &CPU::LSR},
    [0x56] = {6, &CPU::ZPX, &CPU::LSR}, [0x4E] = {6, &CPU::ABS, &CPU::LSR},
    [0x5E] = {7, &CPU::ABX, &CPU::LSR},

    [0xEA] = {2, &CPU::IMP, &CPU::NOP},

    [0x09] = {2, &CPU::IMM, &CPU::ORA}, [0x05] = {3, &CPU::ZER, &CPU::ORA},
    [0x15] = {4, &CPU::ZPX, &CPU::ORA}, [0x0D] = {4, &CPU::ABS, &CPU::ORA},
    [0x1D] = {4, &CPU::ABX, &CPU::ORA}, [0x19] = {4, &CPU::ABY, &CPU::ORA},
    [0x01] = {6, &CPU::IDX, &CPU::ORA}, [0x11] = {5, &CPU::IDY, &CPU::ORA},

    [0x48] = {3, &CPU::IMP, &CPU::PHA}, [0x08] = {3, &CPU::IMP, &CPU::PHP},

    [0x68] = {4, &CPU::IMP, &CPU::PLA}, [0x28] = {4, &CPU::IMP, &CPU::PLP},

    [0X2A] = {2, &CPU::IMM, &CPU::ROL}, [0x26] = {5, &CPU::ZER, &CPU::ROL},
    [0x36] = {6, &CPU::ZPX, &CPU::ROL}, [0x2E] = {6, &CPU::ABS, &CPU::ROL},
    [0x3E] = {7, &CPU::ABX, &CPU::ROL},

    [0X6A] = {2, &CPU::IMM, &CPU::ROR}, [0x66] = {5, &CPU::ZER, &CPU::ROR},
    [0x76] = {6, &CPU::ZPX, &CPU::ROR}, [0x6E] = {6, &CPU::ABS, &CPU::ROR},
    [0x7E] = {7, &CPU::ABX, &CPU::ROR},

    [0x40] = {6, &CPU::IMP, &CPU::RTI}, [0x60] = {6, &CPU::IMP, &CPU::RTS},

    [0xE9] = {2, &CPU::IMM, &CPU::SBC}, [0xE5] = {3, &CPU::ZER, &CPU::SBC},
    [0xF5] = {4, &CPU::ZPX, &CPU::SBC}, [0xED] = {4, &CPU::ABS, &CPU::SBC},
    [0xFD] = {4, &CPU::ABX, &CPU::SBC}, [0xF9] = {4, &CPU::ABY, &CPU::SBC},
    [0xE1] = {6, &CPU::IDX, &CPU::SBC}, [0xF1] = {5, &CPU::IDY, &CPU::SBC},

    [0x38] = {2, &CPU::IMP, &CPU::SEC}, [0xF8] = {2, &CPU::IMP, &CPU::SED},
    [0x78] = {2, &CPU::IMP, &CPU::SEI},

    [0x85] = {3, &CPU::ZER, &CPU::STA}, [0x95] = {4, &CPU::ZPX, &CPU::STA},
    [0x8D] = {4, &CPU::ABS, &CPU::STA}, [0x9D] = {5, &CPU::ABX, &CPU::STA},
    [0x99] = {5, &CPU::ABY, &CPU::STA}, [0x81] = {6, &CPU::IDX, &CPU::STA},
    [0x91] = {6, &CPU::IDY, &CPU::STA},

    [0x86] = {3, &CPU::ZER, &CPU::STX}, [0x96] = {4, &CPU::ZPY, &CPU::STX},
    [0x8E] = {4, &CPU::ABS, &CPU::STX},

    [0x84] = {3, &CPU::ZER, &CPU::STY}, [0x94] = {4, &CPU::ZPX, &CPU::STY},
    [0x8C] = {4, &CPU::ABS, &CPU::STY},

    [0xAA] = {2, &CPU::IMP, &CPU::TAX}, [0xA8] = {2, &CPU::IMP, &CPU::TAY},
    [0xBA] = {2, &CPU::IMP, &CPU::TSX}, [0x8A] = {2, &CPU::IMP, &CPU::TXA},
    [0x9A] = {2, &CPU::IMP, &CPU::TXS}, [0x98] = {2, &CPU::IMP, &CPU::TYA}};

}  // namespace EasyNes

// clang-format on

#endif  // EASYNES_INSTRUCTIONS_HPP
