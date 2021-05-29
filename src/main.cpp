#include <cstdio>
#include "main.h"

std::vector<u8> program{ 0xa9, 0xc0, 0xaa, 0xe8, 0x00 };

int main() {
	cpu.LoadProgram(program);
	while (cpu.IsRunning()) {
		const Opcode opcode = getOpcode();
		cpu.IncrementCounter();
		switch (opcode) {
		case Opcode::LDA_I:
			cpu.LDA_immediate();
			break;
		case Opcode::TAX:
			cpu.TAX();
			break;
		case Opcode::INX:
			cpu.INX();
			break;
		case Opcode::BRK:
			cpu.BRK();
			break;
		default:
			cpu.BRK();
			std::printf("Operation %02x not implemented\n", static_cast<u8>(opcode));
			break;
		}
	}
	return 0;
}

Opcode getOpcode() {
	u8 value = cpu.ReadMemory(cpu.GetCounter());
	return static_cast<Opcode>(value);
}