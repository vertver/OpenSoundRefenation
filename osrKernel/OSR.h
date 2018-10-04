/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* OSR.h
* Master-include header
*********************************************************/
#pragma once
#include <windows.h>
#include <string>
#include <vector>

#define	MAX_NUM_ARGVS			128

#define OSR_DECODER_NAME		L##"osrDecoder.dll"
#define OSR_MIXER_NAME			L##"osrMixer.dll"

#define MSG_LOG(X)				OutputDebugStringW(L##X); OutputDebugStringW(L"\n");
#define WMSG_LOG(X)				OutputDebugStringW(X); OutputDebugStringW(L"\n");

enum OSRCODE
{
	OSR_SUCCESS,

	FS_OSR_BAD_PATH,
	FS_OSR_BAD_ALLOC,
	FS_OSR_BAD_HANDLE,
	FS_OSR_BAD_PTR,
	FS_OSR_NO_SPACE,

	MXR_OSR_NO_FILE,
	MXR_OSR_BAD_WFX,
	MXR_OSR_NO_OUT,
	MXR_OSR_BAD_LIB,
	MXR_OSR_UNSUPPORTED_FMT,
	MXR_OSR_BUFFER_CORRUPT,

	DECODE_OSR_BAD_FILE,
	DECODE_OSR_BAD_PATH,
	DECODE_OSR_UNSUPPORTED_FMT,
	DECODE_OSR_BAD_WFX,
	DECODE_OSR_NO_ACCESS,
	DECODE_OSR_INTER_ERR,
	DECODE_OSR_MWF_FAILED,

	KERN_OSR_NO_LIB,
	KERN_OSR_BAD_ALLOC,
	KERN_OSR_BAD_MAP,
	KERN_OSR_BAD_STR
};

#ifndef USE_INT3
#define DEBUG_BREAK				DebugBreak()
#else
#define DEBUG_BREAK				__debugbreak()
#endif

#ifdef ENGINE_EXPORTS
#define DLL_API					__declspec(dllexport)
#else
#define DLL_API					__declspec(dllimport)
#endif

// assert levels
VOID DLL_API ThrowCriticalError(LPCWSTR lpText);		// first level
BOOL DLL_API ThrowApplicationError(LPCWSTR lpText);		// second level
BOOL DLL_API ThrowDebugError(LPCWSTR lpText);			// third level
BOOL DLL_API ThrowWarning(LPCWSTR lpText);				// fourth level

#define OSRSUCCEDDED(X)			(X == OSR_SUCCESS)
#define OSRFAILED(X)			(!OSRSUCCEDDED(X))

#define THROW1(X)				ThrowCriticalError(X)
#define THROW2(X)				ThrowApplicationError(X)
#define THROW3(X)				ThrowDebugError(X)
#define THROW4(X)				ThrowWarning(X)

#define OSRFAIL1(X, Y)			if (OSRFAILED(X)) { THROW1(Y); }
#define OSRFAIL2(X, Y)			if (OSRFAILED(X)) { if (THROW2(Y)) { return X; } else { ExitProcess(DWORD(-2)); } }
#ifdef DEBUG
#define OSRFAIL3(X, Y)			if (OSRFAILED(X)) { if (!THROW3(Y)) { return X; } else DEBUG_BREAK; }
#else
#define OSRFAIL3(X, Y)			if (OSRFAILED(X)) { if (THROW3(Y)) { return X; } }
#endif
#ifdef DEBUG
#define OSRFAIL4(X, Y)			if (OSRFAILED(X)) { if (!THROW4(Y)) { return X; } }
#else
#define OSRFAIL4(X, Y)			if (OSRFAILED(X)) { THROW4(Y); }
#endif

#define ASSERT1(X, Y)			if (!X) { THROW1(Y); }
#define ASSERT2(X, Y)			if (!X) { if (!THROW2(Y)) { ExitProcess(DWORD(-2)); } }
#define OSRASSERT(X)			if (!X) { ASSERT2(X, L"OSRASSERT"); }

#define _DEB(X)					if (FAILED(X)) { DEBUG_BREAK; }
#define _RELEASE(X)				if (X) { X->Release(); X = NULL; }

using STRING_PATH				= CHAR[MAX_PATH];
using STRING128					= CHAR[128];
using STRING256					= CHAR[256];
using STRING512					= CHAR[512];
using STRING1024				= CHAR[1024];

using WSTRING_PATH				= WCHAR[MAX_PATH];
using WSTRING64					= WCHAR[64];
using WSTRING128				= WCHAR[128];
using WSTRING256				= WCHAR[256];
using WSTRING512				= WCHAR[512];
using WSTRING1024				= WCHAR[1024];

using UnhandledExceptionFilterType = LONG WINAPI(struct _EXCEPTION_POINTERS *pExceptionInfo);
extern DLL_API UnhandledExceptionFilterType *previous_filter;
extern DLL_API HANDLE hHeap;

#define FAILEDX1(X)	\
if (FAILED(X)) \
{ \
WSTRING1024 szError = { NULL }; \
FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, X, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szError, 2047, NULL); \
THROW1(szError); \
}

#define FAILEDX2(X)	\
if (FAILED(X)) \
{ \
WSTRING1024 szError = { NULL }; \
FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, X, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szError, 2047, NULL); \
if (!THROW2(szError)) { ExitProcess(X); } \
}

DLL_API LPCSTR WCSTRToMBCSTR(LPCWSTR lpString);
DLL_API VOID CreateKernelHeap();
DLL_API VOID DestroyKernelHeap();
DLL_API HANDLE GetKernelHeap();
DLL_API LPWSTR FormatError(LONG dwErrorCode);
DLL_API VOID UnloadFile(LPVOID pFile);
DLL_API BOOL GetDiskUsage(LARGE_INTEGER largeSize, LPCWSTR lpPath);

DLL_API VOID InitApplication();
DLL_API VOID DestroyApplication();

DLL_API OSRCODE OpenFileDialog(WSTRING_PATH* lpPath);
DLL_API OSRCODE ReadAudioFile(LPCWSTR lpPath, VOID** lpData, DWORD* dwSizeWritten);
DLL_API OSRCODE ReadAudioFileEx(LPCWSTR lpPath, VOID** lpData, LONGLONG* uSize);
DLL_API OSRCODE WriteFileFromBuffer(LPCWSTR lpPath, BYTE* pFile, DWORD dwSize, WAVEFORMATEX* waveFormat);
DLL_API OSRCODE GetWaveFormatExtented(BYTE* lpWaveFile, DWORD dwFileSize, WAVEFORMATEX* waveFormat);

template <class T>
inline T* AllocateClass()
{
	return new T;
}

template <typename T>
inline T* AllocatePointer()
{
	return (T*)HeapAlloc(GetKernelHeap(), HEAP_ZERO_MEMORY, sizeof(T));
}

__forceinline
LPVOID
FastAlloc(
	size_t uSize
)
{
	ASSERT2(uSize, L"Alloc size can't be 0");
	ASSERT2(hHeap, L"No kernel heap");
	return HeapAlloc(GetKernelHeap(), HEAP_ZERO_MEMORY, uSize);
}

typedef struct
{
	UINT			tag;				// tag of RIFF chunk
	UINT			size;				// chunk size
} RIFFChunk;

typedef struct
{
	UINT			tag;				// tag of RIFF chunk header
	UINT			size;				// chunk header size
	UINT			riff;				// RIFF info
} RIFFChunkHeader;

typedef struct
{
	static const UINT LOOP_TYPE_FORWARD			= 0x00000000;
	static const UINT LOOP_TYPE_RELEASE			= 0x00000001;

	UINT			size;				// chunk size
	UINT			loopType;			// chunk loop type
	UINT			loopStart;			// chunk loop start
	UINT			loopLength;			// chunk length
} DLSLoop;

typedef struct
{
	static const UINT OPTIONS_NOTRUNCATION		= 0x00000001;
	static const UINT OPTIONS_NOCOMPRESSION		= 0x00000002;

	UINT			size;
	WORD			unityNote;
	SHORT			fineTune;
	INT				gain;
	UINT			options;
	UINT			loopCount;
} RIFFDLSSample;

typedef struct
{
	static const UINT LOOP_TYPE_FORWARD			= 0x00000000;
	static const UINT LOOP_TYPE_ALTERNATING		= 0x00000001;
	static const UINT LOOP_TYPE_BACKWARD		= 0x00000002;

	UINT			cuePointId;
	UINT			type;
	UINT			start;
	UINT			end;
	UINT			fraction;
	UINT			playCount;
} MIDILoop;

typedef struct
{
	UINT	        manufacturerId;
	UINT	        productId;
	UINT	        samplePeriod;
	UINT	        unityNode;
	UINT	        pitchFraction;
	UINT	        SMPTEFormat;
	UINT	        SMPTEOffset;
	UINT	        loopCount;
	UINT	        samplerData;
} RIFFMIDISample;

constexpr UINT FOURCC_RIFF_TAG				= MAKEFOURCC('R', 'I', 'F', 'F');
constexpr UINT FOURCC_FORMAT_TAG			= MAKEFOURCC('f', 'm', 't', ' ');
constexpr UINT FOURCC_DATA_TAG				= MAKEFOURCC('d', 'a', 't', 'a');
constexpr UINT FOURCC_WAVE_FILE_TAG			= MAKEFOURCC('W', 'A', 'V', 'E');
constexpr UINT FOURCC_XWMA_FILE_TAG			= MAKEFOURCC('X', 'W', 'M', 'A');
constexpr UINT FOURCC_DLS_SAMPLE			= MAKEFOURCC('w', 's', 'm', 'p');
constexpr UINT FOURCC_MIDI_SAMPLE			= MAKEFOURCC('s', 'm', 'p', 'l');
constexpr UINT FOURCC_XWMA_DPDS				= MAKEFOURCC('d', 'p', 'd', 's');
constexpr UINT FOURCC_XMA_SEEK				= MAKEFOURCC('s', 'e', 'e', 'k');
