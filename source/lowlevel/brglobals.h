/***************************************

	Global variable manager

	Copyright 1995-2014 by Rebecca Ann Heineman becky@burgerbecky.com

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#ifndef __BRGLOBALS_H__
#define __BRGLOBALS_H__

#ifndef __BRTYPES_H__
#include "brtypes.h"
#endif

#ifndef __BRDEBUG_H__
#include "brdebug.h"
#endif

#ifndef __BRGUID_H__
#include "brguid.h"
#endif

#if defined(BURGER_WINDOWS) && !defined(__BRWINDOWSTYPES_H__)
#include "brwindowstypes.h"
#endif

#if defined(BURGER_MACOSX) && !defined(__BRMACOSXTYPES_H__)
#include "brmacosxtypes.h"
#endif

/* BEGIN */
namespace Burger {
class OutputMemoryStream;
class String;
struct Globals {
#if defined(BURGER_WINDOWS) || defined(DOXYGEN)
	enum eWindowsDLLIndex {
		DDRAW_DLL,		///< Index for ddraw.dll
		DINPUT_DLL,		///< Index for dinput.dll
		DINPUT8_DLL,	///< Index for dinput8.dll
		D3D9_DLL,		///< Index for d3d9.dll
		DSOUND_DLL,		///< Index for dsound.dll
		RPCRT4_DLL,		///< Index for rpcrt4.dll
		WINMM_DLL,		///< Index for winmm.dll
		SHLWAPI_DLL,	///< Index for shlwapi.dll
		VERSION_DLL,	///< Index for version.dll
		HID_DLL,		///< Index for hid.dll
		SETUPAPI_DLL,	///< Index for setupapi.dll
		DLL_COUNT		///< Total number of DLLs to be managed
	};
	enum eWindowsCallIndex {
		CALL_DirectInput8Create,
		CALL_DirectInputCreateW,
		CALL_DirectDrawCreate,
		CALL_DirectDrawCreateEx,
		CALL_DirectDrawCreateClipper,
		CALL_DirectDrawEnumerateA,
		CALL_DirectDrawEnumerateW,
		CALL_DirectDrawEnumerateExA,
		CALL_DirectDrawEnumerateExW,
		CALL_Direct3DCreate9,
		CALL_DirectSoundCreate,
		CALL_DirectSoundCreate8,
		CALL_UuidCreateSequential,
		CALL_timeGetTime,
		CALL_PathSearchAndQualifyA,
		CALL_PathSearchAndQualifyW,
		CALL_VerQueryValueA,
		CALL_VerQueryValueW,
		CALL_GetFileVersionInfoA,
		CALL_GetFileVersionInfoW,
		CALL_GetFileVersionInfoSizeA,
		CALL_GetFileVersionInfoSizeW,
		CALL_HidD_GetHidGuid,
		CALL_SetupDiGetClassDevsA,
		CALL_SetupDiGetClassDevsW,
		CALL_SetupDiGetDeviceInterfaceDetailA,
		CALL_SetupDiGetDeviceInterfaceDetailW,
		CALL_SetupDiEnumDeviceInterfaces,
		CALL_SetupDiDestroyDeviceInfoList,
		CALL_COUNT
	};
private:
	HINSTANCE__ *m_hInstance;				///< Current instance of the application (Windows only)
	HWND__ *m_hWindow;						///< Main window for the application (Windows only)
	HINSTANCE__ *m_hInstances[DLL_COUNT];	///< Instances of dynamically loaded system DLLs (Windows only)
	void *m_pWindowsCalls[CALL_COUNT];		///< Pointers to resolved windows function calls (Windows only)
	IDirectInput8W* m_pDirectInput8W;		///< DirectInput8W instance (Windows only)
	IDirectInputW* m_pDirectInputW;	///< DirectInputW instance (Windows only)
	Word32 m_uQuickTimeVersion;		///< QuickTime's version in 0x0102 (1.2) format. (Windows only)
	Word32 m_uDirectXVersion;		///< DirectX version 0x0900 (9.0) format (Windows only)
	Word8 m_bQuickTimeVersionValid;	///< \ref TRUE if Quicktime's version is valid. (Windows only)
	Word8 m_bDirectXVersionValid;	///< \ref TRUE if DirectX's version is valid (Windows only)
	Word8 m_bWindow95;				///< Non-zero if tested, low bit has \ref TRUE or \ref FALSE for the state
	Word8 m_bInstancesTested[DLL_COUNT];	///< Flags to determine if a DLL was tested for loading (Windows only)
#endif
private:
	int m_iErrorCode;		///< Global default error code used by \ref Globals::Shutdown().
	Word m_uTraceFlags;		///< Debug information level
	char m_ErrorMsg[512];	///< Global Buffer containing the last fatal error or warning
	Bool m_bBombFlag;		///< \ref TRUE if non-fatal errors are treated as fatal
	Bool m_bExitFlag;		///< Global \ref TRUE if the app is in the process of shutting down.
	friend class Debug;
	static Globals g_Globals;	///< Singleton instance of the global variables
public:
#if defined(BURGER_WINDOWS) || defined(DOXYGEN)
	~Globals();
#endif
	enum {
		TRACE_MEMORYLEAKS=1,	///< Test and display memory leaks
		TRACE_REZLOAD=2,		///< Print the name of a resource file being loaded
		TRACE_FILELOAD=4,		///< Print the name of a file being loaded
		TRACE_WARNINGS=8,		///< Print possible errors
		TRACE_NETWORK=0x10,		///< Print the network commands
		TRACE_MESSAGES=0x20,	///< Print operating system messages
		TRACE_THEWORKS=0x3F		///< GIMME everything!
	};
#if defined(BURGER_WINDOWS) || defined(DOXYGEN)
	static BURGER_INLINE HINSTANCE__ *GetInstance(void) { return g_Globals.m_hInstance; }
	static BURGER_INLINE void SetInstance(HINSTANCE__ *pInput) { g_Globals.m_hInstance = pInput; }
	static BURGER_INLINE HWND__ *GetWindow(void) { return g_Globals.m_hWindow; }
	static BURGER_INLINE void SetWindow(HWND__ *pInput) { g_Globals.m_hWindow = pInput; }
	static Word BURGER_API IsDirectInputPresent(void);
	static Word BURGER_API IsDirectInput8Present(void);
	static Word BURGER_API IsDirectDrawPresent(void);
	static Word BURGER_API IsD3D9Present(void);
	static Word BURGER_API IsDirectSoundPresent(void);
	static Word BURGER_API IsWin95orWin98(void);
	static Word BURGER_API DirectInputCreateW(IDirectInputW **pOutput);
	static Word BURGER_API DirectInput8Create(IDirectInput8W **pOutput);
	static Word BURGER_API DirectDrawCreateEx(const GUID *pGuid,IDirectDraw7 **pOutput);
	static Word BURGER_API DirectDrawEnumerateExA(void *lpCallback,void *lpContext,Word32 dwFlags);
	static Word BURGER_API DirectDrawEnumerateExW(void *lpCallback,void *lpContext,Word32 dwFlags);
	static IDirect3D9 * BURGER_API Direct3DCreate9(Word uSDKVersion);
	static Word BURGER_API DirectSoundCreate(const GUID *pGuidDevice,IDirectSound **pOutput);
	static Word BURGER_API DirectSoundCreate8(const GUID *pGuidDevice,IDirectSound8 **pOutput);
	static Word BURGER_API timeGetTime(void);
	static Word BURGER_API PathSearchAndQualifyA(const char *pszPath,char *pszBuf,WordPtr cchBuf);
	static Word BURGER_API PathSearchAndQualifyW(const Word16 *pszPath,Word16 *pszBuf,WordPtr cchBuf);
	static Word BURGER_API UuidCreateSequential(GUID *pOutput);
	static Word BURGER_API VerQueryValueA(const void *pBlock,const char *lpSubBlock,void **lplpBuffer,Word *puLen);
	static Word BURGER_API VerQueryValueW(const void *pBlock,const Word16 *lpSubBlock,void **lplpBuffer,Word *puLen);
	static Word BURGER_API GetFileVersionInfoA(const char *lptstrFilename,Word32 dwHandle,Word32 dwLen,void *lpData);
	static Word BURGER_API GetFileVersionInfoW(const Word16 *lptstrFilename,Word32 dwHandle,Word32 dwLen,void *lpData);
	static Word32 BURGER_API GetFileVersionInfoSizeA(const char *lptstrFilename,unsigned long *lpdwHandle);
	static Word32 BURGER_API GetFileVersionInfoSizeW(const Word16 *lptstrFilename,unsigned long *lpdwHandle);
	static void BURGER_API HidD_GetHidGuid(GUID *HidGuid);
	static void * BURGER_API SetupDiGetClassDevsA(const GUID *ClassGuid,const char *Enumerator,HWND__ *hwndParent,Word32 Flags);
	static void * BURGER_API SetupDiGetClassDevsW(const GUID *ClassGuid,const Word16 *Enumerator,HWND__ *hwndParent,Word32 Flags);
	static Word BURGER_API SetupDiGetDeviceInterfaceDetailA(void *DeviceInfoSet,_SP_DEVICE_INTERFACE_DATA *DeviceInterfaceData,_SP_DEVICE_INTERFACE_DETAIL_DATA_A *DeviceInterfaceDetailData,Word32 DeviceInterfaceDetailDataSize,Word32 *RequiredSize,_SP_DEVINFO_DATA *DeviceInfoData);
	static Word BURGER_API SetupDiGetDeviceInterfaceDetailW(void *DeviceInfoSet,_SP_DEVICE_INTERFACE_DATA *DeviceInterfaceData,_SP_DEVICE_INTERFACE_DETAIL_DATA_W *DeviceInterfaceDetailData,Word32 DeviceInterfaceDetailDataSize,Word32 *RequiredSize,_SP_DEVINFO_DATA *DeviceInfoData);
	static Word BURGER_API SetupDiEnumDeviceInterfaces(void *DeviceInfoSet,_SP_DEVINFO_DATA *DeviceInfoData,const GUID *InterfaceClassGuid,Word32 MemberIndex,_SP_DEVICE_INTERFACE_DATA *DeviceInterfaceData);
	static Word BURGER_API SetupDiDestroyDeviceInfoList(void *DeviceInfoSet);
	static Word BURGER_API GetQuickTimeVersion(void);
	static Word BURGER_API GetDirectXVersion(void);
	static Word BURGER_API GetVideoGUID(GUID *pOutput,Word uDevNum);
	static HINSTANCE__ * BURGER_API LoadLibraryA(const char *pInput);
	static HINSTANCE__ * BURGER_API LoadLibraryW(const Word16 *pInput);
	static HINSTANCE__ * BURGER_API LoadLibraryIndex(eWindowsDLLIndex eIndex);
	static void * BURGER_API LoadFunctionIndex(eWindowsCallIndex eIndex);
	static Word BURGER_API AddGroupToProgramMenu(const char *pGroupName);
	static int BURGER_API CreateUserRegistryKey(const char *pKey,const char *pSubKey,const char *pData);
	static void BURGER_API AssociateFileExtensionToExe(const char *pFileExtension,const char *pDescription,const char *pProgramID);
#endif
#if defined(BURGER_MACOSX) || defined(DOXYGEN)
	static void BURGER_API AddToMenubar(NSMenu *pNSMenu);
	static NSString *BURGER_API GetApplicationName(void);
	static void BURGER_API CreateApplicationMenu(void);
	static void BURGER_API CreateWindowMenu(void);
	static void BURGER_API CreateViewMenu(void);
	static void BURGER_API CreateHelpMenu(void);
	static void BURGER_API CreateDefaultMenus(void);
	static void BURGER_API StringCopy(String *pOutput,const struct __CFString *pInput);
	static void BURGER_API GetHIDDeviceName(String *pOutput,__IOHIDDevice *pDevice);
	static __CFDictionary * BURGER_API CreateHIDDictionary(Word uPage,Word uUsage);
#endif
	static BURGER_INLINE int GetErrorCode(void) { return g_Globals.m_iErrorCode; }
	static BURGER_INLINE void SetErrorCode(int iNewError) { g_Globals.m_iErrorCode = iNewError; }
	static BURGER_INLINE const char *GetErrorMsg(void) { return g_Globals.m_ErrorMsg; }
	static void BURGER_ANSIAPI SetErrorMsg(const char *pMessage,...);
	static BURGER_INLINE Word GetTraceFlag(void) { return g_Globals.m_uTraceFlags; }
	static BURGER_INLINE void SetTraceFlag(Word uNewFlag) { g_Globals.m_uTraceFlags = uNewFlag; }
	static BURGER_INLINE Word AreWarningsEnabled(void) { return g_Globals.m_uTraceFlags&TRACE_WARNINGS; }
	static BURGER_INLINE Word GetErrorBombFlag(void) { return g_Globals.m_bBombFlag; }
	static BURGER_INLINE Word SetErrorBombFlag(Word uNewFlag) { Word uOld = g_Globals.m_bBombFlag; g_Globals.m_bBombFlag = (uNewFlag!=0); return uOld; }
	static BURGER_INLINE Word GetExitFlag(void) { return g_Globals.m_bExitFlag; }
	static BURGER_INLINE void SetExitFlag(Word uNewFlag) { g_Globals.m_bExitFlag = (uNewFlag!=0); }
	static void BURGER_API Shutdown(void);
	static void BURGER_API Shutdown(int iError);
	static Word BURGER_API Version(void);
	static Word BURGER_API LaunchURL(const char *pURL);
	static int BURGER_API ExecuteTool(const char *pFilename,const char *pParameters,OutputMemoryStream *pOutput=NULL);
	static const char * BURGER_API GetEnvironmentString(const char *pKey);
	static Word BURGER_API SetEnvironmentString(const char *pKey,const char *pInput);
};
}
/* END */

#endif
