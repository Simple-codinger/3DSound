#include "./audioPlayer.h"

int sound::AudioPlayer::_audioCallback(   const void *inputBuffer, void *outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *userData){
    int finished = paContinue;
    sound::AudioPlayer::SignalData *data = (sound::AudioPlayer::SignalData*)userData; 
    float *out = (float*)outputBuffer;
    (void) inputBuffer; /* Prevent unused variable warning. */

    int framesLeft = ((data->soundContainer->amountSamples/2)) - data->sampleIndex;

    if (framesLeft <= framesPerBuffer*2)
        finished = paComplete;

    for(unsigned long i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->soundContainer->leftChannelSamples[data->sampleIndex];  /* left */
        *out++ = data->soundContainer->rightChannelSamples[data->sampleIndex++];  /* right */
    }
    return finished;
};

sound::AudioPlayer::AudioPlayer():_framesPerBuffer(64){
    this->_paMain = Pa_Initialize();

    // check if there are any initialization errors
    if (this->_paMain != paNoError) {
        throw PortAudioException(Pa_GetErrorText(this->_paMain));
        this->_paMain = Pa_Terminate();
    }

    this->_configureStream();
};

void sound::AudioPlayer::_configureStream() {
    // configure stream
    this->_outputParameters.device = Pa_GetDefaultOutputDevice();
    if (this->_outputParameters.device == paNoDevice) {
        throw PortAudioException("No audio device found");
        this->_paMain = Pa_Terminate();
    }

    this->_outputParameters.channelCount = 2;
    this->_outputParameters.sampleFormat = paFloat32;
    this->_outputParameters.suggestedLatency = Pa_GetDeviceInfo(this->_outputParameters.device)->defaultLowOutputLatency;
    this->_outputParameters.hostApiSpecificStreamInfo = nullptr;
};

void sound::AudioPlayer::play(sound::SoundContainer& soundContainer) {
    PaStream* stream;

    SignalData sd;
    sd.sampleIndex = 0;
    sd.soundContainer = &soundContainer;

    this->_paMain = Pa_OpenStream(
        &stream,
        nullptr,
        &this->_outputParameters,
        soundContainer.sampleRate,
        this->_framesPerBuffer,
        paClipOff,
        sound::AudioPlayer::_audioCallback,
        &sd
    );

    this->_paMain = Pa_SetStreamFinishedCallback(stream, &sound::AudioPlayer::_streamFinished);
    this->_paMain = Pa_StartStream(stream);

    while((this->_paMain = Pa_IsStreamActive(stream)) == 1) Pa_Sleep(100);

    this->_paMain = Pa_CloseStream(stream);

    Pa_Terminate();
};