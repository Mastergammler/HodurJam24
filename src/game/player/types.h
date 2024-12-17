#include "../types.h"

struct Player
{
    v2 position;
    v2 orientation;

    bool in_walk_anim;
    float time_since_anim_start;

    // TODO: need generalized component
    bool inputs_locked;
    float time_since_lock_start;
    float target_lock_time;

    VoiceSettings left_foot;
    VoiceSettings right_foot;
    VoiceSettings body;
};
