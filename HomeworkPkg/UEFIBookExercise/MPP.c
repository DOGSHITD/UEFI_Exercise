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
#include <Library/UefiLib.h>
#include "Protocol/MpService.h"

extern EFI_GUID gEfiMpServiceProtocolGuid;

typedef struct {
	UINTN id;
	UINTN loops;
	UINTN ret;
}APParam;

VOID EFIAPI APFunc(VOID* context)
{
	APParam *param =(APParam*)context;
	UINTN i = 0;
	
	for(i=0;i< param->loops; i++){
		pBS->Stall(1000);
		Print(L"AP %d runs %d\n",param->id,i);
	}
	param->ret = param->loops *10;
}

EFI_STATUS MPPStartup(EFI_MP_SERVICES_PROTOCOL* mpp)
{
	EFI_STATUS Status;
	BOOLEAN Finished = FALSE;
	APParam APParam0 = {0, 5, 0};
	APParam APParam1 = {1, 20,0};
	EFI_EVENT ApEvent;
	UINTN Index;
	
	Status = pBS->CreateEvent(0,TPL_CALLBACK,NULL,NULL,&ApEvent);
	Status = mpp->StartupThisAP(
			mpp,
			APFunc,
			1,
			ApEvent,
			5000*1000,
			(VOID*)&APParam1,
			&Finished
			);
	Print(L"StartupThisAP : %r\n",Status);
	if(Status == 0){
		APFunc((VOID*)&APParam0);
		pBS->WaitForEvent(1,&ApEvent,&Index);
		Print(L"AP %s and return %d\n",Finished == TRUE?L"Finished":L"failed",APParam1.ret);
	}
	return Status;
}

EFI_STATUS MPP_Main(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_STATUS Status;
	EFI_MP_SERVICES_PROTOCOL* mpp = (EFI_MP_SERVICES_PROTOCOL*)NULL;
	UINTN nCores = 0,nRunning = 0;
	
	InitAmiLib(ImageHandle,SystemTable);
	
	Status = pBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **)&mpp);
	if(EFI_ERROR(Status)){
		Print(L"Locate Protocol fail, Status:%r\n",Status);
		return Status;
	}
		
	Status = mpp->GetNumberOfProcessors(mpp,&nCores,&nRunning);
	if(EFI_ERROR(Status)){
		Print(L"GetNumberOfProcessors fail, Status:%r\n",Status);
		return Status;
	}
	
	Print(L"System has %d cores,%d cores are running\n",nCores,nRunning);
	{
		UINTN i = 0;
		for(i =0;i < nCores; i++){
			EFI_PROCESSOR_INFORMATION mcpuInfo;
			Status = mpp->GetProcessorInfo(mpp,i,&mcpuInfo);
			Print(L"CORE %d:\n",i);
			Print(L"  ProcessorId:%d\n",mcpuInfo.ProcessorId);
			Print(L"  StatusFlags:%x\n",mcpuInfo.StatusFlag);
			Print(L"  Location:(%d %d %d)\n",mcpuInfo.Location.Package,\
					mcpuInfo.Location.Core,mcpuInfo.Location.Thread);
		}
	}
	
	Status = MPPStartup(mpp);
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
