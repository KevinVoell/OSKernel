#include "Mouse.h"
#include "../IO.h"
#include "../BasicRenderer.h"

Point MousePosition;
Point MousePositionOld;

uint8_t MousePointer[] = {
    0b11111111, 0b11000000,
    0b11111111, 0b10000000,
    0b11111110, 0b00000000,
    0b11111100, 0b00000000,
    0b11111000, 0b00000000,
    0b11110000, 0b00000000,
    0b11100000, 0b00000000,
    0b11000000, 0b00000000,
    0b10000000, 0b00000000,
    0b10000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
};

void MouseWait(){
    uint64_t timeout = 100000;
    while(timeout--){
        if (inb(0x64 & 0b10) == 0){
            return;
        }
    }
}

void MouseWaitInput(){
    uint64_t timeout = 100000;
    while(timeout--){
        if (inb(0x64 & 0b1)){
            return;
        }
    }
}

void MouseWrite(uint8_t value){
    MouseWait();
    outb(0x64, 0xD4);
    MouseWait();
    outb(0x60, value);
}

uint8_t MouseRead(){
    MouseWaitInput();
    return inb(0x60);
}

void InitPS2Mouse(){
    outb(0x64, 0xA8); // Enable aux mouse
    MouseWait();
    outb(0x64, 0x20); // 
    MouseWaitInput();
    uint8_t status = inb(0x60);

    status |= 0b10;
    MouseWait();
    outb(0x64, 0x60);
    MouseWait();
    outb(0x60, status);

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
}

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady;
void HandlePS2Mouse(uint8_t data){
    ProcessMousePacket();
    static bool skip = true;

    if (skip) { skip = false; return; }

    switch (MouseCycle)
    {
        case 0:
            if (MousePacketReady) break;
            if ((data & 0b00001000) == 0) break;
            MousePacket[0] = data;
            MouseCycle++;
            break;

        case 1:
            if (MousePacketReady) break;
            MousePacket[1] = data;
            MouseCycle++;
            break;

        case 2:
            if (MousePacketReady) break;
            MousePacket[2] = data;
            MousePacketReady = true;
            MouseCycle = 0;
            break;
    }
}

void ProcessMousePacket(){
    if (!MousePacketReady) return;    

    bool xNeg, yNeg, xOver, yOver;
    if (MousePacket[0] & PS2XSign){
        xNeg = true;
    } else xNeg = false;

    if (MousePacket[0] & PS2YSign){
        yNeg = true;
    } else yNeg = false;

    if (MousePacket[0] & PS2XOverflow){
        xOver = true;
    } else xOver = false;

    if (MousePacket[0] & PS2YOverflow){
        yOver = true;
    } else yOver = false;

    if (!xNeg){
        MousePosition.X += MousePacket[1];
        if (xOver){
            MousePosition.X += 255;
        }
    } else {
        MousePacket[1] = 256 - MousePacket[1];
        MousePosition.X -= MousePacket[1];
        if (xOver){
            MousePosition.X -= 255;
        }
    }

    if (!yNeg){
        MousePosition.Y -= MousePacket[2];
        if (yOver){
            MousePosition.Y-= 255;
        }
    } else {
        MousePacket[2] = 256 - MousePacket[2];
        MousePosition.Y += MousePacket[2];
        if (yOver){
            MousePosition.Y += 255;
        }
    }

    if (MousePosition.X < 0) MousePosition.X = 0;
    if (MousePosition.X > GlobalRenderer->buffer->Width - 1) MousePosition.X = GlobalRenderer->buffer->Width -1;

    if (MousePosition.Y < 0) MousePosition.Y = 0;
    if (MousePosition.Y > GlobalRenderer->buffer->Height - 1) MousePosition.Y = GlobalRenderer->buffer->Height -1;
        
    GlobalRenderer->ClearMouseCursor(MousePointer, MousePositionOld);
    GlobalRenderer->DrawOverlayMousePointer(MousePointer, MousePosition, 0xffffffff);

    if (MousePacket[0] & PS2LeftButton){
        
    }
    if (MousePacket[0] & PS2MiddleButton){
        
    }
    if (MousePacket[0] & PS2RightButton){

    }

    MousePacketReady = false;
    MousePositionOld = MousePosition;
}