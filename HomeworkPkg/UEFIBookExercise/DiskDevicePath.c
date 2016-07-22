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
#include <Library/UefiLib.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathToText.h>

extern EFI_GUID gEfiDevicePathToTextProtocolGuid;
extern EFI_GUID gEfiDevicePathProtocolGuid;
extern EFI_GUID gEfiDiskIoProtocolGuid;

EFI_STATUS PrintNode(EFI_DEVICE_PATH_PROTOCOL *Node)
{
	Print(L"(%d  %d)/",Node->Type,Node->SubType);
	return 0;
}

EFI_DEVICE_PATH_PROTOCOL* WalkthroughDevicePath(EFI_DEVICE_PATH_PROTOCOL*
		DevPath,EFI_STATUS (*Callbk)(EFI_DEVICE_PATH_PROTOCOL*)){
	EFI_DEVICE_PATH_PROTOCOL* pDevPath = DevPath;
	while(!IsDevicePathEnd(pDevPath)){
		Callbk(pDevPath);
		pDevPath = NextDevicePathNode(pDevPath);
	}
	return pDevPath;
}
EFI_STATUS DiskDevicePath(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{   
	EFI_STATUS Status;
    UINTN HandleIndex,NumHandles;
    EFI_HANDLE *ControllerHandle = NULL;
    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* Device2TextProtocol = 0;
    
	InitAmiLib(ImageHandle,SystemTable);
	
	Status = pBS->LocateProtocol(&gEfiDevicePathToTextProtocolGuid,
			NULL,
			(VOID**)&Device2TextProtocol);
	
	Status = pBS->LocateHandleBuffer(ByProtocol,
			&gEfiDiskIoProtocolGuid,
			NULL,
			&NumHandles,
			&ControllerHandle);
	if(EFI_ERROR(Status)){
		return Status;
	}
	
	for(HandleIndex = 0;HandleIndex < NumHandles;HandleIndex ++){
		EFI_DEVICE_PATH_PROTOCOL* DiskDevicePath;
		Status = pBS->OpenProtocol(ControllerHandle[HandleIndex],
				&gEfiDevicePathProtocolGuid,
				(VOID**)&DiskDevicePath,
				ImageHandle,
				NULL,
				EFI_OPEN_PROTOCOL_GET_PROTOCOL);
		if(EFI_ERROR(Status)){
			continue;
		}
		{
		  CHAR16* TextDevicePath = NULL;
		  TextDevicePath = Device2TextProtocol ->ConvertDevicePathToText(DiskDevicePath,TRUE,TRUE);
		  Print(L"%s\n",TextDevicePath);
		  if(TextDevicePath)
			  pBS->FreePool(TextDevicePath);
		}	
		WalkthroughDevicePath(DiskDevicePath,PrintNode);
		Print(L"\n\n");
	}
	return EFI_SUCCESS;
}

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
