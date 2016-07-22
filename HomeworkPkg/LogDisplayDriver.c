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
#include <EFI.h>
#include <AmiDxeLib.h>
#include <Protocol/DriverBinding.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/HiiFont.h>
#include <Protocol/GraphicsOutput.h>

// {a1f26873-dc4e-44b0-b0ee-1eafae96f0bc} 
#define LOG_DISPLAY_DRIVER_PROTOCOL_GUID \
  { \
    0xa1f26873, 0xdc4e, 0x44b0, {0xb0, 0xee, 0x1e, 0xaf, 0xae, 0x96, 0xf0, 0xbc} \
  }

typedef struct _LOG_DISPLAY_DRIVER_PROTOCOL LOG_DISPLAY_DRIVER_PROTOCOL;

typedef EFI_STATUS (EFIAPI *HOMEWORK_LOG_DISPLAY)(
  IN UINTN                         Value		
);

typedef struct _LOG_DISPLAY_DRIVER_PROTOCOL{
  HOMEWORK_LOG_DISPLAY Log_Display;
};

static BOOLEAN LogDisplay_Ready = FALSE;

extern EFI_HII_FONT_PROTOCOL  *gHiiFont;

typedef UINT32 HOMEWORK_EFI_STATUS_CODE_VALUE;

typedef struct
 {
     HOMEWORK_EFI_STATUS_CODE_VALUE Value;
     UINT8 Byte;
 } HOMEWORK_STATUS_CODE_TO_BYTE_MAP;
 
 HOMEWORK_STATUS_CODE_TO_BYTE_MAP HomeworkProgressCheckpointMap[] =
 {
     { DXE_CORE_STARTED, 0x60 },
     { DXE_NVRAM_INIT, 0x61 },
     { DXE_SBRUN_INIT, 0x62 },
     { DXE_CPU_INIT, 0x63 },
     //reserved for CPU 0x64 - 0x67
     { DXE_NB_HB_INIT, 0x68 },
     { DXE_NB_INIT, 0x69 },
     { DXE_NB_SMM_INIT, 0x6A },
     //reserved for NB 0x6B - 0x6F
     { DXE_SB_INIT, 0x70 },
     { DXE_SB_SMM_INIT, 0x71 },
     { DXE_SB_DEVICES_INIT, 0x72 },
     //reserved for SB 0x73 - 0x77
     { DXE_ACPI_INIT, 0x78 },
     { DXE_CSM_INIT, 0x79 },
     //reserved for AMI use: 0x7A - 0x7F
     //reserved for OEM use: 0x80 - 0x8F
     { DXE_BDS_STARTED, 0x90 },
     { DXE_BDS_CONNECT_DRIVERS, 0x91 },
     { DXE_PCI_BUS_BEGIN, 0x92 },
     { DXE_PCI_BUS_HPC_INIT, 0x93 },
     { DXE_PCI_BUS_ENUM, 0x94 },
     { DXE_PCI_BUS_REQUEST_RESOURCES, 0x95 },
     { DXE_PCI_BUS_ASSIGN_RESOURCES, 0x96 },
     { DXE_CON_OUT_CONNECT, 0x97 },
     { DXE_CON_IN_CONNECT, 0x98 },
     { DXE_SIO_INIT, 0x99 },
     { DXE_USB_BEGIN, 0x9A },
     { DXE_USB_RESET, 0x9B },
     { DXE_USB_DETECT, 0x9C },
     { DXE_USB_ENABLE, 0x9D },
     //reserved for AMI use: 0x9E - 0x9F
     { DXE_IDE_BEGIN, 0xA0 },
     { DXE_IDE_RESET, 0xA1 },
     { DXE_IDE_DETECT, 0xA2 },
     { DXE_IDE_ENABLE, 0xA3 },
     { DXE_SCSI_BEGIN, 0xA4 },
     { DXE_SCSI_RESET, 0xA5 },
     { DXE_SCSI_DETECT, 0xA6 },
     { DXE_SCSI_ENABLE, 0xA7 },
     { DXE_SETUP_VERIFYING_PASSWORD, 0xA8 },
     { DXE_SETUP_START, 0xA9 },
     //reserved for AML use: 0xAA
     { DXE_SETUP_INPUT_WAIT, 0xAB },
     //reserved for AML use: 0xAC
     { DXE_READY_TO_BOOT, 0xAD },
     { DXE_LEGACY_BOOT, 0xAE },
     { DXE_EXIT_BOOT_SERVICES, 0xAF },
     { RT_SET_VIRTUAL_ADDRESS_MAP_BEGIN, 0xB0 },
     { RT_SET_VIRTUAL_ADDRESS_MAP_END, 0xB1 },
     { DXE_LEGACY_OPROM_INIT, 0xB2 },
     { DXE_RESET_SYSTEM, 0xB3 },
     { DXE_USB_HOTPLUG, 0xB4 },
     { DXE_PCI_BUS_HOTPLUG, 0xB5 },
     { DXE_NVRAM_CLEANUP, 0xB6 },
     { DXE_CONFIGURATION_RESET, 0xB7 },
     //reserved for AMI use: 0xB8 - 0xBF
     //reserved for OEM use: 0xC0 - 0xCF

     {0,0}
 };
 
UINT8 HomeworkFindByteCode(HOMEWORK_STATUS_CODE_TO_BYTE_MAP *Map, HOMEWORK_EFI_STATUS_CODE_VALUE Value)
{
    while (Map->Value!=0)
    {
        if (Map->Value==Value)
        {
            return Map->Byte;
        }

        Map++;
    }

    return 0;
}


EFI_GRAPHICS_OUTPUT_PROTOCOL * get_GraphicsOutput()
{
	EFI_GRAPHICS_OUTPUT_PROTOCOL *g_GraphicsOutput;
	EFI_GUID GraphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_STATUS Status = pBS->LocateProtocol(&GraphicsOutputProtocolGuid, NULL, &g_GraphicsOutput);
	if(EFI_ERROR(Status))
	{
		Print(L"GraphicsOutputProtocolGuid error\n");
		return NULL;
	}
	return g_GraphicsOutput;
}

static EFI_FONT_DISPLAY_INFO DefaultSimpleFont = {
		{118,238,0,0x00},
		{0x00,0x00,0x00,0x00},
		EFI_FONT_INFO_ANY_FONT,
		{EFI_HII_FONT_STYLE_NORMAL,0,L'0'}
};
extern EFI_HII_FONT_PROTOCOL *TmdHiiFont;
EFI_STATUS ShowText(CHAR16* str,UINTN x,UINTN y)
{
	EFI_STATUS Status;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *g_GraphicsOutput=get_GraphicsOutput();
	EFI_IMAGE_OUTPUT *Screen=AllocatePool(sizeof(EFI_IMAGE_OUTPUT));
	
       
	Screen->Width=g_GraphicsOutput->Mode->Info->HorizontalResolution;
	Screen->Height=g_GraphicsOutput->Mode->Info->VerticalResolution;
	Screen->Image.Screen=g_GraphicsOutput;
	DefaultSimpleFont.FontInfo.FontStyle=EFI_HII_FONT_STYLE_SHADOW | EFI_HII_FONT_STYLE_BOLD;
	DefaultSimpleFont.FontInfo.FontSize=20;
	Status=gHiiFont->StringToImage(
				gHiiFont,
				EFI_HII_IGNORE_IF_NO_GLYPH | EFI_HII_OUT_FLAG_CLIP |
				EFI_HII_OUT_FLAG_CLIP_CLEAN_X | EFI_HII_OUT_FLAG_CLIP_CLEAN_Y |
				EFI_HII_IGNORE_LINE_BREAK | EFI_HII_DIRECT_TO_SCREEN | EFI_HII_DIRECT_TO_SCREEN,
				(CHAR16 *)str, (const EFI_FONT_DISPLAY_INFO *)(&DefaultSimpleFont),
				&Screen,
				(UINTN)x,(UINTN)y,
				NULL,NULL,NULL);
	if(EFI_ERROR(Status))
	{
		Print(L"StringToImage error\n");
	}
	return EFI_SUCCESS;
}

EFI_STATUS EFIAPI gLog_Display(
   IN UINT32                  Value
)
{
	CHAR16     LogValue = 0;
	CHAR16     String[20];
//	UINTN      lenColumns,Columns,Rows;
	UINTN      size_of_Info;
	UINT32     frameWidth,frameHeight;
	UINT32     Pixel_format,mode,maxMode;
	EFI_STATUS Status;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOut;
	EFI_GUID GraphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;	
        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *Info;
        
//	String = AllocateZeroPool (15 * sizeof (CHAR16));
        LogValue =(CHAR16)HomeworkFindByteCode(HomeworkProgressCheckpointMap,Value);

       if(LogValue)
	        Swprintf(String,L"PORT 80 : 0x%x ",LogValue);
       else
        	Swprintf(String,L"Value : %x ",Value);
        
	Status = pBS->LocateProtocol(&GraphicsOutputProtocolGuid, NULL, &GraphicsOut);
	if(EFI_ERROR(Status))
	{
		Print(L"GraphicsOutputProtocolGuid error.\n");
		return EFI_SUCCESS;
	}
	maxMode=GraphicsOut->Mode->MaxMode;
	mode=GraphicsOut->Mode->Mode;
	frameWidth=(UINT32)GraphicsOut->Mode->Info->HorizontalResolution;
	frameHeight=(UINT32)GraphicsOut->Mode->Info->VerticalResolution;
	Pixel_format=(UINT32)GraphicsOut->Mode->Info->PixelFormat;
	GraphicsOut->QueryMode(GraphicsOut,mode,&size_of_Info,&Info);
	FreePool(Info);
	ShowText(String,frameWidth-150,frameHeight-40);

	/*  it's text outpit mode!
	lenColumns = Wcslen(String);
	pST->ConOut->QueryMode(pST->ConOut,pST->ConOut->Mode->Mode,&Columns,&Rows);
	pST->ConOut->SetCursorPosition(pST->ConOut,Columns-lenColumns-1,Rows-1);
	pST->ConOut->OutputString(pST->ConOut,String);
	
	Print(L"The NUMBER IN PORT #0x80:%2x\n",LogValue);
        */
	return EFI_SUCCESS;
	
}

EFI_STATUS EFIAPI LogDisplayDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL     *This,
  IN EFI_HANDLE                      ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL        *RemainingDevicePath OPTIONAL
  )
{
       EFI_STATUS Status;      
       EFI_GUID EfiGraphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
       EFI_GRAPHICS_OUTPUT_PROTOCOL *CRISISGraphicsOut = NULL;
       
       if (LogDisplay_Ready == TRUE)
               return EFI_ALREADY_STARTED;

       if (CRISISGraphicsOut == NULL) {
               Status = pBS->HandleProtocol(ControllerHandle, &EfiGraphicsOutputProtocolGuid, &CRISISGraphicsOut);
               if (EFI_ERROR(Status))
	              return Status;
       }
       
       return EFI_SUCCESS;
}

EFI_STATUS EFIAPI LogDisplayDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL     *This,
  IN EFI_HANDLE                      ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL        *RemainingDevicePath OPTIONAL
  )
{
	EFI_STATUS Status = EFI_SUCCESS;
	LOG_DISPLAY_DRIVER_PROTOCOL *LogDisplayDriverProtocol = NULL;
	EFI_GUID   gLogDisplayDriverProtocolGuid = LOG_DISPLAY_DRIVER_PROTOCOL_GUID;
	
	if (LogDisplay_Ready == TRUE)
	        return EFI_ALREADY_STARTED;
	    
	LogDisplay_Ready = TRUE;
	
	//pBS->AllocatePool(EfiBootServicesData, sizeof(LOG_DISPLAY_DRIVER_PROTOCOL), &LogDisplayDriverProtocol);

	LogDisplayDriverProtocol = AllocatePool(sizeof(LOG_DISPLAY_DRIVER_PROTOCOL));
	LogDisplayDriverProtocol->Log_Display=(HOMEWORK_LOG_DISPLAY)gLog_Display;
	Status = pBS->InstallProtocolInterface(
			&ControllerHandle,
			&gLogDisplayDriverProtocolGuid,
			EFI_NATIVE_INTERFACE,
			LogDisplayDriverProtocol);

	if(EFI_ERROR(Status)){
		FreePool(LogDisplayDriverProtocol);
		return Status;
	}
	
	return Status;
}

EFI_STATUS EFIAPI LogDisplayDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL     *This,
  IN EFI_HANDLE                      ControllerHandle,
  IN UINTN                           NumberOfChildren,
  IN EFI_HANDLE                      *ChildHandleBuffer OPTIONAL
  )
{
	EFI_STATUS Status = EFI_SUCCESS;
	       return Status;
}

static EFI_DRIVER_BINDING_PROTOCOL LogDisplayDriverBinding = {
  LogDisplayDriverBindingSupported,
  LogDisplayDriverBindingStart,
  LogDisplayDriverBindingStop,
  0x00,
  NULL,
  NULL
};

EFI_STATUS EFIAPI LogDisplayDriver(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	InitAmiLib(ImageHandle,SystemTable);
	return EfiLibInstallDriverBindingComponentName2 (
		           ImageHandle,
		           SystemTable,
		           &LogDisplayDriverBinding,
		           ImageHandle,
		           NULL,
		           NULL
		           );
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
