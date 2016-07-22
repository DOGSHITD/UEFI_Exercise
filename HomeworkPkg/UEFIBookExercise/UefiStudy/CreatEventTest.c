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
// {52cd1a9a-1c00-460c-a49b-21c0d9c5548b} 
#define CREATE_EVENT_GUID \
  { \
    0x52cd1a9a, 0x1c00, 0x460c, {0xa4, 0x9b, 0x21, 0xc0, 0xd9, 0xc5, 0x54, 0x8b} \
  }

VOID myEventNotify0(
	IN EFI_EVENT Event,
	IN VOID *Context
)
{
	Print(L"Enter myEventNotify0!\n");
}

VOID myEventNotify1(
	IN EFI_EVENT Event,
	IN VOID *Context
)
{
	Print(L"Enter myEventNotify1!\n");
}

VOID myEventNotify2(
	IN EFI_EVENT Event,
	IN VOID *Context
)
{
	Print(L"Enter myEventNotify2!\n");
}

EFI_STATUS CreateEventTest(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_STATUS Status;
	EFI_EVENT myEvent[3] = {0};
	EFI_GUID  CreateEventGuide = CREATE_EVENT_GUID;
	
	InitAmiLib(ImageHandle,SystemTable);
	
	Status = pBS->CreateEventEx(EVT_NOTIFY_SIGNAL,TPL_CALLBACK,\
			(EFI_EVENT_NOTIFY)myEventNotify0,NULL,&CreateEventGuide,&myEvent[0]);
	Print(L"Status0:%lx\n",Status);
	Status = pBS->CreateEventEx(EVT_NOTIFY_SIGNAL,TPL_NOTIFY,\
			(EFI_EVENT_NOTIFY)myEventNotify1,NULL,&CreateEventGuide,&myEvent[1]);
	Print(L"Status1:%lx\n",Status);
	Status = pBS->CreateEventEx(EVT_NOTIFY_SIGNAL,TPL_CALLBACK,\
			(EFI_EVENT_NOTIFY)myEventNotify2,NULL,&CreateEventGuide,&myEvent[2]);
	Print(L"Status2:%lx\n",Status);
	Status = pBS->SignalEvent(myEvent[0]);
	Print(L"Status3:%lx\n",Status);
	Status = pBS->CloseEvent(myEvent[0]);
	Status = pBS->CloseEvent(myEvent[1]);
	Status = pBS->CloseEvent(myEvent[2]);
	Print(L"Status4:%lx\n",Status);
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
