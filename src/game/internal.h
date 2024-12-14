#pragma once

#include "module.h"

#include "map/types.h"
#include "player/types.h"

static const float STEP_ANIM_TIME = .55f;

static ExitCallback ExitFunction;
static HodurInputState GameInputs = {18, new KeyInput[18]};
static LoadedAudio Audio;
static Map CurrentMap = {0, 0};
static Player Player;
