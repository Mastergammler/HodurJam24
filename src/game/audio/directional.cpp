#include "../internal.h"

// 0 = 100% lp, 1 = 0% lp
const float BACK_LP_CO = 0.45;
const float BUMP_PAN = 0.65;

PlaybackSettings DirectionalAudio(v2 direction)
{
    PlaybackSettings playback = {};
    if (direction == WEST)
    {
        playback.pan = -BUMP_PAN;
    }
    else if (direction == EAST)
    {
        playback.pan = BUMP_PAN;
    }
    else if (direction == SOUTH)
    {
        playback.lowpass_filter = BACK_LP_CO;
    }

    return playback;
}
