
#include <Library/ShellLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

#include "GetSmbios.h"

char * LibGetSmbiosString (SMBIOS_STRUCTURE_POINTER *Smbios,UINT16 StringNumber)
{
  UINT16  Index;
  CHAR8   *String;
  
  ASSERT (Smbios != NULL);

  // Skip over formatted section
  String = (CHAR8 *) (Smbios->Raw + Smbios->Hdr->Length);

  // Look through unformated section
  for (Index=1; Index<=StringNumber; Index++) 
  {
    if (StringNumber == Index) 
      return String;

    // Skip string
    for (; *String != 0; String++);
		String++;

    if (*String == 0) 
	{
      // If double NULL then we are done.
      //  Retrun pointer to next structure in Smbios.
      //  if you pass in a -1 you will always get here
      Smbios->Raw = (UINT8 *)++String;
      return NULL;
    }
  }
  return NULL;
}
EFI_STATUS LibGetSystemConfigurationTable(
    IN EFI_GUID *TableGuid,
    IN OUT VOID **Table )
{
    UINTN Index;

    *Table = NULL;

    for ( Index = 0; Index < gST->NumberOfTableEntries; Index++ )
    {
        if ( !CompareMem( TableGuid, &(gST->ConfigurationTable[Index].VendorGuid),
                      sizeof(EFI_GUID)))
        {
            *Table = gST->ConfigurationTable[Index].VendorTable;
            return EFI_SUCCESS;
        }
    }

    return EFI_NOT_FOUND;
}
EFI_STATUS LibGetSmbiosStructure (SMBIOS_STRUCTURE_POINTER * pSmbiosStruct, SMBIOS_STRUCTURE_TABLE * pSmbiosTable,IN OUT UINT16 *Handle, IN OUT UINT8 *Buffer, OUT UINT16 *Length)
{
  SMBIOS_STRUCTURE_POINTER  Smbios;
  SMBIOS_STRUCTURE_POINTER  SmbiosEnd;
  UINT8                     *Raw;

  
  if (*Handle == INVALIDE_HANDLE) 
  {
    *Handle = pSmbiosStruct->Hdr->Handle;
    return DMI_INVALID_HANDLE;
  }

  if (Buffer == NULL) 
	return DMI_INVALID_HANDLE;

  *Length       = 0;
  Smbios.Hdr    = pSmbiosStruct->Hdr;						//head
  SmbiosEnd.Raw = Smbios.Raw + pSmbiosTable->TableLength;	//tail
  
  while (Smbios.Raw < SmbiosEnd.Raw) 
  {
    if (Smbios.Hdr->Handle == *Handle) 
	{
      Raw = Smbios.Raw;
      
      // Walk to next structure
      LibGetSmbiosString(&Smbios, (UINT16)(-1));
      
      // Length = Next structure head - this structure head
      *Length = (UINT16)(Smbios.Raw - Raw);
      CopyMem(Buffer, Raw, *Length);

      // update with the next structure handle.
      if (Smbios.Raw < SmbiosEnd.Raw) 
	  {
        *Handle = Smbios.Hdr->Handle;	//Next Handle
      } 
	  else 
	  {
        *Handle = INVALIDE_HANDLE;
      }
	  
      return DMI_SUCCESS;
    }

    // Walk to next structure
    LibGetSmbiosString (&Smbios, (UINT16)(-1));
  }

  *Handle = INVALIDE_HANDLE;
  return DMI_INVALID_HANDLE;
}

EFI_STATUS InitSMBIOSInfo(SMBIOS_STRUCTURE_TABLE ** pSmbiosTable, STRUCTURE_STATISTICS ** pStatisticsTable, SMBIOS_STRUCTURE_POINTER * pSmbiosStruct)
{
	EFI_STATUS					Status;
	UINT16						Handle;
	UINT8                     	Buffer[512];
	UINT16                    	Length;
	UINT16                    	Offset;
	UINT16                    	Index;
	//EFI_GUID  			EfiSmbiosTableGuid = gEfiSmbiosTableGuid;
	SMBIOS_STRUCTURE_POINTER  	pStruct;
	STRUCTURE_STATISTICS      	*pStatistics;
		
	Status = LibGetSystemConfigurationTable (&gEfiSmbiosTableGuid, pSmbiosTable);
	if ((*pSmbiosTable) == NULL) 
		return EFI_NOT_FOUND;
    
	if (EFI_ERROR (Status)) 
		return Status;
    	
	pSmbiosStruct->Raw  = (UINT8 *) (UINTN) ((*pSmbiosTable)->TableAddress);

	if (CompareMem ((*pSmbiosTable)->AnchorString, "_SM_", 4) != 0) 
		return EFI_INVALID_PARAMETER;
		
	*pStatisticsTable = (STRUCTURE_STATISTICS *) AllocatePool ((*pSmbiosTable)->NumberOfSmbiosStructures * sizeof (STRUCTURE_STATISTICS));
	
	if (*pStatisticsTable == NULL) 
		return EFI_OUT_OF_RESOURCES;
		
	Offset      = 0;

	Handle = STRUCTURE_HANDLE_INVALID;
	LibGetSmbiosStructure (pSmbiosStruct, (*pSmbiosTable), &Handle, NULL, NULL);
	
	for (Index = 0; Index < (*pSmbiosTable)->NumberOfSmbiosStructures; Index++) 
	{
		pStatistics         = *pStatisticsTable + Index;
        // If reach the end of table, break..
        if (Handle == STRUCTURE_HANDLE_INVALID) 
			break;
    
		// After LibGetSmbiosStructure(), handle then point to the next!
        if (LibGetSmbiosStructure (pSmbiosStruct, (*pSmbiosTable), &Handle, Buffer, &Length) != DMI_SUCCESS)
			break;
        
		pStruct.Raw = Buffer;
		Offset      = (UINT16) (Offset + Length);

		// general statistics
		pStatistics->Index  = Index;
		pStatistics->Type   = pStruct.Hdr->Type;
		pStatistics->Handle = pStruct.Hdr->Handle;
		pStatistics->Length = Length;
		pStatistics->Addr   = Offset - Length;
    }

	return EFI_SUCCESS;
}
