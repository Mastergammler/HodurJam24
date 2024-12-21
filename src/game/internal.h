#pragma once

#include "module.h"
#include "types.h"
#include "vector.h"

#include "bear/types.h"
#include "map/types.h"
#include "player/types.h"
#include "systems/types.h"
#include "ui/types.h"

const v2 NORTH = {0, -1};
const v2 WEST = {-1, 0};
const v2 EAST = {1, 0};
const v2 SOUTH = {0, 1};
const v2 INIT = {0, 0};
const v2 TRAPPED = {-1, -1};

static ExitCallback ExitFunction;
static HodurInputState GameInputs = {18, new KeyInput[18]};
static LoadedAudio Audio;
static UiState Ui;

static Player Player;
static LevelState Level;
static Bear Bear;
static NodeList Nodes;

/**
 * Stereo voice playback for non directional things
 */
static VoiceSettings GlobalStereo;
static VoiceSettings Proximity;
static VoiceSettings GlobalMono;
static VoiceSettings Ambience;
