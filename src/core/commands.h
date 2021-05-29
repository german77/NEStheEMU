#pragma once

#include "../common/common_types.h"
#include <string>

enum class AddressingMode {
	Immediate,
	ZeroPage,
	ZeroPage_X,
	ZeroPage_Y,
	Absolute,
	Absolute_X,
	Absolute_Y,
	Indirect_X,
	Indirect_Y,
	NoneAddressing,
};

enum class Opcode : u8 {
	LDA_IMM = 0xA9,
	LDA_ZP = 0xA5,
	LDA_ABS = 0xAD,
	TAX = 0xAA,
	INX = 0xE8,
	BRK = 0x00,
};

struct CommandParameters {
	Opcode opcode{};
	std::string name{};
	u8 parameters{};
	u8 cycles{};
};