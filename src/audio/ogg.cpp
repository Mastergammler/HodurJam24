// TODO: separate win specific from other code
#include "module.h"
#include <xaudio2.h>

/**
 * Ogg Vorbis alsways uses 16 bits per sample
 */
static const int OGG_BITS_PER_SAMLE = 16;

void LoadOggAsPcm(AudioData& audio, const string filePath)
{
    Clock fileClock = {};
    Measure_Start(fileClock);

    audio.file_path = filePath;
    audio.file_name = extract_file_name(filePath);

    OggVorbis_File vf;
    if (ov_fopen(filePath.c_str(), &vf) != 0)
    {
        Logf("Could not open ogg file for loading audio: %s", filePath.c_str());
        return;
    }

    vorbis_info* vi = ov_info(&vf, -1);

    audio.wfx.wFormatTag = WAVE_FORMAT_PCM;
    audio.wfx.nChannels = vi->channels;
    audio.wfx.nSamplesPerSec = vi->rate;
    audio.wfx.wBitsPerSample = OGG_BITS_PER_SAMLE;
    audio.wfx.nBlockAlign = audio.wfx.nChannels *
                            (audio.wfx.wBitsPerSample / 8);
    audio.wfx.nAvgBytesPerSec = audio.wfx.nSamplesPerSec *
                                audio.wfx.nBlockAlign;
    audio.wfx.cbSize = 0;

    // TODO: just have a concurrent strip of pre allocated memory to load all
    // audio into
    //  instead of copying and allocating this twice
    vector<char> buffer;
    int bitstream;
    long bytesRead;
    char readBuffer[4096];

    while ((bytesRead = ov_read(&vf,
                                readBuffer,
                                sizeof(readBuffer),
                                0,
                                2,
                                1,
                                &bitstream)) > 0)
    {
        buffer.insert(buffer.end(), readBuffer, readBuffer + bytesRead);
    }

    audio.byte_count = buffer.size();
    audio.data = new BYTE[audio.byte_count];
    for (int i = 0; i < audio.byte_count; i++)
    {
        audio.data[i] = buffer[i];
    }

    audio.channels = audio.wfx.nChannels;
    audio.samples_per_s = audio.wfx.nSamplesPerSec;
    // block align is the byte count per concurrent sample (all channels)
    audio.sample_count = audio.byte_count / audio.wfx.nBlockAlign;
    audio.length_in_s = (float)audio.sample_count / audio.samples_per_s;

    audio.initalized = true;

    float loadingTime = Measure_Elapsed(fileClock);
    Logf("Loading %s (%i x %.2fs) | %.2fms",
         filePath.c_str(),
         audio.channels,
         audio.length_in_s,
         loadingTime);
}

bool ValidateAudio(AudioData* audio)
{
    if (!AudioDevice.initalized)
    {
        Logf("AudioDevice not initialized: %08x - %s",
             AudioDevice.error_code,
             AudioDevice.error_msg.c_str());
        return false;
    }

    if (!audio->initalized)
    {
        Logf("Audio is not initalized: %s", audio->file_name.c_str());
        return false;
    }

    return true;
}

void CreateVoiceForAudio(AudioData* audio, VoiceSettings& settings)
{
    if (!ValidateAudio(audio)) return;

    // the format info is based on the audio we're playing
    settings.voice_wfx = audio->wfx;
    settings.error_code = AudioDevice.audio_device->CreateSourceVoice(
                                            &settings.voice,
                                            &settings.voice_wfx,
                                            XAUDIO2_VOICE_USEFILTER);

    if (FAILED(settings.error_code))
    {
        settings.error_msg = format("Could not create source voice for audio "
                                    "%s",
                                    audio->file_name.c_str());
        return;
    }

    settings.initalized = true;
}

bool ValidatePlayback(AudioData* audio, VoiceSettings* settings)
{
    if (!ValidateAudio(audio)) return false;

    if (settings == NULL)
    {
        Logf("Voice settings are NULL. Aborting playback. (%s)",
             audio->file_name.c_str());
        return false;
    }

    if (!settings->initalized)
    {
        Logf("Voice is not initalized: %s", settings->error_msg.c_str());
        return false;
    }

    // NOTE: the FormatTag and BitsPerSample also need to stay the same
    //  but since im only using PCM and ogg files these naturally never change
    if (settings->voice_wfx.nChannels != audio->channels)
    {
        Logf("Audio and voice have different amount of channels: %i (audio) : "
             "%i (voice)",
             audio->channels,
             settings->voice_wfx.nChannels);
        return false;
    }

    return true;
}

/**
 * from -1 (full left) to 1.0 (full right)
 * 0 for center
 */
void PanVoice(VoiceSettings* settings, float pan)
{
    int voiceChannels = settings->voice_wfx.nChannels;

    if (pan > 1) pan = 1;
    if (pan < -1) pan = -1;

    float left = 0.5f - pan / 2;
    float right = 0.5f + pan / 2;

    float* outputMatrix = new float[voiceChannels *
                                    AudioDevice.output_channels];

    if (voiceChannels == 1)
    {
        outputMatrix[0] = left;
        outputMatrix[1] = right;
    }
    else if (voiceChannels == 2)
    {
        outputMatrix[0] = left;
        // left source to right output
        outputMatrix[1] = 0;
        // right source to left output
        outputMatrix[2] = 0;
        outputMatrix[3] = right;
    }
    else
    {
        Log("Panning for more than 2 channels is not implemented!");
    }

    HRESULT hr = settings->voice->SetOutputMatrix(NULL,
                                                  voiceChannels,
                                                  AudioDevice.output_channels,
                                                  outputMatrix);
    if (FAILED(hr))
    {
        string msg = system_category().message(hr);
        Logf("Setting output matrix failed! %d: %s", hr, msg.c_str());
    }
}

void ApplyLowpass(VoiceSettings* settings, float lowpassValue)
{
    XAUDIO2_FILTER_PARAMETERS filterParams;
    filterParams.Type = LowPassFilter;
    // cutoff frequency coefficient from 0 - 1
    filterParams.Frequency = lowpassValue;
    // steepness / shelf?
    filterParams.OneOverQ = 1;

    HRESULT hr = settings->voice->SetFilterParameters(&filterParams);
    if (FAILED(hr))
    {
        string msg = system_category().message(hr);
        Logf("Setting lowpass filter with value %.2f failed %d: %s",
             lowpassValue,
             hr,
             msg.c_str());
    }
}

void StopAudio(VoiceSettings& settings)
{
    if (settings.voice)
    {
        HRESULT hr;
        hr = settings.voice->Stop();
        if (FAILED(hr)) Log("Error stoping currently playing audio!");

        hr = settings.voice->FlushSourceBuffers();
        if (FAILED(hr)) Log("Error flushing source buffers!");
    }
}

void PlayAudio(AudioData* audio, PlaybackSettings playback)
{
    if (!ValidatePlayback(audio, playback.settings)) return;

    XAUDIO2_BUFFER audioBuffer = {};
    audioBuffer.pAudioData = audio->data;
    audioBuffer.AudioBytes = audio->byte_count;

    if (playback.looping)
    {
        audioBuffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    IXAudio2SourceVoice* voice = playback.settings->voice;

    HRESULT hr;
    if (playback.interrupt_previous)
    {
        hr = voice->Stop();
        if (FAILED(hr)) Log("Error stoping currently playing audio!");

        hr = voice->FlushSourceBuffers();
        if (FAILED(hr)) Log("Error flushing source buffers!");
    }

    hr = voice->SubmitSourceBuffer(&audioBuffer);
    if (FAILED(hr))
        Logf("Error submitting source buffer for file: %s",
             audio->file_name.c_str());

    hr = voice->SetVolume(playback.volume);
    if (FAILED(hr))
        Logf("Error setting volume to %.2f for audio %s",
             playback.volume,
             audio->file_name.c_str());

    PanVoice(playback.settings, playback.pan);
    ApplyLowpass(playback.settings, playback.lowpass_filter);

    hr = voice->Start();
    if (FAILED(hr))
        Logf("Error starting playback for audio %s", audio->file_name.c_str());
}
