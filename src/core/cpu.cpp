#include <algorithm>
#include "cpu.h"

bool CPU::IsRunning() const {
	return status.break_cmd == 0;
}

u16 CPU::GetCounter() const {
	return program_counter;
}

u16 CPU::IncrementCounter() {
	return program_counter++;
}

u8 CPU::ReadMemory(const u16 address) const {
	return memory[address];
}

void CPU::WriteMemory(const u16 address, const u8 data) {
	memory[address] = data;
	return;
}

void CPU::LoadProgram(const std::vector<u8>& program) {
	std::copy_n(program.begin(), program.size(), memory.begin() + 0x8000);
	program_counter = 0x8000;
	return;
}

void CPU::LDA_immediate() {
	register_a = ReadMemory(program_counter++);
	status.zero.Assign(register_a == 0);
	status.negative.Assign((register_a & 0x80) != 0);
	std::printf("LDA_I %02x\n", register_a);
	return;
}

void CPU::TAX() {
	register_x = register_a;
	status.zero.Assign(register_x == 0);
	status.negative.Assign((register_x & 0x80) != 0);
	std::printf("TAX\n");
	return;
}

void CPU::INX() {
	register_x++;
	status.zero.Assign(register_x == 0);
	status.negative.Assign((register_x & 0x80) != 0);
	std::printf("INX\n");
	return;
}

void CPU::BRK() {
	status.break_cmd.Assign(1);
	std::printf("BRK\n");
	return;
}