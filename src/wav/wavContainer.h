#ifndef WAVCONTAINER_H
#define WAVCONTAINER_H

#include <string>
#include <iostream>
#include <fstream>
#include <ios>
#include "../exception/customException.h"
#include "wavUtil.h"

namespace wav {
    class WavContainer{
        private:
            WavHeader _header;
            void _parseHeader(std::ifstream& wavFile);
        public:
            unsigned int amountSamples;
            uint8_t bytesPerSample;
            int32_t* samples; 
            
            WavContainer(){};
            WavContainer(std::string& fileName);
            WavContainer(char* stream);
            ~WavContainer();

            void parse(std::string& fileName);
            void parse(char* stream);
            inline uint16_t GetBitsPerSample() const {
                return ByteArrayToUInt16LE(this->_header.format.data + 14);
            };
            inline uint32_t GetNumberOfSamples() const {
                return ByteArrayToInt32LE(this->_header.data.size) / (this->GetBitsPerSample() / 8);
            };
    };
}
#endif