#include "bmp.h"

Image::Image(uint16_t width, uint16_t height) {
    b_width = width;
    b_height = height;
}

void Image::FillingHeaderSize(unsigned char* header, uint16_t index, uint16_t parameter) {
    header[index] = parameter;
    header[++index] = (parameter >> kShiftsBits[0]);
    header[++index] = (parameter >> kShiftsBits[1]);
    header[++index] = (parameter >> kShiftsBits[2]);
}

void FillColorTable(unsigned char* data_header) {
    // White color
    data_header[0] = 255;
    data_header[1] = 255;
    data_header[2] = 255;
    data_header[3] = 0;

    // Green color
    data_header[4] = 0;
    data_header[5] = 255;
    data_header[6] = 0;
    data_header[7] = 0;

    // Purple Color 
    data_header[8] = 255;
    data_header[9] = 0;
    data_header[10] = 143;
    data_header[11] = 0;

    // Yellow color
    data_header[12] = 0;
    data_header[13] = 255;
    data_header[14] = 255;
    data_header[15] = 0;

    // Black color
    data_header[16] = 0;
    data_header[17] = 0;
    data_header[18] = 0;
    data_header[19] = 0;

}

void Image::WriteHeader(uint64_t file_size, std::ofstream& file) {
    unsigned char file_header[kFileHeaderSize] = {0};
    file_header[kFileTypeIndexes[0]] = 'B';
    file_header[kFileTypeIndexes[1]] = 'M';
    FillingHeaderSize(file_header, 2, file_size);
    file_header[kFileDataOffsetIndex] = kFileHeaderDataSize + kFileHeaderSize + kColorsTableSize;
    unsigned char information_header[kFileHeaderDataSize] = {0};
    information_header[0] = kFileHeaderDataSize;
    FillingHeaderSize(information_header, 4, b_width);
    FillingHeaderSize(information_header, 8, b_height);
    information_header[kIndexForPlanes] = kQuantityPlanes;
    information_header[kIndexForBitsPerPixel] = kBitsPerPixel;
    information_header[kIndexForQuantityColors] = kColorsQuantity;
    unsigned char color_table[kColorsTableSize] = {0};
    FillColorTable(color_table);
    file.write(reinterpret_cast<char*>(file_header), kFileHeaderSize);
    file.write(reinterpret_cast<char*>(information_header), kFileHeaderDataSize);
    file.write(reinterpret_cast<char*>(color_table), kColorsTableSize);
}

void Image::Export(const char* path, uint64_t** grid) {
    std::ofstream file_out(path, std::ios::out | std::ios::binary);
    uint64_t half_of_width = std::ceil(static_cast<float> (b_width) / 2);
    uint64_t padding_size = (kPixelHeight - half_of_width % kPixelHeight) % kPixelHeight;
    uint64_t file_size = kFileHeaderSize + kFileHeaderDataSize + kColorsTableSize + b_height * (half_of_width + padding_size);
    WriteHeader(file_size, file_out);
    uint64_t color_first;
    uint64_t color_second;
    uint8_t color;
    unsigned char padding = 0;
    for (int y = 0; y < b_height; ++y) {
        for (int x = 0; x < b_width; x += 2) {
            color_first = grid[(b_height - y - 1)][x];
            if (x + 1 < b_width) {
                color_second = grid[(b_height - y - 1)][x + 1];
            } else {
                color_second = 0;
            }
            if (color_first > (kBlackColorIndex - 1)) {
                color_first = kBlackColorIndex;
            }
            if (color_second > (kBlackColorIndex - 1)) {
                color_second = kBlackColorIndex;
            }
            color = (color_first << 4) | color_second;
            file_out.write(reinterpret_cast<char*>(&color), 1);
        }
        file_out.write(reinterpret_cast<char*>(&padding), padding_size);
    }
    file_out.close();
}