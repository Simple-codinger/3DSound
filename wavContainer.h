#ifndef WAVCONTAINER_H
#define WAVCONTAINER_H

#include <string>
#include <iostream>
#include <fstream>
#include <ios>
#include "exception/customException.h"

typedef struct  WavHeader
    {
        /* RIFF Chunk Descriptor */
        uint8_t         ChunkId[4];        // RIFF Header Magic header
        uint32_t        ChunkSize;      // RIFF Chunk Size
        uint8_t         ChunkFormat[4];        // WAVE Header
        /* "fmt" sub-chunk */
        uint8_t         Subchunk1Id[4];         // FMT header
        uint32_t        Subchunk1Size;  // Size of the fmt chunk
        uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
        uint16_t        NumChannels;      // Number of channels 1=Mono 2=Sterio
        uint32_t        SampleRate;  // Sampling Frequency in Hz
        uint32_t        ByteRate;    // bytes per second
        uint16_t        BlockAlign;     // 2=16-bit mono, 4=16-bit stereo
        uint16_t        BitsPerSample;  // Number of bits per sample
        /* "data" sub-chunk */
        uint8_t         Subchunk2Id[4]; // "data"  string
        uint32_t        Subchunk2Size;  // Sampled data length
    } WavHeader;

class WavContainer{
    private:
        WavHeader _header;
        unsigned int _amountSamples;
    public:
        inline void setHeader(WavHeader header) {this->_header = header;};
        inline void setAmountSamples(unsigned int amountSamples) {this->_amountSamples = amountSamples;};

        static WavContainer parse(std::string& fileName);
        static WavContainer parse(char* stream);
};
#endif