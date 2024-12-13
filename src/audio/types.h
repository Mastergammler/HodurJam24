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

struct AudioData
{
    WAVEFORMATEX wfx;
    BYTE* data;
    long byte_count;

    string file_name;
    string file_path;

    /**
     * Samples per channel of audio
     */
    int sample_count;
    int channels;
    int samples_per_s;

    float length_in_s;

    bool initalized;
};
