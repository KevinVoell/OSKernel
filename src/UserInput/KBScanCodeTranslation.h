#pragma once
#include <stdint.h>

namespace QWERTYKeyboard {
    #define LeftShift 0x2A
    #define RightShift 0x36
    #define Enter 0x1C
    #define BackSpace 0x0E
    
    extern const char* ASCIITABLE[];
    char Translate(uint8_t scancode, bool uppercase);

}