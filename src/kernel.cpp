#include "KernelUtil.h"
#include "UserInput/Mouse.h"
#include "Memory/Heap.h"
#include "Scheduling/PIT/PIT.h"

/*
 Entrypiont from the EUFI bootloader.
 */
extern "C" void _start(BootInfo* bootInfo) {
    KernelInfo kernelInfo = InitializeKernel(bootInfo);

    while(true){
        asm ("hlt");
    }
}