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
	ADC, AND, ASL, BCC, BCS, BEQ,
	BIT, BMI, BNE, BPL, BRK, BVC,
	BVS, CLC, CLD, CLI, CLV, CMP,
	CPX, CPY, DEC, DEX, DEY, EOR,
	INC, INX, INY, JMP, JSR, LDA,
	LDX, LDY, LSR, NOP, ORA, PHA,
	PHP, PLA, PLP, ROL, ROR, RTI,
	RTS, SBC, SEC, SED, SEI, STA,
	STX, STY, TAX, TAY, TSX, TXA,
	TXS, TYA,
};

enum class Opcode : u8 {
	BRK = 0x00,
	JSR= 0x20,
	JMP = 0x4C,
	BNE = 0xD0,
	TXA = 0x8A,
	TAX = 0xAA,
	PHA = 0x48,
	PLA = 0x68,
	LDA_IMM = 0xA9,
	LDA_ZP = 0xA5,
	LDA_ZPX = 0xB5,
	LDA_ABS = 0xAD,
	LDX_IMM = 0xA2,
	LDX_ZP = 0xA6,
	LDX_ABS = 0xAE,
	LDY_IMM = 0xA0,
	LDY_ZP = 0xA4,
	LDY_ABS = 0xAC,
	STA_ZP = 0x85,
	STA_ZPX = 0x95,
	STA_ABS = 0x8D,
	STA_ABSX = 0x9D,
	STA_ABSY = 0x99,
	STA_INX = 0x81,
	STA_INY = 0x91,
	INC_ZP = 0xE6,
	INC_ZPX = 0xF6,
	INC_ABS = 0xEE,
	INC_ABSX = 0xFE,
	INX = 0xE8,
	INY = 0xC8,
	CPX_IMM = 0xE0,
	CPX_ZP = 0xE4,
	CPX_ABS = 0xEC,
	CPY_IMM = 0xC0,
	CPY_ZP = 0xC4,
	CPY_ABS = 0xCC,
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
	{Opcode::JSR,     Function::JSR, 2, 3, AddressingMode::Absolute},
	{Opcode::JMP,     Function::JMP, 2, 6, AddressingMode::Absolute},
	{Opcode::BNE,     Function::BNE, 1, 2, AddressingMode::Immediate},
	{Opcode::TXA,     Function::TXA, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::TAX,     Function::TAX, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::PHA,     Function::PHA, 0, 3, AddressingMode::NoneAddressing},
	{Opcode::PLA,     Function::PLA, 0, 4, AddressingMode::NoneAddressing},
	{Opcode::LDA_IMM, Function::LDA, 1, 2, AddressingMode::Immediate},
	{Opcode::LDA_ZP,  Function::LDA, 1, 3, AddressingMode::ZeroPage},
	{Opcode::LDA_ZPX, Function::LDA, 1, 4, AddressingMode::ZeroPage_X},
	{Opcode::LDA_ABS, Function::LDA, 2, 4, AddressingMode::Absolute},
	{Opcode::LDX_IMM, Function::LDX, 1, 2, AddressingMode::Immediate},
	{Opcode::LDX_ZP,  Function::LDX, 1, 3, AddressingMode::ZeroPage},
	{Opcode::LDX_ABS, Function::LDX, 2, 4, AddressingMode::Absolute},
	{Opcode::LDY_IMM, Function::LDY, 1, 2, AddressingMode::Immediate},
	{Opcode::LDY_ZP,  Function::LDY, 1, 3, AddressingMode::ZeroPage},
	{Opcode::LDY_ABS, Function::LDY, 2, 4, AddressingMode::Absolute},
	{Opcode::TAX,     Function::TAX, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::STA_ZP,  Function::STA, 1, 3, AddressingMode::ZeroPage},
	{Opcode::STA_ZPX, Function::STA, 1, 4, AddressingMode::ZeroPage_X},
	{Opcode::STA_ABSY,Function::STA, 2, 4, AddressingMode::Absolute_Y},
	{Opcode::INX,     Function::INX, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::INY,     Function::INY, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::CPX_IMM, Function::CPX, 1, 2, AddressingMode::Immediate},
	{Opcode::CPX_ZP,  Function::CPX, 1, 3, AddressingMode::Immediate},
	{Opcode::CPX_ABS, Function::CPX, 2, 4, AddressingMode::Immediate},
	{Opcode::CPY_IMM, Function::CPY, 1, 2, AddressingMode::Immediate},
	{Opcode::CPY_ZP,  Function::CPY, 1, 3, AddressingMode::Immediate},
	{Opcode::CPY_ABS, Function::CPY, 2, 4, AddressingMode::Immediate},
};