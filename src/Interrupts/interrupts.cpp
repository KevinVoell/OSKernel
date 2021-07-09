#include "interrupts.h"
#include "../Panic.h"
#include "../IO.h"
#include "../cstr.h"
#include "../UserInput/Keyboard.h"
#include "../UserInput/Mouse.h"

__attribute__((interrupt))void PageFaultHandler(interrupt_frame* frame){
    Panic("Page Fault Detected", frame);

    while(true);
}

__attribute__((interrupt))void DoubleFaultHandler(interrupt_frame* frame){
    Panic("Double Fault Detected", frame);
    while(true);
}

__attribute__((interrupt))void GPFaultHandler(interrupt_frame* frame){

    Panic("General Protection Fault Detected", frame);
    while(true);
}

__attribute__((interrupt))void DivideByZeroFaultHandler(interrupt_frame* frame){
    Panic("Divide by Zero Fault Detected", frame);
    while(true);  
}

__attribute__((interrupt))void KBIntHandler(interrupt_frame* frame){
    uint8_t scanCode= inb(0x60);
    
    HandleKeyboard(scanCode);

    PICEndMaster();
}

__attribute__((interrupt))void MouseIntHandler(interrupt_frame* frame){
    uint8_t mouseData= inb(0x60);

    HandlePS2Mouse(mouseData);

    PICEndSlave();
}

__attribute__((interrupt))void PITIntHandler(interrupt_frame* frame){
    PIT::Tick();
    PICEndMaster();
}

void PICEndMaster(){
    outb(PIC1_COMMAND, PIC_EOI);
}

void PICEndSlave(){
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void RemapPIC() {
    uint8_t a1, a2;
    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
    io_wait();
}