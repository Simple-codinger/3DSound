#include "wavContainer.h"

using namespace wav;

void WavContainer::_parseHeader(std::ifstream& wavFile) {
    wavFile.read((char*)this->_header.id, 4);
    wavFile.read((char*)this->_header.file_size, 4);
    wavFile.read((char*)this->_header.file_format, 4);

    // check header
    if(ByteArrayToInt32BE(this->_header.id) != RIFF || ByteArrayToInt32BE(this->_header.file_format) != WAVE) {
        throw WavParserException();
    }
    uint8_t chunkId [4];
    WriteBytes(this->_header.id, 0, 4, chunkId);

    uint32_t chunkIdVal = ByteArrayToInt32BE(chunkId);
    uint32_t headerSize = 12;

    while (chunkIdVal != DATA) {
        wavFile.read((char*)chunkId, 4);
        chunkIdVal = ByteArrayToInt32BE(chunkId);
        uint32_t dataSize;
        uint8_t* listData = nullptr;
        std::cout << "Read Header: " << chunkId << std::endl;
        switch (chunkIdVal)
        {  
            case FMT:
                WriteBytes(chunkId, 0, 4, this->_header.format.id);
                wavFile.read((char*)this->_header.format.size, 4);
                dataSize = ByteArrayToInt32LE(this->_header.format.size);
                this->_header.format.data = new uint8_t[dataSize];
                wavFile.read((char*)this->_header.format.data, dataSize);
                headerSize += 8 + dataSize;
                break;
            case DATA:
                WriteBytes(chunkId, 0, 4, this->_header.data.id);
                wavFile.read((char*)this->_header.data.size, 4);
                headerSize += 8;
                break;
            case LIST:
                WriteBytes(chunkId, 0, 4, this->_header.list.id);
                wavFile.read((char*)this->_header.list.size, 4);
                wavFile.read((char*)this->_header.list.type, 4);

                // Why -4: because of the type, the first 4 bytes in the data 
                dataSize = ByteArrayToInt32LE(this->_header.list.size) - 4;

                wavFile.ignore(dataSize);
                // +12: id:4, size:4, type: 4
                headerSize += 12 + dataSize;
                break;

        default:
            throw WavParserException();
        }
    }

    this->_header.header_size = headerSize;
}

void WavContainer::parse(std::string& fileName) {
    std::ifstream wavFile (fileName, std::ios::in | std::ios::binary);
    
    // parse header
    this->_parseHeader(wavFile);

    // read sound data    
    uint32_t dataSize = ByteArrayToInt32LE(this->_header.data.size);

    uint8_t* soundData = new uint8_t[dataSize];

    char* soundDataTemp = (char*) soundData;


    uint32_t counter = 0;
    while(wavFile.good()) {
        wavFile.read((char*)soundDataTemp + counter, 1);
        counter++;
    }

    soundDataTemp = nullptr;
    
    // read samples
    uint16_t bps = this->GetBitsPerSample();
    uint32_t numOfSamples = this->GetNumberOfSamples();

    this->samples = new float[numOfSamples];

    for (int i = 0; i < numOfSamples; ++i) {
        if (bps == 16) {
            *(this->samples + i) = (float)(ByteArrayToUInt16LE(soundData + (i*2))/MAX_AMPLITUDE_16);
        } else if (bps == 32) {
            *(this->samples + i) = (float)(ByteArrayToInt32LE(soundData + (i*4))/MAX_AMPLITUDE_32);
        }
    }

    delete[] soundData;
    wavFile.close();
}

WavContainer::WavContainer(std::string& fileName) {
    this->_header = CreateWavHeader();
    this->samples = nullptr;
    this->parse(fileName);
}

WavContainer::WavContainer(char* stream) {
    this->_header = CreateWavHeader();
    //this->parse(stream);
}

WavContainer::~WavContainer() {
    delete[] this->samples;
    delete[] this->_header.format.data;
}