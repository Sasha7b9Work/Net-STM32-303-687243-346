// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "opt.h"


#define VERSION 53


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic ignored "-Wundefined-func-template"
    #pragma clang diagnostic ignored "-Wmissing-field-initializers"
    #pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
    #pragma clang diagnostic ignored "-Wold-style-cast"
    #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
    #pragma clang diagnostic ignored "-Wpadded"
    #pragma clang diagnostic ignored "-Wglobal-constructors"
    #pragma clang diagnostic ignored "-Wcast-qual"
    #pragma clang diagnostic ignored "-Wweak-vtables"
    #pragma clang diagnostic ignored "-Winvalid-source-encoding"
    #pragma clang diagnostic ignored "-Wfloat-equal"
    #pragma clang diagnostic ignored "-Wexit-time-destructors"
    #pragma clang diagnostic ignored "-Wformat-nonliteral"
    #pragma clang diagnostic ignored "-Wdeclaration-after-statement"
    #pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif


typedef unsigned char  uint8;
typedef signed char    int8;
typedef unsigned short uint16;
typedef signed short   int16;
typedef unsigned int   uint;
typedef const char    *pchar;
typedef unsigned char  uchar;

#define USE_USB_INTERRUPT_DEFAULT   1

// Промежуток между измерениями
#define TIME_MEASURE  500


#ifdef WIN32
    #define __attribute__(x)
    #define __attribute(x)
    #ifdef GUI
        #define IN_MODE_TEST
    #else
        #define asm(x)
    #endif
#endif


union BitSet32
{
    BitSet32(uint _word = 0) : word(_word) { }
    uint   word;
    uint16 half_word[2];
    uint8  bytes[4];
};


inline void EmptyVV(void) { }
typedef void(*pFuncVV)(void);


#define _GET_BIT(value, bit)   (((value) >> (bit)) & 0x01)
#define _SET_BIT(value, bit)   ((value) |= (1 << (bit)))
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << (bit))))


#define ERROR_VALUE_FLOAT   1.111e29f


#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))

#define BINARY_U8( bits ) _bitset(0##bits)


#include "Utils/Log.h"
#include "Utils/Debug.h"
