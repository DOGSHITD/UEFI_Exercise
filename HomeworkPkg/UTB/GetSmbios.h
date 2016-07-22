#ifndef _GET_SMBIOS_H
#define _GET_SMBIOS_H

#include <efi.h>
#include <Library/UefiLib.h>
#include <Include/SmBios.h>

#define STRUCTURE_HANDLE_INVALID            (UINT16)0xFFFF
#define INVALIDE_HANDLE                 	(UINT16)(-1)
#define DMI_INVALID_HANDLE              	0x83
#define DMI_SUCCESS                     	0x00
#define MANUFACTURER_SIGNATURE				"ASUSTeK Computer Inc."
#define MANUFACTURER_SIGNATURE_2			"ASUSTeK COMPUTER INC."
#define SMBIOS_TYPE_01_STRING_AREA			27


typedef struct {
  UINT8   AnchorString[4];
  UINT8   EntryPointStructureChecksum;
  UINT8   EntryPointLength;
  UINT8   MajorVersion;
  UINT8   MinorVersion;
  UINT16  MaxStructureSize;
  UINT8   EntryPointRevision;
  UINT8   FormattedArea[5];
  UINT8   IntermediateAnchorString[5];
  UINT8   IntermediateChecksum;
  UINT16  TableLength;
  UINT32  TableAddress;
  UINT16  NumberOfSmbiosStructures;
  UINT8   SmbiosBcdRevision;
} SMBIOS_STRUCTURE_TABLE;

typedef struct {
  UINT16  Index;
  UINT8   Type;
  UINT16  Handle;
  UINT16  Addr;   // offset from table head
  UINT16  Length; // total structure length
} STRUCTURE_STATISTICS;

CHAR8 * LibGetSmbiosString (IN SMBIOS_STRUCTURE_POINTER *Smbios, IN UINT16 StringNumber);
EFI_STATUS LibGetSmbiosStructure (SMBIOS_STRUCTURE_POINTER * pSmbiosStruct, SMBIOS_STRUCTURE_TABLE * pSmbiosTable,IN OUT UINT16 *Handle, IN OUT UINT8 *Buffer, OUT UINT16 *Length);
EFI_STATUS InitSMBIOSInfo(SMBIOS_STRUCTURE_TABLE ** pSmbiosTable, STRUCTURE_STATISTICS ** pStatisticsTable, SMBIOS_STRUCTURE_POINTER * pSmbiosStruct);

#endif
