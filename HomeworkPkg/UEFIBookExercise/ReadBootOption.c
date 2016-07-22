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

EFI_LOAD_OPTION* ReadBootoptions(IN UINT16 BootIndex)
{
    EFI_STATUS Status;
    UINTN OptionSize;
    CHAR16 BootVarName[15];
    EFI_LOAD_OPTION *BootOption;
    
    Swprintf( BootVarName, L"Boot%04X", BootIndex);         

    OptionSize = 0;
    Status = pRS->GetVariable( BootVarName, &gEfiGlobalVariableGuid, NULL, &OptionSize, NULL);
    if (Status != EFI_NOT_FOUND)
    {       
        BootOption = Malloc( OptionSize );
        Status = pRS->GetVariable( BootVarName, &gEfiGlobalVariableGuid, NULL, &OptionSize, BootOption);
        if (EFI_ERROR(Status)) {
            return NULL;
        }
        else{
        	Print(L"Boot%04x\n\r",BootIndex);
        	Print(L"Length:%d\n\r",OptionSize);
        	return BootOption;
        }        
    }
    else
        return NULL;

}

EFI_STATUS EFIAPI ParseBootoptions (
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	UINT16     *BootOrder = NULL;
	UINT8      flag=1;
	UINTN      m=0;
	UINTN      n=0;
	CHAR16     string;
	UINTN      BootOrderSize = 0;
	EFI_STATUS Status;
	UINT16     i;	
	EFI_LOAD_OPTION  *BootOption = NULL;
	
	InitAmiLib(ImageHandle,SystemTable);
	
	Status=GetEfiVariable(L"BootOrder", &gEfiGlobalVariableGuid, NULL, &BootOrderSize,(VOID**)&BootOrder);
	if (EFI_ERROR(Status)) 
		return Status;
	for(i=0; i<BootOrderSize/sizeof(UINT16); i++){
		BootOption=ReadBootoptions(BootOrder[i]);
		if(BootOption!=NULL){
			Print(L"Description:");
			while(flag){
				string = *(CHAR16*)((char*)BootOption+6+m); 
				Print(L"%c",string);
				m+=2;
				if(string==0x0000){
					Print(L"\n\r");
					flag=0;
				}									
			}
			Print(L"FilePathListLength:%d\n\r",BootOption->FilePathListLength);
			Print(L"FilePathList:");
			for(n=0;n<(BootOption->FilePathListLength);n+=2){
				if(n>2){                                         
					string = *(CHAR16*)((char*)BootOption+6+m+n);
					if((string>=0x0020)&(string<=0x007e)){
						flag=1;
						Print(L"%c",string);
					}					
				}								
			}
			if(!flag)
				Print(L"No path context can be printed!");
			Print(L"\n\r");
			Print(L"===============================\n\r");
			m=0;
			flag=1;
		}
			
	}    
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
