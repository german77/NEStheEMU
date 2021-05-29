#include <cstdio>
#include "main.h"

std::vector<u8> program{ 0xa2, 0x00 , 0xa0 , 0x00 , 0x8a , 0x99 , 0x00 , 0x02 , 0x48 , 0xe8 , 0xc8 , 0xc0 , 0x10 , 0xd0 , 0xf5 , 0x68
	, 0x99 , 0x00 , 0x02 , 0xc8 , 0xc0 , 0x20 , 0xd0 , 0xf7 };

int main() {
	cpu.LoadProgram(program);
	cpu.Reset();
	while (cpu.IsRunning()) {
		cpu.PrintStatus();
		const CommandParameters parameter = GetCommandParameters();
		bool next = true;
		cpu.IncrementCounter(1);
		switch (parameter.function) {
		case Function::BRK:
			cpu.BRK(parameter.mode);
			break;
		case Function::JSR:
			next=cpu.JSR(parameter.mode);
			break;
		case Function::JMP:
			next = cpu.JMP(parameter.mode);
			break;
		case Function::BNE:
			next = cpu.BNE(parameter.mode);
			break;
		case Function::TXA:
			cpu.TXA(parameter.mode);
			break;
		case Function::TAX:
			cpu.TAX(parameter.mode);
			break;
		case Function::PHA:
			cpu.PHA(parameter.mode);
			break;
		case Function::PLA:
			cpu.PLA(parameter.mode);
			break;
		case Function::LDA:
			cpu.LDA(parameter.mode);
			break;
		case Function::LDX:
			cpu.LDX(parameter.mode);
			break;
		case Function::LDY:
			cpu.LDY(parameter.mode);
			break;
		case Function::STA:
			cpu.STA(parameter.mode);
			break;
		case Function::INX:
			cpu.INX(parameter.mode);
			break;
		case Function::INY:
			cpu.INY(parameter.mode);
			break;
		case Function::CPX:
			cpu.CPX(parameter.mode);
			break;
		case Function::CPY:
			cpu.CPY(parameter.mode);
			break;
		default:
			std::printf("Operation %02x not implemented\n", static_cast<u8>(parameter.opcode));
			break;
		}
			cpu.IncrementCounter(parameter.parameters);
		

	}
	return 0;
}

const CommandParameters GetCommandParameters() {
	const u8 value = cpu.ReadMemory(cpu.GetCounter());
	const Opcode opcode = static_cast<Opcode>(value);
	auto it = find_if(commands.begin(), commands.end(), [opcode]
	(const CommandParameters& cmd) { return opcode == cmd.opcode; });

	if (it == commands.end()) {
		std::printf("Unknow opcode %02x\n", static_cast<u8>(opcode));
		return {Opcode::BRK, Function::BRK, 0, 7, AddressingMode::NoneAddressing};
	}

	return *it;
}