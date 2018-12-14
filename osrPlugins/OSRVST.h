#pragma once
#include <windows.h>

#define VSTHOST_API __declspec(dllexport)

typedef struct  
{
	WCHAR PathToPlugin[260];
	bool isInterfaceEnable;
	bool isInstrument;
} VSTPLUGIN_INFO;

class VSTHOST_API VSTHost
{
public:	
	RECT PluginRc;
	HWND PluginWindowHandle;
	LPVOID pEffect;
	HMODULE hPlugin;
	WCHAR lpModulePath[128];
	LPVOID pUnknown;
	WCHAR szPluginName[64];
	WCHAR szWindowName[128];
	bool IsVSTI;
	bool IsInterfaceEnabled;

	VSTHost() 
		: pEffect(nullptr), hPlugin(NULL), pUnknown(nullptr), IsVSTI(false), PluginWindowHandle(NULL), IsInterfaceEnabled(NULL) { }

	BOOL LoadPlugin(LPCWSTR lpPathToPlugin);
	BOOL CheckPlugin(LPCWSTR lpPathToPlugin, LPBOOL isFactoryEnable);
	VOID UnloadPlugin() { if (hPlugin) { FreeLibrary(hPlugin); hPlugin = NULL; } }

	VOID ResumePlugin();
	VOID SuspendPlugin();

	VOID CreatePluginWindow();
	VOID DestroyPluginWindow();
	VOID OpenPluginWindow();
	VOID ClosePluginWindow();

	BOOL InitPlugin(DWORD dwSampleRate, DWORD dwBlockSize);
	//BOOL DispathPlugin(BYTE cbOpcode, DWORD dwSampleRate, DWORD dwBlockSize, RECT* lpPluginRec);
	VOID ProcessAudio(float** pAudioInput, float** pAudioOutput, DWORD dwSampleFrames);

	~VSTHost()
	{
		if (hPlugin)
		{
			DestroyPluginWindow();
			UnloadPlugin();
		}

		pEffect = nullptr;
		hPlugin = NULL;
		PluginWindowHandle = NULL;
		pUnknown = nullptr;
	}
};

typedef struct PluginVst2xIdMembers
{
	DWORD dwId;
	CHAR* szIdString;
} *PluginVst2xId, VST2_PLUGINHOST, *PVST2_PLUGINHOST;

extern VSTHOST_API HANDLE hPluginHandle;
extern VSTHOST_API VSTHost PluginHost;

//BOOL CheckPluginList(VSTHost* pHost, LPCWSTR lpPluginFolder, VSTPLUGIN_INFO* pPluginInfo, size_t StructSizes);
