
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

#include <AmiLib.h>
#include <AmiDxeLib.h>
#include <Setup.h>
#include <Protocol\PciIo.h>
#include <Protocol\DevicePath.h>
#include <Protocol\PciRootBridgeIo.h>
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
#define DEVICE_ID_GUID \
  { \
    0x76d8e5b6, 0x283a, 0x4b2b, {0x94, 0x27, 0x6a, 0xda, 0xd5, 0x2f, 0x3e, 0x36} \
  }

VOID InitSetupStrings (
    IN EFI_HII_HANDLE   HiiHandle,
    IN UINT16           Class )
{
    EFI_STATUS          Status = EFI_SUCCESS;
    UINT32              DeviceID;
    EFI_GUID            DeviceIDGuid=DEVICE_ID_GUID;
    UINTN               Size=sizeof(DeviceID);
    
    Status = pRS->GetVariable (
	              L"Device_ID",
	              &DeviceIDGuid,
	              NULL,
	              &Size,
	              &DeviceID
	              );
    ASSERT_EFI_ERROR(Status);
    
    if (Class == ADVANCED_FORM_SET_CLASS) {
	    InitString(HiiHandle, STRING_TOKEN(STR_MODULE_ITEM_PROMPT), L"Divice ID %x", DeviceID);
    }
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
