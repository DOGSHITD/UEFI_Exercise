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

#include <efi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/NetHTTPProtocol.h>
#include <Library\MemoryAllocationLib.h>

EFI_STATUS
UefiMain (
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
    )
{
    EFI_STATUS        Status = EFI_SUCCESS;
    EFI_GUID                 gEfiASUSNETProtocolGuid = EFI_ASUSNET_PROTOCOL_GUID ;        
    EFI_ASUSNET_PROTOCOL     *asusnet = NULL;
    UINT8                    *rom;
    UINTN                    romsize;
    EFI_HANDLE               *HandleBuffer = NULL;
    UINTN                    HandleCount = 0;
//    CHAR8                   Filepath[128];
//    CHAR8                   username[20];
//    CHAR8                   password[20]; 
//    CHAR8                    *Filepath = "atsz-ftp-03.asustek.com.cn/12.txt";
    CHAR8                    *username = "zacks03";
    CHAR8                    *password = "#*zacks03";
    UINT16                   n = 0;
    CHAR8                    *Filepath1 ="atsz-ftp-03.asustek.com.cn/test.bin";
//    CHAR16                   String[128];
//    CHAR16                   Temp[128];
    //
    // Initialize the Library. Set BS, RT, &ST globals
    //  BS = Boot Services RT = RunTime Services
    //  ST = System Table
    //
    rom = (UINT8*)AllocateZeroPool(0x800);
    for(n=0;n<200;n++)
    {
    	if(n%2)
    	   *(rom+n) = 0x35;
    	else
    	   *(rom+n) = 0x65;
    }
    
    romsize = 0x800;
    Print(L"AllocateZeroPool\n");
    Status = gBS->LocateHandleBuffer(ByProtocol,
                &gEfiASUSNETProtocolGuid,
                NULL,
                &HandleCount,
                &HandleBuffer);

       if(EFI_ERROR(Status)){
               Print(L"locate error Status:%x\n",Status);
               return Status;
       }
       Print(L"HandleCount = %d\n",HandleCount);


    Status = gBS->HandleProtocol(
                       HandleBuffer[HandleCount-1],
                       &gEfiASUSNETProtocolGuid,
                       &asusnet);

       if(EFI_ERROR(Status)){
               Print(L"handle error Status:%x\n",Status);
               return Status;
       }

    Print(L"asusnet %X\n",asusnet);

//download from HTTP

 //   StrCpy(Filepath,"dlcdnet.asus.com/pub/ASUS/nb/X555LAB/X555LABAS303.zip");
 //   asusnet->HTTPDownloadFile(asusnet, Filepath, &rom, &romsize);


//download from FTP

//    StrCpy(username, "zacks03");
//    StrCpy(password, "#*zacks03");
//    StrCpy(Filepath, "atsz-ftp-03.asustek.com.cn/12.txt");

//    asusnet->FTPDownloadFile(asusnet, Filepath, username, password, &rom, &romsize);

//Sync Time

 //    StrCpy(Filepath, L"time.windows.com");
 //    asusnet->NTPSync_Time(asusnet, Filepath,20); //UTC+8:00

//    AsciiStrToUnicodeStr((CHAR8 *)rom,String);
//   Print(L"the download rom:%s\n",String);
   
//    StrCpy(Temp,L"addmore");
//    StrCat(Temp,String);
//    UnicodeStrToAsciiStr(Temp,(CHAR8 *)rom);
    asusnet->FTPUploadFile(asusnet, Filepath1, username, password, &rom, &romsize);
    if(rom != NULL)
    	gBS->FreePool((void *)rom);
    if(HandleBuffer != NULL)
        	Status = gBS->FreePool(HandleBuffer);
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
