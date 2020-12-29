#ifndef EASYNES_CPU_HPP
#define EASYNES_CPU_HPP
#include <utility>

#include "Types.hpp"

namespace EasyNes {

class Core;
  
constexpr u8  NEGATIVE_BIT = 0b10000000;
constexpr u16 STACK_BASE   = 0x0100;
constexpr u16 IRQ_VECTOR   = 0xFFFE;
constexpr u16 RST_VECTOR   = 0xFFFC;
constexpr u16 NMI_VECTOR   = 0xFFFA;
constexpr u16 PAGE_SIZE    = 256;
  
class CPU {
  friend class Instruction;

 public:
  CPU(Core *core);
  // Perform one cpu clock, returns if the clock fetch a new instruction
  bool Step();
  
  void Interrupt(u16 vector, u8 cycles);
  void IRQ() { if(!m_Status.I) Interrupt(IRQ_VECTOR, 7); }
  void NMI() { Interrupt(NMI_VECTOR, 8); }
  void RST();

  inline bool IsCompleted() const { return m_WaitingCycles == 0; }

  inline u8  GetRegisterA() const { return m_A; }
  inline u8  GetRegisterX() const { return m_X; }
  inline u8  GetRegisterY() const { return m_Y; }
  inline u8  GetRegisterSP() const { return m_SP; }
  inline u16 GetRegisterPC() const { return m_PC; }
  inline u8  GetRegisterStatus() const { return m_Status.value; }
  inline u32 GetElapsedInstructions() const { return m_ElapsedInstructions; }

 private:
  Core *m_Core;
  u32   m_ElapsedInstructions;

  // R/W Memory
  u8 *FetchByte(u16 address);
  u8 *PushByte(u8 value);
  u8 *PullByte();

  u16 PushWord(u16 value);
  u16 PullWord();

  // Internal registers
  u16 m_PC;
  u8  m_SP;
  u8  m_A;
  u8  m_X;
  u8  m_Y;

  union {
    u8 value;
    struct {
      bool C : 1;
      bool Z : 1;
      bool I : 1;
      bool D : 1;
      bool B : 1;
      bool U : 1;
      bool V : 1;
      bool N : 1;
    };
  } m_Status;

  s32 m_WaitingCycles;

  constexpr u16 ZERO_PAGE(u8 address, u16 offset) { return (address + offset) % 256; }

 private:
  u16 ZeroPageAddressing(u8 offset = 0);
  u16 AbsoluteAddressing(u8 offset = 0);

 public:
  inline u8 *ACC() { return &m_A; }
  inline u8 *IMP() { return nullptr; }
  u8 *       IMM();
  u8 *       REL();
  inline u8 *ZER() { return FetchByte(ZeroPageAddressing()); }
  inline u8 *ZPX() { return FetchByte(ZeroPageAddressing(m_X)); }
  inline u8 *ZPY() { return FetchByte(ZeroPageAddressing(m_Y)); }
  inline u8 *ABS() { return FetchByte(AbsoluteAddressing()); }
  inline u8 *ABX() { return FetchByte(AbsoluteAddressing(m_X)); }
  inline u8 *ABY() { return FetchByte(AbsoluteAddressing(m_Y)); }
  u8 *       IND();
  u8 *       IDX();
  u8 *       IDY();

 private:
  u8   AddOperation(u8 operand);
  void BitwiseOperation(u8 operand, u8 (*operation)(u8, u8));
  void BranchOperation(bool condition, u8 destination);
  void CompareOperation(u8 reg, u8 operand);
  void IncrementOperation(u8 *a);
  void DecrementOperation(u8 *a);
  void LoadOperation(u8 &reg, u8 value);
  void ReturnOperation(u16 offset = 0);
  void TransferOperation(u8 &source, u8 &destination);

 public:
  // to keep all inlined and simple to read we momentarily disable clang format
  // clang-format off

  inline void ADC(u8 *operand) { m_A = AddOperation(*operand); }
  inline void AND(u8 *operand) { BitwiseOperation(*operand, [](u8 a, u8 b) -> u8 { return a & b; }); }
  void        ASL(u8 *operand);
  inline void BCC(u8 *destination) { BranchOperation(!m_Status.C, *destination); }
  inline void BCS(u8 *destination) { BranchOperation(m_Status.C, *destination); }
  inline void BEQ(u8 *destination) { BranchOperation(!m_Status.Z, *destination); }
  void        BIT(u8 *operand);
  inline void BMI(u8 *destination) { BranchOperation(m_Status.N, *destination); }
  inline void BNE(u8 *destination) { BranchOperation(m_Status.Z, *destination); }
  inline void BPL(u8 *destination) { BranchOperation(!m_Status.N, *destination); }
  void        BRK(u8 *);
  inline void BVC(u8 *destination) { BranchOperation(!m_Status.V, *destination); }
  inline void BVS(u8 *destination) { BranchOperation(m_Status.V, *destination); }
  inline void CLC(u8 *) { m_Status.C = false; }
  inline void CLD(u8 *) { m_Status.D = false; }
  inline void CLI(u8 *) { m_Status.I = false; }
  inline void CLV(u8 *) { m_Status.V = false; }
  void        CMP(u8 *operand) { CompareOperation(m_A, *operand); }
  void        CPX(u8 *operand) { CompareOperation(m_X, *operand); }
  void        CPY(u8 *operand) { CompareOperation(m_Y, *operand); }
  inline void DEC(u8 *operand) { DecrementOperation(operand); }
  inline void DEX(u8 *) { DecrementOperation(&m_X); }
  inline void DEY(u8 *) { DecrementOperation(&m_Y); }
  inline void EOR(u8 *operand) { BitwiseOperation(*operand, [](u8 a, u8 b) -> u8 { return a ^ b; }); }
  void        INC(u8 *operand) { IncrementOperation(operand); }
  void        INX(u8 *) { IncrementOperation(&m_X); }
  void        INY(u8 *) { IncrementOperation(&m_Y); }
  inline void JMP(u8 *destination) { m_PC = *destination; }
  void        JSR(u8 *destination);
  inline void LDA(u8 *value) { LoadOperation(m_A, *value); }
  inline void LDX(u8 *value) { LoadOperation(m_X, *value); }
  inline void LDY(u8 *value) { LoadOperation(m_Y, *value); }
  void        LSR(u8 *operand);
  inline void NOP(u8 *) {}
  inline void ORA(u8 *operand) { BitwiseOperation(*operand, [](u8 a, u8 b) -> u8 { return a | b; }); }
  inline void PHA(u8 *) { PushByte(m_A); }
  inline void PHP(u8 *) { PushByte(m_Status.value); }
  inline void PLA(u8 *) { m_A = *PullByte(); }
  inline void PLP(u8 *) { m_Status.value = *PullByte(); }
  void        ROL(u8 *operand);
  void        ROR(u8 *operand);
  inline void RTI(u8 *) { ReturnOperation(); }
  inline void RTS(u8 *) { ReturnOperation(1); }
  inline void SBC(u8 *operand) { m_A = AddOperation(~(*operand)); }
  inline void SEC(u8 *) { m_Status.C = 1; }
  inline void SED(u8 *) { m_Status.D = 1; }
  inline void SEI(u8 *) { m_Status.I = 1; }
  inline void STA(u8 *data) { *data = m_A; }
  inline void STX(u8 *data) { *data = m_X; }
  inline void STY(u8 *data) { *data = m_Y; }
  inline void TAX(u8 *) { TransferOperation(m_A, m_X); }
  inline void TAY(u8 *) { TransferOperation(m_A, m_Y); }
  inline void TSX(u8 *) { TransferOperation(m_SP, m_X); }
  inline void TXA(u8 *) { TransferOperation(m_X, m_A); }
  inline void TXS(u8 *) { TransferOperation(m_X, m_SP); }
  inline void TYA(u8 *) { TransferOperation(m_Y, m_A); }
  inline void ILL(u8 *) {}

  // clang-format on
};

}  // namespace EasyNes

#endif  // EASYNES_CPU_HPP
