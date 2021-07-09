#pragma once

#include <stddef.h>
#include <stdint.h>

namespace BMPImg{
    struct Header{
        uint16_t Signature;
        uint32_t FileSize;
        uint32_t Reserved;
        uint32_t DataOffset;
    };

    struct InfoHeader {
        uint32_t Size;
        uint32_t Width;
        uint32_t Height;
        uint16_t Planes;
        uint16_t BitsPerPixel;
        uint32_t Compression;
        uint32_t ImageSize;
        uint32_t XPixelsPerM;
        uint32_t YPixelsPerM;
        uint32_t ColorsUsed;
        uint32_t ImportedColors;
    };

    struct ColorTable{
        uint8_t Red;
        uint8_t Green;
        uint8_t Blue;
        uint8_t Reserved;
    };

    struct Bmp{
        Header header;
        InfoHeader infoHeader;
        void* colorTable;
        void* pixelData;
    };

    void DisplayBmp(BMPImg::Bmp* bmp);
};