#include "BasicRenderer.h"

/*
 Draws a pixel to the screen at the specified x and y position.
 */
void BasicRenderer::PutPix(uint32_t x, uint32_t y, uint32_t color){
    *(uint32_t*)((uint64_t)buffer->BaseAddress + (x*4) + (y * buffer->PixelsPerScanLine * 4)) = color;
}

uint32_t BasicRenderer::GetPix(uint32_t x, uint32_t y){
    return *(uint32_t*)((uint64_t)buffer->BaseAddress + (x*4) + (y * buffer->PixelsPerScanLine * 4));
}

void BasicRenderer::ClearMouseCursor(uint8_t* mouseCursor, Point position){
    if (!MouseDrawn) return;

    int xMax = 16;
    int yMax = 16;

    int differenceX = buffer->Width - position.X;
    int differenceY = buffer->Height - position.Y;

    if (differenceX < 16) xMax = differenceX;
    if (differenceY < 16) yMax = differenceY;

    for (int y = 0; y < yMax; y++){
        for (int x = 0; x < xMax; x++){
            int bit = y * 16 + x;
            int byte = bit / 8;

            if (mouseCursor[byte] & (0b100000000 >> (x % 8))){
                if (GetPix(position.X + x, position.Y + y) == MouseCursorBufferAfter[x + y * 16]){
                    PutPix(position.X + x, position.Y + y, MouseCursorBuffer[x + y * 16]);
                }
            }
        }
    }
}

void BasicRenderer::DrawOverlayMousePointer(uint8_t* mouseCursor, Point position, uint32_t color){
    int xMax = 16;
    int yMax = 16;

    int differenceX = buffer->Width - position.X;
    int differenceY = buffer->Height - position.Y;

    if (differenceX < 16) xMax = differenceX;
    if (differenceY < 16) yMax = differenceY;

    for (int y = 0; y < yMax; y++){
        for (int x = 0; x < xMax; x++){
            int bit = y * 16 + x;
            int byte = bit / 8;

            if (mouseCursor[byte] & (0b100000000 >> (x % 8))){
                MouseCursorBuffer[x + y * 16] = GetPix(position.X + x, position.Y + y);
                PutPix(position.X + x, position.Y + y, color);
                MouseCursorBufferAfter[x + y * 16] = GetPix(position.X + x, position.Y + y);
            }
        }
    }

    MouseDrawn = true;
}