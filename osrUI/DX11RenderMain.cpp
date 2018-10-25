/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR User Interface
**********************************************************
* DX11RenderMain.cpp
* Direct3D11 main implementation
*********************************************************/
#include "stdafx.h"

OSRCODE
DX11Render::CreateRenderWindow(HWND WindowHandle)
{
	HRESULT hr = NULL;
	DXGI_SWAP_CHAIN_DESC &sd = m_pChainDesc;
	DXGI_ADAPTER_DESC LegacyDesc = {};
	DXGI_ADAPTER_DESC1 Desc = {}; 

	if (!WindowHandle) { return DX_OSR_BAD_ARGUMENT; }
	MainHwnd = WindowHandle;

	// if our PC doesn't support DXGIFactor1 - try to use older
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&m_pDXGIFactory))))
	{
		FAILEDX2(CreateDXGIFactory(IID_PPV_ARGS(&m_pLegacyDXGIFactory)));
	}

	if (!m_pDXGIFactory)
	{
		if (!m_pLegacyDXGIFactory) 
		{ 
			return DX_OSR_BAD_HW;
		}
		else
		{
			FAILEDX2(m_pLegacyDXGIFactory->QueryInterface(IID_PPV_ARGS(&m_pDXGIFactory)));
		}
	}

	//#TODO: take set adapters for user
	if (m_pLegacyDXGIFactory)
	{
		m_pDXGIFactory->EnumAdapters(NULL, &m_pLegacyDXGIAdapter);
		m_pLegacyDXGIAdapter->QueryInterface(IID_PPV_ARGS(&m_pDXGIAdapter));

		_RELEASE(m_pLegacyDXGIAdapter);
		_RELEASE(m_pLegacyDXGIFactory);

		m_pDXGIAdapter->GetDesc(&LegacyDesc);
	}
	else
	{
		m_pDXGIFactory->EnumAdapters1(NULL, &m_pDXGIAdapter);
		m_pDXGIAdapter->GetDesc1(&Desc);
	}

	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferCount = 2;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.OutputWindow = MainHwnd;
	sd.Windowed = TRUE;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	D3D_FEATURE_LEVEL pFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL FeatureLevel = {};

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 
		NULL, NULL, pFeatureLevels, sizeof(pFeatureLevels) / sizeof(pFeatureLevels[0]),
		D3D11_SDK_VERSION, &sd, &m_pDXGISwapChain, &m_pDevice, &FeatureLevel, &m_pContext
	);

	if (FAILED(hr))
	{
		FAILEDX2(
			D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
			NULL, NULL, &pFeatureLevels[1], sizeof(pFeatureLevels) / sizeof(pFeatureLevels[0] - 1),
			D3D11_SDK_VERSION, &sd, &m_pDXGISwapChain, &m_pDevice, &FeatureLevel, &m_pContext
		));
	}	

	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pDXGISwapChain->GetBuffer(NULL, IID_PPV_ARGS(&pBackBuffer));
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	_RELEASE(pBackBuffer);

	if (!m_pDevice) { return DX_OSR_BAD_DEVICE; }
	return OSR_SUCCESS;
}
