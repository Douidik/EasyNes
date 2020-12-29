#include "CPU.hpp"

#include <iostream>
#include <utility>

#include "Core.hpp"
#include "Instructions.hpp"

namespace EasyNes {

constexpr u16 BATCH_BYTES(u8 lo, u8 hi) { return (hi << 8) | lo; }

constexpr std::pair<u8, u8> DECOMPOSE_WORD(u16 value) { return {(value >> 8), value | 0x00FF}; }

constexpr bool IS_NEGATIVE(u8 value) { return value & NEGATIVE_BIT; }

constexpr bool HAS_OVERFLOWED(u8 a, u8 b, u16 result) { return ~(a ^ b) & IS_NEGATIVE(a ^ result); }

CPU::CPU(Core *core) : m_Core(core) {}

bool CPU::Step() {
  // If the cpu don't have to wait more cycles we can perform the next instruction
  if (m_WaitingCycles-- == 0) {
    u8          opcode      = *FetchByte(m_PC++);
    Instruction instruction = INSTRUCTION_SET[opcode];

    // Execute the instruction
    u8 *data = (this->*instruction.addressing)();
    (this->*instruction.operation)(data);

    m_WaitingCycles += instruction.cycles;
    m_ElapsedInstructions++;
    return true;
  }
  return false;
}

u8 *CPU::FetchByte(u16 address) { return &m_Core->ram[address]; }

u8 *CPU::PushByte(u8 value) {
  u8 *data = FetchByte(STACK_BASE + m_SP--);
  *data    = value;
  return data;
}

u8 *CPU::PullByte() { return FetchByte(STACK_BASE + ++m_SP); }

u16 CPU::PushWord(u16 value) {
  auto decomposed = DECOMPOSE_WORD(value);
  PushByte(decomposed.first);
  PushByte(decomposed.second);
  return value;
}

u16 CPU::PullWord() { return BATCH_BYTES(*PullByte(), *PullByte()); }

void CPU::Interrupt(u16 vector, u8 cycles) {
  m_Status.B = 0;
  PushByte(m_Status.value);
  PushWord(m_PC);
  m_Status.I = 1;

  u8 lo = *FetchByte(vector);
  u8 hi = *FetchByte(vector + 1);

  m_PC = BATCH_BYTES(lo, hi);

  m_WaitingCycles = cycles;
}
void CPU::RST() {
  u8 lo = *FetchByte(RST_VECTOR);
  u8 hi = *FetchByte(RST_VECTOR + 1);

  m_PC = BATCH_BYTES(lo, hi);

  m_A = m_X = m_Y = 0x00;
  m_SP            = 0xFD;
  m_Status.value  = 0;
  m_Status.U      = 1;

  m_WaitingCycles = 8;
}

u8 *CPU::IMM() { return FetchByte(m_PC++); }

u8 *CPU::REL() {
  u16 offset = *FetchByte(m_PC++);

  if (IS_NEGATIVE(offset)) {
    offset |= 0xFF00;
  }

  return FetchByte(m_PC + offset);
}

u16 CPU::ZeroPageAddressing(u8 offset) {
  u8 address = *FetchByte(m_PC++);
  // Add the offset and wrap the address around the first page
  return (address + offset) % PAGE_SIZE;
}

u16 CPU::AbsoluteAddressing(u8 offset) {
  u8 lo = *FetchByte(m_PC++);
  u8 hi = *FetchByte(m_PC++);

  u16 address = BATCH_BYTES(lo, hi) + offset;

  // Additional cycle if the page is crossed
  if ((hi << 8) != (address & 0xFF00)) {
    m_WaitingCycles++;
  }

  return address;
}

u8 *CPU::IND() {
  u8  pointerLo = *FetchByte(m_PC++);
  u8  pointerHi = *FetchByte(m_PC++);
  u16 pointer   = BATCH_BYTES(*FetchByte(pointerLo), *FetchByte(pointerHi));

  u8 lo = *FetchByte(pointer);
  u8 hi = 0;

  // Page boundary crossed
  if ((pointer & 0x00FF) == 0xFF) {
    hi = (*FetchByte(pointer & 0xFF00) << 8);
  } else {
    hi = (*FetchByte(pointer + 1) << 8);
  }

  return FetchByte(BATCH_BYTES(lo, hi));
}

u8 *CPU::IDX() {
  u8 lo = (*FetchByte(m_PC++) + m_X) % PAGE_SIZE;
  u8 hi = (lo + 1) % PAGE_SIZE;
  return FetchByte(BATCH_BYTES(lo, hi));
}

u8 *CPU::IDY() {
  u8 lo = *FetchByte(m_PC++);
  u8 hi = (lo + 1) % PAGE_SIZE;
  return FetchByte(BATCH_BYTES(lo, hi));
}

u8 CPU::AddOperation(u8 operand) {
  u16 result = m_A + operand + m_Status.C;

  m_Status.C = result >= 0xFF;
  m_Status.Z = result == 0;
  m_Status.N = IS_NEGATIVE(result);
  m_Status.V = HAS_OVERFLOWED(m_A, operand, result);

  return result;
}

void CPU::BitwiseOperation(u8 operand, u8 (*operation)(u8, u8)) {
  m_A        = (*operation)(m_A, operand);
  m_Status.Z = m_A == 0;
  m_Status.N = IS_NEGATIVE(m_A);
}

void CPU::BranchOperation(bool condition, u8 destination) {
  if (condition) {
    m_WaitingCycles++;

    // Additional cycle if page crossed
    if ((m_PC & 0xFF00) != (destination & 0xFF00)) {
      m_WaitingCycles++;
    }

    m_PC = destination;
  }
}

void CPU::CompareOperation(u8 reg, u8 operand) {
  u8 difference = reg - operand;

  m_Status.N = IS_NEGATIVE(difference);
  m_Status.Z = difference == 0;
  m_Status.C = difference >= 0;
}

void CPU::IncrementOperation(u8 *a) {
  *a         = (*a + 1) % 256;
  m_Status.Z = *a == 0;
  m_Status.N = IS_NEGATIVE(*a);
}

void CPU::DecrementOperation(u8 *a) {
  *a         = (*a - 1) % 256;
  m_Status.Z = *a == 0;
  m_Status.N = IS_NEGATIVE(*a);
}

void CPU::LoadOperation(u8 &reg, u8 value) {
  reg        = value;
  m_Status.Z = reg == 0;
  m_Status.N = IS_NEGATIVE(reg);
}

void CPU::ReturnOperation(u16 offset) {
  m_Status.value = *PullByte();
  m_PC           = PullWord() + offset;
}

void CPU::TransferOperation(u8 &source, u8 &destination) {
  destination = source;
  m_Status.N  = IS_NEGATIVE(source);
  m_Status.Z  = source == 0;
}

void CPU::ASL(u8 *operand) {
  u8 result = *operand << 1;

  m_Status.C = *operand & 0b10000000;
  m_Status.N = IS_NEGATIVE(result);
  m_Status.Z = result == 0;

  *operand = result;
}

void CPU::BIT(u8 *operand) {
  u8 result = *operand & m_A;

  m_Status.C = result == 0;
  m_Status.N = IS_NEGATIVE(result);
  m_Status.V = *operand & 0b1000000;
}

void CPU::BRK(u8 *) {
  PushWord(++m_PC);

  // PushByte the status onto the stack with the break bit active
  PushByte(m_Status.value | 0b10000);
  m_Status.I = 1;

  u8 lo = *FetchByte(IRQ_VECTOR);
  u8 hi = *FetchByte(IRQ_VECTOR + 1);
  m_PC  = BATCH_BYTES(lo, hi);
}

void CPU::JSR(u8 *destination) {
  PushWord(--m_PC);
  m_PC = *destination;
}

void CPU::LSR(u8 *operand) {
  u8 result = (*operand >> 1);

  m_Status.C = *operand & 0x01;
  m_Status.N = result == 0;
  m_Status.Z = IS_NEGATIVE(result);

  *operand = result;
}

void CPU::ROL(u8 *operand) {
  u16 result = (*operand << 1) | m_Status.C;

  m_Status.C = result > 0xFF;
  m_Status.N = IS_NEGATIVE(result);
  m_Status.Z = result == 0;

  *operand = result;
}

void CPU::ROR(u8 *operand) {
  u8 result = (*operand >> 1) | (m_Status.C << 7);

  m_Status.C = *operand & 0x01;
  m_Status.N = IS_NEGATIVE(result);
  m_Status.Z = result == 0;

  *operand = result;
}

}  // namespace EasyNes
