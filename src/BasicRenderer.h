#pragma once

#include <stdint.h>

#include "Math.h"
#include "FrameBuffer.h"
#include "SimpleFonts.h"

class BasicRenderer {
    public:
    Point CursorPosition;
    uint64_t Color;
    uint64_t ClearColor;
    BasicRenderer(FrameBuffer* frameBuffer, PSF1_FONT* font);
    void Print(const char* str);
    void PutChar(char chr, unsigned int xOff, unsigned int yOff);
    void PutChar(char chr);
    void Clear();
    void ClearChar();
    void Next();
    FrameBuffer* buffer;
    void DrawOverlayMousePointer(uint8_t* mouseCursor, Point position, uint32_t color);
    void PutPix(uint32_t x, uint32_t y, uint32_t color);
    uint32_t GetPix(uint32_t x, uint32_t y);
    uint32_t MouseCursorBuffer[16 * 16];
    void ClearMouseCursor(uint8_t* mouseCursor, Point position);
    bool MouseDrawn;
    uint32_t MouseCursorBufferAfter[16 * 16];

    private:
    
    PSF1_FONT* Font;
};

extern BasicRenderer* GlobalRenderer;