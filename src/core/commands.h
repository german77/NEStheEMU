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
	NOP = 0xEA,
	BRK = 0x00,
	JSR = 0x20,
	RTS = 0x60,
	JMP = 0x4C,

	BCC = 0x90,
	BCS = 0xB0,
	BEQ = 0xF0,
	BMI = 0x30,
	BNE = 0xD0,
	BPL = 0x10,
	BVC = 0x50,
	BVS = 0x70,

	CLC = 0x18,
	CLD = 0xD8,
	CLI = 0x58,
	CLV = 0xB8,

	TXA = 0x8A,
	TAX = 0xAA,
	PHA = 0x48,
	PLA = 0x68,
	BIT_ZP = 0x24,
	BIT_ABS = 0x2C,
	LSR = 0x4A,

	ADC_IMM = 0x69,
	ADC_ZP = 0x65,
	ADC_ZPX = 0x75,
	ADC_ABS = 0x6D,
	ADC_ABSX = 0x7D,
	ADC_ABSY = 0x79,
	ADC_INX = 0x61,
	ADC_INY = 0x71,

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

	DEX = 0xCA,
	DEY = 0x88,
	INX = 0xE8,
	INY = 0xC8,

	AND_IMM = 0x29,
	AND_ZP = 0x25,
	AND_ZPX = 0x35,
	AND_ABS = 0x2D,
	AND_ABSX = 0x3D,
	AND_ABSY = 0x39,
	AND_INX = 0x21,
	AND_INY = 0x31,

	CMP_IMM = 0xC9,
	CMP_ZP = 0xC5,
	CMP_ZPX = 0xD5,
	CMP_ABS = 0xCD,
	CMP_ABSX = 0xDD,
	CMP_ABSY = 0xD9,
	CMP_INX = 0xC1,
	CMP_INY = 0xD1,

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
	{Opcode::NOP,     Function::NOP, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::BRK,     Function::BRK, 0, 7, AddressingMode::NoneAddressing},
	{Opcode::JSR,     Function::JSR, 2, 3, AddressingMode::Absolute},
	{Opcode::RTS,     Function::RTS, 1, 6, AddressingMode::NoneAddressing},
	{Opcode::JMP,     Function::JMP, 2, 6, AddressingMode::Absolute},

	{Opcode::BCC,     Function::BCC, 1, 2, AddressingMode::Immediate},
	{Opcode::BCS,     Function::BCS, 1, 2, AddressingMode::Immediate},
	{Opcode::BEQ,     Function::BEQ, 1, 2, AddressingMode::Immediate},
	{Opcode::BMI,     Function::BMI, 1, 2, AddressingMode::Immediate},
	{Opcode::BNE,     Function::BNE, 1, 2, AddressingMode::Immediate},
	{Opcode::BPL,     Function::BPL, 1, 2, AddressingMode::Immediate},
	{Opcode::BVC,     Function::BVC, 1, 2, AddressingMode::Immediate},
	{Opcode::BVS,     Function::BVS, 1, 2, AddressingMode::Immediate},

	{Opcode::CLC,     Function::CLC, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::CLD,     Function::CLD, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::CLI,     Function::CLI, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::CLV,     Function::CLV, 0, 2, AddressingMode::NoneAddressing},

	{Opcode::TXA,     Function::TXA, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::TAX,     Function::TAX, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::PHA,     Function::PHA, 0, 3, AddressingMode::NoneAddressing},
	{Opcode::PLA,     Function::PLA, 0, 4, AddressingMode::NoneAddressing},
	{Opcode::BIT_ZP,  Function::BIT, 1, 4, AddressingMode::ZeroPage},
	{Opcode::BIT_ABS, Function::BIT, 2, 4, AddressingMode::Absolute},

	{Opcode::LSR,     Function::LSR, 0, 2, AddressingMode::NoneAddressing},

	{Opcode::ADC_IMM, Function::ADC, 1, 2, AddressingMode::Immediate},
	{Opcode::ADC_ZP,  Function::ADC, 1, 3, AddressingMode::ZeroPage},
	{Opcode::ADC_ZPX, Function::ADC, 1, 4, AddressingMode::ZeroPage_X},
	{Opcode::ADC_ABS, Function::ADC, 2, 4, AddressingMode::Absolute},
	{Opcode::ADC_ABSX,Function::ADC, 2, 4, AddressingMode::Absolute_X},
	{Opcode::ADC_ABSY,Function::ADC, 2, 4, AddressingMode::Absolute_Y},
	{Opcode::ADC_INX, Function::ADC, 1, 6, AddressingMode::Indirect_X},
	{Opcode::ADC_INY, Function::ADC, 1, 5, AddressingMode::Indirect_Y},

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

	{Opcode::STA_ZP,  Function::STA, 1, 3, AddressingMode::ZeroPage},
	{Opcode::STA_ZPX, Function::STA, 1, 4, AddressingMode::ZeroPage_X},
	{Opcode::STA_ABS, Function::STA, 2, 4, AddressingMode::Absolute},
	{Opcode::STA_ABSX,Function::STA, 2, 4, AddressingMode::Absolute_X},
	{Opcode::STA_ABSY,Function::STA, 2, 4, AddressingMode::Absolute_Y},
	{Opcode::STA_INX, Function::STA, 1, 6, AddressingMode::Indirect_X},
	{Opcode::STA_INY, Function::STA, 1, 6, AddressingMode::Indirect_Y},

	{Opcode::INC_ZP,  Function::INC, 1, 5, AddressingMode::ZeroPage},
	{Opcode::INC_ZPX, Function::INC, 1, 6, AddressingMode::ZeroPage_X},
	{Opcode::INC_ABS, Function::INC, 2, 6, AddressingMode::Absolute},
	{Opcode::INC_ABSX,Function::INC, 2, 7, AddressingMode::Absolute_X},

	{Opcode::DEX,     Function::DEX, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::DEY,     Function::DEY, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::INX,     Function::INX, 0, 2, AddressingMode::NoneAddressing},
	{Opcode::INY,     Function::INY, 0, 2, AddressingMode::NoneAddressing},

	{Opcode::AND_IMM, Function::AND, 1, 2, AddressingMode::Immediate},
	{Opcode::AND_ZP,  Function::AND, 1, 3, AddressingMode::ZeroPage},
	{Opcode::AND_ZPX, Function::AND, 1, 4, AddressingMode::ZeroPage_X},
	{Opcode::AND_ABS, Function::AND, 2, 4, AddressingMode::Absolute},
	{Opcode::AND_ABSX,Function::AND, 2, 4, AddressingMode::Absolute_X},
	{Opcode::AND_ABSY,Function::AND, 2, 4, AddressingMode::Absolute_Y},
	{Opcode::AND_INX, Function::AND, 1, 6, AddressingMode::Indirect_X},
	{Opcode::AND_INY, Function::AND, 1, 5, AddressingMode::Indirect_Y},

	{Opcode::CMP_IMM, Function::CMP, 1, 2, AddressingMode::Immediate},
	{Opcode::CMP_ZP,  Function::CMP, 1, 3, AddressingMode::ZeroPage},
	{Opcode::CMP_ZPX, Function::CMP, 1, 4, AddressingMode::ZeroPage_X},
	{Opcode::CMP_ABS, Function::CMP, 2, 4, AddressingMode::Absolute},
	{Opcode::CMP_ABSX,Function::CMP, 2, 4, AddressingMode::Absolute_X},
	{Opcode::CMP_ABSY,Function::CMP, 2, 4, AddressingMode::Absolute_Y},
	{Opcode::CMP_INX, Function::CMP, 1, 6, AddressingMode::Indirect_X},
	{Opcode::CMP_INY, Function::CMP, 1, 5, AddressingMode::Indirect_Y},

	{Opcode::CPX_IMM, Function::CPX, 1, 2, AddressingMode::Immediate},
	{Opcode::CPX_ZP,  Function::CPX, 1, 3, AddressingMode::ZeroPage},
	{Opcode::CPX_ABS, Function::CPX, 2, 4, AddressingMode::Absolute},

	{Opcode::CPY_IMM, Function::CPY, 1, 2, AddressingMode::Immediate},
	{Opcode::CPY_ZP,  Function::CPY, 1, 3, AddressingMode::ZeroPage},
	{Opcode::CPY_ABS, Function::CPY, 2, 4, AddressingMode::Absolute},
};