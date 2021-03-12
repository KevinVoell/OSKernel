#include "Keyboard.h"

bool IsLeftShiftPressed;
bool IsRightShiftPressed;

void HandleKeyboard(uint8_t scancode){
    
    
    switch (scancode)
    {
    case LeftShift:
        IsLeftShiftPressed = true;
        return;

    case LeftShift + 0x80:
        IsLeftShiftPressed = false;
        return;

    case RightShift:
        IsRightShiftPressed = true;
        return;

    case RightShift + 0x80:
        IsRightShiftPressed = false;
        return;        

    case BackSpace:
        GlobalRenderer->ClearChar();
        return;

    case Enter:
        GlobalRenderer->Next();
        return;
    }

    char ascii = QWERTYKeyboard::Translate(scancode, IsRightShiftPressed || IsLeftShiftPressed);

    if (ascii != 0) {
        GlobalRenderer->PutChar(ascii);
    }
}