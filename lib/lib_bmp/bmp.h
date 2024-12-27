#pragma once
#include <iostream>
#include <fstream>

class Image {
public:
    Image(uint16_t width, uint16_t height);
    void Export(const char* path, uint64_t** grid);
private:
    void WriteHeader(uint64_t file_size, std::ofstream& file);
    void FillingHeaderSize(unsigned char* header, uint16_t index, uint16_t parameter);
    uint16_t b_width;
    uint16_t b_height;
    static const uint8_t kPixelHeight = 4;
    static const uint8_t kFileHeaderSize = 14;
    static const uint8_t kFileHeaderDataSize = 40;
    static const uint8_t kColorsTableSize = 20;
    static const uint8_t kIndexForPlanes = 12;
    static const uint8_t kIndexForBitsPerPixel = 14;
    static const uint8_t kBitsPerPixel = 4;
    static const uint8_t kIndexForQuantityColors = 32;
    static const uint8_t kColorsQuantity = 5;
    static const uint8_t kQuantityPlanes = 1;
    static const uint8_t kBlackColorIndex = 4;
    const uint8_t kFileTypeIndexes[2] = {0, 1};
    const uint8_t kShiftsBits[3] = {8, 16, 24};
    static const uint8_t kFileDataOffsetIndex = 10;
};