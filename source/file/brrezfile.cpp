/***************************************

	Resource manager

	Copyright 1995-2014 by Rebecca Ann Heineman becky@burgerbecky.com

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#include "brrezfile.h"
#include "brstringfunctions.h"
#include "brendian.h"
#include "brglobalmemorymanager.h"
#include "brfilemanager.h"
#include "brfile.h"
#include "brfileansihelpers.h"
#include "brglobals.h"
#include <stdlib.h>

/*! ************************************

	\class Burger::RezFile
	\brief Rez file manager
	
	Using the tool MakeRez, an archive file can be generated
	and quickly used to access data used by a game or application
	
	The manager has the ability to load compressed data and decompress
	it on the fly if need be. The high level code has no need to know
	where the data is coming from and how it's cached in memory
	for performance

***************************************/

/*! ************************************

	\struct Burger::RezFile::RootHeader_t
	\brief Data image for the start of a Rez file
	
***************************************/

/*! ************************************

	\struct Burger::RezFile::FileRezEntry_t
	\brief Data image of a resource entry
	
***************************************/

/*! ************************************

	\struct Burger::RezFile::FileRezGroup_t
	\brief Data image of a resource group
	
***************************************/

/*! ************************************

	\struct Burger::RezFile::RezEntry_t
	\brief Data to describe each individual resource in memory
	
***************************************/

/*! ************************************

	\struct Burger::RezFile::RezGroup_t
	\brief Data to describe a group of resources in memory
	
***************************************/

/*! ************************************

	\struct Burger::RezFile::FilenameToRezNum_t
	\brief Sorted list of filenames
	
***************************************/

const char Burger::RezFile::g_RezFileSignature[] = {'B','R','G','R'};

/***************************************

	Internal routines, these are not intended for public consumption
	
***************************************/

/*! ************************************

	\brief Used to sort between filenames

	Called by qsort()
	\param pFirst First FilenameToRezNum_t * to compare
	\param pSecond Second FilenameToRezNum_t * to compare
	\return Result of a call to StringCaseCompare(const char *,const char *) for sorting
	\sa Burger::StringCaseCompare(const char *,const char *)
	
***************************************/

int BURGER_ANSIAPI Burger::RezFile::QSortNames(const void *pFirst,const void *pSecond)
{
	// Get the filename strings
	return StringCaseCompare(static_cast<const FilenameToRezNum_t *>(pFirst)->m_pRezName,
		static_cast<const FilenameToRezNum_t *>(pSecond)->m_pRezName);
}

/*! ************************************

	\brief Calculate the number of bytes m_pGroups occupies

	By traversing a valid RezGroup_t, determine
	the number of bytes it would take to store a RezGroup_t
	structure (Used for rebuilding the structure when
	an entry is added or removed)

	\return The number of bytes the RezGroup_t data is occupying.
	
***************************************/

WordPtr Burger::RezFile::GetRezGroupBytes(void) const
{
	// No data needed, yet
	WordPtr uTotal = 0;
	
	// Get the number of resource groups
	Word uGroupCount = m_uGroupCount;
	// Any resource groups?
	if (uGroupCount) {
		// Get the pointer to the array of data
		const RezGroup_t *pGroups = m_pGroups;
		do {
			// How many resources are in this group?
			// Note: Can't be zero
			Word uCount = pGroups->m_uCount;

			// Add in the array for the group
			uTotal += (uCount*sizeof(RezEntry_t))+(sizeof(RezGroup_t)-sizeof(RezEntry_t));
			
			// Pointer to the first entry
			const RezEntry_t *pEntry = pGroups->m_Array;
		
			// Now check each entry for a name string
			do {
				const char *pName = pEntry->m_pRezName;
				// Is there a name attached to this resource?
				if (pName) {
					// Add in the length of this string
					uTotal += StringLength(pName)+1;
				}
				// Next entry
				++pEntry;
			} while (--uCount);
			// Pointer to the next group
			pGroups = reinterpret_cast<const RezGroup_t *>(pEntry);
			// Loop for all groups
		} while (--uGroupCount);
	}
	// Return the total bytes needed to store the group data
	return uTotal;
}

/*! ************************************

	\brief Adjust pointers for text strings

	Traverse all of the in memory RezGroup_t structures,
	adjust the offsets for all the string pointers based on
	the new data padding difference from the file image
	and the in memory image

	\param uAdjust Number of bytes to add to all of the string offsets
	\return The number of bytes the RezGroup_t data is occupying.
	
***************************************/

void Burger::RezFile::AdjustNamePointers(WordPtr uAdjust)
{
	// Get the number of resource groups
	Word uGroupCount = m_uGroupCount;
	// Any resource groups?
	if (uGroupCount) {
		// Get the pointer to the array of data
		RezGroup_t *pGroups = m_pGroups;
		do {
			// How many resources are in this group?
			// Note: Can't be zero
			Word uCount = pGroups->m_uCount;
			
			// Pointer to the first entry
			RezEntry_t *pEntry = pGroups->m_Array;
		
			// Now check each entry for a name string
			do {
				const char *pName = pEntry->m_pRezName;
				// Is there a name attached to this resource?
				if (pName) {
					pEntry->m_pRezName = pName + uAdjust;
				}
				// Next entry
				++pEntry;
			} while (--uCount);
			// Pointer to the next group
			pGroups = reinterpret_cast<RezGroup_t *>(pEntry);
			// Loop for all groups
		} while (--uGroupCount);
	}
}

/*! ************************************

	\brief Locate a resource entry by using a resource number

	Scan for a resource entry in the resource map.

	\param uRezNum Resource item number
	\return \ref NULL if resource does not exist, a valid Burger::RezFile::RezEntry_t pointer if so.
	\sa Burger::RezFile::FindName()

***************************************/

Burger::RezFile::RezEntry_t * Burger::RezFile::Find(Word uRezNum) const
{
	// Nothing to return
	RezEntry_t *pResult=NULL;
	// Get the number of resource groups
	Word uGroupCount = m_uGroupCount;
	// Any present?
	if (uGroupCount) {
		// Get the main pointer (Assume valid)
		RezGroup_t *pGroups = m_pGroups;
		do {
			// Get the offset from the base resource number
			Word uTemp = uRezNum-pGroups->m_uBaseRezNum;
			Word uCount = pGroups->m_uCount;
			// Is it in range?
			if (uTemp<uCount) {
				// Got it! (Most of the time this is one pass)
				pResult = &pGroups->m_Array[uTemp];
				break;
			}
			// Next group (Each structure is variable sized)
			pGroups = reinterpret_cast<RezGroup_t *>(&pGroups->m_Array[uCount]);
		} while (--uGroupCount);
	}
	// Return NULL or a pointer
	return pResult;
}

/*! ************************************

	\brief Find a resource number from a resource name

	Using a resource name list (Sorted so that
	a binary search can be performed), look
	if there is a match in the resource list
	Return \ref TRUE if an entry was found, \ref FALSE if
	there was no match. However, if \ref FALSE is 
	returned, then the entry that will FOLLOW
	the string is returned for new entry insertion

	\param pRezName Pointer to a "C" string of the requested filename
	\param ppOutput Pointer to accept the pointer to the matching Burger::RezFile::FilenameToRezNum_t structure
	\return \ref TRUE if the name was found, \ref FALSE if not

***************************************/

Word Burger::RezFile::FindName(const char *pRezName,Burger::RezFile::FilenameToRezNum_t **ppOutput) const
{	
	// Work Pointer
	FilenameToRezNum_t *pFoundRezName = NULL;
	// Any names in this resource file?
	FilenameToRezNum_t *pRezNames = m_pRezNames;	
	if (pRezNames) {

		// If the filename has a prefix "20:FileName.txt"
		// Then remove the prefix so it can also match

		Word uMinRec = reinterpret_cast<const Word8 *>(pRezName)[0];
		// Is the first character a number? (Prefix)
		if ((uMinRec>='0') && (uMinRec<('9'+1))) {
			// Is there a colon pathname seperator?
			const char *pAuxName = StringCharacter(pRezName,':');
			if (pAuxName) {
				// Index past it
				pRezName = pAuxName+1;
			}
		}

		// Perform a binary search to find the filename

		// Make the bounds
		uMinRec = 0;
		Word uMaxRec = m_uRezNameCount;
		int Test;
		do {
			// This is the record I'll test (Halfway)
			Word EntryNum = ((uMaxRec-uMinRec)>>1)+uMinRec;
			pFoundRezName = &pRezNames[EntryNum];			/* Check this one */
			// Case insensitive test
			Test = StringCaseCompare(pFoundRezName->m_pRezName,pRezName);
			// Match?
			if (!Test) {
				// Done!
				ppOutput[0] = pFoundRezName;
				return TRUE;
			}
			// Go lower or higher?
			if (Test>0) {
				// Go lower
				uMaxRec = EntryNum;
			} else {
				// Go higher
				uMinRec = EntryNum+1;
			}
			// No more to check?
		} while (uMinRec<uMaxRec);
		// Was the last test a "higher"?
		if (Test<0) {
			// Must be before this one...
			++pFoundRezName;
		}
	}
	// Return the closest match
	ppOutput[0] = pFoundRezName;
	// However, it's NOT a match
	return FALSE;
}

/*! ************************************

	\brief Create the in memory rez file dictionary from a file image

	\note To support the old data type where there were
	unique types in addition to ID's. A hack was applied 
	that will remap type "5" to ID+5000. This way
	old games like Killing Time can use the new burgerlib
	without having to update the .REZ files.

	\param pData Pointer to the Rez File image
	\param pHeader Pointer to the parsed file header
	\param uSwapFlag \ref TRUE if endian should be swapped
	\param uStartOffset Number of bytes the header was from the start of file (For data index offset adjustments)

	\return A newly allocated Burger::RezFile::RezGroup_t * with all the data found from the file image

***************************************/

Burger::RezFile::RezGroup_t * BURGER_API Burger::RezFile::ParseRezFileHeader(const Word8 *pData,const RootHeader_t *pHeader,Word uSwapFlag,Word32 uStartOffset)
{
	// Get the number of groups to parse
	Word uGroupCount = pHeader->m_uGroupCount;
	if (uGroupCount) {

		// First pass, determine how much memory will my header need

		// Init my work pointer
		const Word8 *pWork = pData;

		// This is how many bytes I'm going to need
		WordPtr uNewLength = 0;
		// Parse the old way?
		if (uSwapFlag&OLDFORMAT) {
			do {
				Word uOldCount;
				if (uSwapFlag&SWAPENDIAN) {
					uOldCount = SwapEndian::Load(reinterpret_cast<const Word32 *>(pWork+8));		/* Fix endian if needed */
				} else {
					uOldCount = reinterpret_cast<const Word32 *>(pWork)[2];		// Get the count
				}
				// Number of bytes needed to store this record
				uNewLength += (((uOldCount-1)*sizeof(RezEntry_t))+sizeof(RezGroup_t));
				// Index to the next record
				pWork = pWork + (sizeof(Word32)*3) + (uOldCount*(sizeof(Word32)*3));
			} while (--uGroupCount);
		} else {
		// Parse the new way
  			do {
				Word uNewCount = LittleEndian::Load(&reinterpret_cast<const FileRezGroup_t *>(pWork)->m_uCount);	/* Get the count */
				// Number of bytes needed to store this record
				uNewLength += (uNewCount*sizeof(RezEntry_t))+(sizeof(RezGroup_t)-sizeof(RezEntry_t));
				// Next group
				pWork = pWork + (sizeof(Word32)*2) + (uNewCount*(sizeof(Word32)*4));
	   		} while (--uGroupCount);
		}
		// How many bytes until the end of the data
		WordPtr uAdjust = static_cast<WordPtr>(pWork-pData);
		// Determine the amount of memory the filenames need
		WordPtr uTextLength = pHeader->m_uMemSize - uAdjust;
		uAdjust = uNewLength-uAdjust;

		// Now I have the length I need to make my new header, create it

		RezGroup_t *pResult = static_cast<RezGroup_t *>(Alloc(uNewLength+uTextLength));
		// Valid pointer?
		if (pResult) {
			const char *pAdjusted = reinterpret_cast<const char *>(pResult)+uAdjust;
			// Let's fill in the new records!
			RezGroup_t *pGroup = pResult;
			pWork = pData;
			// Reset the group count
			uGroupCount = pHeader->m_uGroupCount;
			if (uSwapFlag&OLDFORMAT) {
				do {
					// Old rez files had a type record
					Word32 uType;
					// Load the type, base and count
					if (uSwapFlag&SWAPENDIAN) {
						uType = SwapEndian::Load(reinterpret_cast<const Word32 *>(pWork));
						pGroup->m_uBaseRezNum = SwapEndian::Load(&reinterpret_cast<const Word32 *>(pWork)[1]);
						pGroup->m_uCount = SwapEndian::Load(&reinterpret_cast<const Word32 *>(pWork)[2]);
					} else {
						uType = reinterpret_cast<const Word32 *>(pWork)[0];
						pGroup->m_uBaseRezNum = reinterpret_cast<const Word32 *>(pWork)[1];
						pGroup->m_uCount = reinterpret_cast<const Word32 *>(pWork)[2];
					}
					pWork += sizeof(Word32)*3;
					// Patch in sound files in Killing Time
					// Type 5 is now ID+5000
					if (uType==5) {
						pGroup->m_uBaseRezNum+=5000;
					}

					// Process each resource entry
				
					Word uCount = pGroup->m_uCount;
					RezEntry_t *pEntry = pGroup->m_Array;
					if (uCount) {
						do {
							Word uFileOffset;
							Word uLength;
							Word uNameOffset;
							if (uSwapFlag&SWAPENDIAN) {
								uFileOffset = SwapEndian::Load(&reinterpret_cast<const Word32*>(pWork)[0]);
								uLength = SwapEndian::Load(&reinterpret_cast<const Word32*>(pWork)[1]);
								uNameOffset = SwapEndian::Load(&reinterpret_cast<const Word32*>(pWork)[2]);
							} else {
								uFileOffset = reinterpret_cast<const Word32*>(pWork)[0];
								uLength = reinterpret_cast<const Word32*>(pWork)[1];
								uNameOffset = reinterpret_cast<const Word32*>(pWork)[2];
							}
							// Initialize the structure
							Word uFlags = 0;
							pEntry->m_ppData = NULL;
							pEntry->m_pRezName = NULL;
							pEntry->m_uCompressedLength = uLength;
							// Skip the 3 32 bit words (Can never change)
							pWork += sizeof(Word32)*3;
							// Is there a name?
							if (uNameOffset) {
								pEntry->m_pRezName = pAdjusted+uNameOffset;
							}
							// Do I need to load this at the top of memory?
							if (uFileOffset&REZOFFSETFIXED) {
								uFileOffset&=(~REZOFFSETFIXED);
								uFlags = ENTRYFLAGSHIGHMEMORY;
							}
							// Is the data compressed?
							if (uFileOffset&REZOFFSETDECOMPMASK) {
								uFlags |= ((uFileOffset&REZOFFSETDECOMPMASK)>>(REZOFFSETDECOMPSHIFT-ENTRYFLAGSDECOMPSHIFT));
								uFileOffset&=(~REZOFFSETDECOMPMASK);
								uLength = 0;
							}
							pEntry->m_uLength = uLength;
							pEntry->m_uFlags = uFlags;
							// Without the flags, add in the file start for the true offset
							pEntry->m_uFileOffset = uFileOffset+uStartOffset;
							++pEntry;
						} while (--uCount);
					}
					// Next entry
					pGroup = reinterpret_cast<RezGroup_t *>(pEntry);
				} while (--uGroupCount);
				// Count down
			} else {
				// This is the new file format

				do {
					// Get the base resource number and the resource count
					pGroup->m_uBaseRezNum = LittleEndian::Load(&reinterpret_cast<const FileRezGroup_t*>(pWork)->m_uBaseRezNum);
					pGroup->m_uCount = LittleEndian::Load(&reinterpret_cast<const FileRezGroup_t*>(pWork)->m_uCount);
					pWork += (sizeof(Word32)*2);
				
					// Process each resource entry
				
					Word uNewCount = pGroup->m_uCount;
					RezEntry_t *pEntry = pGroup->m_Array;
					if (uNewCount) {
						do {
							pEntry->m_ppData = NULL;
							pEntry->m_pRezName = NULL;
							// Adjust the file offset from the start of the file image
							pEntry->m_uFileOffset = LittleEndian::Load(&reinterpret_cast<const FileRezEntry_t *>(pWork)->m_uFileOffset)+uStartOffset;
							pEntry->m_uLength = LittleEndian::Load(&reinterpret_cast<const FileRezEntry_t *>(pWork)->m_uLength);
							Word uNameOffset = LittleEndian::Load(&reinterpret_cast<const FileRezEntry_t *>(pWork)->m_uNameOffset);
							pEntry->m_uCompressedLength = LittleEndian::Load(&reinterpret_cast<const FileRezEntry_t *>(pWork)->m_uCompressedLength);
							// Next 4 longwords (Rigid)
  							pWork += (sizeof(Word32)*4);
							if (uNameOffset&ENTRYFLAGSNAMEOFFSETMASK) {
								pEntry->m_pRezName = pAdjusted+(uNameOffset&ENTRYFLAGSNAMEOFFSETMASK);
							}
							pEntry->m_uFlags=uNameOffset&(~ENTRYFLAGSNAMEOFFSETMASK);
							++pEntry;
						} while (--uNewCount);
					}
					// Next!
					pGroup = reinterpret_cast<RezFile::RezGroup_t *>(pEntry);
					// Count down
				} while (--uGroupCount);
			}
			// Copy up the filenames if needed
			MemoryCopy(pGroup,pWork,uTextLength);
			return pResult;
		}
	}

	// Don't make a list... (Bad count or memory error)
	return NULL;
}

/*! ************************************

	\brief Create a hash table for all of the filenames

	Release the previous m_pRezNames array and create a new one from
	scratch. m_pRezNames contains a sorted filename lookup
	list that is used by Burger::RezFile::FindName(const char *,RezFile::FilenameToRezNum_t **) const to quickly look up
	a resource entry

	\sa Burger::RezFile::FindName(const char *,RezFile::FilenameToRezNum_t **) const

***************************************/

void Burger::RezFile::ProcessRezNames(void)
{
	// Firstly, determine the number of entries that have filenames

	Word uTotal = 0;
	// Get the group count
	Word uGroupCount = m_uGroupCount;
	// Any groups?
	if (uGroupCount) {
		// Start here
		const RezGroup_t *pGroups = m_pGroups;
		do {
			Word uCount = pGroups->m_uCount;
			const RezEntry_t *pEntry = pGroups->m_Array;
			do {
				// There's a name here
				if (pEntry->m_pRezName) {
					// Add to the count
					++uTotal;
				}
				++pEntry;
				// Is the group scanned?
			} while (--uCount);
			// Next!
			pGroups = reinterpret_cast<const RezGroup_t *>(pEntry);
			// Count down 
		} while (--uGroupCount);
	}
	
	// Any names found?
	
	if (uTotal) {
	
		// Allocate the new hash table only if the data have been resized

		FilenameToRezNum_t *pRezNames = m_pRezNames;
		if ((uTotal!=m_uRezNameCount) || !pRezNames) {
		// Save the main values
			Free(m_pRezNames);
			pRezNames = static_cast<FilenameToRezNum_t *>(Alloc(sizeof(FilenameToRezNum_t)*uTotal));
			m_uRezNameCount = uTotal;
			m_pRezNames = pRezNames;
		}

		// Did the allocation fail?
		if (pRezNames) {
						
			// Let's fill in the hash table

			// Get the group count and input pointer
			uGroupCount = m_uGroupCount;
			const RezGroup_t *pGroups = m_pGroups;
			// Store here!
			FilenameToRezNum_t *pDest = pRezNames;
			do {
				Word uCount = pGroups->m_uCount;
				const RezEntry_t *pEntry = pGroups->m_Array;
				Word uRezNum = pGroups->m_uBaseRezNum;
				do {
					// Is there a name here?
					if (pEntry->m_pRezName) {
						// Store the name and resource number
						pDest->m_pRezName = pEntry->m_pRezName;
						pDest->m_uRezNum = uRezNum;
						++pDest;
					}
					// Up the pointer and the resource number
					++pEntry;
					++uRezNum;
					// Has the group been processed?
				} while (--uCount);
				// Next!
				pGroups = reinterpret_cast<const RezGroup_t *>(pEntry);
				// Count down
			} while (--uGroupCount);
				
			// The hash table MUST be sorted!
			
			qsort(pRezNames,uTotal,sizeof(FilenameToRezNum_t),QSortNames);
			return;
		}
	}
	// No data is present. Just delete any previous data
	Free(m_pRezNames);
	m_uRezNameCount = 0;
	m_pRezNames = NULL;
}

/*! ************************************

	\brief Compact and sort the filenames when a name is
	inserted or deleted.

	The filenames are stored sorted to enable a quick lookup via 
	a binary search. When filenames are added or removed at runtime,
	a fixup may be necessary.

	\param pText Pointer to the array of filenames to insert or compact

***************************************/

void Burger::RezFile::FixupFilenames(char *pText)
{
	RezGroup_t *pNewGroup = m_pGroups;
	Word uGroupCount = m_uGroupCount;
	do {
		RezEntry_t *pNewEntry = pNewGroup->m_Array;
		Word uCount = pNewGroup->m_uCount;
		do {
			const char *pNewText = pNewEntry->m_pRezName;
			if (pNewText) {
				pNewEntry->m_pRezName = pText;
				WordPtr uLength = StringLength(pNewText)+1;
				MemoryCopy(pText,pNewText,uLength);
				pText+=uLength;
			}
			++pNewEntry;
		} while (--uCount);
		pNewGroup = reinterpret_cast<RezGroup_t *>(pNewEntry);
	} while (--uGroupCount);
	// Sort the names
	ProcessRezNames();
}


/*! ************************************

	\brief Initialize the RezFile class to a power up state
	
	This constructor initializes the variables to ensure
	the structure doesn't have dangling pointers

	\param pMemoryManager Pointer to a valid handle based memory manager

***************************************/

Burger::RezFile::RezFile(Burger::MemoryManagerHandle *pMemoryManager) :
	m_File(),
	m_pMemoryManager(pMemoryManager),
	m_uGroupCount(0),
	m_uRezNameCount(0),
	m_pGroups(NULL),
	m_pRezNames(NULL),
	m_bExternalFileEnabled(TRUE)
{
	Word i=0;
	do {
		m_Decompressors[i] = NULL;
	} while (++i<MAXCODECS);
}

/*! ************************************

	\brief Dispose of all allocated resources

	Close any open files and release all memory.
	Any pointers that were given to the application from
	this RezFile will now be considered invalid.

	\sa Burger::RezFile::Shutdown()

***************************************/

Burger::RezFile::~RezFile()
{
	Shutdown();
}

/*! ************************************

	\brief Create a new RezFile instance

	Allocate memory using Burger::Alloc() and 
	initialize a RezFile with it.

	\param pMemoryManager Pointer to a valid handle based memory manager
	\param pFileName Pointer to a "C" string of a filename to a rez file
	\param uStartOffset Offset in bytes from the start of the file where the rezfile image resides. Normally this is zero.

	\return \ref NULL if out of memory or the file is not a valid rezfile
	\sa Burger::Delete(const RezFile *)

***************************************/

Burger::RezFile * BURGER_API Burger::RezFile::New(Burger::MemoryManagerHandle *pMemoryManager,const char *pFileName,Word32 uStartOffset)
{
	// Manually allocate the memory
	RezFile *pThis = new (Alloc(sizeof(RezFile))) RezFile(pMemoryManager);
	if (pThis) {
	// Load up the data
		if (!pThis->Init(pFileName,uStartOffset)) {
			// We're good!
			return pThis;
		}
		// Kill the malformed class
		Delete(pThis);
	}
	// Sorry Charlie!
	return NULL;
}

/*! ************************************

	\brief Dispose of a new RezFile instance

	When New is called, release the memory with this call

	\sa Burger::RezFile::New(Burger::MemoryManagerHandle *,const char *,Word32)

***************************************/

void BURGER_API Burger::Delete(const RezFile *pThis)
{
	if (pThis) {
		const_cast<RezFile *>(pThis)->~RezFile();
		Free(pThis);
	}
}

/*! ************************************

	\brief Open a resource file for reading

	\param pFileName Pointer to "C" string of the filename to open
	\param uStartOffset Offset from the start of the file where the rezfile image resides. Normally zero.

	\return \ref FALSE if no error occurred.
		A non-zero value (error code) if it couldn't open the file

***************************************/

Word Burger::RezFile::Init(const char *pFileName,Word32 uStartOffset)
{
	// If there was a previous file, release it
	Shutdown();

	if (pFileName) {
		// Can I open the file?
		if (m_File.Open(pFileName,File::READONLY)==File::OKAY) {
			// If the data is not at the head, seek
			if (uStartOffset) {
				m_File.SetMark(uStartOffset);
			}
			// Struct for resource file header
			RootHeader_t MyHeader;
			// Read in the header
			if (m_File.Read(&MyHeader,ROOTHEADERSIZE)==ROOTHEADERSIZE) {
				// Check the signature
				if (!MemoryCompare(MyHeader.m_Name,g_RezFileSignature,4)) {
					// Assume new data format
					Word uSwapFlag = 0;
					// Hack test to see if this is an old format file
					if (MyHeader.m_CodecID[0][3]<32) {
						uSwapFlag = OLDFORMAT;
						// Endian test
						if (MyHeader.m_uMemSize >= SwapEndian::Load(&MyHeader.m_uMemSize)) {
							MyHeader.m_uMemSize = SwapEndian::Load(&MyHeader.m_uMemSize);
							MyHeader.m_uGroupCount = SwapEndian::Load(&MyHeader.m_uGroupCount);
							// Swap the endian
							uSwapFlag |= SWAPENDIAN;
						}
						// I read in 24 bytes, it's really 12, so skip back
						m_File.SetMark(uStartOffset+12);
					} else {
						LittleEndian::Fixup(&MyHeader.m_uMemSize);
						LittleEndian::Fixup(&MyHeader.m_uGroupCount);
					}

					// Allocate memory to load header
					Word8 *pData = static_cast<Word8 *>(Alloc(MyHeader.m_uMemSize));
					if (pData) {
						// Read in the file header
						if (m_File.Read(pData,MyHeader.m_uMemSize)==MyHeader.m_uMemSize) {
							RezGroup_t *pRezGroup = ParseRezFileHeader(pData,&MyHeader,uSwapFlag,uStartOffset);
							if (pRezGroup) {
								// Dispose of the loaded data
								Free(pData);
								// Save the file reference
								m_uGroupCount = MyHeader.m_uGroupCount;	// Get the resource count
								m_pGroups = pRezGroup;	// Get the memory
								m_bExternalFileEnabled = TRUE;	// External files are ok
								ProcessRezNames();		// Make the initial name hash
								return FALSE;
							}
						}
						// Release the file header
						Free(pData);
					}
				}
			}
			// Close the file on error
			m_File.Close();
		}
	}
	// Could not open the file
	return TRUE;
}

/*! ************************************

	\brief Dispose of the contents of a resource file

	Release all resources allocated by a resource file. All pointers that have
	been given to the application are now all invalid.

***************************************/

void Burger::RezFile::Shutdown(void)
{
	// Is there an open file?
	m_File.Close();

	// Dispose of any resources in memory
	// Any valid entries?
	Word uGroupCount = m_uGroupCount;
	if (uGroupCount) {
		// Get the pointer locally
		RezGroup_t *pGroups = m_pGroups;
		do {
			RezEntry_t *pEntry = pGroups->m_Array;
			Word uCount = pGroups->m_uCount;
			do {
				// Dispose of the memory
				m_pMemoryManager->FreeHandle(pEntry->m_ppData);
#if defined(_DEBUG)
				// Was every single resource released properly?
				if ((pEntry->m_uFlags&ENTRYFLAGSREFCOUNT) && Globals::AreWarningsEnabled()) {
					Debug::Message("RezFile::Shutdown() : Resource %u still referenced %u times\n",
						(pGroups->m_uCount-uCount)+pGroups->m_uBaseRezNum,(pEntry->m_uFlags>>ENTRYFLAGSREFSHIFT));
				}
#endif
				// Next entry
				++pEntry;
			} while (--uCount);
			pGroups = reinterpret_cast<RezGroup_t *>(pEntry);
		} while (--uGroupCount);
	}
	// Release the resource groups
	Free(m_pGroups);
	// Release the name list
	Free(m_pRezNames);
	m_pGroups = NULL;
	m_pRezNames = NULL;
	m_uRezNameCount = 0;
	m_uGroupCount = 0;
}

/*! ************************************

	\brief Release all cached memory

	Scan all entries in the resource map and dispose
	of all the entries that loaded in the cache but not
	actually discarded.
	I will only dispose of unlocked purgable handles
	This is useful before a level or map load
	to help prevent memory fragmentation in low memory
	situations. Not really needed to call this routine unless
	memory is a a premium.
	
***************************************/

void Burger::RezFile::PurgeCache(void)
{
	// Get the entry count
	Word uGroupCount = m_uGroupCount;
	// Any resources available?
	if (uGroupCount) {
		RezGroup_t *pGroups = m_pGroups;
		do {
			// How many elements?
			Word uCount = pGroups->m_uCount;
			// Index to the data
			RezEntry_t *pEntry = pGroups->m_Array;
			if (uCount) {
				do {
					// Is there a handle here?
					void **ppData = pEntry->m_ppData;
					if (ppData) {
						// Not referenced right now
						if (!(pEntry->m_uFlags&ENTRYFLAGSREFCOUNT)) {
							// Dispose of it.
							m_pMemoryManager->FreeHandle(ppData);
							pEntry->m_ppData = NULL;
						}
					}
					// Next entry
					++pEntry;
				} while (--uCount);
			}
			// Next!
			pGroups = reinterpret_cast<RezGroup_t *>(pEntry);
			// Count down
		} while (--uGroupCount);
	}
}

/*! ************************************

	\brief Enable or disable reading external files
	
	Set the new state of the reading of external file
	and return the current state of the flag
	before the modification

	\param bEnable \ref TRUE to enable file reading, \ref FALSE to disable
	\return Previous flag state
	\sa Word Burger::RezFile::GetExternalFlag(void) const

***************************************/

Word Burger::RezFile::SetExternalFlag(Word bEnable)
{
	Word uOldFlag = m_bExternalFileEnabled;
	m_bExternalFileEnabled = bEnable;
	return uOldFlag;
}

/*! ************************************

	\fn Word Burger::RezFile::GetExternalFlag(void) const
	\brief Return the state of reading external files

	\return Current flag state, \ref TRUE or \ref FALSE
	\sa Word Burger::RezFile::SetExternalFlag(Word)

***************************************/

/*! ************************************

	\brief Log a resource decompressor
	\param uCompressID Compressor ID (1-3)
	\param pProc Pointer to a decompression codec

***************************************/

void Burger::RezFile::LogDecompressor(Word uCompressID,Burger::Decompress *pProc)
{
	// Allowable?
	if (--uCompressID<MAXCODECS) {	
		// Save the function pointer
		m_Decompressors[uCompressID] = pProc;
	}
#if defined(_DEBUG)
	else {
		if (Globals::AreWarningsEnabled()) {
			Debug::Message("RezFile::LogDecompressor() : uCompressID is not 1-3, it's %u\n",uCompressID+1);
		}
	}
#endif
}

/*! ************************************

	\brief Given a resource name, return the resource number
	
	Scan the entries for the filename and
	return the resource entry number

	\param pRezName Pointer to a "C" string with the filename to locate in the rez file
	\return RezFile::INVALIDREZNUM on error or a valid resource number if the resource was found.
	\sa Word Burger::RezFile::GetName(Word,char *,WordPtr) const

***************************************/

Word Burger::RezFile::GetRezNum(const char *pRezName) const
{
	FilenameToRezNum_t *pRezNameEntry;		// Pointer to the master name list
	
	if (FindName(pRezName,&pRezNameEntry)) {
		return pRezNameEntry->m_uRezNum;
	}
	// No good!
	return INVALIDREZNUM;
}

/*! ************************************

	\brief Return the filename associated with a resource number
	\param uRezNum Resource number
	\param pBuffer Pointer to a buffer to receive the filename string
	\param uBufferSize Size, in bytes, of the buffer to receive the filename string

	\return \ref FALSE if no error occurred. Non zero error code if the name wasn't found.
	\sa Word Burger::RezFile::GetRezNum(const char *) const

***************************************/

Word Burger::RezFile::GetName(Word uRezNum,char *pBuffer,WordPtr uBufferSize) const
{
	// Valid output buffer?
	if (pBuffer && uBufferSize) {
		
		// Get the entry count
		Word uGroupCount = m_uGroupCount;
		
		// Any resources available?
		if (uGroupCount) {
			
			// Get the root group pointer
			const RezGroup_t *pGroups = m_pGroups;
			do {
				// Get the offset from base number
				Word uOffset = uRezNum-pGroups->m_uBaseRezNum;

				// In this group?
				if (uOffset<pGroups->m_uCount) {
					// I got the entry
					const RezEntry_t *pEntry = &pGroups->m_Array[uOffset];
					// Do I have a name?
					const char *pText = pEntry->m_pRezName;
					if (pText) {
						// How many bytes to copy?
						StringCopy(pBuffer,uBufferSize,pText);
						// Success!
						return FALSE;
					}
					break;
				}
				// Next?
				pGroups = reinterpret_cast<const RezGroup_t *>(&pGroups->m_Array[pGroups->m_uCount]);
				// Count down
			} while (--uGroupCount);
		}
		// There is no name
		pBuffer[0] = 0;
	}
	// No good
	return TRUE;
}

/*! ************************************

	\brief Add a new filename to be managed by the RezFile
	
	If the named resource doesn't already exist in the 
	resource dictionary, insert it into the first free
	slot.

	There is no check performed to see if the file exists. The name is added to 
	the dictionary.

	\return New or previous resource ID number or RezFile::INVALIDREZNUM if an error of some kind occurs.

***************************************/

Word Burger::RezFile::AddName(const char *pRezName)
{		
	// Is the resource name present?
	Word uRezNum = GetRezNum(pRezName);

	// If it's not present, add it to the dictionary and assign a new number
	
	if (uRezNum!=INVALIDREZNUM) {
		return uRezNum;
	}
	// Ok, It's not present, this is evil
		
	// If the filename has a prefix "20:FileName.txt"
	// Then remove the prefix

	Word uChar = reinterpret_cast<const Word8 *>(pRezName)[0];
	// First char a number?
	if (uChar>='0' && uChar<('9'+1)) {
		// Find the ':'
		const char *pAux = StringCharacter(pRezName,':');
		// Found it?
		if (pAux) {
			pRezName = pAux+1;	// Index past it
		}
	}

	// Ok, RezName has the name FileName.txt
	
	// Length of the new string to add
	WordPtr uNewStringLength = StringLength(pRezName)+1;

	// Get the current group data size in bytes, so I can expand from this
	WordPtr uOldDictionarySize = GetRezGroupBytes();

	// New buffer
	RezGroup_t *pNewGroup = static_cast<RezGroup_t *>(Alloc(uOldDictionarySize + uNewStringLength + sizeof(RezGroup_t)));
	if (!pNewGroup) {
		return INVALIDREZNUM;	// I'm boned
	}

	// If this is this an empty resource file?
	// Then it's pretty easy!
		
	Word uGroupCount = m_uGroupCount;
	if (!uGroupCount) {
		// Create a simple single resource group
		char *pText = reinterpret_cast<char *>(&pNewGroup->m_Array[1]);
		m_uGroupCount = 1;				// I have a group pointer!
		m_uRezNameCount = 1;
		m_pGroups = pNewGroup;
		pNewGroup->m_uCount = 1;		// 1 entry
		pNewGroup->m_uBaseRezNum = 1;	// Resource number 1
		pNewGroup->m_Array[0].m_ppData = NULL;
		pNewGroup->m_Array[0].m_pRezName = pText;
		pNewGroup->m_Array[0].m_uFileOffset = 0;
		pNewGroup->m_Array[0].m_uLength = 0;
		pNewGroup->m_Array[0].m_uFlags = 0;
		pNewGroup->m_Array[0].m_uCompressedLength = 0;
		MemoryCopy(pText,pRezName,uNewStringLength);
		ProcessRezNames();
		return 1;			// Resource 1 allocated
	}


	// How to expand? Can I insert from the start?
		
	RezGroup_t *pGroup = m_pGroups;		// Get the main pointer
	m_pGroups = pNewGroup;
	RezGroup_t *pGroupToDelete = pGroup;	// Save for later
	uRezNum = pGroup->m_uBaseRezNum;	// Get the base number
	Word uCount = pGroup->m_uCount;

	// Is the new number at the beginning or the end?

	pNewGroup->m_uCount = uCount+1;
	RezEntry_t *pNewEntry;
	if (uRezNum>=2) {
		--uRezNum;		// Move back one.
		pNewGroup->m_uBaseRezNum = uRezNum;
		pNewEntry = pNewGroup->m_Array;
		// The data is AFTER the new entry
		MemoryCopy(pNewEntry+1,pGroup->m_Array,sizeof(RezEntry_t)*uCount);
	} else {
		pNewGroup->m_uBaseRezNum = uRezNum;
		uRezNum += uCount;	// Append
		// The data is BEFORE the new entry
		MemoryCopy(pNewGroup->m_Array,pGroup->m_Array,sizeof(RezEntry_t)*uCount);
		pNewEntry = &pNewGroup->m_Array[uCount];
	}

	// Create the new entry
	pNewEntry->m_ppData = NULL;
	pNewEntry->m_pRezName = pRezName;		// Will be corrected with FixupFilenames()
	pNewEntry->m_uFileOffset = 0;
	pNewEntry->m_uLength = 0;
	pNewEntry->m_uFlags = 0;
	pNewEntry->m_uCompressedLength = 0;

	char *pText;

	// Are there other records to process?

	if (--uGroupCount) {
		// Where to continue storing data
		pNewEntry = &pNewGroup->m_Array[uCount+1];
		// Get the next source record
		RezGroup_t *pNextGroup = reinterpret_cast<RezGroup_t *>(&pGroup->m_Array[uCount]);

		// Am I going to merge two groups?
		if (pNextGroup->m_uBaseRezNum==(uRezNum+1)) {
			uCount = pNextGroup->m_uCount;
			MemoryCopy(pNewEntry,pNextGroup->m_Array,sizeof(RezEntry_t)*uCount);
			pNewGroup->m_uCount += uCount;		// Adjust the count to do the merge
			pNewGroup = reinterpret_cast<RezGroup_t *>(pNewEntry+uCount);
			pGroup = reinterpret_cast<RezGroup_t *>(&pNextGroup->m_Array[uCount]);
			m_uGroupCount = uGroupCount;
			--uGroupCount;
		} else {
			pNewGroup = reinterpret_cast<RezGroup_t *>(pNewEntry);
			pGroup = pNextGroup;
		}

		// Copy up the rest of the records
		if (uGroupCount) {
			do {
				uCount = pGroup->m_uCount;
				MemoryCopy(pNewGroup,pGroup,sizeof(RezGroup_t)+(sizeof(RezEntry_t)*(uCount-1)));
				pNewGroup = reinterpret_cast<RezGroup_t *>(&pNewGroup->m_Array[uCount]);
				pGroup = reinterpret_cast<RezGroup_t *>(&pGroup->m_Array[uCount]);
			} while (--uGroupCount);
		}
		pText = reinterpret_cast<char *>(pNewGroup);
	} else {
		// Since there is only one entry, append text immediately after the group plus the new entry
		pText = reinterpret_cast<char *>(&pNewGroup->m_Array[uCount+1]);
	}
	FixupFilenames(pText);
	Free(pGroupToDelete);
	return uRezNum;
}

/*! ************************************

	\brief Remove a resource from the rez file

	Look for the entry and if found, it will be removed from
	the resource dictionary.
	\note The resource file is not altered in any
	way, only the ability to access this resource is removed

	\param uRezNum Number of the resource to remove

	\sa Burger::RezFile::AddName(const char *) or Burger::RezFile::Remove(const char *)

***************************************/

void Burger::RezFile::Remove(Word uRezNum)
{
	// It there a resource by this number?
	
	RezEntry_t *pEntry = Find(uRezNum);
	if (!pEntry) {
		return;			// Get out now...
	}

	// Make sure the data is gone
	
	pEntry->m_pRezName = NULL;			// Garbage collect the filename
	void **ppData = pEntry->m_ppData;
	if (ppData) {		// Was there memory?
		pEntry->m_ppData = NULL;		// Mark as GONE
		Word32 uOffset = pEntry->m_uFlags;
		pEntry->m_uFlags = uOffset&(~ENTRYFLAGSREFCOUNT);	/* No references */
#if defined(_DEBUG)
		// A single reference is not an error. More than 1 is a problem
		if (Globals::AreWarningsEnabled() && (uOffset&ENTRYFLAGSREFCOUNT)>=(ENTRYFLAGSREFADD*2)) {
			Debug::Message("RezFile::Remove() : Removing resource %u that is referenced %lu times\n",uRezNum,uOffset>>ENTRYFLAGSREFSHIFT);
		}
#endif
		m_pMemoryManager->FreeHandle(ppData);
	}

	RezGroup_t *pGroupToDelete = m_pGroups;
	Word uGroupCount = m_uGroupCount;
	// If there is only one entry, surrender
	if (uGroupCount==1 && pGroupToDelete->m_uCount==1) {
		Free(pGroupToDelete);
		Free(m_pRezNames);
		m_pGroups = NULL;
		m_pRezNames = NULL;
		m_uRezNameCount = 0;
		m_uGroupCount = 0;
		return;
	}

	// Note: This scan will remove the deleted string from the size, but retain the size for
	// the deleted record, I need the padding just in case I have a group split.

	RezGroup_t *pNewGroup = static_cast<RezGroup_t *>(Alloc(GetRezGroupBytes()));
	if (!pNewGroup) {
		return;
	}
	m_pGroups = pNewGroup;

	// I can assume there is a count or Find() would have failed earlier

	// Copy all the records to the new group array EXCEPT the
	// resource I'm disposing off. If the deletion is in the middle of
	// a group, I'll split it into two groups

	RezGroup_t *pGroup = pGroupToDelete;
	do {
		Word uTempRezNum = pGroup->m_uBaseRezNum;
		Word uCount = pGroup->m_uCount;
		pNewGroup->m_uBaseRezNum = uTempRezNum;
		pNewGroup->m_uCount = uCount;
		pEntry = pGroup->m_Array;
		RezEntry_t *pNewEntry = pNewGroup->m_Array;
		do {
			if (uTempRezNum!=uRezNum) {
				pNewEntry[0] = pEntry[0];
				++pNewEntry;
			} else {
				// First one?
				if (pGroup->m_uBaseRezNum==uTempRezNum) {
					pNewGroup->m_uBaseRezNum=uTempRezNum+1;
					// Was this the last record?
					if (!--pNewGroup->m_uCount) {
						// Dispose of this single entry group
						pNewEntry = reinterpret_cast<RezEntry_t *>(pNewGroup);
						--m_uGroupCount;
					}
					// Last one?
				} else if (uCount==1) {
					pNewGroup->m_uCount--;
				// I'm going to split this one
				} else {
					// End the previous group here
					pNewGroup->m_uCount = pGroup->m_uCount-uCount;
					// New group starts here
					m_uGroupCount++;
					pNewGroup = reinterpret_cast<RezGroup_t *>(pNewEntry);
					uTempRezNum = uRezNum;
					// Create the new group header
					pNewGroup->m_uBaseRezNum = uTempRezNum+1;
					pNewGroup->m_uCount = uCount-1;
					pNewEntry = pNewGroup->m_Array;
				}
			}
			++pEntry;
			++uTempRezNum;
		} while (--uCount);
		pGroup = reinterpret_cast<RezGroup_t *>(pEntry);
		pNewGroup = reinterpret_cast<RezGroup_t *>(pNewEntry);
	} while (--uGroupCount);

	// Copy all the filename strings at the end of the buffer

	FixupFilenames(reinterpret_cast<char *>(pNewGroup));

	// Release the old buffer now (It had the strings)

	Free(pGroupToDelete);
}

/*! ************************************

	\brief Remove a resource from the rez file
	by name

	\param pRezName Pointer to a "C" string of the name of the resource to remove
	\sa void Burger::RezFile::Remove(Word)

***************************************/

void Burger::RezFile::Remove(const char *pRezName)
{
	// Can I find this?
	Word uRezNum = GetRezNum(pRezName);
	if (uRezNum != INVALIDREZNUM) {
		// Remove it!
		Remove(uRezNum);
	}
}

/*! ************************************

	\fn const Burger::RezFile::FilenameToRezNum_t *Burger::RezFile::GetNameArray(void) const
	\brief Get the pointer to the list of filenames managed by
	the RezFile. 

	\note This pointer is valid as long as no filenames are added or removed.
	\sa Burger::RezFile::GetNameArraySize(void) const

***************************************/

/*! ************************************

	\fn Word Burger::RezFile::GetNameArraySize(void) const
	\brief Return the number of elements returned by Burger::RezFile::GetNameArray(void) const
	\sa Burger::RezFile::GetNameArray(void) const

***************************************/

/*! ************************************

	\brief Return the lowest valid resource number

	\return Lowest resource number or RezFile::INVALIDREZNUM on error (Or no resources present)
	\sa Burger::RezFile::GetHighestRezNum(void) const

***************************************/

Word Burger::RezFile::GetLowestRezNum(void) const
{
	const RezGroup_t *pGroups = m_pGroups;
	if (pGroups) {
		return pGroups->m_uBaseRezNum;
	}
	return INVALIDREZNUM;
}

/*! ************************************

	\brief Return the highest valid resource number

	\return Highest resource number or RezFile::INVALIDREZNUM on error (Or no resources present)
	\sa Burger::RezFile::GetLowestRezNum(void) const

***************************************/

Word Burger::RezFile::GetHighestRezNum(void) const
{
	// Get the number of resource groups
	Word uGroupCount = m_uGroupCount;
	// Any resource groups?
	if (uGroupCount) {
		// Get the pointer to the array of data
		const RezGroup_t *pGroups = m_pGroups;
		if (--uGroupCount) {
			do {
				// Pointer to the next group
				pGroups = reinterpret_cast<const RezGroup_t *>(&pGroups->m_Array[pGroups->m_uCount]);
				// Loop for all groups
			} while (--uGroupCount);
		}
		return (pGroups->m_uCount+pGroups->m_uBaseRezNum)-1;
	}
	return INVALIDREZNUM;
}

/*! ************************************

	\brief Find a resource by a data handle

	Given a handle, scan the resource map to see if it is under
	Resource manager control. If so, return the resource ID number
	and the resource name. Return error code if not present (Right now
	it's \ref TRUE), otherwise return zero for no error if I found it.

	\param ppRez Handle to the memory to check for
	\param pRezNum Pointer to a \ref Word to receive the valid resource number
	\param pBuffer Pointer to a buffer to hold the name of the resource
	\param uBufferSize Number of bytes in size for the resource name buffer
	\return \ref FALSE if found, \ref TRUE if not found

***************************************/

Word Burger::RezFile::GetIDFromHandle(const void **ppRez,Word *pRezNum,char *pBuffer,WordPtr uBufferSize) const
{
	// Valid handle?
	if (ppRez) {
		Word uGroupCount = m_uGroupCount;	// Get the entry count
		if (uGroupCount) {					// Any resources available?			
			RezGroup_t *pGroups = m_pGroups;	// Pointer to the data
			do {
				Word uCount = pGroups->m_uCount;	// Count of this group's size
				RezEntry_t *pEntry = pGroups->m_Array;	// Pointer to the first entry
		
				// Check the entries for this handle
				
				do {
					if (const_cast<const void **>(pEntry->m_ppData) == ppRez) {		// Is it a match?
						if (pRezNum) {						// Do I want the ID number?
							pRezNum[0] = (pGroups->m_uBaseRezNum+pGroups->m_uCount)-uCount;
						}
						if (pBuffer && uBufferSize) {
							if (pEntry->m_pRezName) {
								StringCopy(pBuffer,uBufferSize,pEntry->m_pRezName);
							} else {
								pBuffer[0] = 0;
							}
						}
						return FALSE;				// I found it
					}
					++pEntry;						// Next entry
				} while (--uCount);
				pGroups = reinterpret_cast<RezGroup_t *>(pEntry);	// Next group
			} while (--uGroupCount);	// Count down
		}
	}
	return TRUE;		// I didn't find it.
}

/*! ************************************

	\brief Find a resource by a data pointer

	Given a pointer, scan the resource map to see if it is under
	Resource manager control. If so, return the resource ID number
	and the resource name. Return error code if not present (Right now
	it's \ref TRUE), otherwise return zero for no error if I found it.

	\param pRez Pointer to the memory to check for
	\param pRezNum Pointer to a \ref Word to receive the valid resource number
	\param pBuffer Pointer to a buffer to hold the name of the resource
	\param uBufferSize Number of bytes in size for the resource name buffer
	\return \ref FALSE if found, \ref TRUE if not found

***************************************/

Word Burger::RezFile::GetIDFromPointer(const void *pRez,Word *pRezNum,char *pBuffer,WordPtr uBufferSize) const
{
	// Valid pointer?
	if (pRez) {
		Word uGroupCount = m_uGroupCount;	// Get the entry count
		if (uGroupCount) {					// Any resources available?			
			RezGroup_t *pGroups = m_pGroups;	// Pointer to the data
			do {
				Word uCount = pGroups->m_uCount;	// Count of this group's size
				RezEntry_t *pEntry = pGroups->m_Array;	// Pointer to the first entry
		
				// Check the entries for this handle
				
				do {
					if (pEntry->m_ppData && (pEntry->m_ppData[0] == pRez)) {		// Is it a match?
						if (pRezNum) {						// Do I want the ID number?
							pRezNum[0] = (pGroups->m_uBaseRezNum+pGroups->m_uCount)-uCount;
						}
						if (pBuffer && uBufferSize) {
							if (pEntry->m_pRezName) {
								StringCopy(pBuffer,uBufferSize,pEntry->m_pRezName);
							} else {
								pBuffer[0] = 0;
							}
						}
						return FALSE;				// I found it
					}
					++pEntry;						// Next entry
				} while (--uCount);
				pGroups = reinterpret_cast<RezGroup_t *>(pEntry);	// Next group
			} while (--uGroupCount);	// Count down
		}
	}
	return TRUE;		// I didn't find it.
}

/*! ************************************

	\brief Load in a resource and return the handle.
	
	\note The returned handle is NOT locked.

	\param uRezNum Resource number
	\param pLoadedFlag Pointer to a \ref Word that's
		set to \ref TRUE if the data was freshly loaded
		or \ref FALSE if it's a cached copy
	\return \ref NULL if the data was not found or a valid handle with the data

***************************************/

void ** Burger::RezFile::LoadHandle(Word uRezNum,Word *pLoadedFlag)
{		
	if (pLoadedFlag) {
		pLoadedFlag[0] = FALSE;		// Assume cached or failed
	}
	RezEntry_t *pEntry = Find(uRezNum);		// Find the resource
	if (!pEntry) {			// Find the entry
		return NULL;		// The resource does not exist!
	}

	void **ppData = pEntry->m_ppData;	// Get the current handle
	if (ppData) {						// Valid handle?
		if (ppData[0]) {				// Handle not purged? */
			// Increment the reference count
			pEntry->m_uFlags=pEntry->m_uFlags+ENTRYFLAGSREFADD;
			m_pMemoryManager->SetPurgeFlag(ppData,FALSE);	// Can't purge
			return ppData;				// Return the handle
		}
		pEntry->m_ppData = NULL;	// Mark as gone!
		m_pMemoryManager->FreeHandle(ppData);		// Release the memory
		ppData = NULL;				// Set to zero for future error checking
	}

	Word32 uFileNameOffset = pEntry->m_uFlags;			// Filename offset
 	pEntry->m_uFlags=uFileNameOffset+ENTRYFLAGSREFADD;	// Increase the reference count
	Word uHandleFlags = 0;								// ID the handle as the resource manager
	if (uFileNameOffset&ENTRYFLAGSHIGHMEMORY) {			// Where to load?
		uHandleFlags = MemoryManagerHandle::FIXED;		// Place in fixed memory
	}

	// See if I can load in the file from disk

	if (pEntry->m_pRezName && m_bExternalFileEnabled) {
		// Did I already try to load as a file?
		
		if (!(uFileNameOffset&ENTRYFLAGSTESTED) || (uFileNameOffset&ENTRYFLAGSFILEFOUND)) {
			pEntry->m_uFlags |= ENTRYFLAGSTESTED;	// Mark as tested
			const char *pFileName = pEntry->m_pRezName;
			File TheFile;
			if (TheFile.Open(pFileName,File::READONLY)==File::OKAY) {
				// File detected
				pEntry->m_uFlags |= ENTRYFLAGSFILEFOUND;
				WordPtr uNewLength = TheFile.GetSize();				// Get the NEW length
				if (uNewLength) {
					ppData = m_pMemoryManager->AllocHandle(uNewLength,uHandleFlags);	// Get memory
					if (ppData) {										// Got the memory?
						m_pMemoryManager->SetID(ppData,uRezNum);		// Set the ID to the handle
						if (TheFile.Read(m_pMemoryManager->Lock(ppData),uNewLength)!=uNewLength) {
							m_pMemoryManager->FreeHandle(ppData);		// Discard the memory
							ppData = NULL;				// Can't load it in!
						} else {
							pEntry->m_uLength = static_cast<Word32>(uNewLength); // Save the size
						}
					}
				}
				TheFile.Close();		// Close the file
			}
			if (ppData) {
				m_pMemoryManager->SetPurgeFlag(ppData,FALSE);
				m_pMemoryManager->Unlock(ppData);
				if (pLoadedFlag) {
					pLoadedFlag[0] = TRUE;		// Data is new
				}
				pEntry->m_ppData = ppData;		// Save the handle
#if defined(_DEBUG)
				if (Globals::GetTraceFlag()&Globals::TRACE_REZLOAD) {	// Should I print it?
					Debug::Message("Loaded resource %u from external file %s\n",uRezNum,pFileName);
				}
#endif
				return ppData;		// Return the handle
			}
		}
	}

	// Let's load it in from the .REZ file
	Word32 uFileOffset = pEntry->m_uFileOffset;			
	if (!m_File.IsOpened() || !uFileOffset) {						// No resource file found?
		pEntry->m_uFlags &= (~ENTRYFLAGSREFCOUNT);	// Kill the ref count
		return NULL;
	}
	m_File.SetMark(uFileOffset);						// Seek into the file

	Word32 DataLength = pEntry->m_uLength;				// Preload the length
	Word32 PackedLength = pEntry->m_uCompressedLength;
	if (uFileNameOffset&ENTRYFLAGSDECOMPMASK) {			// Is this compressed?
		// Get the compressor
		Decompress *pDecompressor = m_Decompressors[((uFileNameOffset>>ENTRYFLAGSDECOMPSHIFT)&3)-1];
		if (!pDecompressor) {		// Is there a compressor logged?
			pEntry->m_uFlags &= (~ENTRYFLAGSREFCOUNT);	// Kill the ref count
			return NULL;
		}

		// The old format had the data length in the compressed data
		if (!DataLength) {
			DataLength = m_File.ReadLittleWord32();		// Get the length
			pEntry->m_uLength = DataLength;
			pEntry->m_uFileOffset += 4;
			pEntry->m_uCompressedLength -= 4;
		}

		ppData = m_pMemoryManager->AllocHandle(DataLength,uHandleFlags);	// Get dest buffer
		if (!ppData) {										// No memory for uncompressed data?
			pEntry->m_uFlags &= (~ENTRYFLAGSREFCOUNT);		// Kill the ref count
			return NULL;
		}
		m_pMemoryManager->SetID(ppData,uRezNum);		// Set the ID to the handle
		Word32 BufferSize = (PackedLength<MAXBUFFER) ? PackedLength : MAXBUFFER;
		Word8 *PackedPtr = static_cast<Word8 *>(Alloc(BufferSize));	// Get Buffer
		if (!PackedPtr) {				// No compressed data buffer?
			m_pMemoryManager->FreeHandle(ppData);
			pEntry->m_uFlags &= (~ENTRYFLAGSREFCOUNT);		// Kill the ref count
			return NULL;
		}
		pDecompressor->Reset();
		WordPtr PackedHeader = DataLength;
		WordPtr PackedSize = PackedLength;
		Word8 *pOutput = (Word8 *)m_pMemoryManager->Lock(ppData);
		do {		/* Loop for decompression */
			WordPtr ChunkSize = (BufferSize<PackedSize) ? BufferSize : PackedSize;
			if (m_File.Read(PackedPtr,ChunkSize)!=ChunkSize) {
				m_pMemoryManager->FreeHandle(ppData);
				pEntry->m_uFlags &= (~ENTRYFLAGSREFCOUNT);		// Kill the ref count
				return NULL;
			}
			if (pDecompressor->Process(pOutput,PackedHeader,PackedPtr,ChunkSize)==Decompress::DECOMPRESS_BADINPUT) {
				m_pMemoryManager->FreeHandle(ppData);
				pEntry->m_uFlags &= (~ENTRYFLAGSREFCOUNT);		// Kill the ref count
				return NULL;
			}
			PackedHeader -= pDecompressor->GetProcessedOutputSize();
			pOutput += pDecompressor->GetProcessedOutputSize();
			PackedSize -= ChunkSize;
		} while (PackedSize);
		pDecompressor->Reset();		// Force a shutdown
		Free(PackedPtr);	/* Release the temp buffer */
		m_pMemoryManager->Unlock(ppData);
		if (pLoadedFlag) {
			pLoadedFlag[0] = TRUE;		/* Data is new */
		}
		pEntry->m_ppData = ppData;		/* Save the handle */
#if defined(_DEBUG)
		if (Globals::GetTraceFlag()&Globals::TRACE_REZLOAD) {	/* Should I print it? */
			if (pEntry->m_pRezName) {
				Debug::Message("Loaded compressed resource %u named %s\n",uRezNum,pEntry->m_pRezName);
			} else {
				Debug::Message("Loaded compressed resource %u\n",uRezNum);
			}
		}
#endif
		return ppData;		// Return the handle
	}

	// Uncompressed data

	ppData = m_pMemoryManager->AllocHandle(DataLength,uHandleFlags);	// Get the memory
	if (ppData) {		/* Memory ok? */
		m_pMemoryManager->SetID(ppData,uRezNum);		// Set the ID to the handle
		if (m_File.Read(m_pMemoryManager->Lock(ppData),DataLength)==DataLength) {	/* Read it in */
			m_pMemoryManager->Unlock(ppData);
			if (pLoadedFlag) {
				pLoadedFlag[0] = TRUE;		/* Data is new */
			}
			pEntry->m_ppData = ppData;		/* Save the handle */
#if defined(_DEBUG)
			if (Globals::GetTraceFlag()&Globals::TRACE_REZLOAD) {	/* Should I print it? */
				if (pEntry->m_pRezName) {
					Debug::Message("Loaded resource %u named %s\n",uRezNum,pEntry->m_pRezName);
				} else {
					Debug::Message("Loaded resource %u\n",uRezNum);
				}
			}
#endif
			// Return the handle
			return ppData;
		}
		// Delete on file load error
		m_pMemoryManager->FreeHandle(ppData);	
	}
	// Kill the reference count
	pEntry->m_uFlags &= (~ENTRYFLAGSREFCOUNT);
	pEntry->m_ppData = NULL;
	// Can't load in the data
	return NULL;
}

/*! ************************************

	\brief Load in a resource and return the handle.
	
	\note The returned handle is NOT locked.

	\param pRezName Resource name
	\param pLoadedFlag Pointer to a \ref Word that's
		set to \ref TRUE if the data was freshly loaded
		or \ref FALSE if it's a cached copy
	\return \ref NULL if the data was not found or a valid handle with the data

***************************************/

void ** Burger::RezFile::LoadHandle(const char *pRezName,Word *pLoadedFlag)
{
	// Get the index number
	Word uRezNum = GetRezNum(pRezName);
	if (uRezNum == INVALIDREZNUM) {
		uRezNum = AddName(pRezName);	// Try to add it
		if (uRezNum==INVALIDREZNUM) {	// No good?
			return NULL;				// Bad news
		}
	}
	return LoadHandle(uRezNum,pLoadedFlag);			// Load the file
}

/*! ************************************

	\brief Load in a resource
	\param uRezNum Resource number
	\param pLoadedFlag Pointer to a \ref Word that's
		set to \ref TRUE if the data was freshly loaded
		or \ref FALSE if it's a cached copy
	\return \ref NULL if the data was not found or a valid handle with the data

***************************************/

void * Burger::RezFile::Load(Word uRezNum,Word *pLoadedFlag)
{
	// Load the handle and dereference it
	return m_pMemoryManager->Lock(LoadHandle(uRezNum,pLoadedFlag));
}

/*! ************************************

	\brief Load in a resource
	\param pRezName Resource name
	\param pLoadedFlag Pointer to a \ref Word that's
		set to \ref TRUE if the data was freshly loaded
		or \ref FALSE if it's a cached copy
	\return \ref NULL if the data was not found or a valid handle with the data

***************************************/

void * Burger::RezFile::Load(const char *pRezName,Word *pLoadedFlag)
{
	// Load the handle and dereference it
	return m_pMemoryManager->Lock(LoadHandle(pRezName,pLoadedFlag));
}

/*! ************************************

	\brief Load in a resource into a static buffer
	
	\param uRezNum Resource number
	\param pBuffer Pointer to the buffer to copy the data to
	\param uBufferSize Number of bytes for the buffer to store the data into
	\return \ref FALSE if no error or non-zero if it failed to load

***************************************/

Word Burger::RezFile::Read(Word uRezNum,void *pBuffer,WordPtr uBufferSize)
{	
	// Assume failure
	Word uResult = TRUE;
	void **ppData = LoadHandle(uRezNum);	// Load it in
	if (ppData) {							// Ok?
		RezEntry_t *pEntry = Find(uRezNum);
		WordPtr uLength = pEntry->m_uLength; // How much memory does this take?
		if (uBufferSize>=uLength) {			// Is the output buffer big enough?
			uBufferSize = uLength;			// Use the smaller
			uResult = FALSE;				// It's ok
		}
		MemoryCopy(pBuffer,ppData[0],uBufferSize);	// Copy the data
		Release(uRezNum);	// Release the data
	}
	// Return FALSE if OK
	return uResult;
}

/*! ************************************

	\brief Load in a resource into a static buffer
	
	\param pRezName Resource name
	\param pBuffer Pointer to the buffer to copy the data to
	\param uBufferSize Number of bytes for the buffer to store the data into

	\return \ref FALSE if no error or non-zero if it failed to load

***************************************/

Word Burger::RezFile::Read(const char *pRezName,void *pBuffer,WordPtr uBufferSize)
{
	// Get the index number
	Word uRezNum = GetRezNum(pRezName);
	if (uRezNum == INVALIDREZNUM) {
		uRezNum = AddName(pRezName);	// Try to add it
		if (uRezNum==INVALIDREZNUM) {	// No good?
			return TRUE;				// Bad news
		}
	}
	return Read(uRezNum,pBuffer,uBufferSize);			// Load the file
}

/*! ************************************

	\brief Reduce the data reference count

	Release a resource by marking it purgeable
	and leave it in the cache.

	\param uRezNum Resource number

***************************************/

void Burger::RezFile::Release(Word uRezNum)
{
	RezEntry_t *pEntry = Find(uRezNum);	/* Scan for the resource */
	if (pEntry) {					/* Is it here? */
		Word32 uOffset = pEntry->m_uFlags;
#if defined(_DEBUG)
		if (Globals::AreWarningsEnabled() && !(uOffset&ENTRYFLAGSREFCOUNT)) {		/* Should not be zero */
			Debug::Message("RezFile::Release() : RefCount is zero for resource %u!\n",uRezNum);
		}
#endif
		if (uOffset>=ENTRYFLAGSREFADD) {		// Not referenced?
			uOffset-=ENTRYFLAGSREFADD;			// Release a reference
			pEntry->m_uFlags = uOffset;
			if (!(uOffset&ENTRYFLAGSREFCOUNT)) {		// No longer referenced?
				void **ppData = pEntry->m_ppData;
				if (ppData) {							// Is there a handle?
					m_pMemoryManager->Unlock(ppData);				// Unlock it
					m_pMemoryManager->SetPurgeFlag(ppData,TRUE);	// Mark as purgable
				}
			}
		}
	}
}

/*! ************************************

	\brief Reduce the data reference count

	Release a resource by marking it purgeable
	and leave it in the cache.

	\param pRezName Resource name

***************************************/

void Burger::RezFile::Release(const char *pRezName)
{
	Word uRezNum = GetRezNum(pRezName);	/* Scan for the resource */
	if (uRezNum!=INVALIDREZNUM) {
		RezEntry_t *pEntry = Find(uRezNum);
		if (pEntry) {					/* Is it here? */
			Word32 uOffset = pEntry->m_uFlags;
#if defined(_DEBUG)
			if (Globals::AreWarningsEnabled() && !(uOffset&ENTRYFLAGSREFCOUNT)) {		/* Should not be zero */
				Debug::Message("RezFile::Release() : RefCount is zero for resource %s!\n",pRezName);
			}
#endif
			if (uOffset>=ENTRYFLAGSREFADD) {			/* Not referenced? */
				uOffset-=ENTRYFLAGSREFADD;			/* Release a reference */
				pEntry->m_uFlags = uOffset;
				if (!(uOffset&ENTRYFLAGSREFCOUNT)) {		/* No longer referenced? */
					void **ppData = pEntry->m_ppData;
					if (ppData) {			/* Is there a handle? */
						m_pMemoryManager->Unlock(ppData);		/* Unlock it */
						m_pMemoryManager->SetPurgeFlag(ppData,TRUE);	/* Mark as purgable */
					}
				}
			}
		}
	}
}

/*! ************************************

	\brief Destroy the data associated with a resource

	Release all memory associated with this specific resource

	\param uRezNum Resource number

***************************************/

void Burger::RezFile::Kill(Word uRezNum)
{
	RezEntry_t *pEntry = Find(uRezNum);	/* Scan for the resource */
	if (pEntry) {
		void **ppData = pEntry->m_ppData;
		if (ppData) {		/* Is there a handle? */
			pEntry->m_ppData = NULL;		/* Mark as GONE */
			Word32 uOffset = pEntry->m_uFlags;
			pEntry->m_uFlags = uOffset&(~ENTRYFLAGSREFCOUNT);	/* No references */
#if defined(_DEBUG)
			if (Globals::AreWarningsEnabled() && ((uOffset&ENTRYFLAGSREFCOUNT)>=ENTRYFLAGSREFADD*2)) {
				Debug::Message("RezFile::Kill() : Killing resource %u that is referenced %lu times\n",uRezNum,uOffset>>ENTRYFLAGSREFSHIFT);
			}
#endif
			m_pMemoryManager->FreeHandle(ppData);
		}
	}
}

/*! ************************************

	\brief Destroy the data associated with a resource

	Release all memory associated with this specific resource

	\param pRezName Resource name

***************************************/

void Burger::RezFile::Kill(const char *pRezName)
{
	Word uRezNum = GetRezNum(pRezName);
	if (uRezNum!=INVALIDREZNUM) {
		RezEntry_t *pEntry = Find(uRezNum);	/* Scan for the resource */
		if (pEntry) {
			void **ppData = pEntry->m_ppData;
			if (ppData) {		/* Is there a handle? */
				pEntry->m_ppData = NULL;		/* Mark as GONE */
				Word32 uOffset = pEntry->m_uFlags;
				pEntry->m_uFlags = uOffset & (~ENTRYFLAGSREFCOUNT);	/* No references */
#if defined(_DEBUG)
				if (Globals::AreWarningsEnabled() && (uOffset&ENTRYFLAGSREFCOUNT)>=(ENTRYFLAGSREFADD*2)) {
					Debug::Message("RezFile::Kill() : Killing resource %s that is referenced %lu times\n",pRezName,uOffset>>ENTRYFLAGSREFSHIFT);
				}
#endif
				m_pMemoryManager->FreeHandle(ppData);
			}
		}
	}
}

/*! ************************************

	\brief Detact the data associated with a resource

	Release responsibilty for this resource to the application.

	\param uRezNum Resource number

***************************************/

void Burger::RezFile::Detach(Word uRezNum)
{
	RezEntry_t *pEntry = Find(uRezNum);
	if (pEntry) {		/* Scan for the resource */
		pEntry->m_ppData = NULL;		/* Mark as GONE */
		Word32 uOffset = pEntry->m_uFlags;
		pEntry->m_uFlags = uOffset & (~ENTRYFLAGSREFCOUNT);	/* No references */
#if defined(_DEBUG)
		if (Globals::AreWarningsEnabled() && (uOffset&ENTRYFLAGSREFCOUNT)!=ENTRYFLAGSREFADD) {	/* 1 time is ok */
			Debug::Message("RezFile::Detach() : Detaching resource %u that is referenced %lu times\n",uRezNum,uOffset>>ENTRYFLAGSREFSHIFT);
		}
#endif
	}
}

/*! ************************************

	\brief Detact the data associated with a resource

	Release responsibilty for this resource to the application.

	\param pRezName Resource name

***************************************/

void Burger::RezFile::Detach(const char *pRezName)
{
	Word uRezNum = GetRezNum(pRezName);
	if (uRezNum!=INVALIDREZNUM) {
		RezEntry_t *pEntry = Find(uRezNum);
		if (pEntry) {		/* Scan for the resource */
			pEntry->m_ppData = NULL;		/* Mark as GONE */
			Word32 uOffset = pEntry->m_uFlags;
			pEntry->m_uFlags = uOffset & (~ENTRYFLAGSREFCOUNT);	/* No references */
#if defined(_DEBUG)
			if (Globals::AreWarningsEnabled() && (uOffset&ENTRYFLAGSREFCOUNT)!=ENTRYFLAGSREFADD) {	/* 1 time is ok */
				Debug::Message("ResourceDetachByName() : Detaching resource %s that is referenced %lu times\n",pRezName,uOffset>>ENTRYFLAGSREFSHIFT);
			}
#endif
		}
	}
}

/*! ************************************

	\brief Preload in a resource

	Load in the resource data and then immediate release it
	and keep it in the cache

	\param uRezNum Resource number

***************************************/

void Burger::RezFile::Preload(Word uRezNum)
{
	if (LoadHandle(uRezNum)) {	// Get the handle
		Release(uRezNum);		// Release the resource
	}
}

/*! ************************************

	\brief Preload in a resource

	Load in the resource data and then immediate release it
	and keep it in the cache

	If the resource name is not in the dictionary, add it.

	\param pRezName Resource name

***************************************/

void Burger::RezFile::Preload(const char *pRezName)
{
	Word uRezNum = GetRezNum(pRezName);	// Get the index number
	if (uRezNum == INVALIDREZNUM) {
		uRezNum = AddName(pRezName);	// Try to add it
		if (uRezNum==INVALIDREZNUM) {		// No good?
			return;						// Bad news!
		}
	}
	if (LoadHandle(uRezNum)) {	// Get the handle
		Release(uRezNum);		// Release it
	}
}

