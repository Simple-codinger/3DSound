#include <iostream>
#include <string>
#include <fstream>
#include <ios>
#include <exception>
#include "portaudio.h"

#include "sound/soundContainer.h"
#include "sound/audioPlayer.h"
#include "wav/wavContainer.h"
#include "../lib/matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main(int ac, char** av) {
    std::string fileName = "dosed.wav";

    

    if (ac > 1) {
        fileName = std::string(av[1]);
    }

    try {
        wav::WavContainer wavContainer(fileName);
        sound::SoundContainer wavSignal = wavContainer.getSound();
        wavSignal.plotSignal();

        sound::AudioPlayer aPlayer = sound::AudioPlayer();
        aPlayer.play(wavSignal);

    } catch (exception& e) {
        std::cout << e.what() << std::endl;
    }

    plt::detail::_interpreter::kill();
    return 0;
}