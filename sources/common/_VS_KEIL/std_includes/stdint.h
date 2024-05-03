#pragma once

/* These types MUST be 16-bit or 32-bit */
typedef int				INT;
typedef unsigned int	UINT;

/* This type MUST be 8-bit */
typedef unsigned char	BYTE;

/* These types MUST be 16-bit */
typedef short			SHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

/* These types MUST be 32-bit */
typedef long			LONG;
typedef unsigned long	DWORD;

/* This type MUST be 64-bit (Remove this for ANSI C (C89) compatibility) */
typedef unsigned long long QWORD;

typedef signed   char  int8_t;
typedef unsigned char  uint8_t;
typedef unsigned int   int16_t;
typedef unsigned short uint16_t;
typedef signed   int   int32_t;
typedef unsigned int   uint32_t;
typedef unsigned long  uint64_t;

typedef unsigned int *uintptr_t;

#ifndef __IO
#define __IO volatile
#define __I volatile
#endif

#ifndef NULL
#define NULL 0
#endif
