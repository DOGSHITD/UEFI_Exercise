#ifndef SUB_UTILITY_H
#define SUB_UTILITY_H
#define X64_BUILD  //DRK_UBT008+
//UINTN memcmp(const UINT8 *src1, const UINT8 *src2, UINTN len);
UINT8 ReadMem_8(UINT32 Addr);
UINT16 ReadMem_16(UINT32 Addr);
void ReadMem_n(UINT32 Addr, UINT8 *buf,UINTN len);
void WriteMem_8(UINT32 Addr, UINT8 data);
void WriteMem_16(UINT32 Addr, UINT16 data);
void WriteMem_n(UINT32 Addr, UINT8 *buf,UINTN len);
void WriteMem_32(UINT32 Addr, UINT32 data);
void zout_8(UINT16 port, UINT8 data);

#endif
