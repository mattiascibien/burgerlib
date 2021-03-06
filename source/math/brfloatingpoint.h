/***************************************

	Fixed point math functions

	Copyright 1995-2014 by Rebecca Ann Heineman becky@burgerbecky.com

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!
	
***************************************/

#ifndef __BRFLOATINGPOINT_H__
#define __BRFLOATINGPOINT_H__

#ifndef __BRTYPES_H__
#include "brtypes.h"
#endif

#ifndef __BRWATCOM_H__
#include "brwatcom.h"
#endif

#ifndef __BRVISUALSTUDIO_H__
#include "brvisualstudio.h"
#endif

#ifndef __BRMETROWERKS_H__
#include "brmetrowerks.h"
#endif

/* BEGIN */
namespace Burger {
#define	BURGER_PI 3.1415926535897932384626	///< Pretty accurate, eh?
typedef Word8 Float80Bit[10];				///< 80 bit float (Extended)

struct Word32ToFloat {
	union {
		Word32 w;		///< Value in binary
		float f;		///< Value as a 32 bit float
	};
	BURGER_INLINE operator float() const { return f; }
};

struct Word64ToDouble {
	union {
		Word64 w;		///< Value in binary
		double d;		///< Value as a 64 bit float
	};
	BURGER_INLINE operator double() const { return d; }
};

struct Vector_128Float32 {
	union {
		float BURGER_ALIGN(f[4],16);		///< Value as 4 32 bit floats
		Vector_128 v;	///< Value as a 128 bit vector
	};
	BURGER_INLINE operator Vector_128() const { return v; }
};

struct Vector_128Float64 {
	union {
		double BURGER_ALIGN(d[2],16);		///< Value as 2 64 bit floats
		Vector_128 v;	///< Value as a 128 bit vector
	};
	BURGER_INLINE operator Vector_128() const { return v; }
};

struct Vector_128Int32 {
	union {
		Int32 BURGER_ALIGN(i[4],16);		///< Value as 4 signed 32 bit integers
		Vector_128 v;	///< Value as a 128 bit vector
	};
	BURGER_INLINE operator Vector_128() const { return v; }
};

struct Vector_128Word64 {
	union {
		Word64 BURGER_ALIGN(u[2],16);		///< Value as 2 unsigned 64 bit integers
		Vector_128 v;	///< Value as a 128 bit vector
	};
	BURGER_INLINE operator Vector_128() const { return v; }
};

struct Vector_128Word32 {
	union {
		Word32 BURGER_ALIGN(u[4],16);		///< Value as 4 unsigned 32 bit integers
		Vector_128 v;	///< Value as a 128 bit vector
	};
	BURGER_INLINE operator Vector_128() const { return v; }
};

struct Vector_128Word8 {
	union {
		Word8 BURGER_ALIGN(u[16],16);		///< Value as 16 unsigned 8 bit integers
		Vector_128 v;	///< Value as a 128 bit vector
	};
	BURGER_INLINE operator Vector_128() const { return v; }
};

struct Vector_128Word16 {
	union {
		Word16 BURGER_ALIGN(u[8],16);		///< Value as 8 unsigned 16 bit integers
		Vector_128 v;	///< Value as a 128 bit vector
	};
	BURGER_INLINE operator Vector_128() const { return v; }
};
}
#if defined(BURGER_PS4)
extern "C" float fabsf(float);
extern "C" double fabs(double);
extern "C" float sqrtf(float);
extern "C" double sqrt(double);
#elif defined(BURGER_MACOSX) || (defined(BURGER_IOS) && (defined(BURGER_X86) || defined(BURGER_AMD64)))
extern "C" float sqrtf(float);
extern "C" double sqrt(double);
#if !defined(BURGER_METROWERKS)
extern "C" float fabsf(float);
extern "C" double fabs(double);
#endif
#endif
namespace Burger {
	extern const Word64ToDouble g_dOne;			///< Constant for 1.0 in the double format.
	extern const Word64ToDouble g_dHalf;		///< Constant for 0.5 in the double format.
	extern const Word64ToDouble g_dInf;			///< Constant for Infinity in the double format.
	extern const Word64ToDouble g_dNan;			///< Constant for signaling Not a Number (NaN) in the double format.
	extern const Word64ToDouble g_dQNan;		///< Constant for non signaling Not a Number (Nan) in the double format.
	extern const Word64ToDouble g_dMin;			///< Constant for the smallest number (Non-zero) in the double format.
	extern const Word64ToDouble g_dMax;			///< Constant for the largest number in the double format.
	extern const Word64ToDouble g_dEpsilon;		///< Constant for Epsilon in the double format.
	extern const Word64ToDouble g_dQuarterPi;	///< Constant for Pi*0.25 in the double format.
	extern const Word64ToDouble g_dHalfPi;		///< Constant for Pi*0.5 in the double format.
	extern const Word64ToDouble g_dPi;			///< Constant for Pi in the double format.
	extern const Word64ToDouble g_dPi2;			///< Constant for Pi*2 in the double format.
	extern const Word64ToDouble g_dPi4;			///< Constant for Pi*4 in the double format.
	extern const Word64ToDouble g_dReciprocalHalfPi;	///< Constant for 1/(Pi*0.5) in the double format.
	extern const Word64ToDouble g_dReciprocalPi;		///< Constant for 1/Pi in the double format.
	extern const Word64ToDouble g_dReciprocalPi2;		///< Constant for 1/(Pi*2) in the double format.
	extern const Word64ToDouble g_dRadiansToDegrees;	///< Constant for 180/Pi in the double format.
	extern const Word64ToDouble g_dDegreesToRadians;	///< Constant for Pi/180 in the double format.
	extern const Word32ToFloat g_fOne;			///< Constant for 1.0f in the float format.
	extern const Word32ToFloat g_fHalf;			///< Constant for 0.5f in the float format.
	extern const Word32ToFloat g_fInf;			///< Constant for Infinity in the float format.
	extern const Word32ToFloat g_fNan;			///< Constant for signaling Not a Number (NaN) in the float format.
	extern const Word32ToFloat g_fQNan;			///< Constant for non signaling Not a Number (Nan) in the float format.
	extern const Word32ToFloat g_fMin;			///< Constant for smallest number (Non-zero) in the float format.
	extern const Word32ToFloat g_fMax;			///< Constant for largest number in the float format.
	extern const Word32ToFloat g_fEpsilon;		///< Constant for Epsilon in the float format.
	extern const Word32ToFloat g_fQuarterPi;	///< Constant for Pi*0.25f in the float format.
	extern const Word32ToFloat g_fHalfPi;		///< Constant for Pi*0.5f in the float format.
	extern const Word32ToFloat g_fPi;			///< Constant for Pi in the float format.
	extern const Word32ToFloat g_fPi2;			///< Constant for Pi*2 in the float format.
	extern const Word32ToFloat g_fPi4;			///< Constant for Pi*4 in the float format.
	extern const Word32ToFloat g_fReciprocalHalfPi;	///< Constant for 1/(Pi*0.5f) in the float format.
	extern const Word32ToFloat g_fReciprocalPi;		///< Constant for 1/Pi in the float format.
	extern const Word32ToFloat g_fReciprocalPi2;	///< Constant for 1/(Pi*2.0f) in the float format.
	extern const Word32ToFloat g_fRadiansToDegrees;	///< Constant for 180/Pi in the float format.
	extern const Word32ToFloat g_fDegreesToRadians;	///< Constant for Pi/180 in the float format.

#if defined(BURGER_X86) || defined(DOXYGEN)
	enum e8087Precision {
		PRECISION24=0,	///< Use 24 bit / float precision  (DirectX default)
		PRECISION56=2,	///< Use 56 bit / double precision
		PRECISION64=3	///< Use 64 bit / long double precision (Power up default)
	};
	extern e8087Precision BURGER_API Set8087Precision(e8087Precision eInput);
	enum e8087Rounding {
		ROUNDTONEAREST=0,	///< Use round to nearest integer
		ROUNDDOWN=1,		///< Use Floor() rounding
		ROUNDUP=2,			///< Use Ceil() rounding
		ROUNDTRUNCATE=3		///< Use fraction truncation rounding
	};
	extern e8087Rounding BURGER_API Set8087Rounding(e8087Rounding eInput);
#endif

#if defined(BURGER_WATCOM)
	float BURGER_INLINE Abs(float fInput) { return static_cast<float>(::std::fabs(fInput)); }
	double BURGER_INLINE Abs(double dInput) { return ::std::fabs(dInput); }
	float BURGER_INLINE Sqrt(float fInput) { return static_cast<float>(::std::sqrt(fInput)); }
	double BURGER_INLINE Sqrt(double dInput) { return ::std::sqrt(dInput); }
#elif defined(BURGER_XBOX360) && !defined(DOXYGEN)
	float BURGER_INLINE Abs(float fInput) { return static_cast<float>(fabs(fInput)); }
	double BURGER_INLINE Abs(double dInput) { return fabs(dInput); }
	float BURGER_INLINE Sqrt(float fInput) { return __fsqrts(fInput); }
	double BURGER_INLINE Sqrt(double dInput) { return __fsqrt(dInput); }
#elif defined(BURGER_PS4) && !defined(DOXYGEN)
	float BURGER_INLINE Abs(float fInput) { return fabsf(fInput); }
	double BURGER_INLINE Abs(double dInput) { return fabs(dInput); }
	float BURGER_INLINE Sqrt(float fInput) { return sqrtf(fInput); }
	double BURGER_INLINE Sqrt(double dInput) { return sqrt(dInput); }
#elif defined(BURGER_METROWERKS) && defined(BURGER_POWERPC)
	float BURGER_INLINE Abs(float fInput) { return __fabsf(fInput); }
	double BURGER_INLINE Abs(double dInput) { return __fabs(dInput); }
#if defined(BURGER_MACOSX)
	float BURGER_INLINE Sqrt(float fInput) { return sqrtf(fInput); }
#else
	float BURGER_INLINE Sqrt(float fInput) { return static_cast<float>(sqrt(fInput)); }
#endif
	double BURGER_INLINE Sqrt(double dInput) { return sqrt(dInput); }
#elif defined(BURGER_METROWERKS) && defined(BURGER_X86)
#if __has_intrinsic(__builtin_fabsf)
	float BURGER_INLINE Abs(float fInput) { return ::std::__builtin_fabsf(fInput); }
#else
	float BURGER_INLINE Abs(float fInput) { asm fld fInput
		asm fabs
		asm fstp fInput
	return fInput; }
#endif
#if __has_intrinsic(__builtin_fabs) 
	double BURGER_INLINE Abs(double dInput) { return ::std::__builtin_fabs(dInput); }
#else
	double BURGER_INLINE Abs(double dInput) { asm fld dInput
		asm fabs
		asm fstp dInput
	return dInput; }
#endif
#if __has_intrinsic(__builtin_sqrt)
	float BURGER_INLINE Sqrt(float fInput) { return static_cast<float>(::std::__builtin_sqrt(fInput)); }
	double BURGER_INLINE Sqrt(double dInput) { return ::std::__builtin_sqrt(dInput); }
#else
	float BURGER_INLINE Sqrt(float fInput) { asm fld fInput
		asm fsqrt
		asm fstp fInput
	return fInput; }
	double BURGER_INLINE Sqrt(double dInput) { asm fld dInput
		asm fsqrt
		asm fstp dInput
	return dInput; }
#endif
#elif (defined(BURGER_X86) || defined(BURGER_AMD64) || (defined(BURGER_MACOSX) && !defined(BURGER_METROWERKS))) && !defined(DOXYGEN)
	float BURGER_INLINE Abs(float fInput) { return static_cast<float>(fabs(fInput)); }
	double BURGER_INLINE Abs(double dInput) { return fabs(dInput); }
	float BURGER_INLINE Sqrt(float fInput) { return static_cast<float>(sqrt(static_cast<double>(fInput))); }
	double BURGER_INLINE Sqrt(double dInput) { return sqrt(dInput); }
#else
	extern float BURGER_API Abs(float fInput);
	extern double BURGER_API Abs(double dInput);
	extern float BURGER_API Sqrt(float fInput);
	extern double BURGER_API Sqrt(double dInput);
#endif

	float BURGER_INLINE IntToFloat(Int32 iInput) { return static_cast<float>(iInput); }
	float BURGER_INLINE IntToFloat(const Int32 *pInput) { return static_cast<float>(pInput[0]); }
	void BURGER_INLINE IntToFloat(float *pOutput,Int32 iInput) { pOutput[0] = static_cast<float>(iInput); }
	float BURGER_INLINE FixedToFloat(Fixed32 fInput) { return static_cast<float>(fInput)*(1.0f/65536.0f); }
	float BURGER_INLINE FixedToFloat(const Fixed32 *pInput) { return static_cast<float>(pInput[0])*(1.0f/65536.0f); }
	void BURGER_INLINE FixedToFloat(float *pOutput,Fixed32 fInput) { pOutput[0] = static_cast<float>(fInput)*(1.0f/65536.0f); }
#if defined(BURGER_XBOX360) && !defined(DOXYGEN)
	void BURGER_INLINE IntToFloat(float *pOutput,const Int32 *pInput) {
		__vector4 vTemp = __lvlx(pInput,0);	// Load in VMX128
		vTemp = __vcfsx(vTemp,0);			// Convert
		vTemp = __vspltw(vTemp,0);			// Splat
		__stvewx(vTemp,pOutput,0);			// Store 32 bit
	}		
	void BURGER_INLINE FixedToFloat(float *pOutput,const Fixed32 *pInput) {
		__vector4 vTemp = __lvlx(pInput,0);	// Load in VMX128
		vTemp = __vcfsx(vTemp,16);		// Use 16.16 fixed point conversion
		vTemp = __vspltw(vTemp,0);		// Splat
		__stvewx(vTemp,pOutput,0);		// Store 32 bit
	}		
#else
	void BURGER_INLINE IntToFloat(float *pOutput,const Int32 *pInput) { pOutput[0] = static_cast<float>(pInput[0]); }
	void BURGER_INLINE FixedToFloat(float *pOutput,const Fixed32 *pInput) { pOutput[0] = static_cast<float>(pInput[0])*(1.0f/65536.0f); }
#endif
	BURGER_INLINE float Interpolate(float fFrom,float fTo,float fFactor) { return ((fTo - fFrom) * fFactor) + fFrom; }
	BURGER_INLINE double Interpolate(double dFrom,double dTo,double dFactor) { return ((dTo - dFrom) * dFactor) + dFrom; }
	BURGER_INLINE float Square(float fInput) { return fInput*fInput; }
	BURGER_INLINE double Square(double dInput) { return dInput*dInput; }
	BURGER_INLINE float Sign(float fInput) { return ((fInput > 0.0f) ? 1.0f : ((fInput < 0.0f) ? -1.0f : 0.0f)); }
	BURGER_INLINE double Sign(double dInput) { return ((dInput > 0.0) ? 1.0 : ((dInput < 0.0) ? -1.0 : 0.0)); }
	BURGER_INLINE float Min(float fA,float fB) { return ((fA < fB) ? fA : fB); }
	BURGER_INLINE double Min(double dA,double dB) { return ((dA < dB) ? dA : dB); }
	BURGER_INLINE float Max(float fA,float fB) { return ((fA > fB) ? fA : fB); }
	BURGER_INLINE double Max(double dA,double dB) { return ((dA > dB) ? dA : dB); }
	BURGER_INLINE float Clamp(float fIn,float fMin,float fMax) { fIn = Max(fIn,fMin); return Min(fIn,fMax); }
	BURGER_INLINE double Clamp(double dIn,double dMin,double dMax) { dIn = Max(dIn,dMin); return Min(dIn,dMax); }
	extern Word BURGER_API IsNan(float fInput);
	extern Word BURGER_API IsNan(double dInput);
	extern Word BURGER_API IsInf(float fInput);
	extern Word BURGER_API IsInf(double dInput);
	extern Word BURGER_API IsFinite(float fInput);
	extern Word BURGER_API IsFinite(double dInput);
	extern Word BURGER_API IsNormal(float fInput);
	extern Word BURGER_API IsNormal(double dInput);
	extern Word BURGER_API SignBit(float fInput);
	extern Word BURGER_API SignBit(double dInput);
	extern float BURGER_API Floor(float fInput);
	extern double BURGER_API Floor(double dInput);
	extern float BURGER_API Ceil(float fInput);
	extern double BURGER_API Ceil(double dInput);
	extern float BURGER_API Round(float fInput);
	extern double BURGER_API Round(double dInput);
	extern float BURGER_API RoundToZero(float fInput);
	extern double BURGER_API RoundToZero(double dInput);
	extern float BURGER_API ModuloRadians(float fInput);
	extern double BURGER_API ModuloRadians(double dInput);
	extern float BURGER_API Sin(float fInput);
	extern double BURGER_API Sin(double dInput);
	extern float BURGER_API Cos(float fInput);
	extern double BURGER_API Cos(double dInput);
	extern float BURGER_API Tan(float fInput);
	extern double BURGER_API Tan(double dInput);
	extern float BURGER_API ASin(float fInput);
	extern double BURGER_API ASin(double dInput);
	extern float BURGER_API ACos(float fInput);
	extern double BURGER_API ACos(double dInput);
	extern float BURGER_API ATan(float fInput);
	extern double BURGER_API ATan(double dInput);
	extern float BURGER_API ATan2(float fSin,float fCos);
	extern double BURGER_API ATan2(double dSin,double dCos);
	extern float BURGER_API Exp(float fInput);
	extern double BURGER_API Exp(double dInput);
	extern float BURGER_API Log(float fInput);
	extern double BURGER_API Log(double dInput);
	extern float BURGER_API Log2(float fInput);
	extern double BURGER_API Log2(double dInput);
	extern float BURGER_API Log10(float fInput);
	extern double BURGER_API Log10(double dInput);
	extern float BURGER_API Modf(float fInput,float *pInteger);
	extern double BURGER_API Modf(double dInput,double *pInteger);
	extern float BURGER_API Fmod(float fInput,float fDivisor);
	extern double BURGER_API Fmod(double dInput,double dDivisor);
}
/* END */

#endif
