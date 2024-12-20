#include "../types.h"
#include "../vector.h"

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

    /*
     * Angle starting at the horizontal, always positive
     * The angle from which on something should sound like
     * it is behind the player
     * Unit: degrees
     */
    float behind_cone_angle = 20;
    float lp_start_ratio = 0.65;
    float lp_max_ratio = 0.35;
    float behind_cone_max_angle = 65;
};
