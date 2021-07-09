#include "KernelUtil.h"
#include "GDT/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/interrupts.h"
#include "IO.h"
#include "UserInput/Mouse.h"
#include "Memory/Heap.h"

KernelInfo kernelInfo;
IDTR idtr;
BasicRenderer r = BasicRenderer(NULL, NULL);

void PrepareMemory(BootInfo* bootInfo){
    GlobalAllocator = PageFrameAllocator();
    GlobalAllocator.ReadEFIMemoryMap(bootInfo->memMap, bootInfo->memMapSize, bootInfo->memDescriptorSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

    GlobalPageTableManager = PageTableManager(PML4);

    uint64_t mapEntries = bootInfo->memMapSize / bootInfo->memDescriptorSize;

    for (uint64_t i = 0; i < GetMemorySize(bootInfo->memMap, mapEntries, bootInfo->memDescriptorSize); i += 0x1000) {
        GlobalPageTableManager.MapMemory((void*)i, (void*)i);
    }

    uint64_t fbBase = (uint64_t)bootInfo->frameBuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->frameBuffer->BufferSize + 0x1000;
    GlobalAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);

    for (uint64_t i = fbBase; i < fbBase + fbSize; i += 0x1000){
        GlobalPageTableManager.MapMemory((void*)i, (void*)i);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));

    kernelInfo.pageTableManager = &GlobalPageTableManager;
}

void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector){
    IDTDescriptorEntry* interrupt = (IDTDescriptorEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescriptorEntry));
    interrupt->SetOffset((uint64_t)handler);
    interrupt->Types_Attr = type_attr;
    interrupt->Selector = selector;
}

/*
 Prepares the OS to handle intrrupts.
 */
void PrepareInterrupts(){
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();

    SetIDTGate((void*)PageFaultHandler, 0xE, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)DoubleFaultHandler, 0x8, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)GPFaultHandler, 0xD, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)KBIntHandler, 0x21, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)MouseIntHandler, 0x2c, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)PITIntHandler, 0x20, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)DivideByZeroFaultHandler, 0x0, IDT_TA_InterruptGate, 0x08);

    // Load the interrupt descriptor table
    asm ("lidt %0" : : "m" (idtr));

    RemapPIC();
}

/*
 Loads the Advanced Configuration and Power Interface and prepares it for use.
 ACPI is used for interfacing with PCI devices.
 */
void PrepareACPI(BootInfo* bootInfo){
    ACPI::SDTHeader* xsdt = (ACPI::SDTHeader*)(bootInfo->rsdp->XSDTAddress);

    ACPI::MCFGHeader* mcfg = (ACPI::MCFGHeader*)ACPI::FindTable(xsdt, (char*)"MCFG");    
    PCI::EnumeratePCI(mcfg);
}

KernelInfo InitializeKernel(BootInfo* bootInfo){
    r = BasicRenderer(bootInfo->frameBuffer, bootInfo->psf1_Font);
    GlobalRenderer = &r;

    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);

    PrepareMemory(bootInfo);

    InitializeHeap((void*)0x0000100000000000, 0x10);

    GlobalRenderer->Clear();
    GlobalRenderer->CursorPosition = {0, 0};
    GlobalRenderer->Print("Kernel starting");
    GlobalRenderer->Next();

    PrepareInterrupts();

    // Intialize basic mouse functionality
    InitPS2Mouse();

    PrepareACPI(bootInfo);

    outb(PIC1_DATA, 0b11111000);
    outb(PIC2_DATA, 0b11101111);

    // Enable interrupts
    asm ("sti");

    return kernelInfo;
}