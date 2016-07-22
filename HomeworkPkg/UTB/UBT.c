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

#include "UBT.h"
#include "Subutility.h"
#include "GetSmbios.h"

//#define DEBUG_INFO
#define RD_MODE  //DRK_UBT005

extern UINT8 STRING_ARRAY_NAME[];
EFI_HII_HANDLE  	HiiHandle;

EFI_SYSTEM_TABLE 		*gST;
EFI_BOOT_SERVICES 		*gBS;
EFI_RUNTIME_SERVICES 	*gRS;

EFI_FILE_PROTOCOL 		*gFP;
EFI_GUID gEfiLoadedImageGuid 		= EFI_LOADED_IMAGE_PROTOCOL_GUID;
EFI_GUID gEfiSimpleFileSystemGuid	= EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

//static UINT8 BIOS_EXTFBuf[100];
EXTFStruct BIOS_EXTFBuf[sizeof(EXTFStruct)];
EXTFStruct *BIOS_EXTF = (EXTFStruct *)BIOS_EXTFBuf;

//static UINT8 BIOS_FlashSMIBuf[50];
FLASH_SMI_Table BIOS_FlashSMIBuf[sizeof(FLASH_SMI_Table)];
FLASH_SMI_Table *BIOS_FlashSMI = (FLASH_SMI_Table *)BIOS_FlashSMIBuf;

//static UINT8 BIOS_BIOSInfoBuf[100];
BIOSInfoStruct BIOS_BIOSInfoBuf[sizeof(BIOSInfoStruct)];
BIOSInfoStruct *BIOS_BIOSInfo = (BIOSInfoStruct *)BIOS_BIOSInfoBuf;

//static UINT8 Image_BIOSInfoBuf[100];
//DRK_UBT004- >>>
//BIOSInfoStruct Image_BIOSInfoBuf[sizeof(BIOSInfoStruct)];
//BIOSInfoStruct *Image_BIOSInfo = (BIOSInfoStruct *)Image_BIOSInfoBuf;
//DRK_UBT004- <<<

//static FlashToolSignatureStruct FTStructure = {
FlashToolSignatureStruct FTStructure = {
	'_','F','T','_',	// Signature
	32,					// Length of Structure
	0x00,				// Structure's Minor revision
	0x01,				// Structure's Major revision
	0x0200,				// unique ID number	//0x0200 for AFlash2
	0x00,				// Tool's minor revision
	0x04,				// Tool's major revision
	{0x00,0x00,0x0000},	// Built Date: Day, Month, Year
	0x01,				// bit[0]: allow to flash another BIOS bit[31:1]: reserved
	0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF
};

UINT32 LocateEXTF(UINT32 start, UINT32 len)
{
//	static UINT8 EXTFSig[] = "EXTF";
	UINT8 EXTFSig[] = "EXTF";
//	static UINT8 Mem[4];
	UINT8 Mem[4];
	UINT32 i;

	for (i=0; i<len; i+=4)
	{
		ReadMem_n(start+i, Mem, 4);
		if(memcmp(Mem, EXTFSig, 4) == 0)
			return (start+i);
	}

	return (UINT32)0;
}

VOID GetEXTFInformation(UINT32 linearAddr)
{
	//linearAddr = EXTF的位址
//	UINTN i;
//	UINT8 c;
	//從Addr 讀取資料到 BIOS_EXTFBuf
//	ReadMem_n(linearAddr, BIOS_EXTFBuf, sizeof(EXTFStruct));
	ReadMem_n(linearAddr, (UINT8*)BIOS_EXTF, sizeof(EXTFStruct));
//DRK_UBT003- >>>
//	for (i=0; ; i++)
//	{
//		// EXTFStruct *BIOS_EXTF = (EXTFStruct *)BIOS_EXTFBuf;
//		// BIOS_EXTFBuf第18個Byte代表"的是Address的值"
//		c = ReadMem_8((UINT32)(BIOS_EXTF->ModelNameStrPtr)+(UINT32)(i));
//
//		if (c)
//		{
//			BIOS_EXTF->ModelName[i] = c;
//		}
//		else
//		{
//			BIOS_EXTF->ModelName[i] = '\0';
//			break;
//		}
//	}
//DRK_UBT003- <<<
}

VOID GetFlashSMIInformation(UINT32 linearAddr)
{
//	ReadMem_n(linearAddr, BIOS_FlashSMIBuf, sizeof(FLASH_SMI_Table));
	ReadMem_n(linearAddr, (UINT8*)BIOS_FlashSMI, sizeof(FLASH_SMI_Table));
}

UINT16 GPNV_Initial(UINT32 FADR)
{
	//1FF000
	//FADR: 32-bit physical address of 4 KB Buffer

	UINTN i;
	UINT8 c;
	UINT16 FSTA;
	
	UINT32 ACPIMW = BIOS_FlashSMI->ACPIRAMW;
	UINT32 TRTYPtr = ACPIMW + BIOS_FlashSMI->TRTYOffset;
	UINT32 FADRPtr = ACPIMW + BIOS_FlashSMI->FADROffset;
	UINT32 FSTAPtr = ACPIMW + BIOS_FlashSMI->FSTAOffset;
	UINT32 FSFNPtr = ACPIMW + BIOS_FlashSMI->FSFNOffset;

	FTStructure.s.Month = 0x10;
	FTStructure.s.Day 	= 0x04;
	FTStructure.s.Year 	= 0x2011;
	
	for (i=0; i<sizeof(FlashToolSignatureStruct); i++)
	{
		c = ((UINT8 *)&FTStructure)[i];
		WriteMem_8((UINT32)(FADR)+(UINT32)(i),c);	// 把FlashToolSignatureStruct 的內容寫入 0x1FF000
	}
	
	for (; i<0x1000; i++) 
		WriteMem_8((UINT32)(FADR)+(UINT32)(i),0);	//把0x1FF000+sizeof(FlashToolSignatureStruct)後的資料寫0
	
	WriteMem_8(TRTYPtr, BIOS_FlashSMI->FlashSwSmiSubCode);
	WriteMem_32(FADRPtr, FADR);
	WriteMem_8(FSFNPtr, 0x01);	//WinFlashFun01
	
#ifdef DEBUG_INFO
	Print(L"---GPNV_Initial--- Tirgger SMI\n");
#endif

	zout_8(BIOS_FlashSMI->SMIPort, BIOS_FlashSMI->FlashSwSmiCode);
	FSTA = ReadMem_16(FSTAPtr);
	
#ifdef DEBUG_INFO	
	if (FSTA == 0)
		Print(L"---GPNV_Initial--- Initialize OK\n");
	else if (FSTA == 0x01)
		Print(L"---GPNV_Initial--- Incompatible Flash Tool\n");
	else if (FSTA == 0x04)
		Print(L"---GPNV_Initial--- Incompatible BIOS Signature Tool\n"); //DRK:no AC refer to WinFlashFun01
	else if (FSTA == 0x08)
		Print(L"---GPNV_Initial--- Incompatible Model\n");  //DRK:no Battery refer to WinFlashFun01
#endif
	return FSTA;
}

BOOLEAN GetCurrentBIOSInformation()
{
	UINT32 EXTFPtr, FlashSMIPtr, BIOSInfoPtr, actual_flashsize, ROMStart;
	UINT16 status;
	UINTN tempSize;
	UINTN flashsize = 0;
	UINT8 i = 0;
	
	//Get EXTF
	EXTFPtr = LocateEXTF(0xF0000, 0x10000);	//從0XF0000,開始找字串EXTF,找到後回傳位址
	if (EXTFPtr == (UINT32)0)
	{
#ifdef DEBUG_INFO
		Print(L"---GetCurrentBIOSInfo--- Cannot find the EXTF Structure in Current BIOS!!\n");
#endif
		return FALSE;
	}
	
#ifdef DEBUG_INFO
	Print(L"---GetCurrentBIOSInfo--- EXTFPtr = %xh !\n", EXTFPtr);
#endif
	GetEXTFInformation(EXTFPtr);
	
	//Get SMI Information 
	FlashSMIPtr = (0xF0000 + BIOS_EXTF->FlashSmiTable);//Known issue:Non-exist when disable CSM.

#ifdef DEBUG_INFO
	Print(L"---GetCurrentBIOSInfo--- FlashSMIPtr = %xh !\n", 0xF0000+BIOS_EXTF->FlashSmiTable);
#endif
	GetFlashSMIInformation(FlashSMIPtr);
	
//	Flash_ASL_ROM_Control(0x00);
	
	status = GPNV_Initial(BUFFER_ADDR);

//	Flash_ASL_ROM_Control(0x01);
	
	for (i=0 ;i<4; i++)
	{
		tempSize = ReadMem_8(BUFFER_ADDR + 0x100 + i);
		flashsize = flashsize | tempSize<<(8*i);
	}

//DRK_UBT005 >>>
#ifdef RD_MODE
	if (status != 0 && (status&~NO_AC_POWER) && (status&~NO_BATTERY))
#else
	if (status != 0)
#endif
//DRK_UBT005 <<<
	{
//DRK_UBT002 >>>
		if (status&NO_AC_POWER) Print(L"Please check if AC power plug in...\n");
		else if (status&NO_BATTERY) Print(L"Please check if battery plug in...\n");
		else Print(L"Initialize the BIOS data structure failly!\nError Code:%x", status);
//DRK_UBT002 <<<
#ifdef DEBUG_INFO
		Print(L"---GetCurrentBIOSInfo--- Error:%x\n", status);
		Print(L"flashsize = %x\n", flashsize);
#endif
		return FALSE;
	}
		
	actual_flashsize = (UINT32)flashsize;
	ROMStart = ((UINT32)0xFFFFFFFF) - actual_flashsize + 1;
	
#ifdef DEBUG_INFO
	Print(L"---GetCurrentBIOSInfo--- ROM Start:%x, Size:%x!\n",ROMStart,actual_flashsize);
#endif
	
	//BIOSInfoPtr = LocateBIOSInfo(ROMStart,actual_flashsize);
	BIOSInfoPtr = LocateBIOSInfo(SEARCH_BIOSINFO_ADDRESS, 0, 1);
	
#ifdef DEBUG_INFO
	Print(L"---GetCurrentBIOSInfo--- BIOSInfoPtr %x\n",BIOSInfoPtr);
#endif
	
	if (BIOSInfoPtr == (UINT32)0)
	{
		Print(L"\nCannot find the BIOSInfo structure in Current BIOS!!\n");
		return FALSE;
	}

//	GetBIOSInfoInformation(BIOSInfoPtr,1); //DRK_UBT004-
	GetBIOSInfoInformation(BIOSInfoPtr);  //DRK_UBT004+
		
	return TRUE;
}

//VOID GetBIOSInfoInformation(UINT32 LinearAddr,UINTN BIOSorImage)  //DRK_UBT004-
VOID GetBIOSInfoInformation(UINT32 LinearAddr)  //DRK_UBT004+
{
//DRK_UBT004- >>>
//	UINTN i;
//	UINT8 c;
//	if(BIOSorImage == 1)
//	{
//DRK_UBT004- <<<
//		ReadMem_n(LinearAddr,BIOS_BIOSInfoBuf,sizeof(BIOSInfoStruct));
		ReadMem_n(LinearAddr,(UINT8*)BIOS_BIOSInfo,sizeof(BIOSInfoStruct));
//DRK_UBT004- >>>
//		for(i=0;;i++)
//		{
//			c = ReadMem_8((UINT32)(LinearAddr) + (UINT32)(BIOS_BIOSInfo->BIOSRevOffset)+(UINT32)(i));
//			if(c)
//			{
//				BIOS_BIOSInfo->BIOSRev[i] = c;
//			}
//			else
//			{
//				BIOS_BIOSInfo->BIOSRev[i] = '\0';
//				break;
//			}
//		}
//		for(i=0;;i++)
//		{
//			c = ReadMem_8((UINT32)(LinearAddr) + (UINT32)(BIOS_BIOSInfo->BIOSDateOffset)+(UINT32)(i));
//			if(c)
//			{
//				BIOS_BIOSInfo->BIOSDate[i] = c;
//			}
//			else
//			{
//				BIOS_BIOSInfo->BIOSDate[i] = '\0';
//				break;
//			}
//		}
//		for(i=0;;i++)
//		{
//			c = ReadMem_8((UINT32)(LinearAddr) + (UINT32)(BIOS_BIOSInfo->BIOSCoreRevOffset)+(UINT32)(i));
//			if(c)
//			{
//				BIOS_BIOSInfo->BIOSCoreRev[i] = c;
//			}
//			else
//			{
//				BIOS_BIOSInfo->BIOSCoreRev[i] = '\0';
//				break;
//			}
//		}
//	}
//	else
//	{
//		ReadMem_n(LinearAddr,Image_BIOSInfoBuf,sizeof(BIOSInfoStruct));
//		ReadMem_n(LinearAddr,(UINT8*)Image_BIOSInfo,sizeof(BIOSInfoStruct));
//		for(i=0;;i++)
//		{
//			c = ReadMem_8((UINT32)(LinearAddr) + (UINT32)(Image_BIOSInfo->BIOSRevOffset)+(UINT32)(i));
//			if(c)
//			{
//				Image_BIOSInfo->BIOSRev[i] = c;
//			}
//			else
//			{
//				Image_BIOSInfo->BIOSRev[i] = '\0';
//				break;
//			}
//		}
//		for(i=0;;i++)
//		{
//			c = ReadMem_8((UINT32)(LinearAddr) + (UINT32)(Image_BIOSInfo->BIOSDateOffset)+(UINT32)(i));
//			if(c)
//			{
//				Image_BIOSInfo->BIOSDate[i] = c;
//			}
//			else
//			{
//				Image_BIOSInfo->BIOSDate[i] = '\0';
//				break;
//			}
//		}
//		for(i=0;;i++)
//		{
//			c = ReadMem_8((UINT32)(LinearAddr) + (UINT32)(Image_BIOSInfo->BIOSCoreRevOffset)+(UINT32)(i));
//			if(c)
//			{
//				Image_BIOSInfo->BIOSCoreRev[i] = c;
//			}
//			else
//			{
//				Image_BIOSInfo->BIOSCoreRev[i] = '\0';
//				break;
//			}
//		}
//	}
//DRK_UBT004- <<<
}

//VOID Flash_ASL_ROM_Control(UINT16 FSTA)
//{
//	UINT32 ACPIMW = BIOS_FlashSMI->ACPIRAMW;
//	UINT32 TRTYPtr = ACPIMW + BIOS_FlashSMI->TRTYOffset;
//	UINT32 FSTAPtr = ACPIMW + BIOS_FlashSMI->FSTAOffset;
//	UINT32 FSFNPtr = ACPIMW + BIOS_FlashSMI->FSFNOffset;
//
//	WriteMem_8(TRTYPtr,BIOS_FlashSMI->FlashSwSmiSubCode);
//	WriteMem_16(FSTAPtr,FSTA);	// 0x00 or 0x01
//	WriteMem_8(FSFNPtr,0x00);
//	zout_8(BIOS_FlashSMI->SMIPort,BIOS_FlashSMI->FlashSwSmiCode);
//}

UINT32 LocateBIOSInfo(UINT32 Start, UINT32 len, UINT8 TTB)
{
	UINT8 BIOSInfoSig[] = "BIOSINFO";
	UINT8 Mem[8];
	UINT32 i;
	
	if (TTB)
	{
		for(i=(Start-8+1); i>=len; i-=4)
		{
			ReadMem_n(i,Mem,8);
			if(memcmp(Mem,BIOSInfoSig,8) == 0)
				return (i);
		}
	}
	else
	{
		for(i=0;i<len;i+=4)
		{
			ReadMem_n(Start+i,Mem,8);
			if(memcmp(Mem,BIOSInfoSig,8) == 0)
				return (Start+i);
		}
	}
	return (UINT32)0;
}

UINT16 GetGPNV(UINT32 FADR)
{
	//FSTA: 0 - Enable BIOS ROM writting
	//      1 - Disable BIOS ROM writing
	UINT16 FSTA = 1;
	UINT32 ACPIMW = BIOS_FlashSMI->ACPIRAMW;
	UINT32 TRTYPtr = ACPIMW + BIOS_FlashSMI->TRTYOffset;		//SMI SubCode
	UINT32 FSTAPtr = ACPIMW + BIOS_FlashSMI->FSTAOffset;		//data size
	UINT32 FSFNPtr = ACPIMW + BIOS_FlashSMI->FSFNOffset;		
	UINT32 FADRPtr = ACPIMW + BIOS_FlashSMI->FADROffset;		//CNFG or MFG0
	
	WriteMem_8(TRTYPtr, BIOS_FlashSMI->FlashSwSmiSubCode);
	WriteMem_8(FSFNPtr, 0x05);	// get WinFlashFun05
	WriteMem_32(FADRPtr, FADR);	// CNFG or MFG0

#ifdef DEBUG_INFO	
	Print(L"---GetGPNV--- FlashSwSmiCode = %xh\n", BIOS_FlashSMI->FlashSwSmiCode);
	Print(L"---GetGPNV--- FlashSwSmiSubCode = %xh\n", BIOS_FlashSMI->FlashSwSmiSubCode);
	Print(L"---GetGPNV--- Write SwSmiSubCode to %xh(TRTYPtr)\n", TRTYPtr);
	Print(L"---GetGPNV--- BIOS_FlashSMI->SMIPort = %xh\n", BIOS_FlashSMI->SMIPort);
	Print(L"---GetGPNV--- Tirgger SMI\n");
#endif
	
	zout_8(BIOS_FlashSMI->SMIPort, BIOS_FlashSMI->FlashSwSmiCode);
	FSTA = ReadMem_16(FSTAPtr);

	return FSTA;
}

VOID GetMAC(UINT32 sig, UINT32 offset)
{
	UINT8 GPNVBuffer = 0;
	UINT16 i;
	
	Print(PRINT_TABLE_PARTITION_3);
	Print(L"MAC address = ");
	
	// We check length here, because we want to tell user detailly when error occur
	if (sig == Sig2Int('A', 'S', 'F', 'G'))
	{
		Print(L"Offset out of block size range!!\n\n");
	}
	else if(sig!=Sig2Int('_', 'D', 'M', 'I') && offset+6-1>0xFF)
	{
		Print(L"Offset out of block size range!!\n\n");
	}
	else if(sig==Sig2Int('_', 'D', 'M', 'I') && offset+6-1>0x7ff)
	{
		Print(L"Offset out of block size range!!\n\n");
	}
	else
	{
		for (i=0; i<6; i++)
		{
			ReadMem_n(BUFFER_ADDR+offset+i, &GPNVBuffer, 1);
			Print(L"%02x ", GPNVBuffer);		
		}
		Print(L"\n\n");
	}
}

EFI_STATUS BatchWriteFile(EFI_FILE_PROTOCOL *File, UINT8 * wBuf, UINT8 *wIndex, UINTN *wSize, UINT8 wData)
{
	EFI_STATUS	Status = EFI_SUCCESS;
	UINTN batchSize;
	batchSize = *wSize;
	if (*wIndex < batchSize)
	{
		wBuf[*wIndex] = wData;
		(*wIndex)++;
	}
	else
	{
		Status = File->Write(File, &batchSize, wBuf);
		*wIndex = 0;
		SetMem(wBuf, *wSize, 0);
		wBuf[*wIndex] = wData;
		(*wIndex)++;
	}
	
	return Status;
}

UINT8 ToASCII(UINT8 buf, UINT8 highByte)
{
	UINT8 ascii = 0;
	
	if (highByte)
		ascii = (buf>>4) & 0x0f;
	else
		ascii = buf & 0x0f;
	
	if (ascii <= 0x09)
		ascii = ascii + 0x30;
	else
		ascii = (ascii - 0x0a) + 0x41 ;
	
	return ascii;
}
//DRK_UBT006- >>>
//UINT8 ToRawNumber(UINT8 highByte, UINT8 lowByte)
//{
//	UINT8 rawNumber = 0;
//	
//	if (highByte>=0x30 && highByte<=0x39)
//		highByte = highByte - 0x30;
//	else if (highByte>=0x41 && highByte<=0x46)
//		highByte = (highByte - 0x41) + 0x0a ;
//	else if (highByte>=0x61 && highByte<=0x66)
//		highByte = (highByte - 0x61) + 0x0a ;
//	else
//		return 0;
//	
//	if (lowByte>=0x30 && lowByte<=0x39)
//		lowByte = lowByte - 0x30;
//	else if (lowByte>=0x41 && lowByte<=0x46)
//		lowByte = (lowByte - 0x41) + 0x0a ;
//	else if (lowByte>=0x61 && lowByte<=0x66)
//		lowByte = (lowByte - 0x61) + 0x0a ;	
//	else
//		return 0;
//		
//	rawNumber = (highByte<<4) | lowByte;
//		
//	return rawNumber;
//}
//DRK_UBT006- <<<

UINT16 RWGPNVFile(EFI_HANDLE ImageHandle, UINT32 FADR, UINT32 addr, UINT16 length, CHAR16 * fileName, UINT8 FE, UINT8 RWOP)
{
	UINT8 GPNVBuffer = 0;
	UINT16 i, j;
	UINT64 openAttr;
	CHAR16 EDABStr[5];
	
	EFI_STATUS	Status = EFI_SUCCESS;
	EFI_LOADED_IMAGE_PROTOCOL *imageProt;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fileSysProt;
	EFI_FILE_PROTOCOL *File;
		
	Status = gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageGuid, &imageProt);
	if(!EFI_ERROR(Status))
	{
		Status = gBS->HandleProtocol(imageProt->DeviceHandle, &gEfiSimpleFileSystemGuid, &fileSysProt);
		if(!EFI_ERROR(Status))
		{
			Status = fileSysProt->OpenVolume(fileSysProt, &gFP);
		}
	}
	
	openAttr = EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE;
	
	if (RWOP)
		openAttr = EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE;
	
	
	Status=gFP->Open(gFP, &File, fileName, openAttr, 0);
	
	Print(PRINT_TABLE_PARTITION_3);
	
	if(EFI_ERROR(Status))
	{
		Print(L"Error!! Can't open the file.\n\n\n");
		return 0;
	}
	
	if (RWOP)
	{
//		Print(L"Read offset %02xh ~ %02xh", addr, addr+length-1);  //DRK_UBT006-
		Print(L"Read offset %02xh ~ %02xh", addr, addr+length);  //DRK_UBT006+
		Print(L", Data = ");
		
		if (length > 0x08)
			j = 0x08;
		else
			j = length;
		
		for (i=0; i<j; i++)
		{
			ReadMem_n(BUFFER_ADDR+addr+i, &GPNVBuffer, 1);
			if (i < 0x08)
				Print(L"%x ", GPNVBuffer);
		}
		
		if (length > 0x08)
			Print(L"......");

		Print(L"\nPlease wait for write data to %s\n", fileName);
		SetGPNVToFile(File, addr, length, FE);
	}
	else
	{
		Int2Sig(FADR, EDABStr, 5);
//DRK_UBT006 >>>
		Print(L"Read data from file and write data to %s block offset %02xh ~ %02xh",EDABStr, addr, addr+length);		
//		Print(L"Read data from file and write data to %s block offset %02xh ~ %02xh",EDABStr, addr, addr+length-1);
//		Print(L"\nPlease wait for write data to %s\n", EDABStr);
		Print(L"\nPrepare writing data to %s...\n", EDABStr);
//DRK_UBT006 <<<
		SetFileToGPNV(File, FADR, addr, length, FE);
	}
	
	File->Close(File);
	
	Print(L"\n\n\n");	
		
	return 1; 
}

EFI_STATUS SetFileToGPNV(EFI_FILE_PROTOCOL *File, UINT32 FADR, UINT32 addr, UINT16 length, UINT8 FE)
{
	EFI_STATUS	Status = EFI_SUCCESS;
//	static UINT8 rBuf[256];
//	UINT8 rBuf[256];  //DRK_UBT006-
//	static UINT8 data[256];
//DRK_UBT006- >>>
//	UINT8 data[256];
//	UINTN rSize = 256;  
//	UINTN dSize = 256;
//	UINT16 i, j, d;
//	UINT8 getPair = 0;
//	UINT8 p1 = 0;
//DRK_UBT006- <<<
//DRK_UBT006+ >>>
	UINT8 CNFGData[255];
	UINT8 *ReadBuf;
	UINTN ReadBufSize;
	UINT16 FileDataPtr = 0, NumbersOfData = 0;
	UINT8 datatmp;
	UINT16 WinFlashStatus;

	
	if (FE)  //DRK:TXT
	{
		ReadBufSize=800;
		ReadBuf=(UINT8*)AllocatePool(ReadBufSize*sizeof(UINT8));
		SetMem(ReadBuf, ReadBufSize, 0);
		Status=File->Read(File, &ReadBufSize, ReadBuf);
		if(EFI_ERROR(Status))
		{
			Print(L"...System doesn't have enough memory!!\n");
			FreePool(ReadBuf);
			return Status;
		}
		
		do //DRK:Get how many numbers in file and build CNFGData[]
		{
			if ((UINT8)*(ReadBuf+FileDataPtr)==32) //DRK:32=>Space
			{
				datatmp = 0;
				if ((UINT8)*(ReadBuf+FileDataPtr-2) < 58) datatmp=datatmp+((UINT8)*(ReadBuf+FileDataPtr-2)-48)*16; //DRK:0~9*16
				if ((UINT8)*(ReadBuf+FileDataPtr-2) > 64) datatmp=datatmp+((UINT8)*(ReadBuf+FileDataPtr-2)-55)*16; //DRK:A~F*16
				if ((UINT8)*(ReadBuf+FileDataPtr-1) < 58) datatmp=datatmp+((UINT8)*(ReadBuf+FileDataPtr-1)-48); //DRK:0~9
				if ((UINT8)*(ReadBuf+FileDataPtr-1) > 64) datatmp=datatmp+((UINT8)*(ReadBuf+FileDataPtr-1)-55); //DRK:A~F
				CNFGData[NumbersOfData]=datatmp;
				NumbersOfData++;
			}
			FileDataPtr++;
		} while((UINT8)*(ReadBuf+FileDataPtr)!=0);
		
		if ((NumbersOfData < (length+1) || (addr+length) > 0xFF))  //DRK:check input length and numbers from file
		{
			Print(L"...Input length isn't match!\n");
			FreePool(ReadBuf);
			return EFI_ABORTED;
		}

		WinFlashStatus=SetGPNV(FADR, addr, CNFGData, length+1);  //DRK:FADR=CNFG or GPNV

		if (WinFlashStatus==0xFFFF)
		{
			Print(L"WinFlash Function Error! Writing aborted!\n");
			FreePool(ReadBuf);
			return EFI_ABORTED;
		}
		Print(L"Write Success!\n");
		FreePool(ReadBuf);
		return EFI_SUCCESS;		
	}
	else  //DRK:BIN
	{	
		ReadBufSize=256;
		ReadBuf=(UINT8*)AllocatePool(ReadBufSize*sizeof(UINT8));
//		SetMem(ReadBuf, ReadBufSize, 0);
		Status=File->Read(File, &ReadBufSize, ReadBuf);
		if(EFI_ERROR(Status))
		{
			Print(L"...System doesn't have enough memory!!\n");
			FreePool(ReadBuf);
			return Status;
		}

			
//DRK_UBT007-		if ((ReadBufSize < (length+1) || (addr+length) > 0xFF))  //DRK:check input length and numbers from file
		if (((UINT16)ReadBufSize < (length+1) || (addr+length) > 0xFF))  //DRK:check input length and numbers from file //DRK_UBT007+
		{
			Print(L"...Input length isn't match!\n");
			FreePool(ReadBuf);
			return EFI_ABORTED;
		}
	
		WinFlashStatus=SetGPNV(FADR, addr, ReadBuf, length+1);	//DRK:FADR=CNFG or GPNV
	
		if (WinFlashStatus==0xFFFF)
		{
			Print(L"WinFlash Function Error! Writing aborted!\n");
			FreePool(ReadBuf);
			return EFI_ABORTED;
		}
		Print(L"Write Success!\n");
		FreePool(ReadBuf);
		return EFI_SUCCESS;  
	}
//DRK_UBT006+ <<<

//DRK_UBT006- >>>	
//	gST->ConOut->EnableCursor(gST->ConOut, 0);
//	if (FE)
//	{
//		j = 0;
//		while (j != length)
//		{		
//			rSize = 256;
//			SetMem(rBuf, sizeof(UINT8)*rSize, 0);
//			SetMem(data, sizeof(UINT8)*dSize, 0);
//			Status=File->Read(File, &rSize, rBuf);
//			if(EFI_ERROR(Status) || rSize == 0)
//				return Status;
//			
//			Print(L"\rWriting Data...     ");			
//			d = 0;	
//			
//			for(i=0; i<rSize; i++) 
//			{
//				if (rBuf[i]!=0x20 && rBuf[i]!=0x0D && rBuf[i]!=0x0A)
//				{
//					if (getPair == 0)
//					{
//						getPair = 1;
//						p1 = rBuf[i];
//					}
//					else
//					{
//						data[d] = ToRawNumber(p1, rBuf[i]);
//						getPair = 0;
//						d++;
//					}
//				}
//			}
//			
//			if (j < length)
//			{
//				if (j+d > length)
//					d = length - j;
//					
//				SetGPNV(FADR, addr+j, data, d);
//				j = j + d;
//			}
//			
//			if (j == length)
//			{
//				Print(L"\nSuccess!!");
//				return EFI_SUCCESS;
//			}		
//		}
//	}
//	else
//	{
//		j = 0;
//		while (j != length)
//		{		
//			rSize = 256;
//			SetMem(rBuf, sizeof(UINT8)*rSize, 0);
//			Status=File->Read(File, &rSize, rBuf);
//			if(EFI_ERROR(Status) || rSize == 0)
//				return Status;
//			
//			Print(L"\rWriting Data...     ");
//			
//			if (j < length)
//			{	
//				if (j+rSize > length)
//					rSize = length - j;
//					
//				SetGPNV(FADR, addr+j, rBuf, rSize);
//				j = j + (UINT16)rSize;
//			}
//			
//			if (j == length)
//			{
//				Print(L"\nSuccess!!");
//				return EFI_SUCCESS;
//			}		
//		}
//	}
//	return Status;
//DRK_UBT006- <<<
}

EFI_STATUS SetGPNVToFile(EFI_FILE_PROTOCOL *File, UINT32 addr, UINT16 length, UINT8 FE)
{
	UINT8 GPNVBuffer = 0;
	UINT16 i;
	UINTN size = 1;
	UINT8 toNumberAscii = 0;
	UINT8 writeBuf[128];
	UINTN bufSize = sizeof(writeBuf);
	UINT8 bufIndex= 0;
	EFI_STATUS	Status = EFI_SUCCESS;
	SetMem(writeBuf, bufSize, 0);
	
//	gST->ConOut->EnableCursor(gST->ConOut, 0);
	
	if (FE)
	{
//		for (i=0; i<length; i++)  //DRK_UBT006-
		for (i=0; i<=length; i++)  //DRK_UBT006+
		{
			ReadMem_n(BUFFER_ADDR+addr+i, &GPNVBuffer, 1);
							
			toNumberAscii = ToASCII(GPNVBuffer, 1);
			Status = BatchWriteFile(File, writeBuf, &bufIndex, &bufSize, toNumberAscii);
			if (EFI_ERROR(Status))break;
					
			toNumberAscii = ToASCII(GPNVBuffer, 0);
			Status = BatchWriteFile(File, writeBuf, &bufIndex, &bufSize, toNumberAscii);
			if (EFI_ERROR(Status))break;
				
			toNumberAscii = 0x0020;
			Status = BatchWriteFile(File, writeBuf, &bufIndex, &bufSize, toNumberAscii);
			if (EFI_ERROR(Status))break;
			
			if (i!=0 && ((i+1)%16==0))
			{
				toNumberAscii = 0x000d;
				Status = BatchWriteFile(File, writeBuf, &bufIndex, &bufSize, toNumberAscii);
				if (EFI_ERROR(Status))break;
				
				toNumberAscii = 0x000a;
				Status = BatchWriteFile(File, writeBuf, &bufIndex, &bufSize, toNumberAscii);
				if (EFI_ERROR(Status))break;
			}
								
			if (i%32 == 1)
				Print(L"\rWriting Data.     ");
			else if (i%32 == 10)
				Print(L"\rWriting Data..    ");
			else if (i%32 == 20)
				Print(L"\rWriting Data...   ");
		}
	}
	else
	{	
//		for (i=0; i<length; i++)  //DRK_UBT006-
		for (i=0; i<=length; i++)  //DRK_UBT006+
		{
			ReadMem_n(BUFFER_ADDR+addr+i, &GPNVBuffer, 1);
			toNumberAscii = GPNVBuffer;
			BatchWriteFile(File, writeBuf, &bufIndex, &bufSize, toNumberAscii);
			
			if (i%32 == 1)
				Print(L"\rWriting Data.     ");
			else if (i%32 == 10)
				Print(L"\rWriting Data..    ");
			else if (i%32 == 20)
				Print(L"\rWriting Data...   ");
		}
	}

	if (bufIndex != 0)
	{
		size = (UINTN)bufIndex;
		Status = File->Write(File, &size, writeBuf);
	}

	if (!EFI_ERROR(Status))
		Print(L"\nSuccess!!");
	else
		Print(L"\nFail!!");
		
	return Status;
}

UINT16 SetGPNV(UINT32 FADR, UINT32 addr, UINT8* data, UINTN size)
{
	//FSTA: 0 - Enable BIOS ROM writting
	//      1 - Disable BIOS ROM writing
	UINT16 FSTA = 0;
	UINT32 ACPIMW = BIOS_FlashSMI->ACPIRAMW;
	UINT32 TRTYPtr = ACPIMW + BIOS_FlashSMI->TRTYOffset;		//SMI SubCode
	UINT32 FSTAPtr = ACPIMW + BIOS_FlashSMI->FSTAOffset;		//data size
	UINT32 FSFNPtr = ACPIMW + BIOS_FlashSMI->FSFNOffset;		
	UINT32 FADRPtr = ACPIMW + BIOS_FlashSMI->FADROffset;		//CNFG or MFG0
		
	WriteMem_n(BUFFER_ADDR+addr, data, size);
	WriteMem_8(TRTYPtr, BIOS_FlashSMI->FlashSwSmiSubCode);
	WriteMem_8(FSFNPtr, 0x04);	
	WriteMem_32(FADRPtr, FADR);	// CNFG or MFG0

#ifdef DEBUG_INFO	
	Print(L"---GetGPNV--- FlashSwSmiCode = %xh\n", BIOS_FlashSMI->FlashSwSmiCode);
	Print(L"---GetGPNV--- FlashSwSmiSubCode = %xh\n", BIOS_FlashSMI->FlashSwSmiSubCode);
	Print(L"---GetGPNV--- Write SwSmiSubCode to %xh(TRTYPtr)\n", TRTYPtr);
	Print(L"---GetGPNV--- BIOS_FlashSMI->SMIPort = %xh\n", BIOS_FlashSMI->SMIPort);
	Print(L"---GetGPNV--- Tirgger SMI\n");
#endif
		
	zout_8(BIOS_FlashSMI->SMIPort, BIOS_FlashSMI->FlashSwSmiCode);
	FSTA = ReadMem_16(FSTAPtr);

	WriteMem_8(FSFNPtr, 0x06);	
	WriteMem_32(FADRPtr, FADR);	// CNFG or MFG0
	zout_8(BIOS_FlashSMI->SMIPort, BIOS_FlashSMI->FlashSwSmiCode);
	FSTA = ReadMem_16(FSTAPtr);
	
	return FSTA;
}

UINT16 MultiSetGPNV(UINT32 sig, UINT32 addr, CHAR16 *data)
{
	UINT16 statusGPNV = 0;
	UINT8 wData[MULTI_WRITE_BUFFER_SIZE];
	UINT8 wDataSize;
	UINT8 wSize;
	UINT8 i=0, odd=0;
	CHAR16 HexStr[3] = L"00";
	
	wDataSize = (UINT8)StrLen(data);
	wSize = wDataSize/2 + wDataSize%2;

	if (wSize > MULTI_WRITE_BUFFER_SIZE)
	{
		wSize = MULTI_WRITE_BUFFER_SIZE;
		wDataSize = wDataSize/2 * 2;
	}	
	
	for (i=0; i<wSize; i++)
	{
		SetMem(HexStr, sizeof(HexStr), 0);
		if (wDataSize%2 && i==0)
		{
			SPrint(HexStr, sizeof(HexStr), L"%02c", *(data+i));
			wData[i] = (UINT8)Xtoi(HexStr);
			odd = 1;
		}
		else
		{
			SPrint(HexStr, sizeof(HexStr), L"%c%c", *(data+i*2-odd), *(data+i*2+1-odd));
			wData[i] = (UINT8)Xtoi(HexStr);
		}
	}
	
	ShowWriteProcess(addr, wData, wSize);
	
	Print(L"\rWriting Data...     ");	
	statusGPNV = SetGPNV(sig, addr, wData, (UINTN)wSize);
	Print(L"\nSuccess!!");
	Print(L"\n\n\n");
	
	return statusGPNV;
}

VOID ShowHints(VOID)
{
	Print(L"Error command\n");
	Print(L"Exp:\n");
	Print(L"UBT.efi -c CNFG /r                       --> Read CNFG\n");
	Print(L"UBT.efi -c CNFG /w 33 FF                 --> Write FFh to offset 33h of CNFG\n");
	Print(L"UBT.efi -c CNFG /len 8 /r 7 /txt abc.txt --> Read CNFG 07h~0Fh and output\n");
	Print(L"                                             to abc.txt(Offset:07h,Length:+08h)\n");
	Print(L"UBT.efi -c CNFG /len 8 /r 7 /bin abc     --> Read CNFG 07h~0Fh and output\n");
	Print(L"                                             to abc which is a binary file\n");
	Print(L"                                             (Offset:07h,Length:+08h)\n");
	Print(L"UBT.efi -c CNFG /len F /w 0 /txt abc.txt --> Read data from file abc and write\n");
	Print(L"                                             to CNFG(Offset:00h,Length:+0Fh)\n");
	Print(L"UBT.efi -c CNFG /len F /w 0 /bin abc     --> Read data from file abc and write\n");
	Print(L"                                             to CNFG, abc is a binary file\n");
	Print(L"                                             (Offset:00h,Length:+0Fh)\n");
	Print(L"UBT.efi -c CNFG /rmac                    --> Read CNFG MAC Address from default\n");
	Print(L"                                             offset D7h\n");
	Print(L"UBT.efi -c CNFG /rmac 13                 --> Read CNFG MAC Address from offset\n");
	Print(L"                                             13h\n");
	Print(L"UBT.efi -c CNFG /I                       --> Display CNFG info\n");
}

UINT32 Int2Sig(UINT32 sig, CHAR16* buf, UINT32 size)
{
	UINT32 writeSize = 4;
	
	if (buf==NULL && size<=4)
		return 0;
	
	SetMem(buf, sizeof(CHAR16)*5, 0);
	StrCpy(buf, SI->Argv[2]); //DRK_UBT008+
//DRK-_UBT008 	if (sig == Sig2Int('C', 'N', 'F', 'G'))
//DRK-_UBT008 		StrCpy(buf, L"CNFG");
//DRK-_UBT008 	if (sig == Sig2Int('M', 'F', 'G', '0'))
//DRK-_UBT008 		StrCpy(buf, L"MFG0");
//DRK-_UBT008 	if (sig == Sig2Int('O', 'E', 'M', '1'))
//DRK-_UBT008 		StrCpy(buf, L"OEM1");
//DRK-_UBT008 	if (sig == Sig2Int('O', 'E', 'M', '2'))
//DRK-_UBT008 		StrCpy(buf, L"OEM2");
//DRK-_UBT008 	if (sig == Sig2Int('S', 'L', 'P', '2'))
//DRK-_UBT008 		StrCpy(buf, L"SLP2");
//DRK-_UBT008 	if (sig == Sig2Int('O', 'E', 'M', '3'))
//DRK-_UBT008 		StrCpy(buf, L"OEM3");
//DRK-_UBT008 	if (sig == Sig2Int('O', 'E', 'M', '5'))
//DRK-_UBT008 		StrCpy(buf, L"OEM5");
//DRK-_UBT008 	if (sig == Sig2Int('A', 'S', 'F', 'G'))
//DRK-_UBT008 		StrCpy(buf, L"ASFG");
//DRK-_UBT008 	if (sig == Sig2Int('V', 'K', 'E', 'Y'))
//DRK-_UBT008 		StrCpy(buf, L"VKEY");
//DRK-_UBT008 	if (sig == Sig2Int('_', 'D', 'M', 'I'))	
//DRK-_UBT008 		StrCpy(buf, L"_DMI");
	
	return writeSize;
}

VOID ShowTitle(UINT16 size, UINT32 sig)
{
	CHAR16 EDABStr[5];
	Int2Sig(sig, EDABStr, 5);
		
	Print(PRINT_TABLE_0(size, EDABStr));	
}
		  
VOID ShowGPNV(UINT16 size, UINT32 sig)
{
	UINT32 i = 0, j = 1, a = 0, c = 0, q = 0, r = 0;
	UINT8 GPNVBuffer = 0;
	CHAR16	HexStr[3] = L"00";
	
	ShowTitle(size, sig);
	Print(PRINT_TABLE_PARTITION_0);
	for (i=0; i<16; i++)
		Print(L"%02x ", i);
		
	Print(PRINT_TABLE_1);	
	Print(L" 00 :    ");

	for (i=0; i<size; i++)
	{
		ReadMem_n(BUFFER_ADDR+i, &GPNVBuffer, 1);
		Print(L"%02x ", GPNVBuffer);
					
		if (i!=0 && (i+1)%16==0)
		{
			Print(PRINT_TABLE_PARTITION_1);
			c = i - 15;
			for (a=0; a<16; a++)
			{				
				ReadMem_n(BUFFER_ADDR+c, &GPNVBuffer, 1);
				if (GPNVBuffer<0x20||GPNVBuffer>0x7F)
				{
					Print(L".");
				}
				else
				{
					SetMem(HexStr, sizeof(HexStr), 0);
					SPrint(HexStr, sizeof(HexStr), L"%c", GPNVBuffer);
					Print(HexStr);
				}
				c++;
			}
			
			Print(L"\n");
			
			if (size>256 && ((i+1)%256)==0 && (i+1)!=size)
			{	
				Print(L"\nPlease press any key to display next page !!!!\n");
//				gST->ConOut->EnableCursor(gST->ConOut, 0);
				Input(NULL, HexStr, 1);
				
#ifndef DEBUG_INFO
				gST->ConOut->ClearScreen(gST->ConOut);
#endif

				ShowTitle(size, sig);
				Print(PRINT_TABLE_PARTITION_0);
				for (a=0; a<16; a++)
					Print(L"%02x ", a);
					
				Print(PRINT_TABLE_1);
			}
						
			if (i != (UINT32)size-1)
			{
				Print(L"%2x0 :    ",j);
				j++;
			}
		}
		
		if ((i+1)==size && (size%16)!=0)
		{
			q = (size / 16) * 16;
			r = size%16;
			for (a=0; a<(16-r) ;a++)
				Print(L"   ");
			
			Print(PRINT_TABLE_PARTITION_1);
						
			for (a=0; a<r ;a++)
			{
				ReadMem_n(BUFFER_ADDR+q, &GPNVBuffer, 1);
				if (GPNVBuffer<0x20||GPNVBuffer>0x7F)
				{
					Print(L".");
				}
				else
				{
					SetMem(HexStr, sizeof(HexStr), 0);
					SPrint(HexStr, sizeof(HexStr), L"%c", GPNVBuffer);
					Print(HexStr);
				}
				q++;
			}
			Print(L"\n");
		}
	}
}

VOID ShowWriteProcess(UINT32 offset, UINT8 *data, UINT8 size)
{
	UINT8 i;
	Print(PRINT_TABLE_PARTITION_3);
	Print(L"Write Data ...");
	Print(L"Read and Check -> Offset %xh ~ %xh Data = ", offset, offset+size-1);
	for (i=0; i<size; i++)
	{
		Print(L"%xh ", data[i]);
	}
	Print(L"\n\n\n");
}

UINT32 ParseCommand(VOID)
{
	UINT32 operation = UNKNOWN_OPERATION;
	UINT32 parameter = UNKNOWN_OPERATION;
	UINTN size = 0;
	UINT32 FE = 0;
	
	if (SI->Argc < 4)
	{
		ShowHints();
		return UNKNOWN_OPERATION;
	}
	
	//Parameter 1
	StrLwr(SI->Argv[1]);
	if (!StrCmp(SI->Argv[1], L"-c"))
	{
//TODO: To limit strlen=4
//DRK_UBT008- 		//CNFG or MFG0
//DRK_UBT008- 		parameter = parameter | CMD_BLK;
		
//DRK_UBT008- 		StrUpr(SI->Argv[2]);
//DRK_UBT008- 		if (!StrCmp(SI->Argv[2], L"CNFG"))
//DRK_UBT008- 			operation = operation | CNFG_OPERATION;
//DRK_UBT008- 		else if	(!StrCmp(SI->Argv[2], L"MFG0"))
//DRK_UBT008- 			operation = operation | MFG0_OPERATION;
//DRK_UBT008- 		else if	(!StrCmp(SI->Argv[2], L"OEM1"))
//DRK_UBT008- 			operation = operation | OEM1_OPERATION;
//DRK_UBT008- 		else if	(!StrCmp(SI->Argv[2], L"OEM2"))
//DRK_UBT008- 			operation = operation | OEM2_OPERATION;
//DRK_UBT008- 		else if	(!StrCmp(SI->Argv[2], L"SLP2"))
//DRK_UBT008- 			operation = operation | SLP2_OPERATION;
//DRK_UBT008- 		else if	(!StrCmp(SI->Argv[2], L"OEM3"))
//DRK_UBT008- 			operation = operation | OEM3_OPERATION;
//DRK_UBT008- 		else if	(!StrCmp(SI->Argv[2], L"OEM5"))
//DRK_UBT008- 			operation = operation | OEM5_OPERATION;
//DRK_UBT008- 		else if	(!StrCmp(SI->Argv[2], L"ASFG"))
//DRK_UBT008- 			operation = operation | ASFG_OPERATION;
//DRK_UBT008- 		else if	(!StrCmp(SI->Argv[2], L"VKEY"))
//DRK_UBT008- 			operation = operation | VKEY_OPERATION;
//DRK_UBT008- 		else if	(!StrCmp(SI->Argv[2], L"_DMI"))
//DRK_UBT008- 			operation = operation | _DMI_OPERATION;
//DRK_UBT008- 		else
//DRK_UBT008- 			parameter = parameter & ~CMD_BLK;
	}
		
	//Parameter 3
	StrUpr(SI->Argv[3]);
	if (!StrCmp(SI->Argv[3], L"/R"))
	{
		operation = operation | READ_OPERATION;
		parameter = parameter | CMD_READ;
	}
	else if (!StrCmp(SI->Argv[3], L"/W"))
	{
		operation = operation | WRITE_OPERATION;
		parameter = parameter | CMD_WRITE;
	}
	else if (!StrCmp(SI->Argv[3], L"/I"))
	{
		operation = operation | CNFG_INFO_OPERATION;
		parameter = parameter | CMD_CNFG_INFO;
	}
	else if (!StrCmp(SI->Argv[3], L"/LEN") || !StrCmp(SI->Argv[3], L"/L"))
	{
		if (SI->Argc == 9)
		{
			StrUpr(SI->Argv[5]);
			
			if (!StrCmp(SI->Argv[5], L"/R"))
			{
				operation = operation | WRITE_TXT_OPERATION;
				parameter = parameter | CMD_WFT;
			}
			
			if (!StrCmp(SI->Argv[5], L"/W"))
			{
				operation = operation | READ_TXT_OPERATION;
				parameter = parameter | CMD_RFT;
			}
		}
	}
	else if (!StrCmp(SI->Argv[3], L"/RMAC"))
	{
		if (SI->Argc > 4)
		{
			operation = operation | RMAC_OPERATION;
			parameter = parameter | CMD_RMAC;
			
			if (!IsNumber(SI->Argv[4]))
				parameter = parameter & ~CMD_RMAC;
		}
		else
		{
			operation = operation | DRMAC_OPERATION;
			parameter = parameter | CMD_DRMAC;
		}
	}
	
	if ((parameter&CMD_CNFG_INFO) != 0)
	{	
		StrUpr(SI->Argv[2]);
		if (StrCmp(SI->Argv[2], L"CNFG"))
			parameter = parameter & ~CMD_CNFG_INFO;
	}
	
	if ((parameter&CMD_WRITE) != 0)
	{
		if (SI->Argc < 6)
		{
			ShowHints();
			return UNKNOWN_OPERATION;
		}
		
		if (!IsNumber(SI->Argv[4]))
			parameter = parameter & ~CMD_WRITE;
				
		if (!IsNumber(SI->Argv[5]))
				parameter = parameter & ~CMD_WRITE;				
				
		size = StrLen(SI->Argv[5]);
		
		if (size%2)
			size = size/2+1;
		else
			size = size/2;
		
		if (Xtoi(SI->Argv[4])>=4&&!StrCmp(SI->Argv[2], L"ASFG") ||
		    (Xtoi(SI->Argv[4])+size-1)>=4&&!StrCmp(SI->Argv[2], L"ASFG"))
			parameter = parameter & ~CMD_WRITE;
		else if (Xtoi(SI->Argv[4])>0xff && StrCmp(SI->Argv[2], L"_DMI"))
			parameter = parameter & ~CMD_WRITE;
		else if (Xtoi(SI->Argv[4])> 0x7ff)
			parameter = parameter & ~CMD_WRITE;
	}
	
	if ((parameter&CMD_WFT) != 0)
	{
		FE = CMD_WFT;
		StrUpr(SI->Argv[7]);
		
		if (StrCmp(SI->Argv[7], L"/TXT") && 
			StrCmp(SI->Argv[7], L"/T")   &&
			StrCmp(SI->Argv[7], L"/BIN") &&
			StrCmp(SI->Argv[7], L"/B"))
		{
			parameter = parameter & ~CMD_WFT;
		}
		
		if (!StrCmp(SI->Argv[7], L"/B") || !StrCmp(SI->Argv[7], L"/BIN"))
		{
			parameter = (parameter & ~CMD_WFT) | CMD_WFB;
			operation = (operation & ~WRITE_TXT_OPERATION) | WRITE_BIN_OPERATION;
			FE = CMD_WFB;
		}
		
		if (Xtoi(SI->Argv[4]) == 0)
			parameter = parameter & ~FE;
				
		if (!IsNumber(SI->Argv[6]))
			parameter = parameter & ~FE;

		if ((Xtoi(SI->Argv[4])+Xtoi(SI->Argv[6])-1)>=4 && !StrCmp(SI->Argv[2], L"ASFG"))
			parameter = parameter & ~FE;
		else if ((Xtoi(SI->Argv[4])+Xtoi(SI->Argv[6])-1)>0xff && StrCmp(SI->Argv[2], L"_DMI"))
			parameter = parameter & ~FE;
		else if ((Xtoi(SI->Argv[4])+Xtoi(SI->Argv[6])-1) > 0x7ff)
			parameter = parameter & ~FE;	
	}
	
	if ((parameter&CMD_RFT) != 0)
	{
		FE = CMD_RFT;
		StrUpr(SI->Argv[7]);
		
		if (StrCmp(SI->Argv[7], L"/TXT") && 
			StrCmp(SI->Argv[7], L"/T")   &&
			StrCmp(SI->Argv[7], L"/BIN") &&
			StrCmp(SI->Argv[7], L"/B"))
		{
			parameter = parameter & ~CMD_RFT;
		}
		
		if (!StrCmp(SI->Argv[7], L"/B") || !StrCmp(SI->Argv[7], L"/BIN"))
		{
			parameter = (parameter & ~CMD_RFT) | CMD_RFB;
			operation = (operation & ~READ_TXT_OPERATION) | READ_BIN_OPERATION;
			FE = CMD_RFB;
		}
		
		if (Xtoi(SI->Argv[4])==0)
			parameter = parameter & ~FE;
				
		if (Xtoi(SI->Argv[6])==0)
		{
			if(StrCmp(L"0",SI->Argv[6]) && StrCmp(L"00",SI->Argv[6]))
				parameter = parameter & ~FE;
		}

		if ((Xtoi(SI->Argv[4])+Xtoi(SI->Argv[6])-1)>=4 && !StrCmp(SI->Argv[2], L"ASFG"))
			parameter = parameter & ~FE;
		else if ((Xtoi(SI->Argv[4])+Xtoi(SI->Argv[6])-1)>0xff && StrCmp(SI->Argv[2], L"_DMI"))
			parameter = parameter & ~FE;
		else if ((Xtoi(SI->Argv[4])+Xtoi(SI->Argv[6])-1) > 0x7ff)
			parameter = parameter & ~FE;	
	}
	
//DRK_UBT008- 	if (parameter != (CMD_BLK|CMD_WRITE) &&
//DRK_UBT008- 		parameter != (CMD_BLK|CMD_READ)  &&
//DRK_UBT008- 		parameter != (CMD_BLK|CMD_CNFG_INFO)  &&
//DRK_UBT008- 		parameter != (CMD_BLK|CMD_WFT)	 &&
//DRK_UBT008- 		parameter != (CMD_BLK|CMD_WFB)	 &&
//DRK_UBT008- 		parameter != (CMD_BLK|CMD_RFT)	 &&
//DRK_UBT008- 		parameter != (CMD_BLK|CMD_RFB)	 &&
//DRK_UBT008- 		parameter != (CMD_BLK|CMD_DRMAC) &&
//DRK_UBT008- 		parameter != (CMD_BLK|CMD_RMAC))
//DRK_UBT008- 	{
//DRK_UBT008- 		ShowHints();
//DRK_UBT008- 		return UNKNOWN_OPERATION;
//DRK_UBT008- 	}
		
	return operation;
}

UINT8 IsNumber(CHAR16 * ptr)
{
	while(*ptr != L'\0')
	{
		if ((*ptr < 0x30) 				|| 
			(*ptr>0x39 && *ptr < 0x41) 	||
			(*ptr>0x46 && *ptr < 0x61)	||
			(*ptr > 0x66))
		{
			return 0;
		}
		ptr++;
	}
	return 1;
}

UINTN StrLenASCII(CHAR8 *str)
{
	CHAR8* ptr = NULL;
	UINTN size = 0;
	
	if (str != NULL)
	{
		ptr = str;
		while (*ptr != 0x00)
		{
			ptr++;
			size++;
		}
	}
	
	return size;	
}

EFI_BOOTSHELL_CODE( EFI_DRIVER_ENTRY_POINT (UBTAppEntry) )
EFI_STATUS UBTAppEntry(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS	Status;
	UINT32 operation = UNKNOWN_OPERATION;
	UINT16 statusGPNV;
	UINT16 dataSize;
	UINT32 sig = 0;
//DRK_UBT008+ >>>
	UINT8  SigNumber;
	UINT16  SigTemp;
	CHAR16 *String;
//DRK_UBT008+ <<<

	EFI_SHELL_APP_INIT(ImageHandle, SystemTable);
	Status = EFI_SUCCESS;
	InitializeShellLib(ImageHandle, SystemTable);
	EFI_NO_BOOTSHELL_CODE(EFI_SHELL_APP_INIT (ImageHandle, SystemTable));

	gST = SystemTable;
	gBS = gST->BootServices;
	gRS	= gST->RuntimeServices;

#ifndef DEBUG_INFO
	gST->ConOut->ClearScreen(gST->ConOut);
#endif
	
	operation = ParseCommand();
	if (operation == UNKNOWN_OPERATION)
		return 0;
		
	if (!CheckASUSSignature())
	{
		Print(L"Check manufacture signature error!!");
		return EFI_SUCCESS;
	}
		
	statusGPNV = GetCurrentBIOSInformation();
	if (!statusGPNV)
		return EFI_SUCCESS;

//DRK_UBT008+ >>>
	String = SI->Argv[2];
	for (SigNumber=0;SigNumber<3;SigNumber++)
	{
		SigTemp = (UINT16)*(UINT16*)String;
		if (SigTemp > 0x60 && SigTemp < 0x7B) SigTemp -= 0x20;
		(UINT16)*(UINT16*)String = SigTemp; //DRK: change to Capital
		sig += SigTemp;
		sig = sig << 0x08;
		String += 1;
	}
	SigTemp = (UINT16)*(UINT16*)String;
	if (SigTemp > 0x60 && SigTemp < 0x7B) SigTemp -= 0x20;
	(UINT16)*(UINT16*)String = SigTemp; //DRK: change to Capital
	sig += SigTemp;
//DRK_UBT008+ <<<

//DRK_UBT008- 	if (CheckOperation(operation, CNFG_OPERATION))
//DRK_UBT008- 		sig = Sig2Int('C', 'N', 'F', 'G');
//DRK_UBT008- 	else if(CheckOperation(operation, MFG0_OPERATION))
//DRK_UBT008- 		sig = Sig2Int('M', 'F', 'G', '0');
//DRK_UBT008- 	else if(CheckOperation(operation, OEM1_OPERATION))
//DRK_UBT008- 		sig = Sig2Int('O', 'E', 'M', '1');
//DRK_UBT008- 	else if(CheckOperation(operation, OEM2_OPERATION))
//DRK_UBT008- 		sig = Sig2Int('O', 'E', 'M', '2');
//DRK_UBT008- 	else if(CheckOperation(operation, SLP2_OPERATION))
//DRK_UBT008- 		sig = Sig2Int('S', 'L', 'P', '2');
//DRK_UBT008- 	else if(CheckOperation(operation, OEM3_OPERATION))
//DRK_UBT008- 		sig = Sig2Int('O', 'E', 'M', '3');
//DRK_UBT008- 	else if(CheckOperation(operation, OEM5_OPERATION))
//DRK_UBT008- 		sig = Sig2Int('O', 'E', 'M', '5');
//DRK_UBT008- 	else if(CheckOperation(operation, ASFG_OPERATION))
//DRK_UBT008- 		sig = Sig2Int('A', 'S', 'F', 'G');
//DRK_UBT008- 	else if(CheckOperation(operation, VKEY_OPERATION))
//DRK_UBT008- 		sig = Sig2Int('V', 'K', 'E', 'Y');
//DRK_UBT008- 	else if(CheckOperation(operation, _DMI_OPERATION))
//DRK_UBT008- 		sig = Sig2Int('_', 'D', 'M', 'I');
//DRK_UBT008+ >>>
	dataSize = GetGPNV(sig);
	if (dataSize == 0x00)
	{
		Print(L"There's no block in GPNV area!!\n");
		return EFI_SUCCESS;
	}
//DRK_UBT008+ <<<
	if (CheckOperation(operation, READ_OPERATION))
	{
//DRK_UBT008- 		dataSize = GetGPNV(sig);
		ShowGPNV(dataSize, sig);
	}
	else if (CheckOperation(operation, CNFG_INFO_OPERATION))
	{
//DRK_UBT008- 		dataSize = GetGPNV(sig);
		ShowCNFG();
	}
	else if (CheckOperation(operation, WRITE_OPERATION))
	{
//DRK_UBT008- 		dataSize = GetGPNV(sig);
		ShowTitle(dataSize, sig);
		MultiSetGPNV(sig, (UINT32)Xtoi(SI->Argv[4]), SI->Argv[5]);
	}
	else if (CheckOperation(operation, WRITE_TXT_OPERATION))
	{
//DRK_UBT008- 		dataSize = GetGPNV(sig);
		ShowTitle(dataSize, sig);
		RWGPNVFile(ImageHandle, sig, (UINT32)Xtoi(SI->Argv[6]), (UINT16)Xtoi(SI->Argv[4]), SI->Argv[8], 1, 1);
	}
	else if (CheckOperation(operation, WRITE_BIN_OPERATION))
	{
//DRK_UBT008- 		dataSize = GetGPNV(sig);
		ShowTitle(dataSize, sig);
		RWGPNVFile(ImageHandle, sig, (UINT32)Xtoi(SI->Argv[6]), (UINT16)Xtoi(SI->Argv[4]), SI->Argv[8], 0, 1);
	}
	else if (CheckOperation(operation, READ_TXT_OPERATION))
	{
//DRK_UBT008- 		dataSize = GetGPNV(sig);
		ShowTitle(dataSize, sig);
		RWGPNVFile(ImageHandle, sig, (UINT32)Xtoi(SI->Argv[6]), (UINT16)Xtoi(SI->Argv[4]), SI->Argv[8], 1, 0);
	}
	else if (CheckOperation(operation, READ_BIN_OPERATION))
	{
//DRK_UBT008- 		dataSize = GetGPNV(sig);
		ShowTitle(dataSize, sig);
		RWGPNVFile(ImageHandle, sig, (UINT32)Xtoi(SI->Argv[6]), (UINT16)Xtoi(SI->Argv[4]), SI->Argv[8], 0, 0);
	}
	else if (CheckOperation(operation, RMAC_OPERATION))
	{
//DRK_UBT008- 		dataSize = GetGPNV(sig);
		ShowTitle(dataSize, sig);
		GetMAC(sig, (UINT32)Xtoi(SI->Argv[4]));
	}
	else if (CheckOperation(operation, DRMAC_OPERATION))
	{
//DRK_UBT008- 		dataSize = GetGPNV(sig);
		ShowTitle(dataSize, sig);
		GetMAC(sig, DEFAULT_MAC_ADDRESS_OFFSET);
	}
//	gST->ConOut->EnableCursor(gST->ConOut, 1);

	return EFI_SUCCESS;
}

VOID ShowCNFG(VOID)
{
	UINT8 i = 0;
	GPNV_CNFG * cnfg = NULL;
	CHAR16	HexStr[2] = L"1";
	CHAR16  getInput;
//DRK_UBT007-	cnfg = (GPNV_CNFG *)(UINT64) BUFFER_ADDR;
//DRK_UBT008-	cnfg = (GPNV_CNFG *)(UINT32) BUFFER_ADDR; //DRK_UBT007+
//DRK_UBT008+ >>>
#ifdef X64_BUILD
	cnfg = (GPNV_CNFG *)(UINT64) BUFFER_ADDR;
#else
	cnfg = (GPNV_CNFG *)(UINT32) BUFFER_ADDR;
#endif 
//DRK_UBT008+ <<<
	getInput = HexStr[0];
	
	while (getInput != L'Q')
	{
#ifndef DEBUG_INFO
		gST->ConOut->ClearScreen(gST->ConOut);
#endif
		ShowTitle(0x100, Sig2Int('C', 'N', 'F', 'G'));
		
		if (getInput == L'Q')
			break;
		
		Print(L"------------------------------------Page %02c------------------------------------\n\n", getInput);
	
		if (getInput == L'1')
		{
			Print(L"OEM_MiscFlag:    %xh\n", cnfg->info.OEM_MiscFlag);
			Print(L"KBID:            %xh\n", cnfg->info.KBID);
			Print(L"TV_Standard:     %xh\n", cnfg->info.TV_Standard);
			Print(L"System_Flag:     %xh\n", cnfg->info.System_Flag);
			
			Print(L"UUID:            ");
			for (i=0; i<sizeof(cnfg->info.UUID); i++)
				Print(L"%x", cnfg->info.UUID[i]);
			Print(L"h\n");
			
			Print(L"PANELID:         %xh\n", cnfg->info.PANELID);
			Print(L"TIME_IN_MINUTE:  %xh\n", cnfg->info.TIME_IN_MINUTE);
			Print(L"THRESHOLD_TEMP:  %xh\n", cnfg->info.THRESHOLD_TEMP);
			Print(L"CSTATE_CONTROL:  %xh\n", cnfg->info.CSTATE_CONTROL);
			Print(L"QUIET_FAN_DUTY:  %xh\n", cnfg->info.QUIET_FAN_DUTY);
			Print(L"OEMID:           %xh\n", cnfg->info.OEMID);
			Print(L"SLP20_DATE:      %xh\n", cnfg->info.SLP20_DATE);
			Print(L"SLP20_RNUM:      %xh\n", cnfg->info.SLP20_RNUM[0]);

			Print(L"LANMAC:          ");
			for (i=0; i<sizeof(cnfg->info.LANMAC); i++)
				Print(L"%x", cnfg->info.LANMAC[i]);
			Print(L"h\n");

			Print(PRINT_TABLE_PARTITION_4);

			Print(L"\nPlease input page number(1 or 2) to display CNFG info or [Q] to leave!!!\n");
			Print(L"Page Number:");
		}
		else if (getInput == L'2')
		{
			Print(L"LightingDevice:  %xh\n", cnfg->info.LightingDevice[0]);
			Print(L"SLP20_MKEY:      %xh\n", cnfg->info.SLP20_MKEY);
			
			Print(L"Proj_Use:        ");
			for (i=0; i<sizeof(cnfg->info.Proj_Use); i++)
				Print(L"%x", cnfg->info.Proj_Use[i]);
			Print(L"h\n");
			
			Print(L"Computrace:      ");
			for (i=0; i<sizeof(cnfg->info.Computrace); i++)
				Print(L"%x", cnfg->info.Computrace[i]);
			Print(L"h\n");
			
			Print(L"AntiTheft:       %xh\n", cnfg->info.AntiTheft[0]);
			Print(L"SetupMenu:       %xh\n", cnfg->info.SetupMenu[0]);
			Print(L"\n\n\n\n\n\n\n\n");
			Print(PRINT_TABLE_PARTITION_4);
			
			Print(L"\nPlease input page number(1 or 2) to display CNFG info or [Q] to leave!!!\n");
			Print(L"Page Number:");
		}
		
		Input(NULL, HexStr, 2);
		StrUpr(HexStr);
		if (HexStr[0]==L'1' || HexStr[0]==L'2' || HexStr[0]==L'Q')
			getInput = HexStr[0];
	}
}

UINT8 CheckOperation(UINT32 operation, UINT32 code)
{
	UINT32 mask = OPERATION_IO_MASK;
	
	if (code > OPERATION_IO_MASK)
		mask = OPERATION_BLOCK_MASK;
	
	if ((operation&mask) == code)
		return 1;
		
	return 0;
}

UINT8 SearchASUSSignature(SMBIOS_STRUCTURE_TABLE * pSmbiosTable, STRUCTURE_STATISTICS * pStatisticsTable, SMBIOS_STRUCTURE_POINTER * pSmbiosStruct)
{
	STRUCTURE_STATISTICS *pStatistics = NULL;
	UINT8   Buffer[512];
	UINT8	ManuSig[] = MANUFACTURER_SIGNATURE;
	UINT8	ManuSig2[] = MANUFACTURER_SIGNATURE_2;
	UINT16  Length	= 0;
	UINT16	Index	= 0;
	UINT16  queryType= 1;
	UINT16  Handle = 0;
	UINT16  i=0, j=0;
	UINT16 	getSig = 0;
	
	pStatistics = pStatisticsTable;
	
	for (Index=0; Index<pSmbiosTable->NumberOfSmbiosStructures; Index++) 
	{
		if (pStatistics->Type == queryType)
		{
			Handle = pStatistics->Handle;
	
			if (LibGetSmbiosStructure(pSmbiosStruct, pSmbiosTable, &Handle, Buffer, &Length) == DMI_SUCCESS) 
			{
				j = 0;
				for (i=SMBIOS_TYPE_01_STRING_AREA; i<Length; i++)
				{
					if (*(Buffer+i) == ManuSig[j])
					{
						getSig++;
						j++;
					}
					else
					{
						getSig = 0;
						j = 0;
					}
					
					if (getSig == (UINT16)StrLenASCII(MANUFACTURER_SIGNATURE))
						return 1;
				}
				
				j = 0;
				for (i=SMBIOS_TYPE_01_STRING_AREA; i<Length; i++)
				{
					if (*(Buffer+i) == ManuSig2[j])
					{
						getSig++;
						j++;
					}
					else
					{
						getSig = 0;
						j = 0;
					}
					
					if (getSig == (UINT16)StrLenASCII(MANUFACTURER_SIGNATURE_2))
						return 1;
				}
			}
		}
		pStatistics++;
	}
		
	return 0;
}

UINT8 CheckASUSSignature(VOID)
{
	EFI_STATUS					Status;
	SMBIOS_STRUCTURE_TABLE  	*pSmbiosTable 	= NULL;
	STRUCTURE_STATISTICS 		*pStatisticsTable = NULL;
	SMBIOS_STRUCTURE_POINTER 	SmbiosStruct;
	SMBIOS_STRUCTURE_POINTER 	*pSmbiosStruct = &SmbiosStruct;
	UINT8						getSig = 0;
	
	Status = InitSMBIOSInfo(&pSmbiosTable, &pStatisticsTable, pSmbiosStruct);
	if (EFI_ERROR(Status))
	{
		FreePool(pStatisticsTable);
		return getSig;
	}
		
	getSig = SearchASUSSignature(pSmbiosTable, pStatisticsTable, pSmbiosStruct);
	FreePool(pStatisticsTable);
	return getSig;
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
