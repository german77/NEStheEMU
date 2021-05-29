#pragma once

#include "common_types.h"
#include <string>

enum class Opcode : u8 {
	LDA_I = 0xA9,
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

const CommandParameters commands[]{
	{Opcode::LDA_I, "LDA", 1, 2},
	{Opcode::TAX, "TAX", 0, 2},
	{Opcode::INX, "INX", 0, 2},
	{Opcode::BRK, "BRK", 0, 7},
};