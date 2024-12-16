#include "../internal.h"

void PlayNumberSound(int number)
{
    // sound only go to 19
    assert(number < 20);

    int fifths = number / 5;
    int rest = number % 5;

    if (rest != 0)
    {
        int uiIdxSingles = Audio.num_mapping[rest];
        PlayAudio(&Audio.ui[uiIdxSingles], {&GlobalMono}, false);
    }

    if (fifths != 0)
    { // TODO: it's numerated, singles are 1-4 and 5ths are 5-7
        //-> so this is a bit confusing
        //=> If i scale this beyound 20 this should probably change
        int uiIdxFifths = Audio.num_mapping[fifths + 4];
        PlayAudio(&Audio.ui[uiIdxFifths], {&GlobalMono}, false);
    }
}
