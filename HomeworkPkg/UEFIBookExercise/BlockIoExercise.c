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
#include <Protocol/BlockIo.h>

#define SAFECALL(X) if(EFI_SUCCESS != (X)){return Status;}

extern EFI_GUID gEfiBlockIoProtocolGuid;

//ReadBlock Method 1:asynchronous
/**
VOID EFIAPI BlockIo2OnReadComplete(IN EFI_EVENT Event, IN VOID *Context)
{
	UINT8* Buf = (UINT8*)Context;
	Print(L"the Block Context 2: %s",Buf);
	pBS->FreePool(Buf);
}

EFI_STATUS ReadBlocks2(EFI_BLOCK_IO2_PROTOCOL* BlockIo2)
{
	EFI_STATUS Status = 0;
	UINT8* Buf = NULL;
	EFI_BLOCK_IO2_TOKEN b2Token;
	UINT32 BlockSize = BlockIo2->Media->BlockSize;
	
	SAFECALL(Status = pBS->AllocatePool(EfiBootServicesCode,BlockSize,(VOID**)&Buf));
	SAFECALL(Status = pBS->CreateEvent(EVT_NOTIFY_SIGNAL,TPL_NOTIFY,BlockIo2OnReadComplete,(VOID**)Buf));
	Satus = BlockIo2->ReadBlocksEx(BlockIo2,BlockIo2->Media->MediaId,0,&b2Token,BlockSize,(VOID**)Buf);
	if(EFI_ERROR(Status)){
		SAFECALL(Status = pBS->FreePool(Buf));
	}else{
		UINTN Index = 0;
		pBS->WaitForEvent(1,&b2Token.Event,&Index);
	}
	return Status;
}
*/

//ReadBlock Method 2:PassThru
/**
EFI_STATUS ReadSectors(UINT16 Port,UINT16 PortMultiplierPort,UINT64 StartLba,
		UINT32 SectorCount,CHAR8* Buffer, UINT32 DeviceId)
{
	EFI_STATUS Status;
	UINT32 IsExt = (StartLba > 0xFFFFFFFFFFFFFFFF);
	EFI_ATA_PASS_THRU_COMMAND_PACKET Packet;
	
	Packet.Protocol = EFI_ATA_PASS_THRU_PROTOCOL_UDMA_DATA_IN;
	Packet.Acb->AtaCommand = IsExt? ATA_CMD_READ_DMA_EXT : ATA_CMD_READ_DMA;
	Packet.Timeout = 0;
	Packet.Length = EFI_ATA_PASS_THRU_LENGTH_SECTOR_COUNT;
	Packet.OutTransferLength = 0;
	Packet.OutDataBuffer = NULL;
	Packet.InTransferLength = SectorCount;
	Packet.InDataBuffer = Buffer;
	Packet.Acb->AtaFeatures = 0;
	Packet.Acb->AtaFeaturesExp = 0;
	Packet.Acb->AtaSectorNumber = (UINT8) StartLba;
	Packet.Acb->AtaCylinderLow = (UINT8) RShiftU64(StartLba,8);
	Packet.Acb->AtaCylinderHigh = (UINT8) RShiftU64(StartLba,16);
	Packet.Acb->AtaSectorCount = (UINT8) SectorCount;
	Packet.Acb->AtaDeviceHead = (UINT8) (0xE0 | (PortMultiplierPort<< 4));
	if(IsExt != 0){
		Packet.Acb->AtaSectorNumberExp = (UINT8) RShiftU64(StartLba,24);
		Packet.Acb->AtaCylinderLowExp = (UINT8) RShiftU64(StartLba,32);
		Packet.Acb->AtaCylinderHighExp = (UINT8) RShiftU64(StartLba,40);
		Packet.Acb->AtaSectorCountExp = (UINT8) (SectorCount >> 8);
	}else{
		Packet.Acb->AtaDeviceHead = (UINT8) (Packet.Acb->AtaDeviceHead | RShiftU64(StartLba,24));
	}
	Status = AtaPassThroughProtocol->PassThru(AtaPassThroughProtocol,Port,PortMultiplierPort,&Packet,NULL);
	return Status;
}
**/

//ReadBlock Method 3:Blocking
EFI_STATUS ReadBlocks(EFI_BLOCK_IO_PROTOCOL* BlockIo)
{
	EFI_STATUS Status = 0;
	UINT8* Buf;
	UINT32 BlockSize = BlockIo->Media->BlockSize;
	UINT32 i = 0;
	
	Print(L"the BlockSize is %d",BlockSize);
	SAFECALL(Status = pBS->AllocatePool(EfiBootServicesCode,BlockSize,(VOID **)&Buf));
	SAFECALL(Status = BlockIo->ReadBlocks(BlockIo,BlockIo->Media->MediaId,0,BlockSize,(VOID **)Buf));
	Print(L"the Block Context:");
	while(i++ < BlockSize){
		Print(L"%x",Buf[i]);
		if(i % 16 == 0)
			Print(L"\n");
	}
	SAFECALL(Status = pBS->FreePool(Buf));
	
	return Status;
}

EFI_STATUS BlockIoExercise(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINTN HandleIndex,NumHandles;
	EFI_HANDLE *ControllerHandle = NULL;
	EFI_HANDLE gImageHandle = NULL;
	
	InitAmiLib(ImageHandle,SystemTable);
	
	SAFECALL(Status = pBS->LocateHandleBuffer(ByProtocol,
			&gEfiBlockIoProtocolGuid,
			NULL,
			&NumHandles,
			&ControllerHandle));
	
	for(HandleIndex = 0;HandleIndex < NumHandles;HandleIndex ++){
		EFI_BLOCK_IO_PROTOCOL* BlockIo;
		Status = pBS->OpenProtocol(ControllerHandle[HandleIndex],
			&gEfiBlockIoProtocolGuid,
			(VOID**)&BlockIo,
			gImageHandle,
			NULL,
			EFI_OPEN_PROTOCOL_GET_PROTOCOL);
		
		if(EFI_ERROR(Status)){
			continue;
		}
	
		Status = ReadBlocks(BlockIo);
	}
	return EFI_SUCCESS;
}


//The standard progranm for getting setupdata.At first,must try to get size. 
/**
  Status = pBS->AllocatePool(EfiRuntimeServicesData,sizeof(SETUP_DATA),&AsusSetupData);  //AK-SIM0115+
  AsusSetupDataSize = sizeof(SETUP_DATA);
  Status = pRS->GetVariable(L"Setup", &gSetupGuid, (UINT32 *)&Attribute, &AsusSetupDataSize, AsusSetupData);
    if ( Status == EFI_BUFFER_TOO_SMALL ) {
    Status = pBS->AllocatePool(EfiRuntimeServicesData,AsusSetupDataSize,&AsusSetupData); //AK-SIM0115+
    Status = pRS->GetVariable(L"Setup", &gSetupGuid, (UINT32 *)&Attribute, &AsusSetupDataSize, AsusSetupData);
    if ( EFI_ERROR(Status) )
      AsusSetupData = NULL;
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
