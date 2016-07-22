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

/**
#include <efi.h>
#include <pei.h>
#include <Protocol\SimpleTextOut.h>
#include <Protocol\SimpleTextIn.h>
#include <Protocol\GraphicsOutput.h>
#include <Protocol\SimpleFileSystem.h>
#include <Protocol\ConsoleControl.h>
#include <Protocol\BSProtocol.h>
#include <Protocol\RTProtocol.h>	//AK-TML1003c
#include <Protocol\ProjDxeProtocol.h>
#include <Protocol\DxeLib.h>
#include <token.h>
#include <AmiLib.h>**/
#include<EmbDbg.h>
#include <AMIDxeLib.h>
#include <Library/UefiLib.h>

EFI_STATUS MainEntry(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status=EFI_SUCCESS;
    EFI_GUID gVdbgProtocolGuild = VDBG_PROTOCOL_GUID;
    VDBG_PI_LIST *VDbgProtocol;
	    
    InitAmiLib(ImageHandle, SystemTable);	    

    Status = pBS->LocateProtocol(&gVdbgProtocolGuild,NULL,&VDbgProtocol);
    if (EFI_ERROR(Status))
        return Status;
    VDbgProtocol->Printf("[BoyceHong] SetupInvokeEfi VDB Success!\r\n");
    if (EFI_ERROR(Status))
        return Status;
       
    Print(L"[BoyceHong] SetupInvokeEfi MainEntry!\n");
//    TRACE((-1,"[BoyceHong] SetupInvokeEfi MainEntry!\n"));
    
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
