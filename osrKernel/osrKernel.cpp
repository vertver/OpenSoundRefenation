/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* osrKernel.cpp
* Kernel part
*********************************************************/
#include "stdafx.h"
#ifdef WIN32
#include "OSRWin32kernel.h"

DLL_API HMODULE hMixer;
DLL_API HMODULE hDecoder;
DLL_API HMODULE hLibSndFile;
DLL_API HMODULE hAVCodec;
DLL_API HMODULE hAVUtil;
DLL_API HMODULE hNetwork;
DLL_API bool bNetwork;
#endif

#include "OSRConfig.h"

VOID
InitApplication()
{
#ifdef WIN32
	CreateTempDirectory();
	CreateKernelHeap();
	threadSystem.SetUserThreadName(GetCurrentThreadId(), const_cast<LPWSTR>(L"OSR Main Thread"));
	previous_filter = SetUnhandledExceptionFilter(UnhandledFilter);

  	CreateConfig("Config");
	PARAMS_DATA paramsData = { 0 };
	memset(paramsData.pData, 0xEE, 128);
	CHAR dwParam[5] = "DATA";
	memcpy(paramsData.pData, &dwParam, 4);
	CreateParameter("Config", "TEST_PARAM1", &paramsData);
	BYTE SharedMem[1024] = { 0 };
	memset(SharedMem, 0xFF, 1024);
	CHAR dwParams[5] = "OCFG";
	memcpy(SharedMem, &dwParams, 4);
	paramsData.dwSharedMemorySize = 1024;
	paramsData.pSharedMemory = SharedMem;
	CreateParameter("Config", "TEST_PARAM2", &paramsData);
	CreateParameter("Config", "TEST_PARAM3", &paramsData);
	memset(SharedMem, 0xCC, 1024);
	WriteParameter("Config", "TEST_PARAM3", &paramsData);
	DeleteParameter("Config", "TEST_PARAM2");
	OpenParameter("Config", "TEST_PARAM3", &paramsData);

	//#TODO: create here events for dynamic splash screen
	hDecoder = GetModuleHandleW(OSR_DECODER_NAME);
	if (!hDecoder) 
	{ 
		hDecoder = LoadLibraryW(OSR_DECODER_NAME);
		ASSERT1(hDecoder, L"OSR Error: Can't load 'osrDecoder.dll' library");
	}
	MSG_LOG("OSR: Decoder loaded");

	hMixer = GetModuleHandleW(OSR_MIXER_NAME);
	if (!hMixer) 
	{ 
		hMixer = LoadLibraryW(OSR_MIXER_NAME);
		ASSERT1(hMixer, L"OSR Error: Can't load 'osrMixer.dll' library");
	}
	MSG_LOG("OSR: Mixer loaded");

	hLibSndFile = GetModuleHandleW(L"libsndfile-1.dll");
	if (!hLibSndFile)
	{
		hLibSndFile = LoadLibraryW(L"libsndfile-1.dll");
		ASSERT1(hLibSndFile, L"OSR Error: Can't load 'libsndfile-1.dll' library");
	}
	MSG_LOG("OSR: libsndfile loaded");

	hAVCodec = GetModuleHandleW(L"avcodec-58.dll");
	if (!hAVCodec)
	{
		hAVCodec = LoadLibraryW(L"avcodec-58.dll");
		ASSERT1(hAVCodec, L"OSR Error: Can't load 'avcodec-58.dll' library");
	}
	MSG_LOG("OSR: avcodec loaded");

	hAVUtil = GetModuleHandleW(L"avutil-56.dll");
	if (!hAVUtil)
	{
		hAVUtil = LoadLibraryW(L"avutil-56.dll");
		ASSERT1(hAVUtil, L"OSR Error: Can't load 'avutil-56.dll' library");
	}
	MSG_LOG("OSR: avutil loaded");

	hNetwork = GetModuleHandleW(L"osrNetwork.dll");
	if (!hNetwork)
	{
		hNetwork = LoadLibraryW(L"osrNetwork.dll");
	}
	bNetwork = !!hNetwork;

	OleInitialize(nullptr);
#endif

}

VOID
DestroyApplication(DWORD dwError)
{
#ifdef WIN32
	FreeLibrary(hDecoder);
	FreeLibrary(hMixer);
	FreeLibrary(hNetwork);
	FreeLibrary(hLibSndFile);
	FreeLibrary(hAVUtil);
	FreeLibrary(hAVCodec);
	
	DestroyKernelHeap();
	ExitProcess(dwError);
#else
	exit(dwError);
#endif
}

#ifdef WIN32
VOID
ThrowCriticalError(
	LPCWSTR lpText
)
{
	MSG_LOG("Critical error. That's all, my friend.");
	MSG_LOG("--------------------------------------------");
	WMSG_LOG(lpText);
	MSG_LOG("--------------------------------------------");

	MessageBoxW(NULL, lpText, L"Kernel error", MB_OK | MB_ICONHAND);
	DestroyApplication(GetLastError());
}


BOOL
ThrowApplicationError(
	LPCWSTR lpText
)
{
	DWORD dwError = GetLastError();
	std::wstring szMsg = {};

	if (dwError)
	{
		szMsg = L"Application throw error. Are you wan't to get next?" +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + lpText +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + L"Windows error: " +
			FormatError(dwError);
	}
	else
	{
		szMsg = L"Application throw error. Are you wan't to get next?" +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + lpText +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + L"Windows error: " +
			L"No system error";

	}

	WMSG_LOG(szMsg.c_str());
	int MsgBox = MessageBoxW(NULL, szMsg.c_str(), L"Application error", MB_YESNO | MB_ICONHAND);

	if (MsgBox == IDYES) { return TRUE; }
	return FALSE;
}

BOOL
ThrowDebugError(
	LPCWSTR lpText
)
{
	DWORD dwError = GetLastError();
	std::wstring szMsg = {};
	if (dwError)
	{
		szMsg = L"Application throw debug error. Hey, that's not so bad. Continue?" +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + lpText +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + L"Windows error: " +
			FormatError(dwError);
	}
	else
	{
		szMsg = L"Application throw debug error. Hey, that's not so bad. Continue?" +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + lpText +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + L"Windows error: " +
			L"No system error";
	}

	WMSG_LOG(szMsg.c_str());
	int MsgBox = MessageBoxW(NULL, szMsg.c_str(), L"Application debug error", MB_YESNO | MB_ICONHAND);

	if (MsgBox == IDYES) { return TRUE; }
	return FALSE;
}

BOOL
ThrowWarning(
	LPCWSTR lpText
)
{
	DWORD dwError = GetLastError();

	std::wstring szMsg = {};
	if (dwError)
	{
		szMsg = L"Warning!" +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + lpText +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + L"Windows error: " +
			FormatError(dwError);
	}
	else
	{
		szMsg = L"Warning!" +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + lpText +
			std::wstring(L"\n") + L" " + std::wstring(L"\n") + L"Windows error: " +
			L"No system error";
	}

	WMSG_LOG(szMsg.c_str());
	int MsgBox = MessageBoxW(NULL, szMsg.c_str(), L"Warning", MB_YESNO | MB_ICONWARNING);

	if (MsgBox == IDYES) { return TRUE; }
	return FALSE;
}

BOOL
IsNetworkInstalled()
{
	return bNetwork;
}

#else
void
ThrowCriticalError(
	const char* lpText
)
{
	MSG_LOG("Critical error. That's all, my friend.");
	MSG_LOG("--------------------------------------------");
	MSG_LOG(lpText);
	MSG_LOG("--------------------------------------------");

	//#TODO: make custom message box
	exit(-1);
}

bool
ThrowApplicationError(
	const char* lpText
)
{
	std::string szMsg = {};

	szMsg = "Application throw error. Are you wan't to get next?" +
		std::string("\n") + " " + std::string("\n") + lpText;

	MSG_LOG(szMsg.c_str());
	//#TODO: make custom message box
	return false;
}

bool
ThrowDebugError(
	const char* lpText
)
{
	std::string szMsg = {};

	szMsg = "Application throw debug error. Hey, that's not so bad. Continue?" +
		std::string("\n") + " " + std::string("\n") + lpText;

	MSG_LOG(szMsg.c_str());
	//#TODO: make custom message box
	return false;
}

bool
ThrowWarning(
	const char* lpText
)
{
	std::string szMsg = {};
	szMsg = "Warning!" +
		std::string("\n") + " " + std::string("\n") + lpText;

	MSG_LOG(szMsg.c_str());
	//#TODO: make custom message box
	return false;
}
#endif
