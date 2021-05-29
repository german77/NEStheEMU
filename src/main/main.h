#pragma once

#include <vector>
#include <SDL.h>
#include "../core/cpu.h"
#include "../core/commands.h"
#include "../common/common_types.h"

const CommandParameters GetCommandParameters();
void InitializeSDL();
void ExcecuteCode();
void SDLEvents();
void DrawWindow();
std::tuple<u8,u8,u8> GetColor(u8 color);

CPU cpu{};

SDL_Renderer* renderer;
SDL_Window* window = NULL;