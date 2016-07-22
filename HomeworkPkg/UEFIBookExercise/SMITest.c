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
#include <Protocol\SmmBase2.h>
#include <Protocol\SmmSwDispatch.h>
#include <Protocol\SmmSwDispatch2.h>
#include <Smm.h>
#include <AmiSmm.h>
#include <Library\UefiLib.h>
//extern gEfiSmmSwDispatch2ProtocolGuid;
//extern gEfiSmmBase2ProtocolGuid;

EFI_STATUS
SmiHandler (
    IN  EFI_HANDLE  DispatchHandle,
    IN  CONST VOID	*DispatchContext,
    IN  OUT	VOID	*CommBuffer,
    IN  OUT	UINTN	*CommBufferSize
)
{
	
	//UINT16 printCode=0x00;
	UINT8  Data = 0;
	UINTN  i = 0xFFFFFFFF;
/*	unsigned char min,sec;
	IoWrite8(0x70,0x02);
	min=IoRead8(0x71);
	IoWrite8(0x70,0x00);
	sec=IoRead8(0x71);
	printCode=((UINT16)min<<8) | sec;
	IoWrite16(0x80,printCode);
*/
	Data = IoRead8(0xB3);
	Print(L"The Data in IO #B3 is %X\n",Data);
	while(i--);
	TRACE(((UINTN)TRACE_ALWAYS, "BOYCE:SMITEST SmiHandler\n"));	
	return EFI_SUCCESS;
}

EFI_STATUS InSmmFunction (
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable )
{
	//******************   SW  SMI  Variable declaration**************************************
	EFI_SMM_BASE2_PROTOCOL          *SmmBase;
	EFI_HANDLE                      SwHandle  = NULL;
	EFI_SMM_SW_REGISTER_CONTEXT     SwContext;
	EFI_SMM_SW_DISPATCH2_PROTOCOL   *SwDispatch;
	EFI_SMM_SYSTEM_TABLE2		*mSmst;
	EFI_STATUS                      Status;
	//******************   SW  SMI  Achieve**************************************
	
	Status = pBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, &SmmBase);
	    ASSERT_EFI_ERROR(Status);
	    if(EFI_ERROR(Status)) {
	        return Status;
	    }
	    SmmBase->GetSmstLocation (SmmBase, &mSmst);   // Save the system table pointer
	    // Register the SW SMI handler
	    Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, &SwDispatch);
	    ASSERT_EFI_ERROR(Status);
	    SwContext.SwSmiInputValue = 0xF2;
	    Status = SwDispatch->Register (SwDispatch, SmiHandler, &SwContext, &SwHandle);
	    if (EFI_ERROR (Status)) {
	        return Status;
	    }

	return  EFI_SUCCESS;
}

EFI_STATUS SMITest(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	InitAmiLib(ImageHandle,SystemTable);
	return InitSmmHandler(ImageHandle, SystemTable, InSmmFunction, NULL);
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
