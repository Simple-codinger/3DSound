#ifndef SOUNDCONTAINER_H
#define SOUNDCONTAINER_H

#include "../../lib/matplotlibcpp.h"
#include <vector>
#include <iostream>

namespace plt = matplotlibcpp;

namespace sound {
    struct SoundContainer
    {
        unsigned int amountSamples;
        uint8_t bytesPerSample;
        uint32_t sampleRate;
        float* samples; 

        void plotSignal() {
            std::cout << "Amount Samples: " << amountSamples << std::endl;
            unsigned int amountPerChannel = amountSamples/2;
            std::cout << "Amount per Channel: " << amountPerChannel << std::endl;
            
            std::vector<double>* xLeft = new std::vector<double>(amountPerChannel);
            std::vector<float>* yLeft = new std::vector<float>(amountPerChannel);

            std::vector<double>* xRight = new std::vector<double>(amountPerChannel);
            std::vector<float>* yRight = new std::vector<float>(amountPerChannel);
            
            (*xLeft).reserve(amountPerChannel);
            (*xRight).reserve(amountPerChannel);
            (*yLeft).reserve(amountPerChannel);
            (*yRight).reserve(amountPerChannel);
            
            for (unsigned int i = 0; i < amountPerChannel; ++i) {
                (*yLeft)[i] = *(samples++);
                (*yRight)[i] = *(samples++);

                double time = i/sampleRate;
                (*xLeft)[i] = time;
                (*xRight)[i] = time;
            }
            
            plt::subplot(1, 2, 1);
            plt::plot(*xLeft, *yLeft, "tab:blue");
            plt::title("Left Channel");
            plt::xlabel("Time in seconds");
            plt::ylabel("Amplitude");

            plt::subplot(1, 2, 2);
            plt::plot(*xRight, *yRight, "tab:blue");
            plt::title("Right Channel");
            plt::xlabel("Time in seconds");
            plt::ylabel("Amplitude");
            plt::show();

            delete xLeft;
            delete yLeft;
            delete xRight;
            delete yRight;
        }
    };
}
#endif