#ifndef WAVUTIL_H
#define WAVUTIL_H

#define RIFF 0x52494646	// RIFF
#define FMT  0x666D7420	// fmt 
#define WAVE 0x57415645	// WAVE
#define DATA 0x64617461	// data
#define LIST 0x4C495354	// LIST

#include <cstdint>

namespace wav {
    struct RiffChunk
    {
        uint8_t id[4];
        uint8_t size[4];
        uint8_t* data;			
    };

    struct ListChunk
    {
        uint8_t id[4];
        uint8_t size[4];
        uint8_t type[4];
        RiffChunk* data;
        uint16_t numOfChunks;
    };

    /*
    * Struct based on the RIFF WAV format
    */
    struct WavHeader
    {
        uint8_t id[4];
        uint8_t file_size[4];
        uint8_t file_format[4];
        RiffChunk format;
        RiffChunk data;
        ListChunk list;

        uint32_t header_size;
    };

    inline int32_t ByteArrayToInt32BE(const uint8_t byte[4]){
        return byte[3] | byte[2] << 8 | byte[1] << 16 | byte[0] << 24;
    };

    inline int16_t ByteArrayToUInt16LE(const uint8_t byte[2]) {
        return byte[0] | byte[1] << 8;
    }

    inline int32_t ByteArrayToInt32LE(const uint8_t byte[4]){
        return byte[0] | byte[1] << 8 | byte[2] << 16 | byte[3] << 24;
    };

    inline WavHeader CreateWavHeader() {
        return (WavHeader){
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                nullptr
            },
            {
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                nullptr
            },
            {
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                nullptr
            },
            0
        };
    };

    inline void WriteBytes(const uint8_t* data, const int32_t offset, const uint32_t numOfBytes, uint8_t* outContainer) {
        for (int i = 0; i < numOfBytes; ++i) {
            *(outContainer + i) = *(data + offset + i);
        }
    };
}
#endif