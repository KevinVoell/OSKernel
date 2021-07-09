#include "Panic.h"
#include "video/BasicRenderer.h"
#include "cstr.h"

void Panic(const char* message, interrupt_frame* frame) {
    GlobalRenderer->ClearColor = 0x00ff0000;
    GlobalRenderer->Clear();
    GlobalRenderer->CursorPosition = {0,0};
    GlobalRenderer->Color = 0;
    GlobalRenderer->PrintLine("Kernel Panic");
    GlobalRenderer->Next();
    GlobalRenderer->PrintLine(message);

    GlobalRenderer->Print("RIP: 0x");
    GlobalRenderer->Print(to_hstr(frame->ip));

    GlobalRenderer->Print(" CS: 0x");
    GlobalRenderer->Print(to_hstr(frame->cs));    
    GlobalRenderer->Next();

    GlobalRenderer->Print("Flags: 0x");
    GlobalRenderer->Print(to_hstr(frame->flags));

    GlobalRenderer->Print(" SP: 0x");
    GlobalRenderer->Print(to_hstr(frame->sp));  
    GlobalRenderer->Next();  

    GlobalRenderer->Print("SS: 0x");
    GlobalRenderer->Print(to_hstr(frame->ss));
}