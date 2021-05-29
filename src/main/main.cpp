#include <cstdio>
#include "main.h"

std::vector<u8> program{ 0x20, 0x06, 0x06, 0x20, 0x38, 0x06, 0x20, 0x0d, 0x06, 0x20, 0x2a, 0x06, 0x60, 0xa9, 0x02, 0x85,
	0x02, 0xa9, 0x04, 0x85, 0x03, 0xa9, 0x11, 0x85, 0x10, 0xa9, 0x10, 0x85, 0x12, 0xa9, 0x0f, 0x85,
	0x14, 0xa9, 0x04, 0x85, 0x11, 0x85, 0x13, 0x85, 0x15, 0x60, 0xa5, 0xfe, 0x85, 0x00, 0xa5, 0xfe,
	0x29, 0x03, 0x18, 0x69, 0x02, 0x85, 0x01, 0x60, 0x20, 0x4d, 0x06, 0x20, 0x8d, 0x06, 0x20, 0xc3,
	0x06, 0x20, 0x19, 0x07, 0x20, 0x20, 0x07, 0x20, 0x2d, 0x07, 0x4c, 0x38, 0x06, 0xa5, 0xff, 0xc9,
	0x77, 0xf0, 0x0d, 0xc9, 0x64, 0xf0, 0x14, 0xc9, 0x73, 0xf0, 0x1b, 0xc9, 0x61, 0xf0, 0x22, 0x60,
	0xa9, 0x04, 0x24, 0x02, 0xd0, 0x26, 0xa9, 0x01, 0x85, 0x02, 0x60, 0xa9, 0x08, 0x24, 0x02, 0xd0,
	0x1b, 0xa9, 0x02, 0x85, 0x02, 0x60, 0xa9, 0x01, 0x24, 0x02, 0xd0, 0x10, 0xa9, 0x04, 0x85, 0x02,
	0x60, 0xa9, 0x02, 0x24, 0x02, 0xd0, 0x05, 0xa9, 0x08, 0x85, 0x02, 0x60, 0x60, 0x20, 0x94, 0x06,
	0x20, 0xa8, 0x06, 0x60, 0xa5, 0x00, 0xc5, 0x10, 0xd0, 0x0d, 0xa5, 0x01, 0xc5, 0x11, 0xd0, 0x07,
	0xe6, 0x03, 0xe6, 0x03, 0x20, 0x2a, 0x06, 0x60, 0xa2, 0x02, 0xb5, 0x10, 0xc5, 0x10, 0xd0, 0x06,
	0xb5, 0x11, 0xc5, 0x11, 0xf0, 0x09, 0xe8, 0xe8, 0xe4, 0x03, 0xf0, 0x06, 0x4c, 0xaa, 0x06, 0x4c,
	0x35, 0x07, 0x60, 0xa6, 0x03, 0xca, 0x8a, 0xb5, 0x10, 0x95, 0x12, 0xca, 0x10, 0xf9, 0xa5, 0x02,
	0x4a, 0xb0, 0x09, 0x4a, 0xb0, 0x19, 0x4a, 0xb0, 0x1f, 0x4a, 0xb0, 0x2f, 0xa5, 0x10, 0x38, 0xe9,
	0x20, 0x85, 0x10, 0x90, 0x01, 0x60, 0xc6, 0x11, 0xa9, 0x01, 0xc5, 0x11, 0xf0, 0x28, 0x60, 0xe6,
	0x10, 0xa9, 0x1f, 0x24, 0x10, 0xf0, 0x1f, 0x60, 0xa5, 0x10, 0x18, 0x69, 0x20, 0x85, 0x10, 0xb0,
	0x01, 0x60, 0xe6, 0x11, 0xa9, 0x06, 0xc5, 0x11, 0xf0, 0x0c, 0x60, 0xc6, 0x10, 0xa5, 0x10, 0x29,
	0x1f, 0xc9, 0x1f, 0xf0, 0x01, 0x60, 0x4c, 0x35, 0x07, 0xa0, 0x00, 0xa5, 0xfe, 0x91, 0x00, 0x60,
	0xa6, 0x03, 0xa9, 0x00, 0x81, 0x10, 0xa2, 0x00, 0xa9, 0x01, 0x81, 0x10, 0x60, 0xa2, 0x00, 0xea,
	0xea, 0xca, 0xd0, 0xfb, 0x60 };

int main(int argc, char* argv[]) {
	InitializeSDL();
	cpu.LoadProgram(program);
	cpu.Reset();
	while (cpu.IsRunning()) {
		ExcecuteCode(); 
		DrawWindow();
		SDLEvents();
		SDL_Delay(1);
	}

	// Destroy the window. This will also destroy the surface
	SDL_DestroyWindow(window);

	// Quit SDL
	SDL_Quit();
	return 0;
}

void SDLEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
		case SDL_QUIT:
			exit(0);
		case SDL_KEYDOWN:
			std::printf("KEY DOWn\n");
			break;
		case SDL_KEYUP:
			std::printf("KEY UP\n");
			break;
		}
	}
}

void DrawWindow() {
	u16 addr = 0x0200;
	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 32; y++) {
			u8 pixel = cpu.ReadMemory(addr++);
			int r, g, b;
			std::tie(r,g,b) = GetColor(pixel);
			SDL_SetRenderDrawColor(renderer, r, g, b, 255); 
			SDL_Rect pix = { x * 20, y * 20,20,20 };
			SDL_RenderFillRect(renderer, &pix);
		}
	}
	SDL_RenderPresent(renderer);
}
std::tuple<u8, u8, u8> GetColor(u8 color) {
	switch (color) {
	case 0:return { 0,0,0 };
	case 1:return { 255,255,255 };
	case 2:return { 128,128,128 };
	case 3:return { 255,0,0 };
	case 4:return { 0,255,0 };
	case 5:return { 0,0,255 };
	case 6:return { 255,0,255 };
	case 7:return { 255,255,0 };
	default: return { 0,255,255 };
	}
}
void ExcecuteCode() {
	//cpu.PrintStatus();
	const CommandParameters parameter = GetCommandParameters();
	bool next = true;
	cpu.IncrementCounter(1);
	switch (parameter.function) {
	case Function::BRK:
		cpu.BRK(parameter.mode);
		break;
	case Function::JSR:
		next = cpu.JSR(parameter.mode);
		break;
	case Function::RTS:
		next = cpu.RTS(parameter.mode);
		break;
	case Function::JMP:
		next = cpu.JMP(parameter.mode);
		break;
	case Function::BCC:
		next = cpu.BCC(parameter.mode);
		break;
	case Function::BCS:
		next = cpu.BCS(parameter.mode);
		break;
	case Function::BEQ:
		next = cpu.BEQ(parameter.mode);
		break;
	case Function::BMI:
		next = cpu.BMI(parameter.mode);
		break;
	case Function::BNE:
		next = cpu.BNE(parameter.mode);
		break;
	case Function::BPL:
		next = cpu.BPL(parameter.mode);
		break;
	case Function::BVC:
		next = cpu.BVC(parameter.mode);
		break;
	case Function::BVS:
		next = cpu.BVS(parameter.mode);
		break;
	case Function::CLC:
		cpu.CLC(parameter.mode);
		break;
	case Function::CLD:
		cpu.CLD(parameter.mode);
		break;
	case Function::CLI:
		cpu.CLI(parameter.mode);
		break;
	case Function::CLV:
		cpu.CLV(parameter.mode);
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
	case Function::BIT:
		cpu.BIT(parameter.mode);
		break;
	case Function::LSR:
		cpu.LSR(parameter.mode);
		break;
	case Function::ADC:
		cpu.ADC(parameter.mode);
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
	case Function::DEX:
		cpu.DEX(parameter.mode);
		break;
	case Function::DEY:
		cpu.DEY(parameter.mode);
		break;
	case Function::INC:
		cpu.INX(parameter.mode);
		break;
	case Function::INX:
		cpu.INX(parameter.mode);
		break;
	case Function::INY:
		cpu.INY(parameter.mode);
		break;
	case Function::AND:
		cpu.AND(parameter.mode);
		break;
	case Function::CMP:
		cpu.CMP(parameter.mode);
		break;
	case Function::CPX:
		cpu.CPX(parameter.mode);
		break;
	case Function::CPY:
		cpu.CPY(parameter.mode);
		break;
	case Function::NOP:
		std::printf("NOP\n");
		break;
	default:
		std::printf("Operation %02x not implemented\n", static_cast<u8>(parameter.opcode));
		break;
	}
	if (next) {
		cpu.IncrementCounter(parameter.parameters);
	}
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

void InitializeSDL() {
	// Initialize SDL. SDL_Init will return -1 if it fails.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::printf("Error initializing SDL\n");
		system("pause");
	}

	// Create our window
	window = SDL_CreateWindow("NEStheEMU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 640, SDL_WINDOW_SHOWN);

	// Make sure creating the window succeeded
	if (!window) {
		std::printf("Error creating window\n");
		system("pause");
	}

	int result = SDL_CreateWindowAndRenderer(640, 640, NULL, &window, &renderer);

	// Make sure getting the surface succeeded
	if (result != 0) {
		std::printf("Error getting renderer\n");
		system("pause");
	}
	// Update the window display
	SDL_UpdateWindowSurface(window);
}