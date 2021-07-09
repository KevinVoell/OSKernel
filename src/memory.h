#pragma once

#include <stdint.h>
#include "efiMemory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* memoryMap, uint64_t memoryMapEntries, uint64_t memoryMapDescriptorSize);
void memset(void* start, uint8_t value, uint64_t num);