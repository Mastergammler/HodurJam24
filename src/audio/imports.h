#include "extern/minivorbis.h"
#include <cassert>
#include <string>
#include <vector>

#define XAUDIO2_HELPER_FUNCTIONS
// #include <x3daudio.h>
#include <xaudio2.h>
// #include <xaudio2fx.h>

using std::string;
using std::vector;

/*extern "C" // not needed, because the header handles cpp specific loading
already
{
    // specify the original header file that was compiled already
    // because now we're just linking them together via the precompiled object
    // file
}*/
