#pragma once

#include <stdint.h>

#define IDT_TA_InterruptGate 0b10001110
#define IDT_TA_CallGate 0x10001100
#define IDT_TA_TrapGate 0x10001111

struct IDTDescriptorEntry {
    uint16_t Offset0;
    uint16_t Selector;
    uint8_t IST;
    uint8_t Types_Attr;
    uint16_t Offset1;
    uint32_t Offset2;
    uint32_t Ignore;
    void SetOffset(uint64_t offset);
    uint64_t GetOffset();
};

struct IDTR {
    uint16_t Limit;
    uint64_t Offset;
}__attribute__((packed));