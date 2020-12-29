#include <Core.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Tiny program", "[CPU]") {
  EasyNes::Core core;

  constexpr std::array<EasyNes::u8, 16> program{
      0xA9, 0x10,  // A = 0x10
      0x85, 0x20,  // ram[0x20] = A
      0xA9, 0x01,  // A = 0x1
      0x65, 0x20,  // A += ram[0x20] -> A = 0x10 + 0x1 -> A = 0x11
      0x85, 0x21,  // ram[0x21] = A
      0xE6, 0x21,  // ram[0x21]++ -> 0x11++
      0xA4, 0x21,  // Y = ram[0x21] -> Y = 0x12
      0xC8,        // Y++ -> Y = 0x13
      0x00,        // END
  };

  for (int i = 0; i < program.size(); i++) {
    core.ram[0x8000 + i] = program[i];
  }

  core.ram[0xFFFC] = 0x00;
  core.ram[0xFFFD] = 0x80;

  core.cpu.RST();

#define EXECUTE_INS() while (!core.cpu.Step())

  EXECUTE_INS();
  CHECK(core.cpu.GetRegisterA() == 0x10);
  EXECUTE_INS();
  CHECK(core.ram[0x20] == 0x10);
  EXECUTE_INS();
  CHECK(core.cpu.GetRegisterA() == 0x01);
  EXECUTE_INS();
  CHECK(core.cpu.GetRegisterA() == 0x11);
  EXECUTE_INS();
  CHECK(core.ram[0x21] == 0x11);
  EXECUTE_INS();
  CHECK(core.ram[0x21] == 0x12);
  EXECUTE_INS();
  CHECK(core.cpu.GetRegisterY() == 0x12);
  EXECUTE_INS();
  CHECK(core.cpu.GetRegisterY() == 0x13);
}
