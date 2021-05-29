#include <algorithm>
#include "cpu.h"

bool CPU::IsRunning() const {
	return status.break_cmd == 0;
}

void CPU::Reset() {
	register_a = 0;
	register_x = 0;
	register_y = 0;
	status.raw = 0;
	program_counter = 0x8000;
	stack_pointer = 0;
}

u16 CPU::GetCounter() const {
	return program_counter;
}

u16 CPU::IncrementCounter(u8 times) {
	return program_counter+= times;
}

void CPU::LoadProgram(const std::vector<u8>& program) {
	std::copy_n(program.begin(), program.size(), memory.begin() + 0x8000);
	WriteMemoryU16(0xFFFC, 0x8000);
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
	status.break_cmd.Assign(1);
	std::printf("BRK\n");
}

void CPU::JSR(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	stack_pointer++;
	WriteMemoryU16(0xFFFC-(2* stack_pointer), program_counter);
	program_counter = addr;
	std::printf("JSR %04x %d\n", addr, stack_pointer);
}

void CPU::JMP(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);
	program_counter = addr;
	std::printf("JMP %04x\n", addr);
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

void CPU::LDA(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);

	register_a = ReadMemory(addr);
	status.zero.Assign(register_a == 0);
	status.negative.Assign((register_a & 0x80) != 0);
	std::printf("LDA %02x\n", register_a);
}

void CPU::LDX(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);

	register_x = ReadMemory(addr);
	status.zero.Assign(register_x == 0);
	status.negative.Assign((register_x & 0x80) != 0);
	std::printf("LDX %02x\n", register_x);
}

void CPU::LDY(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);

	register_y = ReadMemory(addr);
	status.zero.Assign(register_y == 0);
	status.negative.Assign((register_y & 0x80) != 0);
	std::printf("LDY %02x\n", register_y);
}

void CPU::STA(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);

	WriteMemory(addr, register_a);
	std::printf("STA %04x %02x\n", addr, register_a);
}

void CPU::INX(AddressingMode mode) {
	register_x++;
	status.zero.Assign(register_x == 0);
	status.negative.Assign((register_x & 0x80) != 0);
	std::printf("INX\n");
}