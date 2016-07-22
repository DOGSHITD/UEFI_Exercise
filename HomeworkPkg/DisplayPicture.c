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
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/ImageDecoderProtocol.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/EfiShellParameters.h>
#include <Protocol/EfiShellInterface.h>

EFI_GUID  guidSimpleFileSystemProtocol = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
EFI_IMAGE_DECODER_PROTOCOL *gImageDecoder = NULL;
EFI_GUID gImageDecoderGuid = EFI_IMAGE_DECODER_GUID;


IMAGE_POSITION Image_Pos;
IMG_CONTEXT* ImageContext;
UINTN ImageType = EFI_IMAGE_TYPE_UNK;

VOID *File = NULL;
//#define PathPrintable;

extern EFI_GUID gEfiShellParametersProtocolGuid;
extern EFI_GUID gEfiShellInterfaceGuid;

typedef struct{
		UINTN Argc;
		CHAR16** Argv;
		}HOMEWORK_COMMAND;

static CHAR16* GenericDevPathst[]={
		L"UNKNOWN",
		L"Type 1  Hardware Device Path",
		L"Type 2   ACPI Device Path.",
		L"Type 3   Messaging Device Path",
		L"Type 4   Media Device Path",
		L"Type 5   BIOS Boot Specification Device Path",
		L"Type 0x7F  End of Hardware Device Path",
		L"UNKNOWN",
		};

UINTN Print(CHAR16 *Format,...)
{
	UINTN   Return;
	va_list ArgList;
	CHAR16  Buffer[512];

	va_start(ArgList, Format);
	Return = Swprintf_s_va_list(Buffer, sizeof (Buffer), Format, ArgList);
	va_end(ArgList);
	
	if (pST->ConOut != NULL) {
		pST->ConOut->OutputString (pST->ConOut, Buffer);
	}

	return Return;
}

HOMEWORK_COMMAND ShellCommand (IN EFI_HANDLE ImageHandle)
{
  EFI_SHELL_PARAMETERS_PROTOCOL *EfiShellParametersProtocol;
  EFI_SHELL_INTERFACE           *EfiShellInterface;
  EFI_STATUS                    Status;
  
  HOMEWORK_COMMAND gHomework_Command;
 
  EfiShellParametersProtocol = NULL;
  EfiShellInterface = NULL;
  Print(L"\n\n[BOYCEHONG] gHomework_Command.0 \n\r");
  Status = pBS->OpenProtocol(ImageHandle,
                             &gEfiShellParametersProtocolGuid,
                             (VOID **)&EfiShellParametersProtocol,
                             ImageHandle,
                             NULL,
                             EFI_OPEN_PROTOCOL_GET_PROTOCOL
                            );
  Print(L"\n\n[BOYCEHONG] gHomework_Command.1:%x \n\r",Status);
  if (!EFI_ERROR(Status)) {
    //
    // use shell 2.0 interface
    //
	  gHomework_Command.Argc = EfiShellParametersProtocol->Argc;
	  gHomework_Command.Argv = EfiShellParametersProtocol->Argv;
	  Print(L"\n\n[BOYCEHONG] gHomework_Command.Argv2[1]: %s \n\r", gHomework_Command.Argv[1]);
  } else {
    //
    // try to get shell 1.0 interface instead.
    //
    Status = pBS->OpenProtocol(ImageHandle,
                               &gEfiShellInterfaceGuid,
                               (VOID **)&EfiShellInterface,
                               ImageHandle,
                               NULL,
                               EFI_OPEN_PROTOCOL_GET_PROTOCOL
                              );

    if (!EFI_ERROR(Status)) {
      //
      // use shell 1.0 interface
      //
	    gHomework_Command.Argc = EfiShellInterface->Argc;
	    gHomework_Command.Argv = EfiShellInterface->Argv;
    } else {
      ASSERT(FALSE);
    }
  }
  return gHomework_Command;
}

UINTN parseDevicePathIsUsb(VOID)
{
	EFI_GUID EfiDevicePathProtocolGuid = EFI_DEVICE_PATH_PROTOCOL_GUID;
	EFI_DEVICE_PATH_PROTOCOL      *DevicePath;
	EFI_STATUS                    Status;
	EFI_HANDLE *gSmplFileSysHndlBuf = NULL;
	
	UINTN Index=0xFFFFFFFF;
	UINTN FileHandleIndex=0;
	UINT8 type,subtype;
	UINT16 Length=0;
	UINTN debug=20;
	UINTN Count = 0;

	
	Status = pBS->LocateHandleBuffer(ByProtocol, &guidSimpleFileSystemProtocol, NULL, &Count, &gSmplFileSysHndlBuf);
	if(!EFI_ERROR(Status)){
		for(FileHandleIndex=0;FileHandleIndex<Count;FileHandleIndex++){
			if (gSmplFileSysHndlBuf[FileHandleIndex] != NULL){
				Status = pBS->HandleProtocol(gSmplFileSysHndlBuf[FileHandleIndex], &EfiDevicePathProtocolGuid, &DevicePath);
				if(DevicePath==NULL)
					Print(L"DevicePath is fail\n\r");
				else{
					while(debug--){//loop debug:20 is for find type==0x7f 
						type=DevicePath->Type;
						subtype=DevicePath->SubType;
						if(type==0x7F){
							type=0x06;
							if(subtype == 0xFF){  //End Entire Device Path 
								debug=0;
								break;
							}
						}
#ifndef PathPrintable	
						switch(type){
						case 0x03:
							switch(subtype){
							case 0x05:     Index = FileHandleIndex;
						                   break;
							default:       break;
							}
							break;
						default:break;       
						}
#else						
						if(type>=1 && type<=6)
							Print(GenericDevPathst[type]);
						else
							Print(L"Error!!! type %d\r\n" ,type);
						switch(type){
							case 0x01: 
							switch(subtype){
								case 0x01:    Print(L"Sub-Type 1   PCI");	break;
								case 0x02:    Print(L"Sub-Type 2   PCCARD");	break;
								case 0x03:    Print(L"Sub-Type 3   Memory Mapped.");	break;
								case 0x04:    Print(L"Sub-Type 4   Vendor.");	break;
								case 0x05:    Print(L"Sub-Type 5   Controller.");	break;
								case 0x06:    Print(L"Sub Type 6   BMC.");	break;
								default:      Print(L"Sub-Type Error. subtype %d\r\n",subtype);	break;		
							}
							break;
							case 0x02: 
							switch(subtype){
								case 0x01:    Print(L"Sub-Type 1 ACPI Device Path.");	break;
								case 0x02:    Print(L"Sub-Type 2 Expanded ACPI Device Path.");	break;
								case 0x03:    Print(L"Sub-Type 3 _ADR Device Path");	break;
								default:      Print(L"Sub-Type Error. subtype %d\r\n",subtype);	break;		
							}
							break;
							case 0x03: 
							switch(subtype){
								case 0x01:    Print(L"Sub-Type 1   ATAPI");	break;
								case 0x02:    Print(L"Sub-Type 2   SCSI");	break;
								case 0x03:    Print(L"Sub-Type 3   Fibre Channel");	break;
								case 0x15:    Print(L"Sub-Type 21   Fibre Channel Ex");	break;
								case 0x04:    Print(L"Sub-Type 4   1394");	break;
								case 0x05:    Print(L"Sub-Type 5   USB"); 
								              Index = FileHandleIndex;
								              break;
								case 0x12:    Print(L"Sub-Type 18   SATA");	break;
								case 16:      Print(L"Sub-Type 16  USB WWID");	break;
								case 17:      Print(L"Sub-Type 17   Device Logical unit");	break;
								case 15:      Print(L"Sub-Type 15 - USB Class.");	break;
								case 6:       Print(L"Sub-Type 6   I2O Random Block Storage Class");	break;
								case 11:      Print(L"Sub-Type 11   MAC Address for a network interface");	break;
								case 12:      Print(L"Sub-Type 12   IPv4");	break;
								case 13:      Print(L"Sub-Type 13   IPv6");	break;
								case 20:      Print(L"Sub-Type 20   Vlan (802.1q)");	break;
								case 9:       Print(L"Sub-Type 9   InfiniBand");	break;
								case 14:      Print(L"Sub-Type 14   UART");	break;
								case 10:      Print(L"Sub-Type 10   Vendor");	break;
								//more just so.
								default:      Print(L"Sub-Type Error. subtype %d\r\n",subtype);	break;	
							}
							break;
							case 0x04: 
							switch(subtype){
								case 1:       Print(L"Sub-Type 1  Hard Drive");	break;//more just so.
								case 2:       Print(L"Sub-Type 2  CD-ROM ¡°El Torito¡± Format.");	break;	
								default:      Print(L"Sub-Type Error. subtype %d\r\n",subtype);	break;	
							}
							break;
							case 0x05: 
							switch(subtype){
								case 1:       Print(L"Sub-Type 1   BIOS Boot Specification Version 1.01.");	break;//more just so.
								case 2:       Print(L"Sub-Type 2   CD-ROM ¡°El Torito¡± Format.");	break;	
								default:      Print(L"Sub-Type Error. subtype %d\r\n",subtype);	break;	
							}
							break;
							default:break;
						}
#endif
							Length=(DevicePath->Length[1]<<8) | DevicePath->Length[0];
							DevicePath=(EFI_DEVICE_PATH_PROTOCOL *)((unsigned char *)DevicePath+Length);
					}
				}
			}
		}
	}

	if(Index==0xFFFFFFFF){
		Print(L"\r\n There is no USB devices!\r\n");
	}
	Print(L"\r\n USB:%d \r\n",Index);	
	return Index;//it mean no usb devices.
}

EFI_FILE_PROTOCOL * CreateFile(CHAR16 *filename, UINT32 index)
{
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *pSFP;
	EFI_STATUS Status = EFI_SUCCESS;
	EFI_FILE_PROTOCOL *pRoot,*FileHandle;
        EFI_HANDLE *gSmplFileSysHndlBuf = NULL;  
	UINTN Count = 0;

	// Locate all the simple file system devices in the system.
    Status = pBS->LocateHandleBuffer(ByProtocol, &guidSimpleFileSystemProtocol, NULL, &Count, &gSmplFileSysHndlBuf);
    Print(L"\n\n[BOYCEHONG] D0:%d\n\r",Count);
	if(!EFI_ERROR(Status))
    {
	 	Status = pBS->HandleProtocol( gSmplFileSysHndlBuf[index], &guidSimpleFileSystemProtocol, &pSFP );
		if (!EFI_ERROR(Status))
		{
			Status = pSFP->OpenVolume(pSFP,&pRoot);
			if (!EFI_ERROR(Status))
			{
				Status = pRoot->Open(pRoot,
				                    &FileHandle,
				                    filename,
				                    EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,
				                    0);
				
				pRoot->Close(pRoot);
				if (!EFI_ERROR(Status))
	 			{
					Print(L"\n\n[BOYCEHONG] D1\n\r");
					return FileHandle;	
				}
			}
		}
	}

	if(gSmplFileSysHndlBuf != NULL) 
        pBS->FreePool(gSmplFileSysHndlBuf);
	return NULL;
}

EFI_STATUS WriteDataToFile(CHAR16 *filename, VOID *Data, UINTN length, UINT32 index)
{
	EFI_STATUS Status = EFI_UNSUPPORTED;
	EFI_FILE_PROTOCOL *FileHandle=NULL;

	UINTN len = length;

	Print(L"\n\n[BOYCEHONG] Entering WriteDataToFile \n\r");
	
	if (Data == NULL)
	{
    	goto DONE;
	}	
  	
	FileHandle = CreateFile(filename, index);
	if (FileHandle != NULL)
	{
		FileHandle->SetPosition(FileHandle,0);
		FileHandle->Write(FileHandle,&len, Data);
		FileHandle->Close(FileHandle);
		Status = EFI_SUCCESS ;

		Print(L"\n\n[BOYCEHONG] Exporting Filename: %s \n\r", filename);
	}
DONE:
	
Print(L"\n\n[BOYCEHONG] Exiting WriteDataToFile, Status: %x \n\r", Status);

	return Status ;
}

VOID * ReadFile(CHAR16 *filename, UINT32 index, CHAR8 printable)
{
	EFI_STATUS Status = EFI_SUCCESS;
	EFI_FILE_PROTOCOL *FileHandle=NULL;        
        UINTN BufferSize=0xFFFFFFFF; 
	UINTN i=0;
	CHAR8 string;
	
	Print(L"\n\n[BOYCEHONG] Entering ReadFile \n\r");
	FileHandle = CreateFile(filename, index);
	
	if (FileHandle != NULL ){
		FileHandle->SetPosition(FileHandle,0);
		Status = FileHandle->Read(FileHandle,&BufferSize, NULL);
		if(EFI_ERROR(Status)) goto DONE;
		pBS->AllocatePool(EfiBootServicesData, BufferSize, &File);
		if (File != NULL) {
		       	pBS->SetMem(File, BufferSize, 0);
		}

		FileHandle->SetPosition(FileHandle,0);
		Status = FileHandle->Read(FileHandle,&BufferSize, File);
		FileHandle->Close(FileHandle);
		if((!EFI_ERROR(Status)) && (BufferSize != 0))
	        {      
			if(printable){
				Print(L"\n\r The file is printed below:\n\r ");
				while(i<strlen(File)){
					string = *((CHAR8 *)File+(i++));
					if((string>=0x0020)&(string<=0x007e)){
						Print(L"%c",string);
					}else
						Print(L"%x",string);
				}
				Print(L"\n\r");
			}
			return File;
	        }		
		Status = EFI_SUCCESS ;
	}
DONE:	
        Print(L"\n\n[BOYCEHONG] Exiting ReadFile, Status: %x \n\r", Status);

	return NULL ;
}

EFI_STATUS EFIAPI DisplayPictures(
		IN EFI_HANDLE ImageHandle,
		IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_STATUS Status;
	CHAR16 *filename=L"Test.txt";
	CHAR16 *Imagename;
	CHAR8 *Data="BoyceHongWriteData1234567890!";
	VOID *gFile = NULL;
        UINT8 *ImageData = NULL;
	UINTN ImageSize;
	HOMEWORK_COMMAND gHomework_Command;
	UINT32 Index;
	
	InitAmiLib(ImageHandle,SystemTable);
	
	Index =(UINT32) parseDevicePathIsUsb();
	if(Index == 0xFFFFFFFF){
		Print(L"\r\n There is no USB devices!\r\n");
		return EFI_SUCCESS;
	}
		
	gHomework_Command=ShellCommand(ImageHandle);
	
	Status = WriteDataToFile(filename, Data, strlen(Data), Index);
	if (EFI_ERROR (Status))		
         	return EFI_SUCCESS;
	
	gFile = ReadFile(filename,Index,1);
	pBS->FreePool(File);	
	if(gFile==NULL)
		return EFI_SUCCESS;
	
	ImageSize=0;
	Image_Pos.x=0;
	Image_Pos.y=0;
	
	Status = pBS->LocateProtocol(&gImageDecoderGuid, NULL, &gImageDecoder);
	if (EFI_ERROR (Status))		
	        return EFI_SUCCESS;
	
	if(gHomework_Command.Argc==2){
		Imagename =(CHAR16*) gHomework_Command.Argv[1];
		gFile = ReadFile(Imagename,Index,0);
		if(gFile==NULL)
			return EFI_SUCCESS;
			
		ImageData=(UINT8*)gFile;
		gImageDecoder->DecodeImage(ImageData,ImageSize,&ImageType,&ImageContext);
		gImageDecoder->DrawImage(ImageContext,ImageType,Image_Pos);
//Concrete realization path:\NB_Bios\X555LDB.305\AsusPkg\COMMON\EM\CORESRC\ImageDecoder\DXE_BS\ImageDecoder.c
	}else{
		Print(L"\r\n Please input correct argument!\r\n");
	}
	
	pBS->FreePool(File);
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
