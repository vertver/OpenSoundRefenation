#include "stdafx.h"

DLL_API XEngine XAudioEngine;

void
OSR::Mixer::CreateMixer(
	HWND hwnd,
	OSRSample Sample
)
{
	WAVEFORMATEX waveFormat = { NULL };
	waveFormat.cbSize = sizeof(WAVEFORMATEX);
	waveFormat.nAvgBytesPerSec = (Sample.BitsOutput * Sample.ChannelsOutput * Sample.SampleRateOutput) / 8;
	waveFormat.nBlockAlign = 4;
	waveFormat.nChannels = Sample.ChannelsOutput;
	waveFormat.nSamplesPerSec = Sample.SampleRateOutput;
	waveFormat.wBitsPerSample = Sample.BitsOutput;
	waveFormat.wFormatTag = 3;

	if (!hwnd)
	{
	
	}
}

void
OSR::Mixer::LoadSample(
	LPCWSTR lpPath
)
{
	LOOP_INFO lInfo = { NULL };
	osrEngine.DecodeFile(lpPath, &lInfo);

	WAVEFORMATEX waveFormat = lInfo.waveFormat;
	waveFormat.wBitsPerSample = 32;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.wFormatTag = 3;
	waveFormat.nAvgBytesPerSec = (waveFormat.nSamplesPerSec * waveFormat.wBitsPerSample * waveFormat.nChannels) / 8;

	XPlay play(waveFormat, lInfo);

	if (!XAudioEngine.pXAudio)
	{
		XAudioEngine.CreateXEngine(play);
	}

	if (XAudioEngine.lpSourceVoice)
	{
		XAudioEngine.lpSourceVoice->DestroyVoice();
		XAudioEngine.lpSourceVoice = nullptr;
	}
	LoopNumber = play.CurrentSampleCount;

	mixer.InitMixer(XAudioEngine);
}

void
OSR::Mixer::PlaySample()
{
	mixer.PlayAsync(LoopNumber);
}

