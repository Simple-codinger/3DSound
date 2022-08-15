#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "./soundContainer.h"
#include "portaudio.h"
#include "../exception/customException.h"

namespace sound {
    class AudioPlayer {
        private:
            PaError _paMain;
            PaStreamParameters _outputParameters;
            unsigned int _framesPerBuffer;
            void _configureStream();
            static int _audioCallback(const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData);
            static void _streamFinished(void* userData) {};
            struct SignalData{
                sound::SoundContainer* soundContainer;
                unsigned int sampleIndex;
            };
        public:
            AudioPlayer();
            void play(sound::SoundContainer& sc);
    };
}
#endif