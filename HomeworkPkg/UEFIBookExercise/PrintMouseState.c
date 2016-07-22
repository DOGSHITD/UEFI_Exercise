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
#include <Protocol/SimplePointer.h>
#include <Setup.h>


extern EFI_GUID gEfiSimplePointerProtocolGuid;

EFI_STATUS PrintMouseState(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_STATUS Status;
	EFI_SIMPLE_POINTER_PROTOCOL* mouse = 0;
	EFI_SIMPLE_POINTER_STATE State;
	EFI_EVENT events[2];
	UINTN DataSize;
	
	InitAmiLib(ImageHandle,SystemTable);
	
	
	DataSize = sizeof(SETUP_DATA);
	Print(L"The setupdata size is %d\n",DataSize);
	
	pST->ConOut->EnableCursor(pST->ConOut,TRUE);
	Status = pBS->LocateProtocol(&gEfiSimplePointerProtocolGuid,NULL,(VOID**)&mouse);
	Status = mouse->Reset(mouse,TRUE);
	events[0] = mouse->WaitForInput;
	events[1] = pST->ConIn->WaitForKey;
	while(1){
		EFI_INPUT_KEY Key;
		UINTN index = 0;
		Status = pBS->WaitForEvent(2,events,&index);
		Print(L"Status1:%x  index:%d\n",Status,index);
		if(index == 0){
			Status = mouse->GetState(mouse,&State);
			Print(L"The Status2 is %x\n",Status);
			Print(L"X:%d, Y:%d X:%d L:%d R:%d\n",
					State.RelativeMovementX,
					State.RelativeMovementY,
					State.RelativeMovementZ,
					State.LeftButton,
					State.RightButton);			
		}else{
			Status = pST->ConIn->ReadKeyStroke(pST->ConIn,&Key);
			if(Key.UnicodeChar == 'q')
				break;
		}		
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
