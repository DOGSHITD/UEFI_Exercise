
#ifndef __PRINT_TEST__
#define __PRINT_TEST__

#include <Protocol\SimpleTextOut.h>
#include <Library/BaseLib.h>

#define LEFT_JUSTIFY      0x01
#define COMMA_TYPE        0x08
#define PREFIX_ZERO       0x20
#define RADIX_HEX         0x80

#define PREFIX_SIGN           BIT1
#define PREFIX_BLANK          BIT2
#define LONG_TYPE             BIT4
#define OUTPUT_UNICODE        BIT6
#define FORMAT_UNICODE        BIT8
#define PAD_TO_WIDTH          BIT9
#define ARGUMENT_UNICODE      BIT10
#define PRECISION             BIT11
#define ARGUMENT_REVERSED     BIT12
#define COUNT_ONLY_NO_PRINT   BIT13

#define CHARACTER_NUMBER_FOR_VALUE  30

UINTN
EfiValueToHexStr (
  IN  OUT CHAR16  *Buffer,
  IN  UINT64      Value,
  IN  UINTN       Flags,
  IN  UINTN       Width
  );
/*++

Routine Description:

  VSPrint worker function that prints a Value as a hex number in Buffer

Arguments:

  Buffer - Location to place ascii hex string of Value.

  Value  - Hex value to convert to a string in Buffer.

  Flags  - Flags to use in printing Hex string, see file header for details.

  Width  - Width of hex value.

Returns: 

  Number of characters printed.  

--*/

STATIC
UINTN
EfiStatusToString (
  IN EFI_STATUS   Status,
  OUT CHAR16      *Buffer,
  IN  UINTN       BufferSize
  );
/*++

Routine Description:

  VSPrint worker function that prints EFI_STATUS as a string. If string is
  not known a hex value will be printed.

Arguments:

  Status     -  EFI_STATUS sturcture to print.

  Buffer     - Buffer to print EFI_STATUS message string into.
  
  BufferSize - Size of Buffer.

Returns: 

  Number of characters printed.  

--*/

STATIC
UINTN
TimeToString (
  IN EFI_TIME   *Time,
  OUT CHAR16    *Buffer,
  IN  UINTN     BufferSize
  );
/*++

Routine Description:

  VSPrint worker function that prints EFI_TIME.

Arguments:

  Time       - Pointer to EFI_TIME sturcture to print.

  Buffer     - Buffer to print Time into.
  
  BufferSize - Size of Buffer.

Returns: 

  Number of characters printed.  

--*/

UINTN
GuidToString (
  IN  EFI_GUID  *Guid,
  IN  CHAR16    *Buffer,
  IN  UINTN     BufferSize
  );

UINTN
SSPrint (
  OUT CHAR16        *Buffer,
  IN  UINTN         BufferSize,
  IN  CONST CHAR16  *Format,
  ...
  );

UINTN XSPrint (
  OUT CHAR16        *StartOfBuffer,\
  IN  UINTN          BufferSize,\
  IN  CONST CHAR16  *FormatString,\
  IN  VA_LIST       Marker\
  );


UINTN
Print (
  IN CONST CHAR16  *Format,
  ...
  );

static
CHAR16 *
GetFlagsAndWidth (
  IN  CHAR16      *Format, 
  OUT UINTN       *Flags, 
  OUT UINTN       *Width,
  IN OUT  VA_LIST *Marker
  );

VOID BackSpace(VOID);  
#endif

