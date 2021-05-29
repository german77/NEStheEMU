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
		BitField<6, 1, u8> overflow;
		BitField<7, 1, u8> negative;
	};
};

class CPU {
public:
	bool IsRunning() const;
	void Reset();
	void PrintStatus();
	u16 GetCounter() const;
	u16 IncrementCounter(u8 times);
	void LoadProgram(const std::vector<u8>& program);

	u8 ReadMemory(const u16 address) const;
	u16 ReadMemoryU16(const u16 address) const;
	void WriteMemory(const u16 address, const u8 data);
	void WriteMemoryU16(const u16 address, const u16 data);

	void BRK(AddressingMode mode);
	bool JSR(AddressingMode mode);
	bool RTS(AddressingMode mode);
	bool JMP(AddressingMode mode);

	bool BCC(AddressingMode mode);
	bool BCS(AddressingMode mode);
	bool BEQ(AddressingMode mode);
	bool BMI(AddressingMode mode);
	bool BNE(AddressingMode mode);
	bool BPL(AddressingMode mode);
	bool BVC(AddressingMode mode);
	bool BVS(AddressingMode mode);

	void CLC(AddressingMode mode);
	void CLD(AddressingMode mode);
	void CLI(AddressingMode mode);
	void CLV(AddressingMode mode);

	void TXA(AddressingMode mode);
	void TAX(AddressingMode mode);
	void PHA(AddressingMode mode);
	void PLA(AddressingMode mode);
	void BIT(AddressingMode mode);

	void LSR(AddressingMode mode);

	void ADC(AddressingMode mode);

	void LDA(AddressingMode mode);
	void LDX(AddressingMode mode);
	void LDY(AddressingMode mode);

	void STA(AddressingMode mode);

	void DEX(AddressingMode mode);
	void DEY(AddressingMode mode);
	void INC(AddressingMode mode);
	void INX(AddressingMode mode);
	void INY(AddressingMode mode);

	void AND(AddressingMode mode);

	void CMP(AddressingMode mode);
	void CPX(AddressingMode mode);
	void CPY(AddressingMode mode);

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
