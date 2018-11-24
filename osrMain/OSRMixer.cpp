#include "stdafx.h"
#include "IMWASAPI.h"

XEngine XAudioEngine;
IMEngine eEngine;
LOOP_INFO lInfo;
HANDLE hStartEvent;

VOID
WINAPIV
DecodeFileProc(
	LPVOID pProc
)
{
	DECODE_STRUCT* pStruct = (DECODE_STRUCT*)pProc;
	
	DWORD SampleNumber = 0;
	pStruct->pEngine->loopList.LoadAudioFile(pStruct->lpPath, USE_LIBSNDFILE, 0, &SampleNumber);
	*pStruct->pLoopInfo = *(pStruct->pEngine->loopList.GetLoopInfo()->pSampleInfo);
	SetEvent(hStartEvent);
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

	hStartEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	threadS.CreateUserThread(nullptr, DecodeFileProc, &decodeStruct, L"OSR Decoder Worker");
}

void
OSR::Mixer::CreateMixer(
	HWND hwnd
)
{
	static VSTHost vstHost = {};
	pVSTHost = &vstHost;

	eEngine.InitEngine(hwnd);
	eEngine.CreateDefaultDevice(1000000);
	eEngine.pHost = (VSTHost*)pVSTHost;

	eEngine.pHost->LoadPlugin(L"I:\\VSTPlugins\\FabFilter Pro-Q 2 x64.dll");
	eEngine.pHost->InitPlugin(eEngine.GetOutputInfo()->waveFormat.nSamplesPerSec, eEngine.GetBufferSize() * eEngine.GetOutputInfo()->waveFormat.nChannels);

	/*WAVEFORMATEX waveFormat = { NULL };
	waveFormat.cbSize = sizeof(WAVEFORMATEX);
	waveFormat.nAvgBytesPerSec = (Sample.BitsOutput * Sample.ChannelsOutput * Sample.SampleRateOutput) / 8;
	waveFormat.nBlockAlign = 4;
	waveFormat.nChannels = Sample.ChannelsOutput;
	waveFormat.nSamplesPerSec = Sample.SampleRateOutput;
	waveFormat.wBitsPerSample = Sample.BitsOutput;
	waveFormat.wFormatTag = 3;

	if (!hwnd)
	{

	}*/
}

void
OSR::Mixer::LoadSample(
	LPCWSTR lpPath
)
{
	osrEngine.DecodeFile(lpPath, &lInfo);

	//WAVEFORMATEX waveFormat = lInfo.waveFormat;
	//waveFormat.wBitsPerSample = 32;
	//waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	//waveFormat.wFormatTag = 3;
	//waveFormat.nAvgBytesPerSec = (waveFormat.nSamplesPerSec * waveFormat.wBitsPerSample * waveFormat.nChannels) / 8;

	//XPlay play(waveFormat, lInfo);

	//if (!XAudioEngine.pXAudio)
	//{
	//	XAudioEngine.CreateXEngine(play);
	//}

	//if (XAudioEngine.lpSourceVoice)
	//{
	//	XAudioEngine.lpSourceVoice->DestroyVoice();
	//	XAudioEngine.lpSourceVoice = nullptr;
	//}
	//LoopNumber = play.CurrentSampleCount;

	//mixer.InitMixer(XAudioEngine);
}

WASAPI_SAMPLE_PROC SampleProc = { 0 };

void
OSR::Mixer::PlaySample()
{
	//mixer.PlayAsync(LoopNumber);
	SampleProc.pEngine = &eEngine;
	SampleProc.pLoopInfo = &lInfo;
	SampleProc.pSample = nullptr;

	WaitForSingleObject(hStartEvent, INFINITE);
	eEngine.StartDevice((LPVOID)&SampleProc);
}

