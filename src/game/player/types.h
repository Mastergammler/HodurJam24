#include "../types.h"

struct Player
{
    v2 position;
    v2 orientation;

    bool in_animation;
    float time_since_anim_start;

    VoiceSettings left_foot;
    VoiceSettings right_foot;
    VoiceSettings body;
};
