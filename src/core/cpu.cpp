#include <algorithm>
#include "cpu.h"

bool CPU::IsRunning() const {
	return status.break_cmd.Value() != 0;
}

void CPU::Reset() {
	register_a = 0;
	register_x = 0;
	register_y = 0;
	status.raw = 0;
	status.break_cmd.Assign(1);
	program_counter = 0x0600;
	stack_pointer = 0xFF;
}

void CPU::PrintStatus() {
	std::printf("\t\tA=0x%02x, X=0x%02x, Y=0x%02x\n",
		register_a, register_x, register_y);
	std::printf("\t\tPC=0x%04x, SP=0x%02x, status=%d%d%d%d%d%d%d%d\n",
		program_counter, stack_pointer, 
		status.negative.Value(),
		status.overflow.Value(),
		1,
		status.break_cmd.Value(),
		status.decimal.Value(),
		status.interrupt.Value(),
		status.zero.Value(),
		status.carry.Value()
	);
}

u16 CPU::GetCounter() const {
	return program_counter;
}

u16 CPU::IncrementCounter(u8 times) {
	return program_counter+= times;
}

void CPU::LoadProgram(const std::vector<u8>& program) {
	std::copy_n(program.begin(), program.size(), memory.begin() + 0x0600);
	WriteMemoryU16(0xFFFC, 0x0600);
}

u8 CPU::ReadMemory(const u16 address) const {
	return memory[address];
}

u16 CPU::ReadMemoryU16(const u16 address) const {
	u16 lo = ReadMemory(address);
	u16 hi = ReadMemory(address + 1) << 8;
	return hi | lo;
}

void CPU::WriteMemory(const u16 address, const u8 data) {
	memory[address] = data;
	return;
}

void CPU::WriteMemoryU16(const u16 address, const u16 data) {
	WriteMemory(address, data & 0xFF);
	WriteMemory(address + 1, data >> 8);
}

u16 CPU::GetOperandAddress(AddressingMode mode) const {
	switch (mode) {
	case AddressingMode::Immediate:
		return program_counter;
	case AddressingMode::ZeroPage:
		return ReadMemory(program_counter);
	case AddressingMode::ZeroPage_X:
		return (ReadMemory(program_counter) + register_x) && 0xFF;
	case AddressingMode::ZeroPage_Y:
		return (ReadMemory(program_counter) + register_y) && 0xFF;
	case AddressingMode::Absolute:
		return ReadMemoryU16(program_counter);
	case AddressingMode::Absolute_X:
		return ReadMemoryU16(program_counter) + register_x;
	case AddressingMode::Absolute_Y:
		return ReadMemoryU16(program_counter) + register_y;
	case AddressingMode::Indirect_X:{
		u16 base = (ReadMemory(program_counter) + register_x) && 0xFF;
		return ReadMemoryU16(base);
	}
	case AddressingMode::Indirect_Y: {
		u16 base = (ReadMemory(program_counter) + register_y) && 0xFF;
		return ReadMemoryU16(base);
	}
	default:
		std::printf("Address mode not implemmented %02x\n", static_cast<u8>(mode));
		return 0;
	}
}

void CPU::BRK(AddressingMode mode) {
	status.break_cmd.Assign(0);
	std::printf("BRK\n");
}

bool CPU::JSR(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	stack_pointer--;
	WriteMemoryU16(0xFDFE+(2* stack_pointer), program_counter + 2);
	program_counter = addr;
	std::printf("JSR 0x%04x 0x%02x\n", addr, stack_pointer);
	return false;
}

bool CPU::RTS(AddressingMode mode) {
	program_counter = ReadMemoryU16(0xFDFE+(2* stack_pointer));
	stack_pointer++;
	std::printf("RTS 0x%04x 0x%02x\n", program_counter, stack_pointer);
	return false;
}

bool CPU::JMP(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	program_counter = addr;
	std::printf("JMP 0x%04x\n", addr);
	return false;
}

bool CPU::BCC(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const s8 offset = ReadMemory(addr);
	if (status.carry.Value()==0) {
		program_counter += offset;
	}
	std::printf("BCC %d %d\n", status.carry.Value()==0, offset);
	return true;
}

bool CPU::BCS(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const s8 offset = ReadMemory(addr);
	if (status.carry.Value()!=0) {
		program_counter += offset;
	}
	std::printf("BCS %d %d\n", status.carry.Value()!=0, offset);
	return true;
}

bool CPU::BEQ(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const s8 offset = ReadMemory(addr);
	if (status.zero.Value()!=0) {
		program_counter += offset;
	}
	std::printf("BEQ %d %d\n", status.zero.Value()!=0, offset);
	return true;
}

bool CPU::BMI(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const s8 offset = ReadMemory(addr);
	if (status.negative.Value()!=0) {
		program_counter += offset;
	}
	std::printf("BMI %d %d\n", status.negative.Value()!=0, offset);
	return true;
}

bool CPU::BNE(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const s8 offset = ReadMemory(addr);
	if (status.zero.Value()==0) {
		program_counter += offset;
	}
	std::printf("BNE %d %d\n", status.zero.Value()==0, offset);
	return true;
}

bool CPU::BPL(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const s8 offset = ReadMemory(addr);
	if (status.negative.Value()==0) {
		program_counter += offset;
	}
	std::printf("BPL %d %d\n", status.negative.Value()==0, offset);
	return true;
}

bool CPU::BVC(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const s8 offset = ReadMemory(addr);
	if (status.overflow.Value()==0) {
		program_counter += offset;
	}
	std::printf("BVC %d %d\n", status.overflow.Value()==0, offset);
	return true;
}

bool CPU::BVS(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const s8 offset = ReadMemory(addr);
	if (status.overflow.Value()!=0) {
		program_counter += offset;
	}
	std::printf("BVS %d %d\n", status.overflow.Value()!=0, offset);
	return true;
}


void CPU::CLC(AddressingMode mode) {
	status.carry.Assign(0);
	std::printf("CLC\n");
}

void CPU::CLD(AddressingMode mode) {
	status.decimal.Assign(0);
	std::printf("CLD\n");
}

void CPU::CLI(AddressingMode mode) {
	status.interrupt.Assign(0);
	std::printf("CLI\n");
}

void CPU::CLV(AddressingMode mode) {
	status.overflow.Assign(0);
	std::printf("CLV\n");
}

void CPU::TXA(AddressingMode mode) {
	register_a = register_x;
	status.zero.Assign(register_a == 0);
	status.negative.Assign((register_a & 0x80) != 0);
	std::printf("TXA\n");
}

void CPU::TAX(AddressingMode mode) {
	register_x = register_a;
	status.zero.Assign(register_x == 0);
	status.negative.Assign((register_x & 0x80) != 0);
	std::printf("TAX\n");
}

void CPU::PHA(AddressingMode mode) {
	stack_pointer--;
	WriteMemoryU16(0xFDFE + (2 * stack_pointer), register_a);
	std::printf("PHA 0x%02x\n", stack_pointer);
}

void CPU::PLA(AddressingMode mode) {
	register_a =ReadMemory(0xFDFE + (2 * stack_pointer));
	stack_pointer++;
	std::printf("PLA 0x%02x %d\n", register_a, stack_pointer);
}

void CPU::ADC(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const u8 data = ReadMemory(addr);
	u16 value = register_a + data;
	if (status.carry.Value() == 1) {
		value++;
	}
	register_a = value;

	status.carry.Assign(value > 0xff);
	status.overflow.Assign(((register_a ^ value) & (data ^ value) & 0x80 )!= 0);
	status.zero.Assign(register_a == 0);
	status.negative.Assign((register_a & 0x80) != 0);
	std::printf("ADC 0x%02x\n", register_a);
}

void CPU::BIT(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const u8 value = ReadMemory(addr);

	status.zero.Assign((register_a & value) == 0);
	status.overflow.Assign((value & 0x40) != 0);
	status.negative.Assign((value & 0x80) != 0);
	std::printf("BIT 0x%02x\n", register_a & value);
}

void CPU::LSR(AddressingMode mode) {
	status.carry.Assign(register_a & 1);
	register_a >>= 1;
	status.zero.Assign(register_a == 0);
	status.negative.Assign((register_a & 0x80) != 0);
	std::printf("LSR 0x%02x\n", register_a);
}

void CPU::LDA(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);

	register_a = ReadMemory(addr);
	status.zero.Assign(register_a == 0);
	status.negative.Assign((register_a & 0x80) != 0);
	std::printf("LDA 0x%02x\n", register_a);
}

void CPU::LDX(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);

	register_x = ReadMemory(addr);
	status.zero.Assign(register_x == 0);
	status.negative.Assign((register_x & 0x80) != 0);
	std::printf("LDX 0x%02x\n", register_x);
}

void CPU::LDY(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);

	register_y = ReadMemory(addr);
	status.zero.Assign(register_y == 0);
	status.negative.Assign((register_y & 0x80) != 0);
	std::printf("LDY 0x%02x\n", register_y);
}

void CPU::STA(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);

	WriteMemory(addr, register_a);
	std::printf("STA 0x%04x %02x\n", addr, register_a);
}

void CPU::DEX(AddressingMode mode) {
	register_x--;
	status.zero.Assign(register_x == 0);
	status.negative.Assign((register_x & 0x80) != 0);
	std::printf("DEX\n");
}

void CPU::DEY(AddressingMode mode) {
	register_y--;
	status.zero.Assign(register_y == 0);
	status.negative.Assign((register_y & 0x80) != 0);
	std::printf("DEY\n");
}

void CPU::INC(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	u8 value = ReadMemory(addr);
	value++;
	WriteMemory(addr, value);

	status.zero.Assign(value == 0);
	status.negative.Assign((value & 0x80) != 0);
	std::printf("INC\n");
}

void CPU::INX(AddressingMode mode) {
	register_x++;
	status.zero.Assign(register_x == 0);
	status.negative.Assign((register_x & 0x80) != 0);
	std::printf("INX\n");
}

void CPU::INY(AddressingMode mode) {
	register_y++;
	status.zero.Assign(register_y == 0);
	status.negative.Assign((register_y & 0x80) != 0);
	std::printf("INY\n");
}

void CPU::AND(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	register_a &= ReadMemory(addr);

	status.zero.Assign(register_a == 0);
	status.negative.Assign((register_a & 0x80) != 0);
	std::printf("AND\n");
}

void CPU::CMP(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const u8 value = ReadMemory(addr);

	status.carry.Assign(register_a >= value);
	status.zero.Assign(register_a == value);
	status.negative.Assign(((register_a-value) & 0x80) != 0);
	std::printf("CMP\n");
}

void CPU::CPX(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const u8 value = ReadMemory(addr);

	status.carry.Assign(register_x >= value);
	status.zero.Assign(register_x == value);
	status.negative.Assign(((register_x-value) & 0x80) != 0);
	std::printf("CPX\n");
}

void CPU::CPY(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const u8 value = ReadMemory(addr);

	status.carry.Assign(register_y >= value);
	status.zero.Assign(register_y == value);
	status.negative.Assign(((register_y-value) & 0x80) != 0);
	std::printf("CPY\n");
}