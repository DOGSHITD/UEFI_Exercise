#ifndef _EFI_SHELL_UBT_
#define _EFI_SHELL_UBT_

#define EFI_UBT_GUID \
  { \
   0xf2a304e6, 0x302a, 0x4f44, 0x8f, 0x5, 0xf8, 0xff, 0xf3, 0x49, 0xd7, 0xc3\
  }

//DRK_UBT001 >>>
#define EDAB_Number 1  
#define Non_Verify_Number 3
//DRK_UBT001 <<<

//DRK_UBT002 >>>
//#define INCOMPATIBLE_FLASH_TOOL 	0x0002
#define NO_AC_POWER 			0x0004
#define NO_BATTERY 			0x0008
//DRK_UBT002 <<<

//#define DEBUG_INFO
#define Sig2Int(a,b,c,d) 		((((UINTN)a)<<24) + (((UINTN)b)<<16) + (((UINTN)c)<<8) + ((d)))
#define EFI_SEGMENT(_Adr)     	(UINT16) ((UINT16) (((UINTN) (_Adr)) >> 4) & 0xf000)
#define EFI_OFFSET(_Adr)      	(UINT16) (((UINT16) ((UINTN) (_Adr))) & 0xffff)
#define OFFSET(S,F) 			((UINTN)&(((S *)0)->F))
#define BIOS_IMAGE_ADDR 		0x200000
#define BUFFER_ADDR 			(BIOS_IMAGE_ADDR - 0x1000) 

#define OPERATION_IO_MASK 		0x0000FFFF
#define OPERATION_BLOCK_MASK	0xFFFF0000

#define UNKNOWN_OPERATION		0x00000000
#define READ_OPERATION			0x00000001
#define WRITE_OPERATION			0x00000002
#define WRITE_TXT_OPERATION		0x00000003
#define WRITE_BIN_OPERATION		0x00000004
#define DRMAC_OPERATION			0x00000005
#define RMAC_OPERATION			0x00000006
#define CNFG_INFO_OPERATION		0x00000007
#define READ_TXT_OPERATION		0x00000008
#define READ_BIN_OPERATION		0x00000009


#define CNFG_OPERATION			0x00010000
#define MFG0_OPERATION			0x00020000
#define OEM1_OPERATION			0x00030000
#define OEM2_OPERATION			0x00040000
#define OEM3_OPERATION			0x00050000
#define OEM5_OPERATION			0x00060000
#define SLP2_OPERATION			0x00070000
#define ASFG_OPERATION			0x00080000
#define VKEY_OPERATION			0x00090000
#define _DMI_OPERATION			0x000a0000

#define CMD_READ 				0x00000001
#define CMD_WRITE 				0x00000002
#define CMD_BLK					0x00000004
#define CMD_WFT					0x00000008
#define CMD_WFB					0x00000010
#define CMD_DRMAC				0x00000020
#define CMD_RMAC				0x00000040
#define CMD_CNFG_INFO			0x00000080
#define CMD_RFT					0x00000100
#define CMD_RFB					0x00000200

#define DEFAULT_MAC_ADDRESS_OFFSET 0xd7
//#define PRINT_TABLE_0(a,b) 		L"                            Build DATA: 2012-08-16            \n                  ASUS BIOS Tool, Copyright by ASUS Rev2.00   Create by Sky Lin\nBlock Size=%xh                     %s                      Maintain by Dark Chang                                       \n\n", a, b
//DRK_UBT008- #define PRINT_TABLE_0(a,b) 		L"                            Build DATA: 2013-07-24            \n                  ASUS BIOS Tool, Copyright by ASUS Rev2.11   Create by Sky Lin\nBlock Size=%xh                     %s                      Maintain by Dark Chang                                       \n\n", a, b
#define PRINT_TABLE_0(a,b) 		L"                            Build DATA: 2013-09-05            \n                  ASUS BIOS Tool, Copyright by ASUS Rev2.20   \nBlock Size=%xh                     %s                                                             \n\n", a, b //DRK_UBT008+
#define PRINT_TABLE_1 			L"        ASCII code\n---------------------------------------------------------     ----------------\n"
#define PRINT_TABLE_PARTITION_0 L"         "	//Between Title and col number
#define PRINT_TABLE_PARTITION_1 L"     "		//Between Data and ASCII CODE
#define PRINT_TABLE_PARTITION_3 L"-------------------------------------------------------------------------------\n\n\n"
#define PRINT_TABLE_PARTITION_4 L"\n-------------------------------------------------------------------------------\n"
#define MULTI_WRITE_BUFFER_SIZE 32

//#define FVBB_OFFSET 131072
#define FVBB_OFFSET 65536
#define SEARCH_BIOSINFO_ADDRESS 0xFFFFFFFF - FVBB_OFFSET

typedef struct _FLASH_SMI_Table
{
	UINT32 ACPIRAMW;
	UINT16 SMIPort;
	UINT8 FlashSwSmiCode;
	UINT8 FlashSwSmiSubCode;
	UINT8 TRTYOffset;
	UINT8 FADROffset;
	UINT8 FSIZOffset;
	UINT8 FSTAOffset;
	UINT8 FSFNOffset;
}FLASH_SMI_Table;

typedef struct _EXTFStruct
{
	UINT8 EXTFSignature[4];		//"EXTF" Signature				4 Byte(0,1,2,3)
	UINT8 ExTFFuncEntry[3];		//Legacy EXTF(16-bit) Entry		3 Byte(4,5,6)
	UINT8 EXTFRev;				//EXTF Revision					1 Byte(7)
	UINT16 FlashSmiTable;		//Offset of Flash SMI Table		2 Byte(8,9)
	UINT32 EXTF32FuncEntry;		//Legacy EXTF(32-bit) Entry		4 Byte(10,11,12,13)
	UINT32 ModelNameStrPtr;		//Pointer to Model Name String (ASCIIZ) 4 Byte(14,15,16,17)
	UINT16 EXTF_Size;			//								2 Byte (18,19)
	FLASH_SMI_Table AsusFlashSmiTbl;//							13 Byte (20-32)
	UINT8 ModelName[16];		//								16 Byte (31-46)
}EXTFStruct;

typedef struct _s_temp{
	UINT8 Day;
	UINT8 Month;
	UINT16 Year;
}s_temp;

typedef struct _FlashToolSignatureStruct
{
	UINT8 Signature[4];				//4
	UINT16 StructLength;			//2
	UINT8 StructMinor;				//1
	UINT8 StructMajor;				//1
	UINT32 IDNumber;				//
	UINT8 MinorRevision;
	UINT8 MajorRevision;
	s_temp s;
	UINT32 RequestFlag;
	UINT8 Reserved[10];	//Fill with 0xFF
}FlashToolSignatureStruct;
//DRK_UBT001 >>> 
//typedef struct _CNFG_INFO{
//	UINT8 Reserved0[176];		//offset 0~175 
//	UINT32 OEM_MiscFlag;		//offset 176~179 	(0xB0)
//	UINT8 KBID;					//offset 180	  	(0xB4)
//	UINT8 TV_Standard;			//offset 181		(0xB5)
//	UINT16 System_Flag;			//offset 182~183	(0xB6)
//	UINT8 UUID[16];				//offset 184~193	(0xB8)
//	UINT32 PANELID;				//offset 200~203	(0xC8)	
//	UINT8 TIME_IN_MINUTE;		//offset 204 		(0xCC)
//	UINT8 THRESHOLD_TEMP;		//offset 205 		(0xCD)
//	UINT16 CSTATE_CONTROL;		//offset	206~207 C2/C3/C4/C5 inhibit nibble			(0xCE)
//								//nibble value 8 mean inhibit, others mean not control
//	UINT16 QUIET_FAN_DUTY;		//offset 208~209										(0xD0)	
//	UINT8 OEMID;				//offset 210											(0xD2) 				
//	UINT8 Reserved1;			//offset 211								
//	UINT8 SLP20_DATE;			//offset 212											(0xD4)
//	UINT8 Reserved2;			//offset 213	
//	UINT8 SLP20_RNUM[1];		//offset 214											(0xD6)
//	UINT8 LANMAC[6];			//offset 215~220;										(0xD7)
//
//	UINT8 LightingDevice[1];	//offset 221											(0xDD)
//	UINT8 SLP20_MKEY;			//offset 222											(0xDE)	
//	UINT8 Reserved3[1];			//offset 223												
//
//	UINT8 Proj_Use[16];			//offset 224~239										(0xE0)
//	UINT8 Computrace[4];		//offset 240~243										(0xF0)
//	UINT8 AntiTheft[1];			//offset 244											(0xF4)
//	UINT8 SetupMenu[1];			//offset 245											(0xF5)		
//	UINT8 Reserved4[10];		//offset 246~255;	
//}CNFG_INFO;
typedef struct _CNFG_INFO  //DRK:refer to GPNV.h
{
	UINT8 SKU_Number[16];	//offset 0~15
	UINT8 Reserved0[160];	//offset 0~175	 
	UINT32 OEM_MiscFlag;	//offset 176~179 
	UINT8 KBID;		//offset 180
	UINT8 TV_Standard;	//offset 181
	UINT16 System_Flag;	//offset 182~183
	UINT8 UUID[16]; 	//offset 184~193
	UINT32 PANELID; 	//offset 200~203	
	UINT8 TIME_IN_MINUTE;	//offset 204 (0xCC)
	UINT8 THRESHOLD_TEMP;	//offset 205 (0xCD)
	UINT16 CSTATE_CONTROL;	//offset	206~207 C2/C3/C4/C5 inhibit nibble
	UINT16 QUIET_FAN_DUTY;	//offset 208~209	
	UINT8 OEMID;		//offset210 
	UINT8 SLIC_FLAG;	//offset 211		//AK-LCS0646
	UINT8 SLP20_DATE;	//offset 212	
	UINT8 SECUREBOOT_CSM;	//offset 213
	UINT8 SLP20_RNUM[1];	//offset 214
	UINT8 LANMAC[6];	//offset 215~220;
	UINT8 LightingDevice[1];//offset 221;
	UINT8 SLP20_MKEY;	//offset 222	
	UINT8 Reserved3[1];	//offset 223	
	UINT8 Proj_Use[16];	//offset 224~239
	UINT8 Computrace[4];	//offset 240~243
	UINT8 AntiTheft[1];	//offset 244
	UINT8 SetupMenu[1];	//offset 245		
	UINT8 Reserved4[10];	//offset 246~255;	
}CNFG_INFO;
//DRK_UBT001 <<<

typedef union _GPNV_CNFG
{
	UINT8 data[0x100];
	CNFG_INFO info;
}GPNV_CNFG;

typedef struct _BiosSignatureStruct
{
	UINT8 Signature[4];	//_BS_
	UINT8 MinorRevision;
	UINT8 MajorRevision;
	UINT16 TotalLength;
	UINT16 BiosImageOffset;
	UINT32 BiosImageSize;
	UINT8 Reserved[2];
	UINT16 BiosCodeNameOffset;
	UINT8 ProjectCount;
	UINT16 ProjectListOffset;
	UINT8 DeviceCount;
	UINT16 DeviceListOffset;
	UINT8 BiosBuildDay;
	UINT8 BiosBuildMonth;
	UINT16 BiosBuildYear;
	UINT16 RevisionStrOffset;
	UINT16 PatchCodeOffset;
	UINT32 EncryptionKey;
	UINT32 BBStartOffset;
	UINT32 BBSize;
	UINT32 BBChecksumOffset;
	UINT32 BBChecksum;
	UINT32 ModuleListHeadOffset;
	UINT32 ModuleChecksum;
	UINT8 ModuleNoChecksumList[8];
}BiosSignatureStruct;
//DRK_UBT001 >>>
//typedef struct _BIOSInfoStruct
//{
//	UINT8 BIOSInfoSignature[8];
//	UINT8 BIOSInfoRev;
//	UINT8 Reserved;
//	UINT16 BIOSRevOffset;
//	UINT16 BIOSDateOffset;
//	UINT16 BIOSCoreRevOffset;
//	UINT8 BIOSRev[10];
//	UINT8 BIOSDate[40];
//	UINT8 BIOSCoreRev[10];
//}BIOSInfoStruct; 


typedef struct _EDABStruct
{
	UINT32 EDAB_Addr;
	UINT32 EDAB_Len;
}EDABStruct;
	
	
typedef struct _BIOSInfoStruct	//DRK:refer to BiosInfo.h
{
	UINT8 BIOSInfoSignagure[8];
	UINT8 BIOSInfoRev;
	UINT8 Reserved;
	UINT16 BIOSRevOffset;
	UINT16 BIOSDateOffset;
	UINT16 BIOSCoreRevOffset;
	UINT16 BIOSCoreIdOffset;	
	UINT16 BIOSInfo_Size;	
	UINT8 Reserved1[12];
	UINT32 BB_BaseBootBlock;	//Boot Block Base
	UINT32 BB_Size;
	UINT32 NumEDAB; 		//The current num of EDAB is 3
	EDABStruct EDAB[EDAB_Number];		//
	UINT32 NumberNonVerify; 			    //AK-SWL1418a   
//#if Non_Verify_Number != 0				  //AK-SWL1418a 
	EDABStruct NonVereifyBlock[Non_Verify_Number];	    //AK-SWL1418a 
//#endif						  //AK-SWL1418a 
	UINT8 BIOSRev[16];
	UINT8 BIOSDate[64];
	UINT8 BIOSCoreRev[16];
	UINT8 BIOSCoreId[16];		//Project name	AK-TML1003 
}BIOSInfoStruct;
	
	
//DRK_UBT001 <<<


UINT32 		LocateEXTF(UINT32 start, UINT32 len);
VOID 		GetEXTFInformation(UINT32 linearAddr);
VOID 		GetFlashSMIInformation(UINT32 linearAddr);
VOID		Flash_ASL_ROM_Control(UINT16 FSTA);
UINT32 		LocateBIOSInfo(UINT32 Start, UINT32 len, UINT8 TTB);
//VOID 		GetBIOSInfoInformation(UINT32 LinearAddr,UINTN BIOSorImage);  //DRK_UBT004-
VOID 		GetBIOSInfoInformation(UINT32 LinearAddr);  //DRK_UBT004+
UINT16 		GPNV_Initial(UINT32 FADR);
BOOLEAN 	GetCurrentBIOSInformation(VOID);
UINT16 		GetGPNV(UINT32 FADR);

EFI_STATUS  SetFileToGPNV(EFI_FILE_PROTOCOL *File, UINT32 FADR, UINT32 addr, UINT16 length, UINT8 FE);
EFI_STATUS 	SetGPNVToFile(EFI_FILE_PROTOCOL *File, UINT32 addr, UINT16 length, UINT8 FE);
UINT16 		RWGPNVFile(EFI_HANDLE ImageHandle, UINT32 FADR, UINT32 addr, UINT16 length, CHAR16 * fileName, UINT8 FE, UINT8 RWOP);

EFI_STATUS 	BatchWriteFile(EFI_FILE_PROTOCOL *File, UINT8 * wBuf, UINT8 *wIndex, UINTN *wSize, UINT8 wData);
VOID 		GetMAC(UINT32 sig, UINT32 offset);
UINT16 		SetGPNV(UINT32 FADR, UINT32 addr, UINT8* data, UINTN size);
UINT16 		MultiSetGPNV(UINT32 sig, UINT32 addr, CHAR16 *data);

VOID 		ShowHints(VOID);
VOID 		ShowTitle(UINT16 size, UINT32 sig);
VOID 		ShowGPNV(UINT16 size, UINT32 sig);
VOID 		ShowCNFG(VOID);
VOID 		ShowWriteProcess(UINT32 offset, UINT8 *data, UINT8 size);

UINT32 		Int2Sig(UINT32 sig, CHAR16* buf, UINT32 size);
UINT32 		ParseCommand(VOID);
UINT8 		IsNumber(CHAR16 * ptr);
UINT8 		ToASCII(UINT8 buf, UINT8 highbyte);
UINT8 		ToRawNumber(UINT8 highByte, UINT8 lowByte);
UINT8 		CheckOperation(UINT32 operation, UINT32 code);
UINT8 		CheckASUSSignature(VOID);
#endif
