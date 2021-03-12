#pragma once

#include "../BasicRenderer.h"
#include "../Scheduling/PIT/PIT.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xa0
#define PIC2_DATA 0xa1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PIC_EOI 0x20

struct interrupt_frame;
__attribute__((interrupt))void PageFaultHandler(interrupt_frame* frame);
__attribute__((interrupt))void DoubleFaultHandler(interrupt_frame* frame);
__attribute__((interrupt))void GPFaultHandler(interrupt_frame* frame);
__attribute__((interrupt))void KBIntHandler(interrupt_frame* frame);
__attribute__((interrupt))void MouseIntHandler(interrupt_frame* frame);
__attribute__((interrupt))void PITIntHandler(interrupt_frame* frame);

void RemapPIC();
void PICEndMaster();
void PICEndSlave();