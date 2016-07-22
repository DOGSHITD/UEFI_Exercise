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
#include<efi.h>
#include<token.h>
#include<AmiLib.h>
#include<AmiHobs.h>
#include<AmiDxeLib.h>
#include<Setup.h>
#include<Dxe.h>
#include<PCI.h>
#include<AmiHobs.h>
#include<Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
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

#define EFI_HOMEWORK_HOB_GUID \
  { \
    0x2b47ac16, 0x7d6b, 0x449c, {0xaa, 0x95, 0x85, 0xe3, 0xfc, 0x30, 0x61, 0x94} \
  }
// {51073869-7254-4e67-96ec-adb0120594d5} 
#define HOMEWORK_PROTOCOL_GUID \
  { \
    0x51073869, 0x7254, 0x4e67, {0x96, 0xec, 0xad, 0xb0, 0x12, 0x5, 0x94, 0xd5} \
  }
// {76d8e5b6-283a-4b2b-9427-6adad52f3e36} 
#define DEVICE_ID_GUID \
  { \
    0x76d8e5b6, 0x283a, 0x4b2b, {0x94, 0x27, 0x6a, 0xda, 0xd5, 0x2f, 0x3e, 0x36} \
  }
typedef EFI_STATUS(EFIAPI *HOMEWORK_TEST_FUNC)();

typedef struct _TEMPLATE_PROTOCOL TEMPLATE_PROTOCOL;
typedef struct _TEMPLATE_PROTOCOL{
	HOMEWORK_TEST_FUNC TempFunc;
};

typedef struct{
		EFI_HOB_GUID_TYPE HobGuidType;
		UINT16            OptionData;
		}EFI_HOMEWORK_HOB;

UINT16 DeviceIDScan(UINT16 DevNum)
{
        UINT16 DeviceID;
	
        IoWrite32(0xCF8,0x80000000|(DevNum<<11));
        DeviceID=(UINT16)(IoRead32(0xCFC)>>16);
        return DeviceID;
}

EFI_STATUS HomeworkInterface(	
)
{
	EFI_STATUS Status;
	
	void *pHobList=NULL;
	EFI_HOMEWORK_HOB *pHomeworkHob=NULL;
	EFI_GUID         mHomeworkHobGuid=EFI_HOMEWORK_HOB_GUID;
	EFI_GUID         HobListGuid = HOB_LIST_GUID;
	EFI_GUID         DeviceIDGuid=DEVICE_ID_GUID;
	UINT16           DeviceID;
	UINT32           shareNum=0;

		//Find HOB		
		pHobList=GetEfiConfigurationTable(pST,&HobListGuid);
	        if(pHobList==NULL)
	        	return EFI_SUCCESS;

		pHomeworkHob=(EFI_HOMEWORK_HOB*)pHobList;
		while(EFI_SUCCESS==(Status=FindNextHobByType(EFI_HOB_TYPE_GUID_EXTENSION,
						&pHomeworkHob))){
			if(0==guidcmp(&pHomeworkHob->HobGuidType.Name,&mHomeworkHobGuid)){
				break;
			}
		}
		if(Status==EFI_SUCCESS){
		}
                
		//PCI:DeviceID 		
		DeviceID=DeviceIDScan(pHomeworkHob->OptionData);

		//SetVariable to transmit the DeviceID to HWSetup.c		
		shareNum=(UINT32)DeviceID;
		Status = pRS->SetVariable (
		              L"Device_ID",\
		              &DeviceIDGuid,\
		              EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
		              sizeof(UINT32),
		              &shareNum
		              );
		ASSERT_EFI_ERROR(Status);
		return Status;
}

EFI_STATUS HomeworkCallBack(
     IN EFI_EVENT Event,
     IN VOID *Context
)
{
	EFI_STATUS Status = EFI_UNSUPPORTED;
	EFI_GUID   gHomeworkProtocolGuid = HOMEWORK_PROTOCOL_GUID;
        TEMPLATE_PROTOCOL *pHomeworkProtocol = NULL;

        //LocateProtocol to invoke HomeworkInterface       
	Status = pBS->LocateProtocol(&gHomeworkProtocolGuid,NULL,&pHomeworkProtocol);
	if(!EFI_ERROR(Status))
		pHomeworkProtocol->TempFunc();
	return Status;
}

EFI_STATUS EFIAPI HomeworkDXE(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_STATUS Status;
	
	EFI_GUID   gHomeworkProtocolGuid = HOMEWORK_PROTOCOL_GUID;
	EFI_EVENT  HomeworkEvent;
	VOID       *HomeworkEventPoint;
	TEMPLATE_PROTOCOL *gHomeworkProtocol=NULL;
			
	InitAmiLib(ImageHandle,SystemTable);
	TRACE((-1,"ASSERT in %s on %i: BOYCEHONG\n",__FILE__, __LINE__));
	//CreateEvent to invoke HomeworkCallBack	
	Status = pBS->CreateEvent(
			EVT_NOTIFY_SIGNAL,
			TPL_CALLBACK,
			HomeworkCallBack,
			NULL,
			&HomeworkEvent
			);
	ASSERT_EFI_ERROR(Status);
	IoWrite16(0x80,0xD0);

	//Detect the Protocol if installed,if installed,activate the HomeworkEvent.       
	Status = pBS->RegisterProtocolNotify(
        			&gHomeworkProtocolGuid,
        			HomeworkEvent,
        			&HomeworkEventPoint
        			);
        ASSERT_EFI_ERROR(Status);

        //InstallProtocol
        gHomeworkProtocol = AllocatePool(sizeof(TEMPLATE_PROTOCOL));
	gHomeworkProtocol->TempFunc=(HOMEWORK_TEST_FUNC)HomeworkInterface;	
        Status = pBS->InstallProtocolInterface(
			&ImageHandle,
			&gHomeworkProtocolGuid,
			EFI_NATIVE_INTERFACE,
			gHomeworkProtocol);

	if(EFI_ERROR(Status)){
		FreePool(gHomeworkProtocol);
		return Status;
	}

	
	return Status;
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
