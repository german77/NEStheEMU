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
	WriteMemory(address, data >> 8);
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
	default:
		std::printf("LDA mode not implemmented %02x\n", static_cast<u8>(mode));
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
	WriteMemoryU16(0xFDFE+(2* stack_pointer), program_counter);
	program_counter = addr;
	std::printf("JSR 0x%04x %d\n", addr, stack_pointer);
	return false;
}

bool CPU::JMP(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	program_counter = addr;
	std::printf("JMP 0x%04x\n", addr);
	return false;
}

bool CPU::BNE(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const s8 offset = ReadMemory(addr);
	if (status.zero.Value()==0) {
		program_counter += offset;
	}
	std::printf("BNE %d %d\n", status.zero.Value()==0, offset);
	return status.zero.Value() != 0;
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

void CPU::CPX(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const u8 value = ReadMemory(addr);

	status.carry.Assign(register_x > value);
	status.zero.Assign(register_x == value);
	status.negative.Assign(((register_x-value) & 0x80) != 0);
	std::printf("CPX\n");
}

void CPU::CPY(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	const u8 value = ReadMemory(addr);

	status.carry.Assign(register_y > value);
	status.zero.Assign(register_y == value);
	status.negative.Assign(((register_y-value) & 0x80) != 0);
	std::printf("CPY\n");
}