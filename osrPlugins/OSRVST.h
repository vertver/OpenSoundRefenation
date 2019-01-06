#pragma once
#include <windows.h>
#define VSTHOST_API __declspec(dllexport)

typedef struct  
{
	WCHAR PathToPlugin[260];
	bool isInterfaceEnable;
	bool isInstrument;
} VSTPLUGIN_INFO;

class IVSTHost
{
public:	
	virtual int  LoadPlugin(LPCWSTR lpPathToPlugin) = 0;
	virtual int  CheckPlugin(LPCWSTR lpPathToPlugin, LPBOOL isFactoryEnable) = 0;
	virtual void UnloadPlugin() = 0; 

	virtual void ResumePlugin() = 0;
	virtual void SuspendPlugin() = 0;

	virtual void CreatePluginWindow() = 0;
	virtual void DestroyPluginWindow() = 0;
	virtual void OpenPluginWindow() = 0;
	virtual void ClosePluginWindow() = 0;

	virtual int  InitPlugin(DWORD dwSampleRate, DWORD dwBlockSize) = 0;
	virtual void ProcessAudio(float** pAudioInput, float** pAudioOutput, DWORD dwSampleFrames) = 0;
};

#ifdef WIN32
class VSTHOST_API IWin32VSTHost : public IVSTHost
{
public:
	RECT PluginRc;
 	void* PluginWindowHandle;
 	void* pEffect;
 	void* hPlugin;
 	wchar_t lpModulePath[128];
 	void* pUnknown;
 	wchar_t szPluginName[64];
 	wchar_t szWindowName[128];
 	bool IsVSTI;
 	bool IsInterfaceEnabled;

	IWin32VSTHost() : pEffect(nullptr), hPlugin(NULL), pUnknown(nullptr), IsVSTI(false), PluginWindowHandle(NULL), IsInterfaceEnabled(NULL)
	{
		memset(&PluginRc, 0, sizeof(RECT));
		memset(lpModulePath, 0, sizeof(wchar_t) * 128);
		memset(szPluginName, 0, sizeof(wchar_t) * 64);
		memset(szWindowName, 0, sizeof(wchar_t) * 128);
	}

	int  LoadPlugin(LPCWSTR lpPathToPlugin) override;
	int  CheckPlugin(LPCWSTR lpPathToPlugin, LPBOOL isFactoryEnable)  override;
	void UnloadPlugin() override { if (hPlugin) { FreeLibrary((HMODULE)hPlugin); hPlugin = nullptr; } }

	void ResumePlugin() override;
	void SuspendPlugin() override;

	void CreatePluginWindow() override;
	void DestroyPluginWindow() override;
	void OpenPluginWindow() override;
	void ClosePluginWindow() override;

	int  InitPlugin(DWORD dwSampleRate, DWORD dwBlockSize) override;
	void ProcessAudio(float** pAudioInput, float** pAudioOutput, DWORD dwSampleFrames) override;

	~IWin32VSTHost()
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
#else

#endif

typedef struct PluginVst2xIdMembers
{
	DWORD dwId;
	CHAR* szIdString;
} *PluginVst2xId, VST2_PLUGINHOST, *PVST2_PLUGINHOST;

#ifdef WIN32
extern VSTHOST_API HANDLE hPluginHandle;
extern VSTHOST_API IWin32VSTHost PluginHost;
#else

#endif

//BOOL CheckPluginList(VSTHost* pHost, LPCWSTR lpPluginFolder, VSTPLUGIN_INFO* pPluginInfo, size_t StructSizes);
