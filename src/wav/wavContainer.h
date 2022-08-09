#ifndef WAVCONTAINER_H
#define WAVCONTAINER_H

#include <string>
#include <iostream>
#include <fstream>
#include <ios>
#include "../exception/customException.h"
#include "wavUtil.h"
#include "../sound/soundContainer.h"

#define MAX_AMPLITUDE_32 2147483648.0f
#define MAX_AMPLITUDE_16 32768.0f

namespace wav {
    class WavContainer{
        private:
            WavHeader _header;
            void _parseHeader(std::ifstream& wavFile);
        public:
            unsigned int amountSamples;
            uint8_t bytesPerSample;
            float* samples; 
            
            
            WavContainer(){};
            WavContainer(std::string& fileName);
            WavContainer(char* stream);
            ~WavContainer();

            void parse(std::string& fileName);
            void parse(char* stream);
            inline sound::SoundContainer getSound() {
                sound::SoundContainer sound;

                sound.amountSamples = this->GetNumberOfSamples();
                sound.bytesPerSample = this->GetBitsPerSample()/8;
                sound.sampleRate = this->GetSampleRate();
                sound.samples = this->samples;

                return sound;
            }
            inline uint16_t GetBitsPerSample() const {
                return ByteArrayToUInt16LE(this->_header.format.data + 14);
            };
            inline uint32_t GetNumberOfSamples() const {
                return ByteArrayToInt32LE(this->_header.data.size) / (this->GetBitsPerSample() / 8);
            };
            inline uint32_t GetSampleRate() const {
                return ByteArrayToInt32LE(this->_header.format.data + 4);
            };
    };
}
#endif