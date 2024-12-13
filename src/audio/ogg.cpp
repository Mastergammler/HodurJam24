#include "module.h"

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

void PlayNewAudio(AudioData* audio)
{
    if (!AudioDevice.initalized)
    {
        Logf("AudioDevice not initialized: %08x - %s",
             AudioDevice.error_code,
             AudioDevice.error_msg.c_str());
        return;
    }

    if (!audio->initalized)
    {
        Logf("Trying to play uninitialized audio: %s",
             audio->file_name.c_str());
        return;
    }

    XAUDIO2_BUFFER audioBuffer = {};
    audioBuffer.pAudioData = audio->data;
    audioBuffer.AudioBytes = audio->byte_count;

    IXAudio2SourceVoice* voice;
    HRESULT hr = AudioDevice.audio_device->CreateSourceVoice(&voice,
                                                             &audio->wfx);
    if (FAILED(hr)) Log("Error creating source voice");

    hr = voice->SubmitSourceBuffer(&audioBuffer);
    if (FAILED(hr)) Log("Error submitting source buffer");

    hr = voice->Start();
    if (FAILED(hr)) Log("Error starting playback");
}
