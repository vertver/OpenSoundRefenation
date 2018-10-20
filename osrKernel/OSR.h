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
#ifndef POSIX_PLATFORM
#include <windows.h>
#else
#include <sys/statvfs.h>
#endif
#include <string>
#include <vector>

#define	MAX_NUM_ARGVS			128
#define VIRTUAL_MEMORY_ALLOC	0xFF00FF00
#define HEAP_MEMORY_ALLOC		0x00FF00FF
#define MAPPED_MEMORY_ALLOC		0xF0F0F0F0

#define USE_FFMPEG				0x1
#define USE_LIBSNDFILE			0x2
#define USE_WMF					0x3

#ifdef WIN32
#define OSR_DECODER_NAME		L##"osrDecoder.dll"
#define OSR_MIXER_NAME			L##"osrMixer.dll"
#define MSG_LOG(X)				OutputDebugStringW(L##X); OutputDebugStringW(L"\n");
#define WMSG_LOG(X)				OutputDebugStringW(X); OutputDebugStringW(L"\n");
#else
#define MSG_LOG(X)				printf(X); printf("\n");
#define WMSG_LOG(X)				MSG_LOG(X)
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef WIN32
using HANDLE = VOID*;
using LPVOID = VOID*;
using BOOL = int;
using WCHAR = wchar_t;
using CHAR = char;
using LPCSTR = const char*;
using PCSTR = LPCSTR;
#ifndef WITHOUT_WIDECHAR
using LPCWSTR = const wchar_t*;
using PCWSTR = LPCWSTR;
#endif
using INT = int;
using UINT = unsigned int;
using SHORT = short;
using WORD = unsigned short;
using LONG = long;
using DWORD = unsigned long;
using LONGLONG = long long;
using ULONGLONG = unsigned long long;
using DWORD64 = ULONGLONG;

typedef struct tWAVEFORMATEX
{
	WORD        wFormatTag;
	WORD        nChannels;
	DWORD       nSamplesPerSec;
	DWORD       nAvgBytesPerSec;
	WORD        nBlockAlign;
	WORD        wBitsPerSample;
	WORD        cbSize;
} WAVEFORMATEX, *PWAVEFORMATEX, *LPWAVEFORMATEX;

typedef struct waveformat_tag {
	WORD    wFormatTag;
	WORD    nChannels;
	DWORD   nSamplesPerSec; 
	DWORD   nAvgBytesPerSec; 
	WORD    nBlockAlign;    
} WAVEFORMAT, *PWAVEFORMAT, *LPWAVEFORMAT;
#endif

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

#ifdef WIN32
#ifndef USE_INT3
#define DEBUG_BREAK				DebugBreak()
#else
#define DEBUG_BREAK				__debugbreak()
#endif
#else
#define DEBUG_BREAK				__asm { int3; }
#endif

#ifdef WIN32
#ifdef ENGINE_EXPORTS
#define DLL_API					__declspec(dllexport)
#else
#define DLL_API					__declspec(dllimport)
#endif
#else
#define DLL_API					
#endif

#ifdef WIN32
// assert levels
VOID DLL_API ThrowCriticalError(LPCWSTR lpText);		// first level
BOOL DLL_API ThrowApplicationError(LPCWSTR lpText);		// second level
BOOL DLL_API ThrowDebugError(LPCWSTR lpText);			// third level
BOOL DLL_API ThrowWarning(LPCWSTR lpText);				// fourth level
#else
// assert levels
void ThrowCriticalError(const char* lpText);		// first level
bool ThrowApplicationError(const char* lpText);		// second level
bool ThrowDebugError(const char* lpText);			// third level
bool ThrowWarning(const char* lpText);				// fourth level
#endif

#define OSRSUCCEDDED(X)			(X == OSR_SUCCESS)
#define OSRFAILED(X)			(!OSRSUCCEDDED(X))

#define THROW1(X)				ThrowCriticalError(X)
#define THROW2(X)				ThrowApplicationError(X)
#define THROW3(X)				ThrowDebugError(X)
#define THROW4(X)				ThrowWarning(X)

#define OSRFAIL1(X, Y)			if (OSRFAILED(X)) { THROW1(Y); }
#ifdef WIN32
#define OSRFAIL2(X, Y)			if (OSRFAILED(X)) { if (THROW2(Y)) { return X; } else { DestroyApplication(DWORD(-2)); } }
#else
#define OSRFAIL2(X, Y)			if (OSRFAILED(X)) { if (THROW2(Y)) { return X; } else { exit(-1); } }
#endif
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
#ifdef WIN32
#define ASSERT2(X, Y)			if (!X) { if (!THROW2(Y)) { DestroyApplication(DWORD(-2)); } }
#else
#define ASSERT2(X, Y)			if (!X) { if (!THROW2(Y)) { exit(-1); } }
#endif
#ifndef WITHOUT_WIDECHAR
#define OSRASSERT(X)			if (!X) { ASSERT2(X, L"OSRASSERT"); }
#else
#define OSRASSERT(X)			if (!X) { ASSERT2(X, "OSRASSERT"); }
#endif
#ifdef WIN32
#define _DEB(X)					if (FAILED(X)) { DEBUG_BREAK; }
#endif
#define _RELEASE(X)				if (X) { X->Release(); X = NULL; }

using STRING_PATH				= char[MAX_PATH];
using STRING128					= char[128];
using STRING256					= char[256];
using STRING512					= char[512];
using STRING1024				= char[1024];

#ifndef WITHOUT_WIDECHAR
using WSTRING_PATH				= wchar_t[MAX_PATH];
using WSTRING64					= wchar_t[64];
using WSTRING128				= wchar_t[128];
using WSTRING256				= wchar_t[256];
using WSTRING512				= wchar_t[512];
using WSTRING1024				= wchar_t[1024];
#endif

#ifdef WIN32
using UnhandledExceptionFilterType = LONG WINAPI(struct _EXCEPTION_POINTERS *pExceptionInfo);
extern DLL_API UnhandledExceptionFilterType *previous_filter;
extern DLL_API HANDLE hHeap;

#define FAILEDX1(X)	\
if (FAILED(X)) \
{ \
static WSTRING1024 szError = { NULL }; \
FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, X, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szError, 2047, NULL); \
THROW1(szError); \
}

#define FAILEDX2(X)	\
if (FAILED(X)) \
{ \
static WSTRING1024 szError = { NULL }; \
FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, X, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szError, 2047, NULL); \
if (!THROW2(szError)) { ExitProcess(X); } \
}
#endif

#ifndef WITHOUT_WIDECHAR
DLL_API LPCSTR WCSTRToMBCSTR(LPCWSTR lpString);
#endif

#ifdef WIN32
DLL_API VOID CreateKernelHeap();
DLL_API VOID DestroyKernelHeap();
DLL_API HANDLE GetKernelHeap();
DLL_API BOOL IsNetworkInstalled();
DLL_API LPWSTR FormatError(LONG dwErrorCode);
DLL_API BOOL GetDiskUsage(LARGE_INTEGER largeSize, LPCWSTR lpPath);
DLL_API OSRCODE ReadAudioFile(LPCWSTR lpPath, VOID** lpData, DWORD* dwSizeWritten);
DLL_API OSRCODE ReadAudioFileEx(LPCWSTR lpPath, VOID** lpData, LONGLONG* uSize, LPDWORD dwHeaderSize);
DLL_API OSRCODE WriteFileFromBuffer(LPCWSTR lpPath, BYTE* pFile, DWORD dwSize, WAVEFORMATEX* waveFormat);
DLL_API OSRCODE OpenFileDialog(WSTRING_PATH* lpPath);
#else
DLL_API OSRCODE ReadAudioFile(const char* lpPath, VOID** lpData, unsigned long long* dwSizeWritten);
DLL_API BOOL GetDiskUsage(size_t FileSize, const char* lpPath);
DLL_API OSRCODE OpenFileDialog(STRING_PATH* lpPath);
#endif

DLL_API VOID InitApplication();
DLL_API VOID DestroyApplication(DWORD dwError);

DLL_API OSRCODE GetWaveFormatExtented(BYTE* lpWaveFile, DWORD dwFileSize, WAVEFORMATEX* waveFormat);

template 
<class T>
inline
T*
AllocateClass()
{
	return new T;
}

#ifdef WIN32
template 
<typename T>
__forceinline
T* 
AllocatePointer()
{
	return (T*)HeapAlloc(GetKernelHeap(), HEAP_ZERO_MEMORY, sizeof(T));
}

template 
<typename T, SIZE_T Size>
__forceinline 
T* 
AllocatePointer()
{
	ASSERT2(Size, L"Can't alloc file with 0 size");
	return (T*)HeapAlloc(GetKernelHeap(), HEAP_ZERO_MEMORY, Size);
}

template 
<typename T, SIZE_T Size>
__forceinline 
T*
AllocateFile()
{
	ASSERT2(Size, L"Can't alloc file with 0 size");
	return (T*)VirtualAlloc(NULL, Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

__forceinline
LPVOID
FastAlloc(
	SIZE_T uSize
)
{
	ASSERT2(uSize, L"Alloc size can't be 0");
	ASSERT2(GetKernelHeap(), L"No kernel heap");
	return HeapAlloc(GetKernelHeap(), HEAP_ZERO_MEMORY, uSize);
}

__forceinline
LPVOID
MapFile(
	SIZE_T PointerSize,
	LPCWSTR lpSharedMemoryName
)
{
	LPVOID lpSharedMemory = nullptr;

	// create file mapping at paging file
	HANDLE hSharedMemory = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, (DWORD)PointerSize, lpSharedMemoryName);
	lpSharedMemory = MapViewOfFile(hSharedMemory, FILE_MAP_ALL_ACCESS, NULL, NULL, PointerSize);

	return lpSharedMemory;
}

/***********************************************************
* WINDOWS FUNCTION
* dwType reference:
* MAPPED_MEMORY_ALLOC - pointer from mapped pagefile memory
* HEAP_MEMORY_ALLOC - pointer from kernel heap
* VIRTUAL_MEMORY_ALLOC - pointer from mapped memory
* NULL - pointer from process heap
************************************************************/
__forceinline
LPVOID
AdvanceAlloc(
	SIZE_T PointerSize,
	DWORD dwType
)
{
	LPVOID pRet = nullptr;

	switch (dwType)
	{
	case MAPPED_MEMORY_ALLOC:
		pRet = MapFile(PointerSize, NULL);
		break;
	case VIRTUAL_MEMORY_ALLOC:
		pRet = VirtualAlloc(NULL, PointerSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		break;
	case HEAP_MEMORY_ALLOC:
		pRet = HeapAlloc(GetKernelHeap(), HEAP_ZERO_MEMORY, PointerSize);
		break;
	case NULL:
		pRet = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, PointerSize);
		break;
	}

	return pRet;
}

__forceinline
VOID
FreePointer(
	LPVOID pPointer,
	SIZE_T PointerSize,
	DWORD dwType
)
{
	switch (dwType)
	{
	case MAPPED_MEMORY_ALLOC:
		if (pPointer) { ASSERT2(UnmapViewOfFile(pPointer), L"Can't free pointer. Maybe file doesn't allocated by mapped memory?"); }
		pPointer = nullptr;
		break;
	case VIRTUAL_MEMORY_ALLOC:
		if (pPointer) { ASSERT2(VirtualFree(pPointer, PointerSize, MEM_RELEASE | MEM_DECOMMIT), L"Can't free pointer. Maybe file doesn't allocated by virtual memory?"); }
		pPointer = nullptr;
		break;
	case HEAP_MEMORY_ALLOC:
		if (pPointer) { ASSERT2(HeapFree(GetKernelHeap(), NULL, pPointer), L"Can't free pointer. Maybe file doesn't allocated by kernel heap?"); }
		pPointer = nullptr;
		break;
	case NULL:
		if (pPointer) { ASSERT2(HeapFree(GetProcessHeap(), NULL, pPointer), L"Can't free pointer. Maybe file doesn't allocated by process heap?"); }
		pPointer = nullptr;
		break;
	default:
		THROW2(L"Can't free pointer because type is N/A");
		break;
	}
}

__forceinline
VOID
UnloadFile(
	LPVOID pFile
)
{
	ASSERT1(HeapFree(GetKernelHeap(), NULL, pFile), L"Can't free pointer from kernel heap");
	pFile = nullptr;
}

#else
template 
<typename T>
inline 
T*
AllocatePointer()
{
	return (T*)malloc(sizeof(T));
}

template 
<typename T, size_t Size>
inline 
T* 
AllocatePointer()
{
	ASSERT2(Size, L"Can't alloc file with 0 size");
	return (T*)malloc(Size);
}

template 
<typename T, size_t Size>
inline
T* 
AllocateFile()
{
	// map pointer by POSIX mapping
	void* pFile = mmap(NULL, Size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	ASSERT2(pFile, "Can't map pointer");

	// map fixed pointer 
	pFile = mmap(pFile, Size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
	ASSERT2(pFile, "Can't alloc fixed memory");

	// protect mapped memory for R/W only
	mprotect(pFile, Size, PROT_READ | PROT_WRITE);
	return pFile;
}

inline
void*
FastAlloc(
	size_t uSize
)
{
	ASSERT2(uSize, L"Alloc size can't be 0");
	return malloc(uSize);
}

/***********************************************************
* POSIX FUNCTION
* dwType reference:
* HEAP_MEMORY_ALLOC & NULL - pointer from kernel heap
* VIRTUAL_MEMORY_ALLOC & MAPPED_MEMORY_ALLOC -
* pointer from mapped memory
*
* #NOTE: for free pointer in POSIX systems, we must know
* the size of it, because system doesn't know about it.
************************************************************/
inline
void*
AdvanceAlloc(
	size_t PointerSize,
	unsigned long  dwType
)
{
	void* pRet = nullptr;

	switch (dwType)
	{
	case VIRTUAL_MEMORY_ALLOC:
	case MAPPED_MEMORY_ALLOC:
		// map pointer by POSIX mapping
		pRet = mmap(NULL, PointerSize, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		ASSERT2(pFile, "Can't map pointer");

		// map fixed pointer 
		pRet = mmap(pRet, PointerSize, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
		ASSERT2(pRet, "Can't alloc fixed memory");

		// protect mapped memory for R/W only
		mprotect(pRet, PointerSize, PROT_READ | PROT_WRITE);
		break;
	case HEAP_MEMORY_ALLOC:
	case NULL:
		pRet = malloc(PointerSize);
		break;
	}

	return pRet;
}

inline
void*
MapFile(
	size_t PointerSize
)
{
	return AdvanceAlloc(PointerSize, MAPPED_MEMORY_ALLOC);
}

inline
void
FreePointer(
	void* pPointer,
	size_t PointerSize,
	unsigned long dwType
)
{
	switch (dwType)
	{
	case VIRTUAL_MEMORY_ALLOC:
	case MAPPED_MEMORY_ALLOC:
		// free paged memory by POSIX
		if (pPointer)
		{
			mprotect(pPointer, pointerSize, PROT_NONE);
			munmap(pPointer, pointerSize);
		}
		pPointer = nullptr;
		break;
	case HEAP_MEMORY_ALLOC:
	case NULL:
		if (pPointer) 
		{
			free(pPointer);
		}
		pPointer = nullptr;
		break;
	default:
		THROW2("Can't free pointer because type is N/A");
		break;
	}
}

inline
void
UnloadFile(
	void* pFile
)
{
	// #NOTE: it's must be allocated by malloc
	free(pFile);
	pFile = nullptr;
}
#endif

#define FREEPROCESSHEAP(Pointer) FreePointer(Pointer, NULL, NULL)
#define FREEKERNELHEAP(Pointer) FreePointer(Pointer, NULL, HEAP_MEMORY_ALLOC)
#define FREEVIRTUALMEM(Pointer, Size) FreePointer(Pointer, Size, VIRTUAL_MEMORY_ALLOC)
#define FREEMAPPEDMEM(Pointer, Size) FreePointer(Pointer, Size, MAPPED_MEMORY_ALLOC)		// in windows set Size to NULL

typedef struct
{
	unsigned int			tag;				// tag of RIFF chunk
	unsigned int			size;				// chunk size
} RIFFChunk;

typedef struct
{
	unsigned int			tag;				// tag of RIFF chunk header
	unsigned int			size;				// chunk header size
	unsigned int			riff;				// RIFF info
} RIFFChunkHeader;

typedef struct
{
	static const unsigned int LOOP_TYPE_FORWARD			= 0x00000000;
	static const unsigned int LOOP_TYPE_RELEASE			= 0x00000001;

	unsigned int			size;				// chunk size
	unsigned int			loopType;			// chunk loop type
	unsigned int			loopStart;			// chunk loop start
	unsigned int			loopLength;			// chunk length
} DLSLoop;

typedef struct
{
	static const unsigned int OPTIONS_NOTRUNCATION		= 0x00000001;
	static const unsigned int OPTIONS_NOCOMPRESSION		= 0x00000002;

	unsigned int			size;
	unsigned short			unityNote;
	short					fineTune;
	int						gain;
	unsigned int			options;
	unsigned int			loopCount;
} RIFFDLSSample;

typedef struct
{
	static const unsigned int LOOP_TYPE_FORWARD			= 0x00000000;
	static const unsigned int LOOP_TYPE_ALTERNATING		= 0x00000001;
	static const unsigned int LOOP_TYPE_BACKWARD		= 0x00000002;

	unsigned int			cuePointId;
	unsigned int			type;
	unsigned int			start;
	unsigned int			end;
	unsigned int			fraction;
	unsigned int			playCount;
} MIDILoop;

typedef struct
{
	unsigned int	        manufacturerId;
	unsigned int	        productId;
	unsigned int	        samplePeriod;
	unsigned int	        unityNode;
	unsigned int	        pitchFraction;
	unsigned int	        SMPTEFormat;
	unsigned int	        SMPTEOffset;
	unsigned int	        loopCount;
	unsigned int	        samplerData;
} RIFFMIDISample;


struct WAV_RIFF_HEADER 
{
	DWORD	dwRIFFId;
	DWORD	dwFileSize;
};

struct WAV_FMT_HEADER 
{
	DWORD	dwFMTId;
	DWORD	dwFileSize;
	WORD	wAudioFormat;
	WORD	wChannels;
	DWORD	dwSampleRate;
	DWORD	dwByterate;
	WORD	wBlockAlign;
	WORD	wBits;
};

struct WAV_CHUNK_HEADER 
{
	DWORD	dwChunkId;
	DWORD	dwFileSize;
};

struct MP_WAVREADER_HEADER 
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subchunk1Id[4];
	unsigned long subchunk1Size;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long byteRate;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char subchunk2Id[4];
	unsigned long subchunk2Size;
};

typedef struct  
{
	BYTE bit1 : 1;
	BYTE bit2 : 1;
	BYTE bit3 : 1;
	BYTE bit4 : 1;
	BYTE bit5 : 1;
	BYTE bit6 : 1;
	BYTE bit7 : 1;
	BYTE bit8 : 1;
} BIT_BYTE;

constexpr unsigned int FOURCC_RIFF_TAG					= MAKEFOURCC('R', 'I', 'F', 'F');
constexpr unsigned int FOURCC_FORMAT_TAG				= MAKEFOURCC('f', 'm', 't', ' ');
constexpr unsigned int FOURCC_DATA_TAG					= MAKEFOURCC('d', 'a', 't', 'a');
constexpr unsigned int FOURCC_WAVE_FILE_TAG				= MAKEFOURCC('W', 'A', 'V', 'E');
constexpr unsigned int FOURCC_XWMA_FILE_TAG				= MAKEFOURCC('X', 'W', 'M', 'A');
constexpr unsigned int FOURCC_DLS_SAMPLE				= MAKEFOURCC('w', 's', 'm', 'p');
constexpr unsigned int FOURCC_MIDI_SAMPLE				= MAKEFOURCC('s', 'm', 'p', 'l');
constexpr unsigned int FOURCC_XWMA_DPDS					= MAKEFOURCC('d', 'p', 'd', 's');
constexpr unsigned int FOURCC_XMA_SEEK					= MAKEFOURCC('s', 'e', 'e', 'k');
