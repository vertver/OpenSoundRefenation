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

		for (u8 i = 0; i < 8; i++)
		{
			pInputBuffer[i] = nullptr;
			pOutputBuffer[i] = nullptr;
		}

		pNextSample = nullptr;
		pPreviousSample = nullptr;
	}

	OSRSample(u8 Bits, u8 Channels, u32 BufferSize, u32 SampleRate, void* pData, u32 DataSize)
	{
		i8* p8 = nullptr;
		i16* p16 = nullptr;
		i24* p24 = nullptr;
		f32* pf32 = nullptr;

		for (u32 i = 0; i < Channels; i++)
		{
			if (!pOutputBuffer[i]) { pOutputBuffer[i] = (f32*)FastAlloc(BufferSize * 4); }
		}

		switch (Bits)
		{
		case 8:
			p8 = (i8*)pData;

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

	void LoadSample(void* pData, u32 BufferSize, u8 Bits, u8 Channels, u32 SampleRate)
	{
		i8* p8 = nullptr;
		i16* p16 = nullptr;
		i24* p24 = nullptr;
		f32* pf32 = nullptr;

		for (u32 i = 0; i < Channels; i++)
		{
			if (!pOutputBuffer[i]) { pOutputBuffer[i] = (f32*)FastAlloc(BufferSize * 4); }
		}

		switch (Bits)
		{
		case 8:
			p8 = (i8*)pData;

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

#ifdef WIN32
	void _loadsample(void* pData, u32 BufferSize, u8 Bits, u8 Channels, u32 SampleRate)
	{
		__m128i xmmLoad;
		__m128i xmmConvert;

		for (u32 i = 0; i < Channels; i++)
		{
			if (!pOutputBuffer[i]) { pOutputBuffer[i] = (f32*)FastAlloc(BufferSize * 4); }
		}

		switch (Bits)
		{
		case 8:
			for (u32 i = 0; i < BufferSize; i++)
			{
				xmmLoad = _mm_loadu_si128(reinterpret_cast<__m128i*>((size_t*)pData + (i * sizeof(__m128i))));
			}
			break;
		default:
			break;
		}
	}
#endif

	void FreeSample()
	{
		for (u32 i = 0; i < 8; i++)
		{
			FREEKERNELHEAP(pOutputBuffer[i]);
			FREEKERNELHEAP(pInputBuffer[i]);
		}
	}

	~OSRSample()
	{
		for (u32 i = 0; i < 8; i++)
		{
			FREEKERNELHEAP(pOutputBuffer[i]);
			FREEKERNELHEAP(pInputBuffer[i]);
		}
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

	f32* pInputBuffer[8];
	f32* pOutputBuffer[8];

	OSRSample* pNextSample;
	OSRSample* pPreviousSample;
};

