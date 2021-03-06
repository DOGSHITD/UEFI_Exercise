#include <Library/IoLib.h>
#include "Subutility.h"

//#define X64_BUILD  //DRK_UBT007+

#ifdef X64_BUILD   //DRK_UBT007+
//extern int _OutB2(UINT16 Port, UINT8 data);
#endif //DRK_UBT007+

//UINTN memcmp(const UINT8 *src1,const UINT8 *src2,UINTN len)
//{
//	UINTN i;
//
//	for(i=0;i<len;i++){
//		if(src1[i] != src2[i])
//			return (UINTN)(src1[i] - src2[i]);
//	}
//
//	return 0;
//}

UINT8 ReadMem_8 (UINT32 Addr)
{
	return *((volatile UINT8 *)(UINTN)Addr);
}

UINT16  ReadMem_16(UINT32 Addr)
{
	return *((volatile UINT16 *)(UINTN)Addr);
}

void ReadMem_n(UINT32 Addr,UINT8 *buf,UINTN len)
{
	UINTN i;
	for(i=0;i<len;i++) buf[i] = ((volatile UINT8 *)(UINTN)Addr)[i];
}

void WriteMem_8 (UINT32 Addr, UINT8 data)
{
	*((volatile UINT8 *)(UINTN)Addr) = data;
}

void WriteMem_16(UINT32 Addr, UINT16 data)
{
	*((volatile UINT16 *)(UINTN)Addr) = data;
}

void WriteMem_n(UINT32 Addr,UINT8 *buf,UINTN len)
{
	UINTN i;
	for(i=0;i<len;i++) ((volatile UINT8 *)(UINTN)Addr)[i] = buf[i];
}

void WriteMem_32(UINT32 Addr, UINT32 data)
{
	*((volatile UINT32 *)(UINTN)Addr) = data;
}

void zout_8(UINT16 port, UINT8 data)
{
//	IoWrite8(port,data);
//#ifdef X64_BUILD //DRK_UBT007+
//	_OutB2(port,data);
//DRK_UBT007+ >>>
//#else
	IoWrite8(port,data);
//_asm {
//	mov dx, port
//	mov al, data
//	out dx, al
//}
//#endif
//DRK_UBT007+ <<<
}
