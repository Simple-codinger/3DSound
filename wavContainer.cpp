#include "wavContainer.h"

void WavContainer::parse(std::string& fileName) {
    std::ifstream wavFile (fileName, std::ios::in | std::ios::binary);

    WavHeader header;
    int headerSize = sizeof(header);
    wavFile.read((char*)&header, headerSize);

    // check header
    if(std::string(header.ChunkId, header.ChunkId + 4).compare("RIFF") || std::string(header.ChunkFormat, header.ChunkFormat + 4).compare("WAVE")) {
        throw WavParserException();
    }

    this->_header = header;
    
    // read samples
    unsigned short bytesPerSample = header.BitsPerSample / 8;
    this->amountSamples = header.Subchunk2Size / bytesPerSample; //How many samples are in the wav file?



}