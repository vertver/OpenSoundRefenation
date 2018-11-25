/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* OSRSystem.h
* Sample system implementation
*********************************************************/
#pragma once
#include "OSRTypes.h"
#include "OSR.h"

#ifdef WIN32
#include <intrin.h> 
#endif

typedef struct
{
	u32 StructSize;					// size of struct
	u32 SampleSize;					// size of current sample
	u32 SampleNumber;				// if 0 - random sample, else - numeric sample of audio file
	u64 SampleDuration;				// duration of sample (in msecs)
	WAVEFORMATEX waveFormat;		// sample format info
	u8* pSample;					// pointer to data of sample
} LOOP_INFO, *LPLOOP_INFO, *PLOOP_INFO;

typedef struct
{
	u32 SampleCount;				// count of all samples in 1 file
	u32 FileSize;					// file size (includes header information)
	u64 FileDuration;				// final file duration (include all samples duration)
	LPLOOP_INFO pSampleInfo;		// to count size of this struct - sizeof(LOOP_INFO) * dwSampleCount
} AUDIO_INFO, *LPAUDIO_INFO, *PAUDIO_INFO;

class OSRSample
{
public:
	OSRSample()
	{
		IsFloatInput = 0;
		IsFloatOutput = 0;
		BitsInput = 0;
		BitsOutput = 0;
		ChannelsInput = 0;
		ChannelsOutput = 0;
		BufferSizeInput = 0;
		BufferSizeOutput = 0;
		SampleRateInput = 0;
		SampleRateOutput = 0;

		for (u8 i = 0; i < 8; i++)
		{
			pInputBuffer[i] = nullptr;
			pOutputBuffer[i] = nullptr;
		}

		pNextSample = nullptr;
		pPreviousSample = nullptr;
	}

	OSRSample(u8 Bits, u8 Channels, u32 BufferSize, u32 SampleRate)
	{
		IsFloatInput = (!!(Bits >= 32));
		IsFloatOutput = (!!(Bits >= 32));

		BitsInput = Bits;
		BitsOutput = Bits;
		ChannelsInput = Channels;
		ChannelsOutput = Channels;
		BufferSizeInput = BufferSize;
		BufferSizeOutput = BufferSize;
		SampleRateInput = SampleRate;
		SampleRateOutput = SampleRate;
		ToEndFileSize = NULL;
		SamplePosition = NULL;

		for (u8 i = 0; i < 8; i++)
		{
			pInputBuffer[i] = nullptr;
			pOutputBuffer[i] = nullptr;
		}

		pNextSample = nullptr;
		pPreviousSample = nullptr;
	}

	void LoadSample(void* pData, u32 BufferSize, u8 Bits, u8 Channels, u32 SampleRate)
	{
		u8* p8 = nullptr;
		i16* p16 = nullptr;
		i24* p24 = nullptr;
		f32* pf32 = nullptr;

		for (u32 i = 0; i < Channels; i++)
		{
			if (!pOutputBuffer[i]) { pOutputBuffer[i] = (f32*)AdvanceAlloc(BufferSize * sizeof(f32), NULL); }
		}

		switch (Bits)
		{
		case 8:
			p8 = (u8*)pData;

			for (u32 i = 0; i < BufferSize; i++)
			{
				pOutputBuffer[i % Channels][i / Channels] = i16tof32((i16)p8[i]);
			}
			break;
		case 16:
			p16 = (i16*)pData;

			for (u32 i = 0; i < BufferSize; i++)
			{
				pOutputBuffer[i % Channels][i / Channels] = i16tof32(p16[i]);
			}
			break;
		case 24:
			p24 = (i24*)pData;

			for (u32 i = 0; i < BufferSize; i++)
			{
				pOutputBuffer[i % Channels][i / Channels] = i24tof32(p24[i]);
			}
			break;
		case 32:
			pf32 = (f32*)pData;

			for (u32 i = 0; i < BufferSize; i++)
			{
				pOutputBuffer[i % Channels][i / Channels] = pf32[i];
			}
			break;
		}

		IsFloatInput = (!!(Bits >= 32));
		IsFloatOutput = (!!(Bits >= 32));

		BitsInput = Bits;
		BitsOutput = Bits;
		ChannelsInput = Channels;
		ChannelsOutput = Channels;
		BufferSizeInput = BufferSize;
		BufferSizeOutput = BufferSize;
		SampleRateInput = SampleRate;
		SampleRateOutput = SampleRate;
	}

	OSRSample* OnBufferEnd(LPLOOP_INFO pLoop)
	{
		void* pData = nullptr;
		bool isEnd = false;

		if (!BitsOutput)
		{
			BitsOutput = (BYTE)pLoop->waveFormat.wBitsPerSample;
			ChannelsOutput = (BYTE)pLoop->waveFormat.nChannels;
			SampleRateOutput = pLoop->waveFormat.nSamplesPerSec;
			BufferSizeOutput = 16384;
		}

		if (!pNextSample) 
		{ 
			pNextSample = new OSRSample(BitsOutput, ChannelsOutput, BufferSizeOutput, SampleRateOutput); 
			pNextSample->pPreviousSample = this;

			if (!ToEndFileSize && !SamplePosition)  { ToEndFileSize = pLoop->SampleSize; }
			pNextSample->SamplePosition = SamplePosition + (BufferSizeOutput * (BitsOutput / 8));
			pNextSample->ToEndFileSize = ToEndFileSize - (BufferSizeOutput * (BitsOutput / 8));
		}

		pData = (void*)ptrdiff_t(pLoop->pSample + pNextSample->SamplePosition);

		if (pNextSample->ToEndFileSize <= 0) 
		{
			isEnd = true;
		}

		if (pNextSample->ToEndFileSize > (BufferSizeOutput * (BitsOutput / 8)) && !isEnd)
		{
			pNextSample->LoadSample(pData, BufferSizeOutput, BitsOutput, ChannelsOutput, SampleRateOutput);
		}
		else
		{	
			u8 bufFloat[44100 * 2 * 4] = { NULL };

			if (pNextSample->ToEndFileSize > 0) { memcpy(bufFloat, pData, pNextSample->ToEndFileSize); }
			pNextSample->LoadSample(bufFloat, BufferSizeOutput, BitsOutput, ChannelsOutput, SampleRateOutput);
		}

		return pNextSample;
	}

	void ConvertToPlay(void* pOutData)
	{
		f32* pOutBuf = (f32*)pOutData;

		if (pOutData)
		{
			for (u32 i = 0; i < BufferSizeOutput; i++)
			{
				pOutBuf[i] = pOutputBuffer[i % ChannelsOutput][i / ChannelsOutput];
			}
		}
	}

	void ConvertToPlay(void* pOutData, u8 Bits)
	{
		u8* p8 = nullptr;
		i16* p16 = nullptr;
		i24* p24 = nullptr;
		f32* pf32 = nullptr;

		switch (Bits)
		{
		case 8:
			p8 = (u8*)pOutData;

			for (u32 i = 0; i < BufferSizeOutput; i++)
			{
				p8[i] = (u8)f32toi16(pOutputBuffer[i % ChannelsOutput][i / ChannelsOutput]);
			}
			break;
		case 16:
			p16 = (i16*)pOutData;

			for (u32 i = 0; i < BufferSizeOutput; i++)
			{
				p16[i] = f32toi16(pOutputBuffer[i % ChannelsOutput][i / ChannelsOutput]);
			}
			break;
		case 24:
			p24 = (i24*)pOutData;

			for (u32 i = 0; i < BufferSizeOutput; i++)
			{
				p24[i] = f32toi24(pOutputBuffer[i % ChannelsOutput][i / ChannelsOutput]);
			}
			break;
		case 32:
			pf32 = (f32*)pOutData;

			for (u32 i = 0; i < BufferSizeOutput; i++)
			{
				pf32[i] = pOutputBuffer[i % ChannelsOutput][i / ChannelsOutput];
			}
			break;
		}
	}

	void FreeSample()
	{
		for (u32 i = 0; i < 8; i++)
		{
			FREEPROCESSHEAP(pOutputBuffer[i]);
			FREEPROCESSHEAP(pInputBuffer[i]);
		}
	}

	~OSRSample()
	{
		FreeSample();
	}

	u8 BitsInput;
	u8 BitsOutput;
	u8 IsFloatInput;
	u8 IsFloatOutput;
	u8 ChannelsInput;
	u8 ChannelsOutput;
	u32 BufferSizeInput;
	u32 BufferSizeOutput;
	u32 SampleRateInput;
	u32 SampleRateOutput;
	u64 SamplePosition; 
	i32 ToEndFileSize;

	f32* pInputBuffer[8];
	f32* pOutputBuffer[8];

	OSRSample* pNextSample;
	OSRSample* pPreviousSample;
};

class OSRSampleEx : public OSRSample
{
	u32 CurrentSystem;
	u8 IsBigEndian;
};
