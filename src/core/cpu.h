#pragma once

#include <array>
#include <vector>
#include "../common/common_types.h"
#include "../common/bit_field.h"
#include "commands.h"

struct CpuFlag {
	union {
		u8 raw{};

		BitField<0, 1, u8> carry;
		BitField<1, 1, u8> zero;
		BitField<2, 1, u8> interrupt;
		BitField<3, 1, u8> decimal;
		BitField<4, 1, u8> break_cmd;
		BitField<5, 1, u8> overflow;
		BitField<6, 1, u8> negative;
	};
};

class CPU {
public:
	bool IsRunning() const;
	void Reset();
	u16 GetCounter() const;
	u16 IncrementCounter(u8 times);
	void LoadProgram(const std::vector<u8>& program);

	u8 ReadMemory(const u16 address) const;
	u16 ReadMemoryU16(const u16 address) const;
	void WriteMemory(const u16 address, const u8 data);
	void WriteMemoryU16(const u16 address, const u16 data);

	void BRK(AddressingMode mode);
	void JSR(AddressingMode mode);
	void JMP(AddressingMode mode);
	void TXA(AddressingMode mode);
	void TAX(AddressingMode mode);
	void LDA(AddressingMode mode);
	void LDX(AddressingMode mode);
	void LDY(AddressingMode mode);
	void STA(AddressingMode mode);
	void INX(AddressingMode mode);

private:
	u16 GetOperandAddress(AddressingMode mode) const;

	u8 register_a{};
	u8 register_x{};
	u8 register_y{};
	CpuFlag status{};
	u16 program_counter{};
	std::array<u8, 0xFFFF> memory{};
	u8 stack_pointer;
};
