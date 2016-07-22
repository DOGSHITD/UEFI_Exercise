//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

//**********************************************************************
// $Header: /Alaska/Tools/template.c 6     1/13/10 2:13p Felixp $
//
// $Revision: 6 $
//
// $Date: 1/13/10 2:13p $
//**********************************************************************
// Revision History
// ----------------
// $Log: /Alaska/Tools/template.c $
// 
// 6     1/13/10 2:13p Felixp
// 
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:  <This File's Name>
//
// Description:	
//
//<AMI_FHDR_END>
//**********************************************************************

#include <AmiDxeLib.h>
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <BoyceLib\Include\sstd.h>

typedef struct _LOG_DISPLAY_DRIVER_PROTOCOL LOG_DISPLAY_DRIVER_PROTOCOL;

typedef EFI_STATUS (EFIAPI *HOMEWORK_LOG_DISPLAY)(
  IN UINTN                         Value		
);

typedef struct _LOG_DISPLAY_DRIVER_PROTOCOL{
  HOMEWORK_LOG_DISPLAY Log_Display;
};

extern EFI_GUID gLogDisplayDriverProtocolGuid;
/**
EFI_STATUS TestMMap()
{
    EFI_STATUS Status = 0;
    UINTN                       MemoryMapSize = 0;
    EFI_MEMORY_DESCRIPTOR       *MemoryMap = 0;
    UINTN                       MapKey = 0;
    UINTN                       DescriptorSize = 0;
    UINT32                      DescriptorVersion = 0;
    UINTN                       i;
    EFI_MEMORY_DESCRIPTOR       *MMap = 0;
    Status = gBS->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    if(Status != EFI_BUFFER_TOO_SMALL){
        return Status;
    }
    Status = gBS->AllocatePool(EfiBootServicesData, MemoryMapSize, &MemoryMap);
    Status = gBS->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    for( i = 0; i< MemoryMapSize / (DescriptorSize); i++)
    {
        MMap = (EFI_MEMORY_DESCRIPTOR*) ( ((CHAR8*)MemoryMap) + i * DescriptorSize);
        Print(L"MemoryMap %4d %10d :", MMap[0].Type, MMap[0].NumberOfPages);
        Print(L"%10lx<->", MMap[0].PhysicalStart);
        Print(L"%10lx\n",   MMap[0].VirtualStart);
    }
    Status = gBS->FreePool(MemoryMap);
    return Status;
}

EFI_STATUS TestAllocateAddress()
{
    EFI_STATUS Status = 0;
    EFI_PHYSICAL_ADDRESS pages = 1024 *1024 * 10;
    Status = gBS->AllocatePages(AllocateAddress, EfiBootServicesData, 3, &pages);
    Print(L"AllocateAddress:%r %x\n", Status, pages);
    if(Status == 0){
        CHAR16* str;
        str = (CHAR16*) pages;
        str[0] = 'H';
        str[1] = 0;
        Print(L"Test Str:%s\n", str);
        Status = gBS->FreePages(pages, 3);
    }else {
    	Print(L"Failed in Allocate address!\n");
    } 
    return Status;
}
EFI_STATUS TestAllocateAnyPages()
{
    EFI_STATUS Status = 0;
    EFI_PHYSICAL_ADDRESS pages;
    Status = gBS->AllocatePages(AllocateAnyPages, EfiBootServicesData, 3, &pages);
    Print(L"AllocatePages:%r %x\n", Status, pages);
    if(Status == 0){
        CHAR16* str;
        str = (CHAR16*) pages;
        str[0] = 'H';
        str[1] = 0;
        Print(L"Test Str:%s\n", str);
        Status = gBS->FreePages(pages, 3);
    }else {
    	Print(L"Failed in Allocate any pages!\n");
    } 
    return Status;
}
**/
INTN ShellAppMain (IN UINTN Argc, IN CHAR16 **Argv)
/**EFI_STATUS API HomeworkTest(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)**/
{
	EFI_STATUS Status = EFI_SUCCESS;
	LOG_DISPLAY_DRIVER_PROTOCOL *LogDisplayDriverProtocol = NULL;
	UINT32 Value = 0;
//	EFI_TIME Time;
	
	if(Argc!=2 || Argv[1]==NULL){
		Print(L"Please input correct decimal argument!\n");
		return 0;
	}
	Print(L"the argv[1] is 0x%04X\n",StrDecimalToUintn(Argv[1]));
	Value = (UINT32)StrDecimalToUintn(Argv[1]);

	Status = gBS->LocateProtocol(&gLogDisplayDriverProtocolGuid,NULL,&LogDisplayDriverProtocol);	
	if(!EFI_ERROR(Status))
		LogDisplayDriverProtocol->Log_Display(logic_Add(Value,0x01));//Invoke the LibTest in BoyceLib.
/**
	Print(L"Enter 1!\n");
	Status = TestAllocateAnyPages();
	Print(L"Enter 2!\n");
	Status = TestAllocateAddress();
	Print(L"Enter 3!\n");
	Status = TestMMap();
	Print(L"Enter 4!\n");	
	Status = gRT->GetTime(&Time,NULL);	
	Print(L"SystemTime:%04d/%02d/%02d %02d:%02d:%02d\n",\
			Time.Year,Time.Month,Time.Day,Time.Hour,Time.Minute,Time.Second);
**/
	return (INTN)Status;                    
}

/**
EFI_STATUS HomeworkTest(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_STATUS Status;
	LOG_DISPLAY_DRIVER_PROTOCOL *LogDisplayDriverProtocol = NULL;

	InitAmiLib(ImageHandle,SystemTable);
	
	Status = pBS->LocateProtocol(&gLogDisplayDriverProtocolGuid,NULL,&LogDisplayDriverProtocol);
	if(!EFI_ERROR(Status))
		LogDisplayDriverProtocol->Log_Display(0xAA);
	
	return EFI_SUCCESS;
}
**/
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
