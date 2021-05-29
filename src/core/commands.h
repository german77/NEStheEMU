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

enum class Function : u8 {
	BRK,
	JMP,
	TAX,
	LDA,
	STA,
	INX,
};

enum class Opcode : u8 {
	BRK = 0x00,
	JMP = 0x20,
	TAX = 0xAA,
	LDA_IMM = 0xA9,
	LDA_ZP = 0xA5,
	LDA_ZPX = 0xB5,
	LDA_ABS = 0xAD,
	STA_ZP = 0x85,
	STA_ZPX = 0x95,
	INX = 0xE8,
};

struct CommandParameters {
	Opcode opcode{};
	Function function{};
	u8 parameters{};
	u8 cycles{};
	AddressingMode mode{};
};

const std::vector<CommandParameters> commands{
	{Opcode::BRK,     Function::BRK, 0, 7, AddressingMode::NoneAddressing},
	{Opcode::JMP,     Function::JMP, 0, 3, AddressingMode::Absolute},
	{Opcode::TAX,     Function::TAX, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::LDA_IMM, Function::LDA, 1, 2, AddressingMode::Immediate},
	{Opcode::LDA_ZP,  Function::LDA, 1, 3, AddressingMode::ZeroPage},
	{Opcode::LDA_ZPX, Function::LDA, 1, 4, AddressingMode::ZeroPage_X},
	{Opcode::LDA_ABS, Function::LDA, 2, 4, AddressingMode::Absolute},
	{Opcode::TAX,     Function::TAX, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::STA_ZP,  Function::STA, 1, 3, AddressingMode::ZeroPage},
	{Opcode::STA_ZPX, Function::STA, 1, 4, AddressingMode::ZeroPage_X},
};