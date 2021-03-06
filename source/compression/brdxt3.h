/***************************************

	DXT3 compressor and decompresser

	Copyright 1995-2014 by Rebecca Ann Heineman becky@burgerbecky.com

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#ifndef __BRDXT3_H__
#define __BRDXT3_H__

#ifndef __BRTYPES_H__
#include "brtypes.h"
#endif

#ifndef __BRPALETTE_H__
#include "brpalette.h"
#endif

/* BEGIN */
namespace Burger {
struct Dxt3Packet_t {
	Word8 m_uAlpha[8];				///< Array of 4 bits per pixel alpha
	Word16 m_uRGB565Color1;			///< First color endpoint in R5:G6:B5 little endian format
	Word16 m_uRGB565Color2;			///< Second color endpoint in R5:G6:B5 little endian format
	Word8 m_uColorIndexes[4];		///< 2 bits per pixel color indexes for 4x4 tile
	void Decompress(RGBAWord8_t *pOutput,WordPtr uStride = sizeof(RGBAWord8_t)*4) const;
	void Compress(const RGBAWord8_t *pInput,WordPtr uStride = sizeof(RGBAWord8_t)*4);
};
extern void BURGER_API DecompressImage(RGBAWord8_t *pOutput,WordPtr uOutputStride,Word uWidth,Word uHeight,const Dxt3Packet_t *pInput,WordPtr uInputStride);
}
/* END */

#endif
