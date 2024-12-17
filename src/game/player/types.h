#include "../types.h"

struct Player
{
    v2 position;
    v2 orientation;

    // TODO: refactor to delay playback / schedule action etc
    bool in_walk_anim;
    float time_since_anim_start;

    bool inputs_locked;

    VoiceSettings left_foot;
    VoiceSettings right_foot;
    VoiceSettings body;
};
