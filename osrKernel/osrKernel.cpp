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
#include "OSRWin32kernel.h"

DLL_API HMODULE hMixer;
DLL_API HMODULE hDecoder;
DLL_API HMODULE hLibSndFile;
DLL_API HMODULE hAVCodec;
DLL_API HMODULE hAVUtil;

VOID
InitApplication()
{
	CreateTempDirectory();
	CreateKernelHeap();

	previous_filter = SetUnhandledExceptionFilter(UnhandledFilter);

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

}

VOID
DestroyApplication()
{
	FreeLibrary(hDecoder);
	FreeLibrary(hMixer);
	FreeLibrary(hLibSndFile);
	FreeLibrary(hAVUtil);
	FreeLibrary(hAVCodec);
	
	DestroyKernelHeap();
	ExitProcess(GetCurrentProcessId());
}

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

	ExitProcess(GetLastError());
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
