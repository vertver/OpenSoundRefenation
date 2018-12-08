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

void
OSR::Mixer::CreateMixer(
	HWND hwnd
)
{
	if (false)
	{
		static VSTHost vstHost = {};
		pVSTHost = nullptr;//&vstHost;

		eEngine.InitEngine(hwnd);
		eEngine.CreateDefaultDevice(1000000);
		eEngine.pHost = (VSTHost*)pVSTHost;
	}

	mmEngine.InitEngine(hwnd);
	mmEngine.CreateDefaultDevice(100);

	//eEngine.pHost->LoadPlugin(L"I:\\VSTPlugins\\FabFilter Pro-Q 2 x64.dll");
	//eEngine.pHost->InitPlugin(eEngine.GetOutputInfo()->waveFormat.nSamplesPerSec, eEngine.GetBufferSize() * eEngine.GetOutputInfo()->waveFormat.nChannels);
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
	static size_t Sample1 = 0;

	MProc.pEngine = &mmEngine;
	MProc.pLoopInfo = &lInfo;
	MProc.pSample = nullptr;

	SampleProc.pEngine = &eEngine;
	SampleProc.pLoopInfo = &lInfo;
	SampleProc.pSample = nullptr;

	if (false)
	{
		if (uPlay)
		{
			eEngine.RestartDevice(1000000);
		}

		WaitForSingleObject(hStartEvent, INFINITE);
		eEngine.StartDevice((LPVOID)&SampleProc);
	}

	mmEngine.StartDevice(&MProc);
	uPlay++;
}

void
OSR::Mixer::StopSample()
{
	eEngine.StopDevice();
}