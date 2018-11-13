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

#define _GetProc(fun, type, name)  { fun = (type) GetProcAddress(hDInputDLL, name); if (!fun) { return FALSE; } }                                                       \

typedef BOOL(WINAPI *FAvRtCreateThreadOrderingGroup)  (PHANDLE, PLARGE_INTEGER, GUID*, PLARGE_INTEGER);
typedef BOOL(WINAPI *FAvRtDeleteThreadOrderingGroup)  (HANDLE);
typedef BOOL(WINAPI *FAvRtWaitOnThreadOrderingGroup)  (HANDLE);
typedef HANDLE(WINAPI *FAvSetMmThreadCharacteristics)   (LPCSTR, LPDWORD);
typedef BOOL(WINAPI *FAvRevertMmThreadCharacteristics)(HANDLE);
typedef BOOL(WINAPI *FAvSetMmThreadPriority)          (HANDLE, AVRT_PRIORITY);

static HMODULE hDInputDLL = 0;
FAvRtCreateThreadOrderingGroup   pAvRtCreateThreadOrderingGroup = NULL;
FAvRtDeleteThreadOrderingGroup   pAvRtDeleteThreadOrderingGroup = NULL;
FAvRtWaitOnThreadOrderingGroup   pAvRtWaitOnThreadOrderingGroup = NULL;
FAvSetMmThreadCharacteristics    pAvSetMmThreadCharacteristics = NULL;
FAvRevertMmThreadCharacteristics pAvRevertMmThreadCharacteristics = NULL;
FAvSetMmThreadPriority           pAvSetMmThreadPriority = NULL;

static BOOL SetupAVRT()
{
	hDInputDLL = LoadLibraryA("avrt.dll");
	if (!hDInputDLL) { return FALSE; }

	_GetProc(pAvRtCreateThreadOrderingGroup, FAvRtCreateThreadOrderingGroup, "AvRtCreateThreadOrderingGroup");
	_GetProc(pAvRtDeleteThreadOrderingGroup, FAvRtDeleteThreadOrderingGroup, "AvRtDeleteThreadOrderingGroup");
	_GetProc(pAvRtWaitOnThreadOrderingGroup, FAvRtWaitOnThreadOrderingGroup, "AvRtWaitOnThreadOrderingGroup");
	_GetProc(pAvSetMmThreadCharacteristics, FAvSetMmThreadCharacteristics, "AvSetMmThreadCharacteristicsA");
	_GetProc(pAvRevertMmThreadCharacteristics, FAvRevertMmThreadCharacteristics, "AvRevertMmThreadCharacteristics");
	_GetProc(pAvSetMmThreadPriority, FAvSetMmThreadPriority, "AvSetMmThreadPriority");

	return pAvRtCreateThreadOrderingGroup && gGroup &&
		pAvRtWaitOnThreadOrderingGroup &&
		pAvSetMmThreadCharacteristics &&
		pAvRevertMmThreadCharacteristics &&
		pAvSetMmThreadPriority;
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

	ASSERT2(SetupAVRT(), L"Can't init AVRT");

	{ 
		IMMDevice* pRenderer = nullptr;

		if (SUCCEEDED(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator))))
		{
			FAILEDX2(deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pRenderer));

			WCHAR* pDeviceId = nullptr;
			FAILEDX2(pRenderer->GetId(&pDeviceId));
			FAILEDX2(pRenderer->GetState(&dwOutputState));

			wcsncpy(OutputDeviceInfo.szDeviceId, pDeviceId, sizeof(WSTRING512) - 1);
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
		
		if (SUCCEEDED(deviceEnumerator->GetDefaultAudioEndpoint(eCapture, eMultimedia, &pCapturer)))
		{
			WCHAR* pDeviceId = nullptr;
			FAILEDX2(pCapturer->GetId(&pDeviceId));
			FAILEDX2(pCapturer->GetState(&dwInputState));

			wcsncpy(InputDeviceInfo.szDeviceId, pDeviceId, sizeof(WSTRING512) - 1);
			CoTaskMemFree(pDeviceId);
			_RELEASE(pCapturer);
		}
	}

	FAILEDX2(deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pEndPoints));
	FAILEDX2(pEndPoints->GetCount((UINT*)&dwRenderCount));
	_RELEASE(pEndPoints);

	FAILEDX2(deviceEnumerator->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE, &pEndPoints));
	FAILEDX2(pEndPoints->GetCount((UINT*)&DeviceCount));
	DeviceCount += dwRenderCount;

	if (DeviceCount)
	{
		DWORD devIndex = 0;
		WASAPI_DEVICE_INFO** pDeviceInfo = (WASAPI_DEVICE_INFO**)FastAlloc(sizeof(WASAPI_DEVICE_INFO) * DeviceCount);

		for (DWORD i = 0; i < DeviceCount; ++i, ++devIndex)
		{
			IMMDevice* pCurrentDevice = nullptr;
			
			IPropertyStore *pProperty;
			pEndPoints->Item(devIndex, &pCurrentDevice);
			pCurrentDevice->OpenPropertyStore(STGM_READ, &pProperty);

			{
				{
					PROPVARIANT value = {};
					PropVariantInit(&value);
					FAILEDX2(pProperty->GetValue(PKEY_AudioEngine_DeviceFormat, &value));

					memcpy(&pDeviceInfo[i]->waveFormat, value.blob.pBlobData, min(sizeof(pDeviceInfo[i]->waveFormat), value.blob.cbSize));
					PropVariantClear(&value);
				}

				{
					PROPVARIANT value = {};
					PropVariantInit(&value);
					FAILEDX2(pProperty->GetValue(PKEY_AudioEndpoint_FormFactor, &value));

					memcpy(&pDeviceInfo[i]->formFactor, value.blob.pBlobData, min(sizeof(pDeviceInfo[i]->formFactor), value.blob.cbSize));
					PropVariantClear(&value);
				}

				{
					IMMEndpoint* pEndpoint = nullptr;

					FAILEDX2(pCurrentDevice->QueryInterface(IID_PPV_ARGS(&pEndpoint)));

					if (pEndpoint)
					{
						pEndpoint->GetDataFlow(&pDeviceInfo[i]->dataFlow);
						_RELEASE(pEndpoint);
					}
				}

				{
					IAudioClient* tmpClient = nullptr;

					FAILEDX2(pCurrentDevice->Activate(*GetAudioClientIID(), CLSCTX_INPROC_SERVER, nullptr, (LPVOID*)&tmpClient));

					if (FAILED(tmpClient->GetDevicePeriod(&pDeviceInfo[i]->DefaultDevicePeriod, &pDeviceInfo[i]->MinimumDevicePeriod)))
					{
						pDeviceInfo[i]->DefaultDevicePeriod = 100000;
						pDeviceInfo[i]->MinimumDevicePeriod = 30000;
					}

					_RELEASE(tmpClient);
				}	
			}

			if (devIndex)
			{

			}

			pCurrentDevice->QueryInterface(IID_PPV_ARGS(&pDeviceInfo[i]->device));
			_RELEASE(pCurrentDevice)
			_RELEASE(pProperty);
		}
	}

	return OSR_SUCCESS;
}
//
//HRESULT 
//IMEngine::ActivateCompleted(
//	IActivateAudioInterfaceAsyncOperation *operation
//)
//{
//	HRESULT hr = S_OK;
//	HRESULT hrActivateResult = S_OK;
//	IUnknown *punkAudioInterface = nullptr;
//
//	hr = operation->GetActivateResult(&hrActivateResult, &punkAudioInterface);
//	if (FAILED(hr) && FAILED(hrActivateResult)) { return E_FAIL; }
//
//	punkAudioInterface->QueryInterface(IID_PPV_ARGS(&m_AudioClient));
//	if (!m_AudioClient) { return E_FAIL; }
//
//	AudioClientProperties audioProps = { 0 };
//	audioProps.cbSize = sizeof(AudioClientProperties);
//	audioProps.bIsOffload = TRUE;
//	audioProps.eCategory = AudioCategory_ForegroundOnlyMedia;
//	audioProps.Options = AUDCLNT_STREAMOPTIONS_RAW;
//
//	hr = m_AudioClient->SetClientProperties(&audioProps);
//	if (FAILED(hr)) { return E_FAIL; }
//
//	hr = m_AudioClient->GetMixFormat(&m_MixFormat);
//	if (FAILED(hr)) { return E_FAIL; }
//
//	REFERENCE_TIME hnsMinBufferDuration;
//	REFERENCE_TIME hnsMaxBufferDuration;
//
//	hr = m_AudioClient->GetBufferSizeLimits(m_MixFormat, true, &hnsMinBufferDuration, &hnsMaxBufferDuration);
//	if (SUCCEEDED(hr))
//	{
//		if (hnsBufferDuration < hnsMinBufferDuration)
//		{
//			 using MINIMUM size instead
//			hnsBufferDuration = hnsMinBufferDuration;
//		}
//		else if (hnsBufferDuration > hnsMaxBufferDuration)
//		{
//			 using MAXIMUM size instead
//			hnsBufferDuration = hnsMaxBufferDuration;
//		}
//	}
//
//}