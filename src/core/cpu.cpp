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
	program_counter = 0xFFFC;
}

u16 CPU::GetCounter() const {
	return program_counter;
}

u16 CPU::IncrementCounter() {
	return program_counter++;
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
	case AddressingMode::Absolute:
		return ReadMemoryU16(program_counter);
	default:
		std::printf("LDA mode not implemmented %02x\n", static_cast<u8>(mode));
		return 0;
	}
}

void CPU::LDA(AddressingMode mode) {
	const u16 addr = GetOperandAddress(mode);

	register_a = ReadMemory(addr);
	status.zero.Assign(register_a == 0);
	status.negative.Assign((register_a & 0x80) != 0);
	std::printf("LDA %02x\n", register_a);
}

void CPU::TAX() {
	register_x = register_a;
	status.zero.Assign(register_x == 0);
	status.negative.Assign((register_x & 0x80) != 0);
	std::printf("TAX\n");
}

void CPU::INX() {
	register_x++;
	status.zero.Assign(register_x == 0);
	status.negative.Assign((register_x & 0x80) != 0);
	std::printf("INX\n");
}

void CPU::BRK() {
	status.break_cmd.Assign(1);
	std::printf("BRK\n");
}