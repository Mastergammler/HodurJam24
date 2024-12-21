#include "../internal.h"

void PlayNumberSound(int number, float delay)
{
    // sound only go to 19
    assert(number > 0 && number < 20);

    int fifths = number / 5;
    int rest = number % 5;

    bool interrupt = true;
    if (rest != 0)
    {
        int uiIdxSingles = Audio.num_mapping[rest];
        SchedulePlayback(&Audio.ui[uiIdxSingles],
                         {&GlobalMono, interrupt},
                         delay);
        // we just scheduled the first part of playback
        interrupt = false;
    }

    if (fifths != 0)
    { // TODO: it's numerated, singles are 1-4 and 5ths are 5-7
        //-> so this is a bit confusing
        //=> If i scale this beyound 20 this should probably change
        int uiIdxFifths = Audio.num_mapping[fifths + 4];
        SchedulePlayback(&Audio.ui[uiIdxFifths],
                         {&GlobalMono, interrupt},
                         delay);
    }
}
