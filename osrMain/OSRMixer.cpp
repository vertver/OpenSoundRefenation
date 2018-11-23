#include "stdafx.h"
#include "IMWASAPI.h"

XEngine XAudioEngine;
IMEngine eEngine;
LOOP_INFO lInfo;

void
OSR::Mixer::CreateMixer(
	HWND hwnd
)
{
	eEngine.InitEngine(hwnd);
	eEngine.CreateDefaultDevice();

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

	eEngine.StartDevice((LPVOID)&SampleProc);
}
