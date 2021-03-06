/***************************************

	Intrinsics and subroutines exclusive to the Freescale and Metrowerks compilers

	Copyright 1995-2014 by Rebecca Ann Heineman becky@burgerbecky.com

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#ifndef __BRMETROWERKS_H__
#define __BRMETROWERKS_H__

#ifndef __BRTYPES_H__
#include "brtypes.h"
#endif

/* BEGIN */
#if defined(BURGER_METROWERKS) && !defined(DOXYGEN)
extern "C" {
#if defined(BURGER_X86)
inline long _InterlockedExchange(register long volatile*pOutput,register long lValue) { asm { lock xchg lValue,[pOutput] } return lValue; }
inline long _InterlockedIncrement(register long volatile*pOutput) { register long lTemp=1; asm { lock xadd [pOutput],lTemp } return lTemp+1; }
inline long _InterlockedDecrement(register long volatile*pOutput) { register long lTemp=-1; asm { lock xadd [pOutput],lTemp } return lTemp-1; }
inline long _InterlockedExchangeAdd(register long volatile*pOutput,register long lValue) { asm { lock xadd [pOutput],lValue } return lValue; }
inline long _InterlockedCompareExchange(register long volatile*pOutput,register long lAfter,register long lBefore) { asm { mov eax,lBefore
	lock xadd [pOutput],lAfter
	mov lBefore,eax} return lBefore; }
namespace std {
#if __has_intrinsic(__builtin_fabsf)
extern float __builtin_fabsf(float) __attribute__((nothrow)) __attribute__((const));
#endif
#if __has_intrinsic(__builtin_fabs)
extern double __builtin_fabs(double) __attribute__((nothrow)) __attribute__((const));
#endif
#if __has_intrinsic(__builtin_sqrt) 
extern double __builtin_sqrt(double) __attribute__((nothrow)) __attribute__((const));
#endif
#if __has_intrinsic(__builtin___count_leading_zero32)
extern unsigned int __builtin___count_leading_zero32(unsigned int) __attribute__((nothrow)) __attribute__((const));
#endif
#if __has_intrinsic(__builtin___count_trailing_zero32)
extern unsigned int __builtin___count_trailing_zero32(unsigned int) __attribute__((nothrow)) __attribute__((const));
#endif
#if __has_intrinsic(__builtin___count_leading_zero64)
extern unsigned int __builtin___count_leading_zero64(unsigned long long) __attribute__((nothrow)) __attribute__((const));
#endif
#if __has_intrinsic(__builtin___count_bits64)
extern unsigned int __builtin___count_bits64(unsigned long long) __attribute__((nothrow)) __attribute__((const));
#endif
}

#elif defined(BURGER_68K)

Word16 BURGER_INLINE _bswap16(Word16:__D0):__D0 = { 0xE058 };
Word32 BURGER_INLINE _bswap(Word32:__D0):__D0 = { 0xE058, 0x4840, 0xE058 };
#pragma parameter __D1 BurgerIntMathMul32GetUpper32(__D0,__D1)
Int32 BurgerIntMathMul32GetUpper32(Int32 iInputMulA,Int32 iInputMulB) = {0x4c01,0xc01};	// muls.l d1,d1:d0
#pragma parameter __D0 BurgerIntMathMul32x32To64Div32(__D0,__D1,__D2)
Int32 BurgerIntMathMul32x32To64Div32(Int32 iInputMulA,Int32 iInputMulB,Int32 iInputDiv) = {0x4c01,0xc01,0x4c42,0xc01};	// muls.l d1,d1:d0 divs.l ds,d1:d0

#elif defined(BURGER_POWERPC)

extern double sqrt(double);
float BURGER_INLINE sqrtf(float fInput) { return static_cast<float>(sqrt(fInput)); }
#if __has_intrinsic(__builtin___rotate_left32)
extern unsigned int __builtin___rotate_left32(unsigned int,int) __attribute__((nothrow)) __attribute__((const));
#endif
#if __has_intrinsic(__builtin___rotate_right32)
extern unsigned int __builtin___rotate_right32(unsigned int,int) __attribute__((nothrow)) __attribute__((const));
#endif

#endif
}
#endif
/* END */

#endif
