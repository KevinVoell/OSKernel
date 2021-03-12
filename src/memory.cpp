#include "memory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* memoryMap, uint64_t memoryMapEntries, uint64_t memoryMapDescriptorSize) {
    static uint64_t memorySizeBytes = 0;

    if (memorySizeBytes > 0) return memorySizeBytes;

    for (int i = 0; i < memoryMapEntries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)memoryMap + (i * memoryMapDescriptorSize));
        memorySizeBytes += desc->numPages * 4096;
    }

    return memorySizeBytes;
}

void memset(void* start, uint8_t value, uint64_t num) {
    for(int i = 0; i < num; i++){
        *(uint8_t*)((uint64_t)start + i) = value;
    }
}