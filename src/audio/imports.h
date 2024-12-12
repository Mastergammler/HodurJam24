#include <string>
#include <vector>
#include <xaudio2.h>

using std::string;
using std::vector;

extern "C"
{
// specify the original header file that was compiled already
// because now we're just linking them together via the precompiled object file
#include "extern/minivorbis.h"
}
