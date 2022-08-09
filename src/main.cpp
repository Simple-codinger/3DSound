#include <iostream>
#include <string>
#include <fstream>
#include <ios>
#include <exception>
#include "portaudio.h"

#include "./sound/soundContainer.h"
#include "wav/wavContainer.h"
#include "../lib/matplotlibcpp.h"

#include <math.h>

namespace plt = matplotlibcpp;

#define FRAMES_PER_BUFFER  (64)

int sampleIndex = 0;
sound::SoundContainer soundContainer;

/* This routine will be called by the PortAudio engine when audio is needed.
 * It may called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
*/ 
static int paTestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    int finished = paContinue;
    sound::SoundContainer *data = (sound::SoundContainer*)userData; 
    float *out = (float*)outputBuffer;
    (void) inputBuffer; /* Prevent unused variable warning. */

    int framesLeft = ((data->amountSamples)/2) - sampleIndex;

    if (framesLeft <= framesPerBuffer)
        finished = paComplete;

    for(unsigned long i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->samples[sampleIndex];  /* left */
        *out++ = data->samples[sampleIndex++];  /* right */
    }
    return finished;
}

static void StreamFinished(void* userData) {
    std::cout << "Stream Completed" << std::endl;
}


int main(int ac, char** av) {
    std::string fileName = "dosed.wav";

    

    if (ac > 1) {
        fileName = std::string(av[1]);
    }

    try {
        wav::WavContainer wavContainer(fileName);

        soundContainer = wavContainer.getSound();

        PaError err = Pa_Initialize();

        PaStreamParameters outputParameters;
        PaStream* stream;

        if (err != paNoError) {
            throw PortAudioException(Pa_GetErrorText(err));
            err = Pa_Terminate();
        }

        // configure stream
        outputParameters.device = Pa_GetDefaultOutputDevice();
        if (outputParameters.device == paNoDevice) {
            throw PortAudioException("No audio device found");
            err = Pa_Terminate();
        }

        outputParameters.channelCount = 2;
        outputParameters.sampleFormat = paFloat32;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = nullptr;

        err = Pa_OpenStream(
            &stream,
            nullptr,
            &outputParameters,
            soundContainer.sampleRate*outputParameters.channelCount,
            FRAMES_PER_BUFFER,
            paClipOff,
            paTestCallback,
            &soundContainer
        );

        err = Pa_SetStreamFinishedCallback(stream, &StreamFinished);
        err = Pa_StartStream(stream);

        while((err = Pa_IsStreamActive(stream)) == 1) Pa_Sleep(100);

        err = Pa_CloseStream(stream);

        Pa_Terminate();


    } catch (exception& e) {
        std::cout << e.what() << std::endl;
    }

    plt::detail::_interpreter::kill();
    return 0;
}