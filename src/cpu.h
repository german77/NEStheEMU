#pragma once

#include <array>
#include <vector>
#include "common_types.h"
#include "bit_field.h"

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
	u16 GetCounter() const;
	u16 IncrementCounter();
	u8 ReadMemory(const u16 address) const;
	void WriteMemory(const u16 address, const u8 data);
	void LoadProgram(const std::vector<u8>& program);

	void LDA_immediate();
	void TAX();
	void INX();
	void BRK();

private:
	u8 register_a{};
	u8 register_x{};
	u8 register_y{};
	CpuFlag status{};
	u16 program_counter{};
	std::array<u8, 0xFFFF> memory{};
};
