/***************************************

	Decompression manager version of RLE

	Copyright 1995-2014 by Rebecca Ann Heineman becky@burgerbecky.com

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#include "brdecompresslbmrle.h"

/*! ************************************

	\class Burger::DecompressILBMRLE
	\brief Decompress RLE format
	
	Decompress data in RLE format (Documented here Burger::CompressRLE )
	Token >=0x80 = 0x101-Token Repeat (Min 2, Max 129)
	Token < 0x80 = Token+1 Data[Token+1] (Min 1, Max 128)

	\sa Burger::Decompress and Burger::CompressILBMRLE

***************************************/

/*! ************************************

	\brief Default constructor

	Initializes the defaults

***************************************/

Burger::DecompressILBMRLE::DecompressILBMRLE(void) :
	Decompress(),
	m_uRun(0),
	m_uFill(0),
	m_eState(STATE_INIT)
{
	reinterpret_cast<Word32 *>(m_uSignature)[0] = Signature;
}

/*! ************************************

	\brief Reset the RLE decompression

	\return Decompress::DECOMPRESS_OKAY (No error is possible)

***************************************/

Burger::Decompress::eError Burger::DecompressILBMRLE::Reset(void)
{
	m_uTotalOutput = 0;
	m_uTotalInput = 0;
	m_eState = STATE_INIT;
	return DECOMPRESS_OKAY;
}

/*! ************************************

	\brief Decompress data using RLE compression

	Using the RLE compression algorithm, decompress the data

	\param pOutput Pointer to the buffer to accept the decompressed data
	\param uOutputChunkLength Number of bytes in the output buffer
	\param pInput Pointer to data to compress
	\param uInputChunkLength Number of bytes in the data to decompress

	\return Decompress::eError code with zero if no failure, non-zero is an error code
	\sa Burger::SimpleDecompressILBMRLE()

***************************************/

Burger::Decompress::eError Burger::DecompressILBMRLE::Process(void *pOutput,WordPtr uOutputChunkLength,const void *pInput,WordPtr uInputChunkLength)
{
	m_uInputLength = uInputChunkLength;
	m_uOutputLength = uOutputChunkLength;

	// Default state is assumed

	eState MyState = m_eState;
	if (uInputChunkLength || (MyState!=STATE_INIT)) {
		m_eState = STATE_INIT;		// Reset the state

		// Restore the state variables

		WordPtr uRunLength = m_uRun;	// Restore the run length
		Word uFillTemp = m_uFill;		// Fill value

		if (MyState==STATE_FILLTOKEN) {
			goto FillToken;
		}
		if (MyState==STATE_FILL) {
			goto Fill;
		}
		if (MyState==STATE_RUN) {
			goto Run; 
		}

		do {
			// Get the run token
			uRunLength = static_cast<const Word8 *>(pInput)[0];
			pInput = static_cast<const Word8 *>(pInput)+1;
			--uInputChunkLength;

			if (uRunLength>=128) {		// Run length?
				uRunLength = 257-uRunLength;	// Count the run (2-129)

				if (!uInputChunkLength) {
					m_eState = STATE_FILLTOKEN;
					m_uRun = uRunLength;
					break;
				}
FillToken:
				// Filler value
				uFillTemp = static_cast<const Word8 *>(pInput)[0];
				pInput = static_cast<const Word8 *>(pInput)+1;
				--uInputChunkLength;

				// Perform a memory fill
Fill:
				if (uOutputChunkLength<uRunLength) {
					// Save the memory fill state
					m_eState = STATE_FILL;
					m_uRun = uRunLength-uOutputChunkLength;
					m_uFill = uFillTemp;
					if (uOutputChunkLength) {
						do {
							static_cast<Word8 *>(pOutput)[0] = static_cast<Word8>(uFillTemp);
							pOutput = static_cast<Word8 *>(pOutput)+1;
						} while (--uOutputChunkLength);
					}
					break;
				}
				// Perform the memory fill
				uOutputChunkLength -= uRunLength;
				do {
					static_cast<Word8 *>(pOutput)[0] = static_cast<Word8>(uFillTemp);
					pOutput = static_cast<Word8 *>(pOutput)+1;
				} while (--uRunLength);

			} else {

				// Handle a memory copy
				++uRunLength;		// +1 to the count
Run:
				if ((uOutputChunkLength<uRunLength) ||
					(uInputChunkLength<uRunLength)) {
					// Use the smaller run
					WordPtr uMaxRun = uRunLength;
					if (uInputChunkLength<uMaxRun) {
						uMaxRun = uInputChunkLength;
					}
					if (uOutputChunkLength<uMaxRun) {
						uMaxRun = uOutputChunkLength;
					}
					// Store the remainder for the next time
					uInputChunkLength -= uMaxRun;
					uOutputChunkLength -= uMaxRun;
					m_eState = STATE_RUN;
					m_uRun = uRunLength-uMaxRun;
					if (uMaxRun) {
						do {
							static_cast<Word8 *>(pOutput)[0] = static_cast<const Word8 *>(pInput)[0];
							pInput = static_cast<const Word8 *>(pInput)+1;
							pOutput = static_cast<Word8 *>(pOutput)+1;
						} while (--uMaxRun);
					}
					break;
				}
				// Perform the memory copy
				uOutputChunkLength -= uRunLength;
				uInputChunkLength -= uRunLength;
				do {
					static_cast<Word8 *>(pOutput)[0] = static_cast<const Word8 *>(pInput)[0];
					pInput = static_cast<const Word8 *>(pInput)+1;
					pOutput = static_cast<Word8 *>(pOutput)+1;
				} while (--uRunLength);
			}
		} while (uInputChunkLength);	// More?
	}

	// How did the decompression go?

	m_uOutputLength -= uOutputChunkLength;
	m_uInputLength -= uInputChunkLength;
	m_uTotalOutput += m_uOutputLength;
	m_uTotalInput += m_uInputLength;

	// Output buffer not big enough?
	if (uOutputChunkLength) {
		return DECOMPRESS_OUTPUTUNDERRUN;
	}

	// Input data remaining?
	if (uInputChunkLength || (m_eState!=STATE_INIT)) {
		return DECOMPRESS_OUTPUTOVERRUN;
	}
	// Decompression is complete
	return DECOMPRESS_OKAY;
}

/*! ************************************

	\brief Decompress data using RLE compression

	Using the RLE compression algorithm, decompress the data in one pass

	\param pOutput Pointer to the buffer to accept the decompressed data
	\param uOutputChunkLength Number of bytes in the output buffer
	\param pInput Pointer to data to compress
	\param uInputChunkLength Number of bytes in the data to decompress

	\return Decompress::eError code with zero if no failure, non-zero is an error code
	\sa DecompressILBMRLE::Process()

***************************************/

Burger::Decompress::eError BURGER_API Burger::SimpleDecompressILBMRLE(void *pOutput,WordPtr uOutputChunkLength,const void *pInput,WordPtr uInputChunkLength)
{
	Burger::DecompressILBMRLE Local;
	Local.DecompressILBMRLE::Reset();
	return Local.DecompressILBMRLE::Process(pOutput,uOutputChunkLength,pInput,uInputChunkLength);
}
