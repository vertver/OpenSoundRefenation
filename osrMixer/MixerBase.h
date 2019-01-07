/*********************************************************
* Copyright (C) VERTVER, 2019. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR Mixer base 
**********************************************************
* MixerBase.h
* Master-include mixer header
*********************************************************/
#pragma once
#include "OSR.h"
#include "OSRSystem.h"

typedef enum
{
	SoundData,
	DeviceOutputData,
	DeviceInputData,
	DevicePacketData		// pack data with zlib
} PacketType;

typedef enum 
{
	AcceptData,
	DiscardData,
	StartRecording,
	StopRecording,
	FlushBuffers
} InputData;

typedef enum
{
	AcceptData,
	DiscardData,
	StartPlay,	
	StopPlay,				// is not equ to "StopHost" function: just put 0 if this packet was accepted
	FlushBuffers
} OutputData;

typedef enum HostDeviceType
{
	InputDevice = 0,
	OutputDevice = 1,
	SpecialDevice = 2,		// for OSRHostProc.sys
	UnknownType = u8(-1)
};

typedef struct
{
	HostDeviceType DeviceType;
	HostFormatType FormatType;
	STRING256 DeviceName;		// UTF-8 only
	u32 BufferSize;				// in frames
	i32 DeviceNumber;			// current device number
} AUDIO_HOST;

/**************************************************
* RecvPacket() - Receive packet to output/input
***************************************************
* to send packet to input or output (like accept
* new data), include "DeviceInputData" or 
* "DeviceOutputData" packet type, and set pData 
* pointer to variable with current state.
***************************************************/
class ISoundInterface : public IObject
{
	virtual OSRCODE EnumerateAudioInput(AUDIO_HOST** pHostsList, u32& HostCounts) = 0;
	virtual OSRCODE EnumerateAudioOutputs(AUDIO_HOST** pHostsList, u32& HostCounts) = 0;

	virtual OSRCODE GetHostDeviceInInfo(u32 DeviceNumber, AUDIO_HOST& HostInfo) = 0;
	virtual OSRCODE GetHostDeviceOutInfo(u32 DeviceNumber, AUDIO_HOST& HostInfo) = 0;
	virtual OSRCODE GetHostDevicesInCount(u32& DeviceCount) = 0;
	virtual OSRCODE GetHostDevicesOutCount(u32& DeviceCount) = 0;
	virtual OSRCODE GetVolumeLevel(f32& Volume) = 0;
	virtual OSRCODE GetDefaultDevice(u32& DeviceNumber) = 0;

	virtual OSRCODE SetDelayLevel(f64 HostDelay) = 0;
	virtual OSRCODE SetVolumeLevel(f32 Volume) = 0;

	virtual OSRCODE CreateRenderSoundDevice(f64 HostDelay, u32 DeviceNumber) = 0;
	virtual OSRCODE CreateCaptureSoundDevice(f64 HostDelay, u32 DeviceNumber) = 0;

	virtual OSRCODE CreateRenderDefaultSoundDevice(f64 HostDelay) = 0;
	virtual OSRCODE CreateCaptureDefaultSoundDevice(f64 HostDelay) = 0;

	virtual OSRCODE RestartRenderSoundDevice(f64 HostDelay, u32 DeviceNumber) = 0;
	virtual OSRCODE RestartCaptureSoundDevice(f64 HostDelay, u32 DeviceNumber) = 0;

	virtual OSRCODE RestartRenderDefaultSoundDevice(f64 HostDelay) = 0;
	virtual OSRCODE RestartCaptureDefaultSoundDevice(f64 HostDelay) = 0;

	virtual OSRCODE CloseRenderSoundDevice() = 0;
	virtual OSRCODE CloseCaptureSoundDevice() = 0;

	virtual OSRCODE PlayHost() = 0;
	virtual OSRCODE StopHost() = 0;

	virtual OSRCODE RecvPacket(void* pData, PacketType Type, size_t DataSize) = 0;
	virtual OSRCODE GetPacket(void*& pData, PacketType Type, size_t& DataSize) = 0;

	virtual OSRCODE GetLoadBuffer(void*& pData, size_t& BufferSize) = 0;
};

#ifdef WIN32
__forceinline 
HostFormatType 
ConvertToSingleFormat(
	WAVEFORMATEX waveFormat
)
{
	switch (waveFormat.nSamplesPerSec)
	{
	case 22050:
		switch (waveFormat.wBitsPerSample)
		{
		case 8:
			if (waveFormat.nChannels == 1)
			{
				return MONO_22050_U8;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_22050_U8;
			}
			break;

		case 16:
			if (waveFormat.nChannels == 1)
			{
				return MONO_22050_S16;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_22050_S16;
			}
			break;

		case 24:
			if (waveFormat.nChannels == 1)
			{
				return MONO_22050_S24;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_22050_S24;
			}
			break;

		case 32:
			if (waveFormat.wFormatTag == 1)
			{
				if (waveFormat.nChannels == 1)
				{
					return MONO_22050_S32;
				}
				else if (waveFormat.nChannels == 2)
				{
					return STEREO_22050_S32;
				}
			}
			else
			{
				if (waveFormat.nChannels == 1)
				{
					return MONO_22050_F32;
				}
				else if (waveFormat.nChannels == 2)
				{
					return STEREO_22050_F32;
				}
			}
			break;

		default:
			break;
		}
		break;
	case 44100:
		switch (waveFormat.wBitsPerSample)
		{
		case 8:
			if (waveFormat.nChannels == 1)
			{
				return MONO_44100_U8;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_44100_U8;
			}
			break;

		case 16:
			if (waveFormat.nChannels == 1)
			{
				return MONO_44100_S16;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_44100_S16;
			}
			break;

		case 24:
			if (waveFormat.nChannels == 1)
			{
				return MONO_44100_S24;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_44100_S24;
			}
			break;

		case 32:
			if (waveFormat.wFormatTag == 1)
			{
				if (waveFormat.nChannels == 1)
				{
					return MONO_44100_S32;
				}
				else if (waveFormat.nChannels == 2)
				{
					return STEREO_44100_S32;
				}
			}
			else
			{
				if (waveFormat.nChannels == 1)
				{
					return MONO_44100_F32;
				}
				else if (waveFormat.nChannels == 2)
				{
					return STEREO_44100_F32;
				}
			}
			break;

		default:
			break;
		}
		break;

	case 88200:
		switch (waveFormat.wBitsPerSample)
		{
		case 8:
			if (waveFormat.nChannels == 1)
			{
				return MONO_88200_U8;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_88200_U8;
			}
			break;

		case 16:
			if (waveFormat.nChannels == 1)
			{
				return MONO_88200_S16;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_88200_S16;
			}
			break;

		case 24:
			if (waveFormat.nChannels == 1)
			{
				return MONO_88200_S24;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_88200_S24;
			}
			break;

		case 32:
			if (waveFormat.wFormatTag == 1)
			{
				if (waveFormat.nChannels == 1)
				{
					return MONO_88200_S32;
				}
				else if (waveFormat.nChannels == 2)
				{
					return STEREO_88200_S32;
				}
			}
			else
			{
				if (waveFormat.nChannels == 1)
				{
					return MONO_88200_F32;
				}
				else if (waveFormat.nChannels == 2)
				{
					return STEREO_88200_F32;
				}
			}
			break;

		default:
			break;
		}
		break;

	case 96000:
		switch (waveFormat.wBitsPerSample)
		{
		case 8:
			if (waveFormat.nChannels == 1)
			{
				return MONO_96000_U8;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_96000_U8;
			}
			break;

		case 16:
			if (waveFormat.nChannels == 1)
			{
				return MONO_96000_S16;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_96000_S16;
			}
			break;

		case 24:
			if (waveFormat.nChannels == 1)
			{
				return MONO_96000_S24;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_96000_S24;
			}
			break;

		case 32:
			if (waveFormat.wFormatTag == 1)
			{
				if (waveFormat.nChannels == 1)
				{
					return MONO_96000_S32;
				}
				else if (waveFormat.nChannels == 2)
				{
					return STEREO_96000_S32;
				}
			}
			else
			{
				if (waveFormat.nChannels == 1)
				{
					return MONO_96000_F32;
				}
				else if (waveFormat.nChannels == 2)
				{
					return STEREO_96000_F32;
				}
			}
			break;

		default:
			break;
		}
		break;

	case 192000:
		switch (waveFormat.wBitsPerSample)
		{
		case 8:
			if (waveFormat.nChannels == 1)
			{
				return MONO_192000_U8;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_192000_U8;
			}
			break;

		case 16:
			if (waveFormat.nChannels == 1)
			{
				return MONO_192000_S16;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_192000_S16;
			}
			break;

		case 24:
			if (waveFormat.nChannels == 1)
			{
				return MONO_192000_S24;
			}
			else if (waveFormat.nChannels == 2)
			{
				return STEREO_192000_S24;
			}
			break;

		case 32:
			if (waveFormat.wFormatTag == 1)
			{
				if (waveFormat.nChannels == 1)
				{
					return MONO_192000_S32;
				}
				else if (waveFormat.nChannels == 2)
				{
					return STEREO_192000_S32;
				}
			}
			else
			{
				if (waveFormat.nChannels == 1)
				{
					return MONO_192000_F32;
				}
				else if (waveFormat.nChannels == 2)
				{
					return STEREO_192000_F32;
				}
			}
			break;

		default:
			break;
		}
		break;
	default:
		break;
	}
}

__forceinline
WAVEFORMATEX
ConvertToWaveFormat(
	HostFormatType HostFormat
)
{
	WAVEFORMATEX waveFormat = { 0 };
	switch (HostFormat)
	{
	case MONO_22050_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 22050;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_22050_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 22050;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_22050_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 22050;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_22050_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 22050;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_22050_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 22050;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 3;
		break;
	case STEREO_22050_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 22050;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_22050_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 22050;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_22050_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 22050;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_22050_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 22050;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_22050_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 22050;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 3;
		break;

	case MONO_44100_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_44100_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_44100_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_44100_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_44100_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 3;
		break;
	case STEREO_44100_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_44100_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_44100_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_44100_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_44100_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 3;
		break;

	case MONO_48000_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 48000;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_48000_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 48000;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_48000_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 48000;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_48000_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 48000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_48000_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 48000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 3;
		break;
	case STEREO_48000_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 48000;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_48000_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 48000;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_48000_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 48000;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_48000_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 48000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_48000_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 48000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 3;
		break;

	case MONO_88200_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 88200;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_88200_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 88200;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_88200_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 88200;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_88200_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 88200;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_88200_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 88200;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 3;
		break;
	case STEREO_88200_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 88200;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_88200_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 88200;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_88200_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 88200;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_88200_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 88200;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_88200_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 88200;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 3;
		break;

	case MONO_96000_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 96000;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_96000_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 96000;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_96000_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 96000;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_96000_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 96000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_96000_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 96000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 3;
		break;
	case STEREO_96000_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 96000;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_96000_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 96000;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_96000_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 96000;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_96000_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 96000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_96000_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 96000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 3;
		break;

	case MONO_192000_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 192000;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_192000_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 192000;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_192000_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 192000;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_192000_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 192000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case MONO_192000_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 1;
		waveFormat.nSamplesPerSec = 192000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 3;
		break;
	case STEREO_192000_U8:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 192000;
		waveFormat.wBitsPerSample = 8;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_192000_S16:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 192000;
		waveFormat.wBitsPerSample = 16;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_192000_S24:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 192000;
		waveFormat.wBitsPerSample = 24;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_192000_S32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 192000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 1;
		break;
	case STEREO_192000_F32:
		waveFormat.cbSize = sizeof(WAVEFORMATEX);
		waveFormat.nChannels = 2;
		waveFormat.nSamplesPerSec = 192000;
		waveFormat.wBitsPerSample = 32;
		waveFormat.wFormatTag = 3;
		break;
	default:
		break;
	}

	waveFormat.nBlockAlign = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
}
#endif