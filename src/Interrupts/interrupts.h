#pragma once
#include <stddef.h>

#include "../video/BasicRenderer.h"
#include "../Scheduling/PIT/PIT.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xa0
#define PIC2_DATA 0xa1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PIC_EOI 0x20

struct interrupt_frame {
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
};

__attribute__((interrupt))void PageFaultHandler(interrupt_frame* frame);
__attribute__((interrupt))void DoubleFaultHandler(interrupt_frame* frame);
__attribute__((interrupt))void DivideByZeroFaultHandler(interrupt_frame* frame);
__attribute__((interrupt))void GPFaultHandler(interrupt_frame* frame);
__attribute__((interrupt))void KBIntHandler(interrupt_frame* frame);
__attribute__((interrupt))void MouseIntHandler(interrupt_frame* frame);
__attribute__((interrupt))void PITIntHandler(interrupt_frame* frame);

void RemapPIC();
void PICEndMaster();
void PICEndSlave();