#include "Paging.h"

void PageDirectoryEntry::SetFlag(PT_Flag flag, bool enabled){
    uint64_t selector = (uint64_t)1 << flag;

    Value &= ~selector;

    if (enabled){
        Value |= selector;
    }
}

bool PageDirectoryEntry::GetFlag(PT_Flag flag){
    uint64_t selector = (uint64_t)1 << flag;
    return Value & selector > 0 ? true : false;
}

uint64_t PageDirectoryEntry::GetAddress(){
    return (Value & 0x000ffffffffff000) >> 12;
}

void PageDirectoryEntry::SetAddress(uint64_t address){
    address &= 0x000000ffffffffff;
    Value &= 0xfff0000000000fff;
    Value |= (address << 12);
}