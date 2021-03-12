#include "ACPI.h"

namespace ACPI {
    void* FindTable(ACPI::SDTHeader* sdtHeader, char* signature){
        int entries = (sdtHeader->Length - sizeof(ACPI::SDTHeader)) / 8;

        for (int i = 0; i < entries; i++){
            ACPI::SDTHeader* newSDTHeader = (ACPI::SDTHeader*)*(uint64_t*)((uint64_t)sdtHeader + sizeof(ACPI::SDTHeader) + (i * 8));
            for(int t = 0; t < 4; t++){
                if (newSDTHeader->Signature[t] != signature[t]){
                    break;
                }
                if (i == 3) return newSDTHeader;
            }
        }

        return 0;
    }
}