#include "../internal.h"

void PlayNumberSound(int number, float delay)
{
    // sound only goes to 99
    assert(number > 0 && number < 100);

    // TODO: handle number over 100 -> tewnties > 4
    int twenties = number / 20;
    int twentiesRest = number % 20;

    int fifths = twentiesRest / 5;
    int rest = twentiesRest % 5;

    bool interrupt = true;

    if (twenties != 0)
    {
        // TODO: if i want extra delay in between, i should just create a longer
        // tail?
        //  -> but that obstructs then from the internal one? Dunno,
        //  => Else i need to do everything via scheduling
        int uiIdxSingles = Audio.num_mapping[twenties + 7];
        SchedulePlayback(&Audio.ui[uiIdxSingles],
                         {&GlobalMono, interrupt},
                         delay);
        interrupt = false;
    }

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
