#include "PCI.h"
#include "../ahci/ahci.h"
#include "../Memory/Heap.h"

namespace PCI{
    void EnumerateFunction(uint64_t deviceAddress, uint64_t function){
        uint64_t offset = function << 12;
        uint64_t functionAddress = deviceAddress + offset;

        GlobalPageTableManager.MapMemory((void*)functionAddress, (void*)functionAddress);

        PCIDeviceHeader* deviceHeader = (PCIDeviceHeader*)functionAddress;

        if (deviceHeader->DeviceID == 0) return;
        if (deviceHeader->DeviceID == 0xFFFF) return;

        GlobalRenderer->Print(GetVendorName(deviceHeader->VendorID));
        GlobalRenderer->Print(" / ");
        GlobalRenderer->Print(GetDeviceName(deviceHeader->VendorID, deviceHeader->DeviceID));
        GlobalRenderer->Print(" / ");
        GlobalRenderer->Print(DeviceClasses[deviceHeader->Class]);
        GlobalRenderer->Print(" / ");
        GlobalRenderer->Print(GetSubclassName(deviceHeader->Class, deviceHeader->Subclass));
        GlobalRenderer->Print(" / ");
        GlobalRenderer->Print(GetProgIFName(deviceHeader->Class, deviceHeader->Subclass, deviceHeader->ProgIF));
        GlobalRenderer->Next();

        switch (deviceHeader->Class){
            case 0x01:
                switch (deviceHeader->Subclass){
                    case 0x06:
                        switch (deviceHeader->ProgIF){
                            case 0x01:
                                new AHCI::AHCIDriver(deviceHeader);
                        }
                }
        }
    }

    void EnumerateDevice(uint64_t busAddress, uint64_t device){
        uint64_t offset = device << 15;
        uint64_t deviceAddress = busAddress + offset;

        GlobalPageTableManager.MapMemory((void*)deviceAddress, (void*)deviceAddress);

        PCIDeviceHeader* deviceHeader = (PCIDeviceHeader*)deviceAddress;

        if (deviceHeader->DeviceID == 0) return;
        if (deviceHeader->DeviceID == 0xFFFF) return;

        for (uint64_t function = 0; function < 32; function++){
            EnumerateFunction(deviceAddress, function);
        }
    }

    void EnumerateBus(uint64_t baseAddress, uint64_t bus){
        uint64_t offset = bus << 20;
        uint64_t busAddress = baseAddress + offset;

        GlobalPageTableManager.MapMemory((void*)busAddress, (void*)busAddress);

        PCIDeviceHeader* deviceHeader = (PCIDeviceHeader*)busAddress;

        if (deviceHeader->DeviceID == 0) return;
        if (deviceHeader->DeviceID == 0xFFFF) return;

        for (uint64_t device = 0; device < 32; device++){
            EnumerateDevice(busAddress, device);
        }
    }

    void EnumeratePCI(ACPI::MCFGHeader* mcfg){
        int entries = ((mcfg->Header.Length) - sizeof(ACPI::MCFGHeader)) / sizeof(ACPI::DeviceConfig);
        for (int t = 0; t < entries; t++){
            ACPI::DeviceConfig* newDeviceConfig = (ACPI::DeviceConfig*)((uint64_t)mcfg + sizeof(ACPI::MCFGHeader) + (sizeof(ACPI::DeviceConfig) * t));
            for (uint64_t bus = newDeviceConfig->StartBus; bus < newDeviceConfig->EndBus; bus++){
                EnumerateBus(newDeviceConfig->BaseAddress, bus);
            }
        }
    }
}