#include "stdafx.h"
#include "IMWASAPI.h"
#include "WaveOutHeader.h"

MMEngine mmEngine;
IMEngine eEngine;
LOOP_INFO lInfo;
HANDLE hStartEvent;
DWORD SamplesCount = 0;
size_t uPlay;

extern DLL_API bool IsBlur;
extern DLL_API bool IsLoad; 

VOID
WINAPIV
DecodeFileProc(
	LPVOID pProc
)
{
	IsLoad = true;
	DECODE_STRUCT* pStruct = (DECODE_STRUCT*)pProc;
	
	DWORD SampleNumber = 0;
	pStruct->pEngine->loopList.LoadAudioFile(pStruct->lpPath, USE_LIBSNDFILE, 0, &SampleNumber);
	*pStruct->pLoopInfo = *(pStruct->pEngine->loopList.GetLoopInfo()->pSampleInfo);
	SetEvent(hStartEvent);
	IsLoad = false;
} 

void
OSR::Engine::DecodeFile(
	LPCWSTR lpPath,
	LPLOOP_INFO pLoopInfo
)
{
	static ThreadSystem threadS = {};
	static DECODE_STRUCT decodeStruct = { 0 };
	static WSTRING_PATH szPath = { 0 };

	memcpy(szPath, lpPath, sizeof(WSTRING_PATH));

	decodeStruct.pEngine = this;
	decodeStruct.lpPath = szPath;
	decodeStruct.pLoopInfo = pLoopInfo;

	hStartEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	threadS.CreateUserThread(nullptr, DecodeFileProc, &decodeStruct, L"OSR Decoder Worker");
}

#define TEST_VST 1

void
OSR::Mixer::CreateMixer(
	HWND hwnd
)
{
	WSTRING_PATH pathToDll = { 0 };

	eEngine.InitEngine(hwnd);
	eEngine.CreateDefaultDevice(300000);

#ifdef TEST_VST
	static VSTHost vstHost = {};
	pVSTHost = &vstHost;
	eEngine.pHost = (VSTHost*)pVSTHost;
	if (OSRSUCCEDDED(OpenFileDialog(&pathToDll)))
	{
		eEngine.pHost->LoadPlugin(pathToDll);
		eEngine.pHost->InitPlugin(eEngine.GetOutputInfo()->waveFormat.nSamplesPerSec, eEngine.GetBufferSize() * eEngine.GetOutputInfo()->waveFormat.nChannels);
	}
#endif
}

void
OSR::Mixer::LoadSample(
	LPCWSTR lpPath
)
{
	osrEngine.DecodeFile(lpPath, &lInfo);
	uPlay = 0;
}

WASAPI_SAMPLE_PROC SampleProc = { nullptr };
WAVOUT_SAMPLE_PROC MProc = { nullptr };

void
OSR::Mixer::PlaySample()
{
	SampleProc.pEngine = &eEngine;
	SampleProc.pLoopInfo = &lInfo;
	SampleProc.pSample = nullptr;

	if (uPlay)
	{
		eEngine.RestartDevice(1000000);
	}

	WaitForSingleObject(hStartEvent, INFINITE);
	eEngine.StartDevice((LPVOID)&SampleProc);

	uPlay++;
}

void
OSR::Mixer::StopSample()
{
	eEngine.StopDevice();
}