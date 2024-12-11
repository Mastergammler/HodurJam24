#pragma once

#include "imports.h"

#include "../io/module.h"
#include "../logging/module.h"
#include "../win32/module.h"

void Game_Init(ExitCallback exitFunction);
void Game_Update();
void Game_Dispose();
