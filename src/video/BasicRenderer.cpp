#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer(FrameBuffer* frameBuffer, PSF1_FONT* font) {
    buffer = frameBuffer;
    Font = font;
    Color = 0xffffffff;
    ClearColor = 0x00000000;
}

void BasicRenderer::Print(const char* str) {
    char* chr = (char*)str;
    
    while(*chr != 0) {
        PutChar(*chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X += 8;
        if (CursorPosition.X + 8 > buffer->Width){
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }
        chr++;
    }
}

void BasicRenderer::PrintLine(const char* str) {
    Print(str);
    Next();
}

void BasicRenderer::PutChar(char chr, unsigned int xOff, unsigned int yOff) {
    unsigned int* pixPtr = (unsigned int*)buffer->BaseAddress;
    char* fontPtr = (char*)Font->glyphBuffer + (chr * Font->psf1_header->charsize);

    for (unsigned long y = yOff; y < yOff + 16; y++) {
        for (unsigned long x = xOff; x < xOff + 8; x++) {
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
                *(unsigned int*)(pixPtr + x + (y * buffer->PixelsPerScanLine)) = Color;
            }
        }

        fontPtr++;
    }
}

void BasicRenderer::PutChar(char chr) {
    PutChar(chr, CursorPosition.X, CursorPosition.Y);

    CursorPosition.X += 8;
    if (CursorPosition.X + 8 > buffer->Width){
        CursorPosition.X = 0;
        CursorPosition.Y += 16;
    }
}

void BasicRenderer::ClearChar(){
    if (CursorPosition.X == 0){
        CursorPosition.X = buffer->Width;
        CursorPosition.Y -= 16;
        if (CursorPosition.Y < 0){
            CursorPosition.Y = 0;
        }
    }

    unsigned int xOff = CursorPosition.X;
    unsigned int yOff = CursorPosition.Y;
    unsigned int* pixPtr = (unsigned int*)buffer->BaseAddress;

    for (unsigned long y = yOff; y < yOff + 16; y++) {
        for (unsigned long x = xOff - 8; x < xOff; x++) {
            *(unsigned int*)(pixPtr + x + (y * buffer->PixelsPerScanLine)) = ClearColor;
        }
    }

    CursorPosition.X -= 8;
    if (CursorPosition.X < 0){
        CursorPosition.X = buffer->Width;
        CursorPosition.Y -= 16;
        if (CursorPosition.Y < 0){
            CursorPosition.Y = 0;
        }
    }
}

void BasicRenderer::Clear(){
    uint64_t fbBase = (uint64_t)buffer->BaseAddress;
    uint64_t bytesPerScanLine = buffer->PixelsPerScanLine * 4;
    uint64_t fbHeight = buffer->Height;
    uint64_t fbSize = buffer->BufferSize;

    for (int verticalScanLine = 0; verticalScanLine < fbHeight; verticalScanLine++){
        uint64_t pixPointerBase = fbBase + (bytesPerScanLine * verticalScanLine);
        for (uint32_t* pixPtr = (uint32_t*)pixPointerBase; pixPtr < (uint32_t*)(pixPointerBase + bytesPerScanLine); pixPtr++){
            *pixPtr = ClearColor;
        }
    }
}

void BasicRenderer::Next(){
    CursorPosition.X = 0;
    CursorPosition.Y += 16;
}