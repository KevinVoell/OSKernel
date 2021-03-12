#pragma once

#include <stddef.h>
#include "BasicRenderer.h"
#include "cstr.h"
#include "efiMemory.h"
#include "memory.h"
#include "Bitmap.h"
#include "Paging/PageFrameAllocator.h"
#include "Paging/PageMapIndexer.h"
#include "Paging/Paging.h"
#include "Paging/PageTableManager.h"
#include "ACPI.h"
#include "PCI.h"

struct BootInfo {
	FrameBuffer* frameBuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* memMap;
	uint64_t memMapSize;
	uint64_t memDescriptorSize;
    ACPI::RSDP2* rsdp;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo {
    PageTableManager* pageTableManager;
};

KernelInfo InitializeKernel(BootInfo* bootInfo);