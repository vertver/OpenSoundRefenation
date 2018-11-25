/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR WASAPI
**********************************************************
* IMWASAPIKernel.cpp
* WASAPI kernel implementation
*********************************************************/
#include "stdafx.h"
#include "IMWASAPI.h"

DLL_API HANDLE hThreadExitEvent = NULL;
DLL_API HANDLE hThreadLoadSamplesEvent = NULL;

OSRSample* SampleArray[128] = { nullptr };

DWORD
WINAPIV
WASAPIThreadProc(LPVOID pParam)
{
	// blockalign = channels * bits / 8
	if (!hThreadExitEvent) hThreadExitEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
	if (!hThreadLoadSamplesEvent) hThreadLoadSamplesEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
	
	HRESULT hr = 0;
	DWORD dwTask = 0;
	DWORD dwSampleNumber = 0;
	HANDLE hMMCSS = NULL;
	BOOL isPlaying = TRUE;
	OSRSample* Sample = nullptr;
	HANDLE hHandlesArray[] = { hThreadExitEvent, hThreadLoadSamplesEvent };
	WASAPI_SAMPLE_PROC* pProc = reinterpret_cast<WASAPI_SAMPLE_PROC*>(pParam);
	static BYTE* pData = nullptr;

	DWORD dwChannels = pProc->pEngine->GetOutputInfo()->waveFormat.nChannels;
	DWORD dwBits = pProc->pEngine->GetOutputInfo()->waveFormat.wBitsPerSample;
	DWORD dwSampleRate = pProc->pEngine->GetOutputInfo()->waveFormat.nSamplesPerSec;
	DWORD BlockAlign = pProc->pEngine->GetOutputInfo()->waveFormat.nBlockAlign;
	DWORD FrameSize = 0;
	pProc->pEngine->pAudioClient->GetBufferSize((UINT32*)&FrameSize);

	pData = (BYTE*)FastAlloc(FrameSize * dwChannels * sizeof(f32));

	Sample = pProc->pSample;

	if (!Sample && pProc->pLoopInfo)
	{
		Sample = new OSRSample(
			pProc->pLoopInfo->waveFormat.wBitsPerSample,
			pProc->pLoopInfo->waveFormat.nChannels,
			FrameSize * dwChannels,
			pProc->pLoopInfo->waveFormat.nSamplesPerSec
		);

		SampleArray[0] = Sample;

		Sample->LoadSample(
			(void*)pProc->pLoopInfo->pSample,
			FrameSize * dwChannels,
			pProc->pLoopInfo->waveFormat.wBitsPerSample,
			pProc->pLoopInfo->waveFormat.nChannels,
			pProc->pLoopInfo->waveFormat.nSamplesPerSec
		);
	}
	else if (Sample && !pProc->pLoopInfo)
	{
		// play only this sample. No more
	}
	else if (Sample && pProc->pLoopInfo)
	{
		// do nothing
	}
	else
	{
		THROW1(L"Can't play audio because no loop info");
	}

	hMMCSS = AvSetMmThreadCharacteristicsW(L"Audio", &dwTask);
	ASSERT2(hMMCSS, L"Can't init MMCSS");

	AvSetMmThreadPriority(hMMCSS, AVRT_PRIORITY_CRITICAL);

	Sample->ConvertToPlay((VOID*)pData, 32);

	REFERENCE_TIME hndActual = 1000000 * FrameSize * dwSampleRate;

	while (isPlaying)
	{
		DWORD dwWait = WaitForMultipleObjects(2, hHandlesArray, FALSE, INFINITY);
		BYTE* pByte = nullptr;

		switch (dwWait)
		{
		case WAIT_OBJECT_0:
			isPlaying = FALSE;
			break;
		case WAIT_OBJECT_0 + 1:
		{
			DWORD dwPadding = 0;
			static size_t Samples = 0;

			DWORD dwSleep = hndActual / 100000000;
			Sleep((dwSleep / 2));

			hr = pProc->pEngine->pAudioClient->GetCurrentPadding((UINT32*)&dwPadding);

			DWORD dwFramesToWrite = FrameSize;

			if (SUCCEEDED(hr))
			{
				

				hr = pProc->pEngine->pAudioRenderClient->GetBuffer(dwFramesToWrite, &pByte);

				if (hr == AUDCLNT_E_BUFFER_TOO_LARGE) { continue; }

				if (pByte)
				{
					memcpy(pByte, pData, dwFramesToWrite * dwChannels * sizeof(f32));
				}

				hr = pProc->pEngine->pAudioRenderClient->ReleaseBuffer(dwFramesToWrite, 0);
			}
			
			if (FAILED(hr)) 
			{ 
				isPlaying = FALSE; 

				hr = pProc->pEngine->pAudioRenderClient->GetBuffer(dwFramesToWrite, &pByte);

				memset(pData, 0, dwFramesToWrite * dwChannels * sizeof(f32));

				if (pByte)
				{
					memcpy(pByte, pData, dwFramesToWrite * dwChannels * sizeof(f32));
				}

				hr = pProc->pEngine->pAudioRenderClient->ReleaseBuffer(dwFramesToWrite, 0);
			}
			else
			{
				if (dwSampleNumber > 127)
				{
					for (size_t i = 0; i < 127; i++)
					{
						if (SampleArray[i]) 
						{
							delete SampleArray[i]; SampleArray[i] = nullptr;
						}
					}

					SampleArray[0] = SampleArray[127];
					dwSampleNumber = 0;
				}

				SampleArray[dwSampleNumber + 1] = Sample->OnBufferEnd(pProc->pLoopInfo);
				dwSampleNumber++;
				Sample = SampleArray[dwSampleNumber];

				if (pProc->pEngine->pHost)
				{
					ProcessAudio(
						Sample->pOutputBuffer,
						Sample->pOutputBuffer,
						dwSampleRate,
						Sample->BufferSizeOutput,
						Sample->ChannelsOutput,
						pProc->pEngine->pHost
					);
				}

				Sample->ConvertToPlay(pData, 32);
				Samples++;
			}
		}
		break;
		default:
			break;
		}
	}

	for (size_t i = 0; i < dwSampleNumber; i++)
	{
		if (SampleArray[i])
		{
			delete SampleArray[i]; SampleArray[i] = nullptr;
		}
	}

	return 0;
}

OSRCODE
IMEngine::InitEngine(
	HWND hwnd
)
{
	DWORD dwInputState = 0;
	DWORD dwOutputState = 0;
	DWORD dwRenderCount = 0;
	IMMDeviceEnumerator* deviceEnumerator = nullptr;
	IMMDeviceCollection* pEndPoints = nullptr;

	{ 
		IMMDevice* pRenderer = nullptr;

		// create instance for enumerator
		if (SUCCEEDED(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator))))
		{
			// get default output device
			FAILEDX2(deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pRenderer));

			// get device id
			WCHAR* pDeviceId = nullptr;
			FAILEDX2(pRenderer->GetId(&pDeviceId));
			FAILEDX2(pRenderer->GetState(&dwOutputState));

			// copy to device info and free com pointer
			wcsncpy_s(OutputDeviceInfo.szDeviceId, pDeviceId, sizeof(WSTRING512) - 1);
			CoTaskMemFree(pDeviceId);
			_RELEASE(pRenderer);
		}
		else
		{
			return MXR_OSR_NO_OUT;
		}
	}

	{
		IMMDevice* pCapturer = nullptr;
		
		// get default input device
		if (SUCCEEDED(deviceEnumerator->GetDefaultAudioEndpoint(eCapture, eMultimedia, &pCapturer)))
		{
			// get device id
			WCHAR* pDeviceId = nullptr;
			FAILEDX2(pCapturer->GetId(&pDeviceId));
			FAILEDX2(pCapturer->GetState(&dwInputState));

			// copy to device info and free com pointer
			wcsncpy_s(InputDeviceInfo.szDeviceId, pDeviceId, sizeof(WSTRING512) - 1);
			CoTaskMemFree(pDeviceId);
			_RELEASE(pCapturer);
		}
	}

	// get count of output devices
	FAILEDX2(deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pEndPoints));
	FAILEDX2(pEndPoints->GetCount((UINT*)&dwRenderCount));
	_RELEASE(pEndPoints);

	// get count of all devices
	FAILEDX2(deviceEnumerator->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE, &pEndPoints));
	FAILEDX2(pEndPoints->GetCount((UINT*)&DeviceCount));
	DeviceCount += dwRenderCount;

	_RELEASE(pEndPoints);
	_RELEASE(deviceEnumerator);

	return OSR_SUCCESS;
}

static REFERENCE_TIME MakeHnsPeriod(UINT32 nFrames, DWORD nSamplesPerSec)
{
	return (REFERENCE_TIME)((10000.0 * 1000 / nSamplesPerSec * nFrames) + 0.5);
}

OSRCODE
IMEngine::CreateDefaultDevice(REFERENCE_TIME referTime)
{
	HRESULT hr = 0;	
	DWORD dwFrames = 0;	
	PROPVARIANT value = {};	
	IMMDevice* pRenderer = nullptr;
	REFERENCE_TIME refTime = 0;
	IPropertyStore *pProperty = nullptr;
	IMMDeviceEnumerator* deviceEnumerator = nullptr;

	// create instance for enumerator
	if (SUCCEEDED(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator))))
	{
		// get default device
		FAILEDX2(deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pRenderer));
	}
	else
	{
		return MXR_OSR_NO_OUT;
	}

	// activate audio client and get property to search a lot of information
	FAILEDX2(pRenderer->Activate(*GetAudioClientIID(), CLSCTX_ALL, nullptr, (LPVOID*)&pAudioClient));
	FAILEDX2(pRenderer->OpenPropertyStore(STGM_READ, &pProperty));
	
	// init variant to get WAVEFORMATEX
	PropVariantInit(&value);
	FAILEDX2(pProperty->GetValue(PKEY_AudioEngine_DeviceFormat, &value));

	// copy our WAVEFORMAT to device format
	memcpy(&OutputDeviceInfo.waveFormat, value.blob.pBlobData, min(sizeof(OutputDeviceInfo.waveFormat), value.blob.cbSize));
	PropVariantClear(&value);
	_RELEASE(pProperty);

	//#NOTE: can be failed on AC97. Be careful
	if (FAILED(pAudioClient->GetDevicePeriod(&OutputDeviceInfo.DefaultDevicePeriod, &OutputDeviceInfo.MinimumDevicePeriod)))
	{
		OutputDeviceInfo.DefaultDevicePeriod = 1000000;
		OutputDeviceInfo.MinimumDevicePeriod = 300000;
	}

	WAVEFORMATEX* closeFormat = nullptr;
	pAudioClient->GetMixFormat(&closeFormat);

	memcpy(&OutputDeviceInfo.waveFormat, closeFormat, sizeof(WAVEFORMATEX));

	// init audio client
	hr = pAudioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED, 
		0,
		referTime,		
		0,
		closeFormat,
		nullptr
	);

	// get current buffer size
	pAudioClient->GetBufferSize((UINT32*)&dwFrames);

	//#NOTE: can be triggered on Windows 7+ systems
	if (hr == AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED)
	{
		_RELEASE(pAudioClient);

		// activate audio client
		pRenderer->Activate(*GetAudioClientIID(), CLSCTX_ALL, nullptr, (LPVOID*)&pAudioClient);

		// create optimal period for device and init it
		refTime = MakeHnsPeriod(dwFrames, OutputDeviceInfo.waveFormat.nSamplesPerSec);
		hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, refTime, 0, closeFormat, nullptr);
		if (FAILED(hr)) { return MXR_OSR_NO_OUT; }
	}

	pAudioClient->GetBufferSize((UINT32*)&dwFrames);
	BufferSize = dwFrames;

	// get render client and set event for output
	FAILEDX2(pAudioClient->GetService(IID_PPV_ARGS(&pAudioRenderClient)));

	hOutput = CreateEventW(nullptr, TRUE, FALSE, nullptr);
	//FAILEDX2(pAudioClient->SetEventHandle(hOutput));

	CoTaskMemFree(closeFormat);

	return OSR_SUCCESS;
}

ThreadSystem thread;

OSRCODE
IMEngine::StartDevice(LPVOID pProc)
{
	static DWORD dwStart = 0;
	WASAPI_SAMPLE_PROC* pProce = (WASAPI_SAMPLE_PROC*)pProc;

	if (dwStart)
	{
		Sleep(0);
	}

	if (pProce->pLoopInfo->pSample)
	{
		WasapiThread = thread.CreateUserThread(nullptr, (ThreadFunc*)(WASAPIThreadProc), (LPVOID)pProc, L"OSR WASAPI worker thread");

		if (FAILED(pAudioClient->Start()))
		{
			return MXR_OSR_NO_OUT;
		}

		if (hThreadExitEvent) { ResetEvent(hThreadExitEvent); }
		if (hThreadLoadSamplesEvent) { ResetEvent(hThreadLoadSamplesEvent); }
		SetEvent(hStart);
		while (!hThreadLoadSamplesEvent) { Sleep(0); }
		SetEvent(hThreadLoadSamplesEvent);
	}

	dwStart++;

	return OSR_SUCCESS;
}

OSRCODE
IMEngine::StopDevice()
{
	if (pAudioClient)
	{
		if (FAILED(pAudioClient->Stop()))
		{
			return MXR_OSR_NO_OUT;
		}
	}

	thread.EnterSection();
	if (hThreadLoadSamplesEvent)	{ ResetEvent(hThreadLoadSamplesEvent); }
	if (hThreadExitEvent)			{ SetEvent(hThreadExitEvent); }
	thread.LeaveSection();

	return OSR_SUCCESS;
}
