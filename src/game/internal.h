#pragma once

#include "module.h"
#include "types.h"

#include "map/types.h"
#include "player/types.h"
#include "ui/types.h"

static ExitCallback ExitFunction;
static HodurInputState GameInputs = {18, new KeyInput[18]};
static LoadedAudio Audio;
static Player Player;
static LevelState Level;
static UiState Ui;

/**
 * Stereo voice playback for non directional things
 */
static VoiceSettings GlobalStereo;
static VoiceSettings GlobalMono;
