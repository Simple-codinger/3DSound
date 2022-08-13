#ifndef AUDIOPLAYER_H
#define AUIDOPLAYER_H

#include "./soundContainer.h"
#include "portaudio.h"
#include "../exception/customException.h"

namespace sound {
    class AudioPlayer {
        private:
            unsigned int _sampleIndex;
            int _audioCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );
        public:
            AudioPlayer(): _sampleIndex(0){};
            void play(sound::SoundContainer& sc);
    };
}
#endif