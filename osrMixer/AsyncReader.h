/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR loop system
**********************************************************
* AsyncReader.h
* Acync audio reader implementation
*********************************************************/
#pragma once
#include "OSR.h"
#include "LoopList.h"
#include "OSRVST.h"

inline
void
ProcessAudio(
	float** pInput,
	float** pOutput,
	DWORD SampleRate,
	DWORD BufferSize,
	DWORD Channels,
	VSTHost* pHost
)
{
	float* pCustomOutput[8] = { nullptr };
	float* pCustomInput[8] = { nullptr };

	for (size_t i = 0; i < Channels * 2; i++)
	{
		pCustomOutput[i] = (float*)FastAlloc(BufferSize * sizeof(f32));
		pCustomInput[i] = (float*)FastAlloc(BufferSize * sizeof(f32));

#ifndef WIN32
		memset(pCustomInput[i], 0, BufferSize * sizeof(f32));
		memset(pCustomOutput[i], 0, BufferSize * sizeof(f32));
#endif
	}

	for (size_t i = 0; i < Channels; i++)
	{
		memcpy(pCustomInput[i], pInput[i], BufferSize * sizeof(f32));
	}

	// process function
	if (!pHost) { return; }
	pHost->ProcessAudio(pCustomInput, pCustomOutput, BufferSize / Channels);

	for (size_t i = 0; i < Channels; i++)
	{
		memcpy(pOutput[i], pCustomOutput[i], BufferSize * sizeof(f32));
	}

	for (size_t i = 0; i < Channels * 2; i++)
	{
		FREEKERNELHEAP(pCustomOutput[i]);
		FREEKERNELHEAP(pCustomInput[i]);
	}
}


class DLL_API AsyncReader
{
public:
	AsyncReader()
	{
		CurrentSample = nullptr;

		for (size_t i = 0; i < 128; i++)
		{
			SamplesArray[i] = nullptr;
		}

		memset(&loopInfo, 0, sizeof(LOOP_INFO));
	}

	~AsyncReader()
	{

		DWORD dwSampleN = SampleNum + 1;
		if (dwSampleN > 127) { dwSampleN = 127; }
		for (size_t i = 0; i < dwSampleN; i++)
		{
			if (SamplesArray[i])
			{
				delete SamplesArray[i];
				SamplesArray[i] = nullptr;
			}
		}
	}

	OSRSample* FirstInit(LPLOOP_INFO LoopInfo, u32 BufferSize, u32 SampleRate, u8 Channels);
	OSRSample* RenderAudio(DWORD SampleNumber);
	VOID SetNumber(DWORD SampleNumber) { SampleNum = SampleNumber; }

private: 
	DWORD SampleNum;
	OSRSample* SamplesArray[128];
	LOOP_INFO loopInfo;
	OSRSample* CurrentSample;
};
