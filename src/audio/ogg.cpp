#include "module.h"

void PlayTest(const string filePath)
{
    OggVorbis_File vf;
    if (ov_fopen(filePath.c_str(), &vf) != 0)
    {
        Logf("Unable to open file for playback: %s", filePath.c_str());
        return;
    }

    vorbis_info* vi = ov_info(&vf, -1);

    WAVEFORMATEX wfx = {};
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = vi->channels;
    wfx.nSamplesPerSec = vi->rate;
    wfx.wBitsPerSample = 16; // ogg vorbis is always 16
    wfx.nBlockAlign = wfx.nChannels * (wfx.wBitsPerSample / 8);
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize = 0;

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

    Logf("Loading file with %i channels %i sps and %i bytes",
         wfx.nChannels,
         wfx.nSamplesPerSec,
         buffer.size());

    XAUDIO2_BUFFER audioBuffer = {};
    audioBuffer.pAudioData = (BYTE*)buffer.data();
    audioBuffer.AudioBytes = buffer.size();
    audioBuffer.Flags = XAUDIO2_END_OF_STREAM;

    IXAudio2SourceVoice* voice;
    HRESULT hr = Mixer.audio_device->CreateSourceVoice(&voice, &wfx);
    if (FAILED(hr)) Log("Error creating source voice");

    hr = voice->SubmitSourceBuffer(&audioBuffer);
    if (FAILED(hr)) Log("Error submitting source buffer");

    hr = voice->Start();
    if (FAILED(hr)) Log("Error starting playback");
}
