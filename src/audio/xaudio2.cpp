#include "module.h"

void Audio_Init()
{
    HRESULT hr;

    // com has to be initalized
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))

    {
        AudioDevice.error_code = hr;
        AudioDevice.error_msg = "COM initalization failed";
        return;
    }

    if (FAILED(hr = XAudio2Create(&AudioDevice.audio_device,
                                  0,
                                  XAUDIO2_DEFAULT_PROCESSOR)))
    {
        AudioDevice.error_msg = "Audio Device initalization failed";
        AudioDevice.error_code = hr;
        return;
    }

    if (FAILED(hr = AudioDevice.audio_device->CreateMasteringVoice(&AudioDevice.master)))
    {
        AudioDevice.error_msg = "Master voice initalization failde";
        AudioDevice.error_code = hr;
        return;
    }

    DWORD channelMask;
    if (FAILED(hr = AudioDevice.master->GetChannelMask(&channelMask)))
    {
        AudioDevice.error_msg = "Unable to determine output channel count";
        AudioDevice.error_code = hr;
    }

    XAUDIO2_VOICE_DETAILS deviceDetails;
    AudioDevice.master->GetVoiceDetails(&deviceDetails);
    AudioDevice.channel_mask = channelMask;
    AudioDevice.output_channels = __builtin_popcount(channelMask);
    AudioDevice.sample_rate = deviceDetails.InputSampleRate;

    Logf("Audio device output channels: %d - sample rate:  %d",
         AudioDevice.output_channels,
         AudioDevice.sample_rate);

    AudioDevice.initalized = true;
}

void Audio_Dispose()
{
    // TODO: release reverb effects etc
    if (AudioDevice.verb_mix) AudioDevice.verb_mix->DestroyVoice();
    if (AudioDevice.master) AudioDevice.master->DestroyVoice();
    if (AudioDevice.audio_device) AudioDevice.audio_device->Release();

    CoUninitialize();
}
