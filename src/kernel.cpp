#include "KernelUtil.h"
#include "UserInput/Mouse.h"
#include "Memory/Heap.h"
#include "Scheduling/PIT/PIT.h"

extern "C" void _start(BootInfo* bootInfo) {
    KernelInfo kernelInfo = InitializeKernel(bootInfo);

    PIT::SetDivisor(20000);
    
    GlobalRenderer->Print("Kernel initialized");
    GlobalRenderer->Next();

    GlobalRenderer->Print(to_hstr((uint64_t)bootInfo->rsdp));

    GlobalRenderer->Next();
    GlobalRenderer->Print(to_hstr((uint64_t)malloc(0x8000)));
    GlobalRenderer->Next();
    void* address = malloc(0x8000);
    GlobalRenderer->Print(to_hstr((uint64_t)address));
    GlobalRenderer->Next();
    GlobalRenderer->Print(to_hstr((uint64_t)malloc(0x100)));
    GlobalRenderer->Next();
    free(address);
    address = malloc(0x8000);
    GlobalRenderer->Print(to_hstr((uint64_t)address));

    for (int i = 0; i < 200; i++)
    {
        GlobalRenderer->Print("g");
        PIT::Sleep(500);
    }

    while(true);
}