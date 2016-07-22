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

#include <token.h>
#include <AmiPeiLib.h>
#include <AmiHobs.h>
#include <Setup.h>
#include <PPI\STALL.h>
#include <Ppi\ReadOnlyVariable.h>
#include <Token.h>
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

// {2b47ac16-7d6b-449c-aa95-85e3fc306194} 
#define EFI_HOMEWORK_HOB_GUID \
  { \
    0x2b47ac16, 0x7d6b, 0x449c, {0xaa, 0x95, 0x85, 0xe3, 0xfc, 0x30, 0x61, 0x94} \
  }


EFI_STATUS EFIAPI HomeworkPEI(
		IN EFI_PEI_FILE_HANDLE       FileHandle,
		IN CONST EFI_PEI_SERVICES    **PeiServices)
{
	        EFI_GUID	gClarePpiGuid = EFI_PEI_READ_ONLY_VARIABLE_PPI_GUID;
		EFI_GUID	gSetupGuid = SETUP_GUID;
		EFI_GUID gClareStallGuid=EFI_PEI_STALL_PPI_GUID;
		EFI_PEI_STALL_PPI	*StallPpi;
		EFI_PEI_READ_ONLY_VARIABLE_PPI *HomeworkPpi;
		SETUP_DATA	SetupData;
		UINTN	nVariableSize = sizeof(SETUP_DATA);
		
		typedef struct{
			EFI_HOB_GUID_TYPE HobGuidType;
			UINT16            OptionData;
		}EFI_HOMEWORK_HOB;
		
		EFI_HOMEWORK_HOB *pHomeworkHob=NULL;
		EFI_GUID         gHomeworkHobGuid=EFI_HOMEWORK_HOB_GUID;
		
		EFI_STATUS Status =  (**PeiServices).LocatePpi (
			    		PeiServices,
			    		&gClarePpiGuid, 
			    		0,
					NULL, 
					&HomeworkPpi              // PPI
	        );
		if(EFI_ERROR(Status))
		{
			return Status;
		}
		Status = HomeworkPpi->PeiGetVariable(
			PeiServices,
			L"Setup",
			&gSetupGuid,
			NULL,
			&nVariableSize,
			(VOID *)&SetupData);
		if(EFI_ERROR(Status))
		{
			return Status;
		}

		Status = (**PeiServices).LocatePpi (
		PeiServices,
		&gClareStallGuid,
		0,
		NULL,
		&StallPpi
		);
		if(EFI_ERROR(Status))
		{
			return Status;
		}
		
                
		//Define HOB;		
		Status =(**PeiServices).CreateHob(PeiServices,\
				EFI_HOB_TYPE_GUID_EXTENSION,\
				sizeof(EFI_HOMEWORK_HOB),\
				&pHomeworkHob);
		if(EFI_ERROR(Status)){
			return Status;
		}
		pHomeworkHob->HobGuidType.Name=gHomeworkHobGuid;
		pHomeworkHob->OptionData=SetupData.HomeworkOption;
#ifdef Module_Trace_Enable //Token.h for the Token definition in sdl.
#if Module_Trace_Enable
		PEI_TRACE((-1,PeiServices,"ASSERT in %s on %i: BOYCEHONG\n",__FILE__, __LINE__));
#endif
#endif
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
