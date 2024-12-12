#include "module.h"

void Audio_Init()
{
    HRESULT hr;

    // com has to be initalized
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))

    {
        Audio.error_code = hr;
        Audio.error_msg = "COM initalization failed";
        return;
    }

    if (FAILED(hr = XAudio2Create(&Audio.audio_device,
                                  0,
                                  XAUDIO2_DEFAULT_PROCESSOR)))
    {
        Audio.error_msg = "Audio Device initalization failed";
        Audio.error_code = hr;
        return;
    }

    if (FAILED(hr = Audio.audio_device->CreateMasteringVoice(&Audio.master)))
    {
        Audio.error_msg = "Master voice initalization failde";
        Audio.error_code = hr;
        return;
    }

    DWORD channelMask;
    if (FAILED(hr = Audio.master->GetChannelMask(&channelMask)))
    {
        Audio.error_msg = "Unable to determine output channel count";
        Audio.error_code = hr;
    }

    XAUDIO2_VOICE_DETAILS deviceDetails;
    Audio.master->GetVoiceDetails(&deviceDetails);
    Audio.channel_mask = channelMask;
    Audio.output_channels = __builtin_popcount(channelMask);
    Audio.sample_rate = deviceDetails.InputSampleRate;

    Logf("Audio device output channels: %d - sample rate:  %d",
         Audio.output_channels,
         Audio.sample_rate);

    Audio.initalized = true;
}

void Audio_Dispose()
{
    // TODO: release reverb effects etc
    if (Audio.verb_mix) Audio.verb_mix->DestroyVoice();
    if (Audio.master) Audio.master->DestroyVoice();
    if (Audio.audio_device) Audio.audio_device->Release();

    CoUninitialize();
}
