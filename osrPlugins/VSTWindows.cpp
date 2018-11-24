#include "stdafx.h"
#pragma hdrstop

float fBlockSize = 0.f;
float fSampleRate = 0.f;

typedef AEffect* (VSTCALLBACK *PluginMain) (audioMasterCallback audioMaster);
AEffect* pCustomEffect;

VSTHOST_API HANDLE hPluginHandle = nullptr;
VSTHOST_API VSTHost PluginHost = {};

PluginVst2xId 
NewPluginVst2xId()
{
	PluginVst2xId pluginVst2xId = (PluginVst2xId)HeapAlloc(GetProcessHeap(), 0, sizeof(PluginVst2xIdMembers));

	pluginVst2xId->dwId = 0;
	pluginVst2xId->szIdString = (CHAR*)HeapAlloc(GetProcessHeap(), 0, 260);

	if (pluginVst2xId->szIdString) 
	{
		strcpy(pluginVst2xId->szIdString, "????");
	}

	return pluginVst2xId;
}

PluginVst2xId
NewPluginVst2xIdWithId(
	DWORD id
)
{
	PluginVst2xId pluginVst2xId = NewPluginVst2xId();

	pluginVst2xId->dwId = id;
	snprintf(pluginVst2xId->szIdString, 260, "%u", id);

	return pluginVst2xId;
}

static
BOOL
IsSupportByHost(
	LPCSTR luginName,
	LPCSTR canDoString
) 
{
	bool supported = false;

	if (!strcmp(canDoString, "")) 
	{
		// do nothing. plugin bug
	}
	else if (!strcmp(canDoString, "sendVstEvents")) 
	{
		supported = true;
	}
	else if (!strcmp(canDoString, "sendVstMidiEvent"))
	{
		supported = true;
	}
	else if (!strcmp(canDoString, "sendVstTimeInfo"))
	{
		supported = true;
	}
	else if (!strcmp(canDoString, "receiveVstEvents")) 
	{
		supported = false;
	}
	else if (!strcmp(canDoString, "receiveVstMidiEvent"))
	{
		supported = false;
	}
	else if (!strcmp(canDoString, "reportConnectionChanges"))
	{
		supported = false;
	}
	else if (!strcmp(canDoString, "acceptIOChanges"))
	{
		supported = false;
	}
	else if (!strcmp(canDoString, "sizeWindow")) 
	{
		supported = false;
	}
	else if (!strcmp(canDoString, "offline"))
	{
		supported = false;
	}
	else if (!strcmp(canDoString, "openFileSelector")) 
	{
		supported = true;
	}
	else if (!strcmp(canDoString, "closeFileSelector"))
	{
		supported = true;
	}
	else if (!strcmp(canDoString, "startStopProcess"))
	{
		supported = true;
	}
	else if (!strcmp(canDoString, "shellCategory")) 
	{
		supported = true;
	}
	else if (!strcmp(canDoString, "sendVstMidiEventFlagIsRealtime"))
	{
		supported = false;
	}

	return supported;
}

VstIntPtr
VSTMasterAudioCallback(
	AEffect* effect,
	VstInt32 opcode,
	VstInt32 index,
	VstIntPtr value,
	void* ptr,
	float opt
)
{
	// this string is used in a bunch of logging calls below
	PluginVst2xId pluginId;

	if (!effect) 
	{
		pluginId = NewPluginVst2xId();
	}
	else
	{
		pluginId = NewPluginVst2xIdWithId((DWORD)effect->uniqueID);
	}

	const char *pluginIdString = pluginId->szIdString;
	VstIntPtr result = 0;

	switch (opcode) 
	{
	case audioMasterAutomate:
		break;
	case audioMasterVersion:
		result = 2400;		// we are a VST 2.4 compatible host
		break;
	case audioMasterCurrentId:
		result = 'AASH';
		break;
	case audioMasterIdle:
	{ 
		effect->dispatcher(effect, effEditIdle, 0, 0, NULL, 0.0f);
		return 0; 
	}
		break;
	case audioMasterGetTime: 
		return 0;
		break;
	case audioMasterIOChanged: 
		result = 1;
		break;
	case audioMasterSizeWindow:
		result = 0;
		break;
	case audioMasterProcessEvents: 
	{ 
		return 0;
	}
	case audioMasterGetSampleRate:
		result = (int)fSampleRate;
		break;
	case audioMasterGetBlockSize:
		result = (VstIntPtr)fBlockSize;
		break;
	case audioMasterGetInputLatency:
		// input latency is not used, and is always 0
		result = 0;
		break;
	case audioMasterGetOutputLatency:
		// output latency is not used, and is always 0
		result = 0;
		break;
	case audioMasterGetCurrentProcessLevel:
		result = 0;
		break;
	case audioMasterGetAutomationState:
		// automation is also not supported (for now)
		result = kVstAutomationUnsupported;
		break;
	case audioMasterGetVendorString:
		strncpy((LPSTR)ptr, "OpenSoundRefenation", kVstMaxVendorStrLen);
		result = 1;
		break;
	case audioMasterGetProductString:
		strncpy((LPSTR)ptr, "OSR_DAW", kVstMaxProductStrLen);
		result = 1;
		break;
	case audioMasterGetVendorVersion:
		result = 1;
		break;
	case audioMasterVendorSpecific: 
	{
		return 0; 
	}
	case audioMasterCanDo:
		result = IsSupportByHost(pluginIdString, (LPSTR)ptr);
		break;
	case audioMasterGetLanguage:
		result = kVstLangEnglish;
		break;
	case audioMasterUpdateDisplay:
		effect->dispatcher(pCustomEffect, effEditIdle, 0, 0, NULL, 0.0f); 
		return 0;
		// ignore
		break;
	default:
		// unknown opcode
		break;
	}

	if (pluginId->szIdString) { HeapFree(GetProcessHeap(), 0, pluginId->szIdString); }
	if (pluginId) { HeapFree(GetProcessHeap(), 0, pluginId); }

	return result;
}

BOOL
VSTHost::LoadPlugin(
	LPCWSTR lpPathToPlugin
)
{
	FARPROC pVSTFunc = nullptr;
	FARPROC pVSTEntryPoint = nullptr;
	PluginMain pMain = nullptr;
	IPluginFactory* pFactory = nullptr;

	memset(&szPluginName, 0, sizeof(szPluginName));
	hPlugin = LoadLibraryW(lpPathToPlugin);
	hPluginHandle = CreateEventW(nullptr, TRUE, FALSE, L"VST_PLUGIN_EVENT");

	if (!hPlugin)
	{
		return FALSE;
	}

	// checking for VST host
	if (!pVSTEntryPoint)
	{
		pVSTEntryPoint = GetProcAddress(hPlugin, "VSTPluginMain");
		if (!pVSTEntryPoint)
		{
			return FALSE;
		}
	}
	if (!pVSTFunc)
	{
		pVSTFunc = GetProcAddress(hPlugin, "GetPluginFactory");
		if (!pVSTFunc)
		{
			IsInterfaceEnabled = false;
		}
		else
		{
			IsInterfaceEnabled = true;
		}
	}

	pMain = (PluginMain)pVSTEntryPoint;
	pEffect = pMain(&VSTMasterAudioCallback);
	pCustomEffect = (AEffect*)pEffect;

	if (IsInterfaceEnabled)
	{
		// get plugin factory
		pFactory = (IPluginFactory*)pVSTFunc();

		if (pFactory)
		{
			// get name of plugin
			PClassInfo classInfo = { NULL };
			for (long i = 0; i < pFactory->countClasses(); i++)
			{
				pFactory->getClassInfo(i, &classInfo);
			}

			for (int i = 0; i < 64; i++) {szPluginName[i] = classInfo.name[i]; }
		}
	}
	else
	{
		if (!GetModuleFileNameW(hPlugin, szPluginName, sizeof(szPluginName)))
		{
			wchar_t szOut[256] = { NULL };

			_snwprintf(szOut, sizeof(szOut), L"Function failed with %d error code", GetLastError());
			OutputDebugStringW(szOut);

			wcscpy_s(szPluginName, L"No name plugin");
		}
	}

	return TRUE;
}

VOID
VSTHost::ProcessAudio(
	float** pAudioInput,
	float** pAudioOutput,
	DWORD dwSampleFrames
)
{
	AEffect* pCurrentEffect = nullptr;

	if (pEffect)
	{
		// process data
		pCurrentEffect = (AEffect*)pEffect;
		pCurrentEffect->processReplacing(pCurrentEffect, pAudioInput, pAudioOutput, (VstInt32)dwSampleFrames);
	}
}

BOOL
VSTHost::InitPlugin(
	DWORD dwSampleRate, 
	DWORD dwBlockSize
)
{
	AEffect* pCurrentEffect = nullptr;
	ERect* rec = nullptr;

	if (!pEffect) { return FALSE; }	
	pCurrentEffect = (AEffect*)pEffect;

	// open VST host 
	pCurrentEffect->dispatcher(pCurrentEffect, effOpen, 0, 0, nullptr, 0);
	pCurrentEffect->dispatcher(pCurrentEffect, effSetSampleRate, 0, 0, nullptr, (float)dwSampleRate);
	pCurrentEffect->dispatcher(pCurrentEffect, effSetBlockSize, 0, dwBlockSize, nullptr, 0);

	// set message to resume plugin
	ResumePlugin();

	// copy sample rate and block size 
	fSampleRate = (float)dwSampleRate;
	fBlockSize = (float)dwBlockSize;

	// create and open VST plugin window (with controls)
	CreatePluginWindow();
	OpenPluginWindow();

	return TRUE;
}

VOID
VSTHost::ResumePlugin()
{
	AEffect* pCurrentEffect = (AEffect*)pEffect;

	// send to dispatcher that main was changed to 1
	if (!pCurrentEffect) return;
	pCurrentEffect->dispatcher(pCurrentEffect, effMainsChanged, 0, 1, nullptr, 0.0f);
}

VOID
VSTHost::SuspendPlugin()
{
	AEffect* pCurrentEffect = (AEffect*)pEffect;

	// send to dispatcher that main was changed to 0
	if (!pCurrentEffect) return;
	pCurrentEffect->dispatcher(pCurrentEffect, effMainsChanged, 0, 0, nullptr, 0.0f);
}

VOID   
VSTHost::OpenPluginWindow() 
{
	ERect* rec = nullptr;
	AEffect* pCurrentEffect = (AEffect*)pEffect;

	if (!pCurrentEffect) return;
	if (!PluginWindowHandle) return;

	// set window handle and get plugin rect
	pCurrentEffect->dispatcher(pCurrentEffect, effEditOpen, 0, 0, (LPVOID)PluginWindowHandle, 0.0f);
	pCurrentEffect->dispatcher(pCurrentEffect, effEditGetRect, 1, 0, &rec, 0);

	PluginRc.bottom = rec->bottom;
	PluginRc.left = rec->left;
	PluginRc.right = rec->right;
	PluginRc.top = rec->top;

	const auto style = GetWindowLongPtrW(PluginWindowHandle, GWL_STYLE);
	const auto exStyle = GetWindowLongPtrW(PluginWindowHandle, GWL_EXSTYLE);

	// set custom rect and show window
	AdjustWindowRectEx(&PluginRc, style, NULL, exStyle);
	MoveWindow(PluginWindowHandle, 0, 0, PluginRc.right - PluginRc.left, PluginRc.bottom - PluginRc.top, TRUE);
	ShowWindow(PluginWindowHandle, SW_SHOWDEFAULT);
	UpdateWindow(PluginWindowHandle);
}

VOID 
VSTHost::ClosePluginWindow() 
{
	AEffect* pCurrentEffect = (AEffect*)pEffect;

	// close plugin window handle
	if (!pCurrentEffect) return;
	pCurrentEffect->dispatcher(pCurrentEffect, effEditOpen, 0, 0, nullptr, 0.0f);
}

LRESULT
WINAPI
PluginProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (msg)
	{
	case WM_DESTROY:
		SetEvent(hPluginHandle);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

VOID 
VSTHost::CreatePluginWindow()
{
	// register plugin window class
	WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_CLASSDC, PluginProc, 0L, 0L, GetModuleHandleW(L"osrPlugins.dll"), nullptr, nullptr, nullptr, nullptr, L"OSR_PLUGIN", nullptr };
	RegisterClassExW(&wc);

	// copy plugin name to other static buffer
	static wchar_t szCreateBuf[64] = { NULL };
	memcpy(szCreateBuf, szPluginName, sizeof(wchar_t) * 64);

	// set name of plugin
	_snwprintf(szWindowName, sizeof(szPluginName), L"%s%s", L"OSR hosted plugin name: ", szCreateBuf);

	PluginWindowHandle = CreateWindowExW(
		0L,
		L"OSR_PLUGIN",
		szWindowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);
}

VOID
VSTHost::DestroyPluginWindow()
{
	// if window handle is exist - destroy it
	if (PluginWindowHandle)
	{
		DestroyWindow(PluginWindowHandle);
		UnregisterClassW(L"OSR_PLUGIN", GetModuleHandleW(L"osrPlugins.dll"));
	}
}
