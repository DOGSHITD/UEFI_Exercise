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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library\MemoryAllocationLib.h>
#include <AmiDxeLib.h>
#include <Setup.h>
//#include <Library/UefiBootServicesTableLib.h>
//#include <Library/DebugLib.h>
#include "AmiNetworkPkg\Include\Guid\NetworkStackSetup.h"
#include "AmiCompatibilityPkg\Include\Protocol\UsbPolicy.h"
#include "AsusPkg\Include\Protocol\RTProtocol.h"
#include "AsusPkg\Include\Ppi\GPNV.h"
extern EFI_GUID gEfiSetupVariableGuid;
//extern EFI_GUID gEfiSimplePointerProtocolGuid;

#ifndef Offsetof
#define Offsetof(st, m) \
     ((UINTN) ( (char *)&((st *)0)->m - (char *)0 ))
#endif

#pragma pack(1)
typedef union{
	UINT8 DATA[10];
	struct{
	UINT8 A;
	UINT8 B[4];
	UINT16 C;
	UINT8 D;
	};
}DFGH;
#pragma pack()
/*
UINTN AskOffset(UINT8 *VariableStruct,UINT8 *Variable)
{
	UINTN VariableOffset;
	
	VariableOffset = Variable - VariableStruct;
	
	return VariableOffset;
}
*/
SETUP_DATA * GetPreSetupVariable(EFI_RUNTIME_SERVICES *pRS)
{
	EFI_STATUS      Status = EFI_SUCCESS;
	SETUP_DATA *    PreSetupVariable;
	UINTN           VarSize;
	
	VarSize = sizeof(SETUP_DATA);
	PreSetupVariable = (SETUP_DATA *)AllocateZeroPool (VarSize);
	Status = pRS->GetVariable(L"Setup",
				&gEfiSetupVariableGuid,
				NULL,
				&VarSize,
				PreSetupVariable);
	if(!EFI_ERROR(Status))
		return PreSetupVariable;
	
	return (SETUP_DATA *)NULL;	
}
//ASUS-Boyce-16011401 >>>
typedef unsigned int        UINT;
typedef struct _COMPARE_VARIABLE_INFO {
	UINT8           Group;          //0:SETUP_DATA 1:NETWORK_STACK 2:USB_SUPPORT_SETUP
	CHAR16		*Name;       	//Variable name
	UINTN		NvramOffset;    //Offset comparing variable in SETUP_DATA 
	UINTN           GpnvOffset;     //Offset comparing variable in GPNV_STVC
	UINT            VariableSize;
}COMPARE_VARIABLE_INFO;

COMPARE_VARIABLE_INFO CompareVariableInfo[] = {\
  {0,L"PxeOpRom",Offsetof(SETUP_DATA,PxeOpRom),0,sizeof(((SETUP_DATA *)(0))->PxeOpRom)},
  {0,L"CsmSupport",Offsetof(SETUP_DATA,CsmSupport),1,sizeof(((SETUP_DATA *)(0))->CsmSupport)},
  {0,L"PcieRootPortEn2",Offsetof(SETUP_DATA,PcieRootPortEn[2]),2,sizeof(((SETUP_DATA *)(0))->PcieRootPortEn[2])},         //SETUP_DATA.PcieRootPortEn[2]
  {0,L"SetPointDevice",Offsetof(SETUP_DATA,SetPointDevice),3,sizeof(((SETUP_DATA *)(0))->SetPointDevice)},
  {0,L"SetWakeOnLidOpen",Offsetof(SETUP_DATA,SetWakeOnLidOpen),4,sizeof(((SETUP_DATA *)(0))->SetWakeOnLidOpen)},
  {0,L"AesEnable",Offsetof(SETUP_DATA,AesEnable),5,sizeof(((SETUP_DATA *)(0))->AesEnable)},
  {0,L"SmartSelfTest",Offsetof(SETUP_DATA,SmartSelfTest),6,sizeof(((SETUP_DATA *)(0))->SmartSelfTest)},
  {0,L"IgdDvmt50PreAlloc",Offsetof(SETUP_DATA,IgdDvmt50PreAlloc),7,sizeof(((SETUP_DATA *)(0))->IgdDvmt50PreAlloc)},
  {0,L"SataInterfaceMode",Offsetof(SETUP_DATA,SataInterfaceMode),8,sizeof(((SETUP_DATA *)(0))->SataInterfaceMode)},
  {0,L"WirelessLock",Offsetof(SETUP_DATA,WirelessLock),9,sizeof(((SETUP_DATA *)(0))->WirelessLock)},
  {0,L"PchAzalia",Offsetof(SETUP_DATA,PchAzalia),10,sizeof(((SETUP_DATA *)(0))->PchAzalia)},
  {0,L"ODDLock",Offsetof(SETUP_DATA,ODDLock),11,sizeof(((SETUP_DATA *)(0))->ODDLock)},
  {0,L"UsbLock",Offsetof(SETUP_DATA,UsbLock),12,sizeof(((SETUP_DATA *)(0))->UsbLock)},
  {0,L"UsbExternalPortLock",Offsetof(SETUP_DATA,UsbExternalPortLock),13,sizeof(((SETUP_DATA *)(0))->UsbExternalPortLock)},
//  {0,NULL,0, 0, 0},
  {0,L"CameraLock",Offsetof(SETUP_DATA,CameraLock),14,sizeof(((SETUP_DATA *)(0))->CameraLock)},
  {1,L"NetworkStackEnable",Offsetof(NETWORK_STACK,Enable),15,sizeof(((NETWORK_STACK *)(0))->Enable)},              //NETWORK_STACK.Enable
  {2,L"UsbLegacySupport",Offsetof(USB_SUPPORT_SETUP,UsbLegacySupport),16,sizeof(((USB_SUPPORT_SETUP *)(0))->UsbLegacySupport)},        //USB_SUPPORT_SETUP.UsbLegacySupport
  {2,L"UsbMassDriverSupport",Offsetof(USB_SUPPORT_SETUP,UsbMassDriverSupport),17,sizeof(((USB_SUPPORT_SETUP *)(0))->UsbMassDriverSupport)},    //USB_SUPPORT_SETUP.UsbMassDriverSupport
  {0,NULL,0, 0, 0},
};

EFI_STATUS UefiMain(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable	
)
{
	SETUP_DATA *    PreSetupVariable;
	ASUS_RT_PROTOCOL *AsusRTProtocol_Interface = NULL;
	EFI_GUID   AsusRTProtocolGuid = ASUS_RT_PROTOCOL_GUID;
	GPNV_STVC  *AsusSTVC;
	UINT16      *STVCBlock;
	UINTN      DataSize;
	UINT16      i;
	
	InitAmiLib(ImageHandle,SystemTable);
	
	DataSize = sizeof(GPNV_STVC);
	pBS->AllocatePool(EfiBootServicesData, DataSize, &STVCBlock);
	pBS->LocateProtocol(&AsusRTProtocolGuid, NULL, &AsusRTProtocol_Interface);
	AsusRTProtocol_Interface->GetGPNVVariable(L"STVC", &DataSize, STVCBlock);
	AsusSTVC = (GPNV_STVC *)STVCBlock;
		
	PreSetupVariable = GetPreSetupVariable(pRS);
	if(PreSetupVariable != (SETUP_DATA *)NULL){
		Print(L" PreSetupVariable:%d\n",(*PreSetupVariable).PxeOpRom);
		Print(L" PxeOpRom Size:%d\n\n",sizeof(((SETUP_DATA *)(0))->PxeOpRom));
		pBS->FreePool(PreSetupVariable);
	}
	
	Print(L"=========================================\n");
	Print(L" Count Variable in GPNV Table List Below\n");
	Print(L"=========================================\n\n");
	for(i = 0;CompareVariableInfo[i].Name != NULL;i++)
	{
		UINT16 Count;
		Count = *(STVCBlock+(CompareVariableInfo[i].GpnvOffset));
		Print(L" %s Count:%d\n",CompareVariableInfo[i].Name,Count);
	}
	pBS->FreePool(STVCBlock);
	return EFI_SUCCESS;	
}



/**
EFI_STATUS UefiMain (
        IN EFI_HANDLE        ImageHandle,
        IN EFI_SYSTEM_TABLE  *SystemTable
        )
{
    SystemTable -> ConOut-> OutputString(SystemTable->ConOut, L"Test ExitBootServices\n");
    {
        UINTN MemMapSize = 0;
        EFI_MEMORY_DESCRIPTOR* MemMap = 0;
        UINTN MapKey = 0;
        UINTN DesSize = 0;
        UINT32 DesVersion = 0;
        CHAR16* vendor = SystemTable->FirmwareVendor;
        //Output vendor name.
        SystemTable -> ConOut-> OutputString(SystemTable->ConOut, vendor);
        __asm int 3;
        // Get MapKey
        gBS->GetMemoryMap(&MemMapSize, MemMap, &MapKey, &DesSize, &DesVersion);
        // End Boot Services
        gBS->ExitBootServices(ImageHandle, MapKey);
        __asm int 3;
        ASSERT(SystemTable -> BootServices == NULL); 

        //SystemTable -> ConOut-> OutputString(SystemTable->ConOut, L"Crash!\n"); 
        while(DesSize >0){}
    }
    return (EFI_STATUS)-1;
}
**/
/**
EFI_STATUS Task(EFI_INPUT_KEY Key){
	CHAR16 Str;
	
	Str = Key.UnicodeChar;
	Print(L"The Input Key is %c\n",Str);
	
	return EFI_SUCCESS;
}

EFI_STATUS TimeOut(){
	
	Print(L"5s is TIMEOUT!!\n");
	return EFI_SUCCESS;
}
EFI_STATUS UefiMain(
		IN EFI_HANDLE ImageHandle,
		IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_STATUS Status;
	UINTN Index=0;
	EFI_EVENT myEvents[2];
	EFI_INPUT_KEY Key;
	
	InitAmiLib(ImageHandle,SystemTable);
	/**
	SystemTable->BootServices->WaitForEvent(1,&SystemTable->ConIn->WaitForKey,&Index);
	Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn,&Key);
	StrBuffer[0] = Key.UnicodeChar;
	StrBuffer[1] = '\n';
	SystemTable->ConOut->OutputString(SystemTable->ConOut,StrBuffer);
	**/
/**
	Status = pBS->CreateEvent(EVT_TIMER,TPL_CALLBACK,(EFI_EVENT_NOTIFY)NULL,\
			(VOID*)NULL,&myEvents[0]);
	Status = pBS->SetTimer(myEvents[0],TimerRelative,30*1000*1000);
	myEvents[1] = pST->ConIn->WaitForKey;
	while(1){
		Status = pBS->WaitForEvent(2,myEvents,&Index);
		Status = pBS->SetTimer(myEvents[0],TimerCancel,30*1000*1000);
		if(Index == 0)
			TimeOut();
		else if(Index == 1){
			Status = pST->ConIn->ReadKeyStroke(pST->ConIn,&Key);
			Task(Key);
		}
	    Status = pBS->SetTimer(myEvents[0],TimerRelative,30*1000*1000);
	}
	return EFI_SUCCESS;
}
**/
/**
EFI_STATUS EFIAPI DetectBootablePartiton(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_STATUS Status;
	EFI_HANDLE *ControllerHandles = NULL;
	UINTN      HandIndex, NumHandles;
	
	InitAmiLib(ImageHandle,SystemTable);
	
	Status = pBS->LocateHandleBuffer(ByProtocol,
			&gEfiSimpleFileSystemProtocolGuid,
			NULL,
			&NumHandles,
			&ControllerHandles);
	if(EFI_ERROR(Status)){
		return Status;
	}
	
	for(HandleIndex = 0; HandlesIndex < NumHandles; HandleIndex){
		EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimPleFileSystem;
		Status = pBS->HandleProtocol(
				ControllerHandles[HandlerIndex],
				&gEfiSimpleFileSystemProtocolGuid,
				(VOID**)&SimpleFileSystem);
		//Check file¡Gefi/boot/bootx64.efi
	}
	
	if(ControllerHandles != NULL)
		Status = pBS->FreePool(ControllerHandles);
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
