#include "module.h"

void Audio_Init()
{
    HRESULT hr;

    // com has to be initalized
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))

    {
        Mixer.error_code = hr;
        Mixer.error_msg = "COM initalization failed";
        return;
    }

    if (FAILED(hr = XAudio2Create(&Mixer.audio_device,
                                  0,
                                  XAUDIO2_DEFAULT_PROCESSOR)))
    {
        Mixer.error_msg = "Audio Device initalization failed";
        Mixer.error_code = hr;
        return;
    }

    if (FAILED(hr = Mixer.audio_device->CreateMasteringVoice(&Mixer.master)))
    {
        Mixer.error_msg = "Master voice initalization failde";
        Mixer.error_code = hr;
        return;
    }

    DWORD channelMask;
    if (FAILED(hr = Mixer.master->GetChannelMask(&channelMask)))
    {
        Mixer.error_msg = "Unable to determine output channel count";
        Mixer.error_code = hr;
    }

    XAUDIO2_VOICE_DETAILS deviceDetails;
    Mixer.master->GetVoiceDetails(&deviceDetails);
    Mixer.channel_mask = channelMask;
    Mixer.output_channels = __builtin_popcount(channelMask);
    Mixer.sample_rate = deviceDetails.InputSampleRate;

    Logf("Audio device output channels: %d - sample rate:  %d",
         Mixer.output_channels,
         Mixer.sample_rate);

    Mixer.initalized = true;
}

void Audio_Dispose()
{
    // TODO: release reverb effects etc
    if (Mixer.verb_mix) Mixer.verb_mix->DestroyVoice();
    if (Mixer.master) Mixer.master->DestroyVoice();
    if (Mixer.audio_device) Mixer.audio_device->Release();

    CoUninitialize();
}
