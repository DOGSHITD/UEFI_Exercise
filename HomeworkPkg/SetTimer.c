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

extern EFI_RUNTIME_SERVICES *gRT;
VOID StartImageOverTimeNotify(IN EFI_EVENT Event, IN VOID *Context)
{
	/*
	static UINTN times = 0;
	
	time ++;
	if(5 == times)
		printf("trigger time X5 \n");
	*/
	gRT->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL);
}

extern EFI_STATUS TimerCreateTimer( EFI_EVENT *Event, EFI_EVENT_NOTIFY Callback,
		VOID *Context, EFI_TIMER_DELAY Delay, UINT64 Trigger, EFI_TPL CallBackTPL );

static EFI_STATUS SETTIMER(VOID)
{
	EFI_STATUS Status;
	UINTN index = 0;
	EFI_EVENT StartImageNotifyEvent = 0;
		
	Status = pBS->CreateEvent((EVT_TIMER | EVT_NOTIFY_SIGNAL), TPL_CALLBACK, (EFI_EVENT_NOTIFY)StartImageOverTimeNotify, \
			(void*)NULL, &StartImageNotifyEvent);
	if(EFI_ERROR(Status))
		return Status;
	
	Status = pBS->SetTimer(StartImageNotifyEvent, TimerRelative, 50 * 1000 * 1000);//10s,unit(100ns)
	if(EFI_ERROR(Status))
		return Status;
	
	return Status;

}

EFI_STATUS SetTimerEntrypoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_EVENT StartImageNotifyEvent = 0;
	
	InitAmiLib(ImageHandle,SystemTable);
	
	TimerCreateTimer( &StartImageNotifyEvent, StartImageOverTimeNotify, \
			    		(VOID *)NULL, TimerRelative, 100 * 1000 * 1000, TPL_CALLBACK );
	//pBS->WaitForEvent(1, &StartImageNotifyEvent, &index);
	Print(L"SetTimerEntrypoint 1\n");
	pBS->Stall(15*1000*1000);
	Print(L"SetTimerEntrypoint End\n");
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
