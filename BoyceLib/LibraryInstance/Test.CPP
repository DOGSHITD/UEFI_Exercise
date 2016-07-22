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
extern "C" {
#include <Uefi.h>
#include <Library/BaseLib.h>

//Constructor and Destructor are optional.

UINT16 logic_Add(UINT16 inta,UINT16 intb)
{
	return inta+intb;
}

}
/**
extern EFI_HANDLE gImageHandle;//cant add = NULL¡I¡Ibut need add extern¡I
extern EFI_SYSTEM_TABLE *gST;
extern EFI_BOOT_SERVICES *gBS;

EFI_STATUS EFIAPI BoyceLibConstructor(
		IN EFI_HANDLE ImageHandle,
		IN EFI_SYSTEM_TABLE* SystemTable)
{
	
	gImageHandle = ImageHandle;
	gST = SystemTable;
	gBS = SystemTable->BootServices;
	Print(L"Enter BoyceLibConstructor!\n");
	return EFI_SUCCESS;
}

EFI_STATUS EFIAPI BoyceLibDestructor(
		IN EFI_HANDLE ImageHandle,
		IN EFI_SYSTEM_TABLE* SystemTable)
{
	
	if(gImageHandle != NULL)
		SystemTable->BootServices->FreePool(gImageHandle);
	if(gST != NULL)
		SystemTable->BootServices->FreePool(gST);
	if(gBS != NULL)
		SystemTable->BootServices->FreePool(gBS);
	Print(L"Enter BoyceLibDestructor!\n");
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

