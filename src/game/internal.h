#pragma once

#include "module.h"

#include "types.h"

#include "map/types.h"

#define NAMEOF(x) #x

static const char DELIMITER = ':';
static const string INPUT_CONF = "configs/keyboard.conf";

static HodurInputState GameInputs = {18, new KeyInput[18]};
static ExitCallback ExitFunction;
static LoadedAudio Audio;
static Map CurrentMap = {0, 0};
static Player Player;
