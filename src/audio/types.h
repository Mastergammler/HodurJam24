#include "imports.h"

struct AudioMaster
{
    int channel_mask;
    int output_channels;
    int sample_rate;

    IXAudio2* audio_device;
    IXAudio2MasteringVoice* master;
    IXAudio2SubmixVoice* verb_mix;

    bool initalized;
    HRESULT error_code;
    string error_msg;
};
