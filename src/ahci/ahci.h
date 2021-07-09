#pragma once
#include <stdint.h>
#include "../pci/PCI.h";

namespace AHCI {
    #define HBA_PORT_DEV_PRESENT 0x3
    #define HBA_PORT_IPM_ACTIVE 0x1
    #define SATA_SIG_ATAPI 0xEB140101
    #define SATA_SIG_ATA 0x00000101
    #define SATA_SIG_SEMB 0xC33C0101
    #define SATA_SIG_PM 0x96690101

    #define HBA_PxCMD_CR 0x8000
    #define HBA_PxCMD_FR 0x4000
    #define HBA_PxCMD_FRE 0x0010
    #define HBA_PxCMD_ST 0x0001

    #define ATA_CMD_READ_DMA_EX 0x25
    #define ATA_DEV_BUSY 0x80
    #define ATA_DEV_DRQ 0x08
    #define HBA_PxIS_TFES (1 << 30)

    enum PortType {
        None = 0,
        SATA = 1,
        SEMB = 2,
        PM = 3,
        SATAPI = 4
    };

    enum FIS_TYPE{
        FIS_TYPE_REG_H2D = 0x27,
        FIS_TYPE_REG_D2H = 0x34,
        FIS_TYPE_DMA_ACT = 0x39,
        FIS_TYPE_DMA_SETUP = 0x41,
        FIS_TYPE_DATA = 0x46,
        FIS_TYPE_BIST = 0x58,
        FIS_TYPE_PIO_SETUP = 0x5F,
        FIS_TYPE_DEV_BITS = 0xA1
    };

    struct HBAPort {
        uint32_t CommandListBase;
        uint32_t CommandListBaseUpper;
        uint32_t FISBaseAddress;
        uint32_t FISBaseAddressUpper;
        uint32_t InterruptStatus;
        uint32_t InterruptEnable;
        uint32_t CMDStatus;
        uint32_t rsv0;
        uint32_t TaskFileData;
        uint32_t Signature;
        uint32_t SataStatus;
        uint32_t SataControl;
        uint32_t SataError;
        uint32_t SataActive;
        uint32_t CommandIssue;
        uint32_t SataNotification;
        uint32_t FISSwitchControl;
        uint32_t rsv1[11];
        uint32_t Vendor[4];
    };

    struct HBAMemory {
        uint32_t HostCapability;
        uint32_t GlobalHostControl;
        uint32_t InterruptStatus;
        uint32_t PortsImplemented;
        uint32_t Version;
        uint32_t cccControl;
        uint32_t cccPorts;
        uint32_t EnclosureManagementLocation;
        uint32_t EnclosureManagementControl;
        uint32_t HostCapabilitiesExtended;
        uint32_t BiosHandoffCtrlSts;
        uint8_t rsv0[0x74];
        uint8_t vendor[0x60];
        HBAPort Ports[1];
    };

    class Port {
        public:
        HBAPort* hbaPort;
        PortType portType;
        uint8_t* buffer;
        uint8_t portNumber;
        void Configure();
        void StartCMD();
        void StopCMD();
        bool Read(uint64_t sector, uint32_t sectorCount, void* buffer);
    };

    struct HBACommandHeader{
        uint8_t commandFISLength:5;
        uint8_t atapi:1;
        uint8_t write:1;
        uint8_t fetechable:1;

        uint8_t reset:1;
        uint8_t bist:1;
        uint8_t clearBusy:1;
        uint8_t rsv0:1;
        uint8_t portMultiplier:4;

        uint16_t prdtLength;
        uint32_t prdbCount;
        uint32_t commandTableBaseAddress;
        uint32_t commandTableBaseAddressUpper;
        uint32_t rsv1[4];
    };

    struct FIS_REG_H2D{
        uint8_t fisType;
        uint8_t portMultiplier:4;
        uint8_t rsv0:3;
        uint8_t commandControl:1;

        uint8_t command;
        uint8_t featureLow;

        uint8_t lba0;
        uint8_t lba1;
        uint8_t lba2;
        uint8_t deviceRegister;

        uint8_t lba3;
        uint8_t lba4;
        uint8_t lba5;

        uint8_t featureHigh;
        uint8_t countLow;
        uint8_t countHigh;
        uint8_t isoCommandCompletion;
        uint8_t control;

        uint8_t rsv1[4];
    };

    struct HBAPRDTEntry{
        uint32_t dataBaseAddress;
        uint32_t dataBaseAddressUpper;
        uint32_t rsv0;

        uint32_t byteCount:22;
        uint32_t rsv1:9;
        uint32_t interruptOnCompletion:1;
    };

    struct HBACommandTable{
        uint8_t commandFIS[64];
        uint8_t atapiCommand[16];
        uint8_t rsv[48];

        HBAPRDTEntry prdtEntry[];
    };

    class AHCIDriver{
        public:
        AHCIDriver(PCI::PCIDeviceHeader* pciBaseAddress);
        ~AHCIDriver();
        PCI::PCIDeviceHeader* PCIBaseAddress;
        HBAMemory* ABAR;
        void ProbePorts();
        Port* ports[32];
        uint8_t portCount;
    };
}