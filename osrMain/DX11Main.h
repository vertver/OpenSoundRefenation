/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR DirectX
**********************************************************
* DX11Maim.h
* Direct3D11 main implementation
*********************************************************/
#pragma once
#include <d3d11.h>
#include "dxgi.h"
#include "OSR.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class DX11Render
{
public:
	HWND GetCurrentHwnd() { return MainHwnd; }
	OSRCODE CreateRenderWindow(HWND WindowHandle);

	DXGI_SWAP_CHAIN_DESC m_pChainDesc = { NULL };
	IDXGIFactory1* m_pDXGIFactory = nullptr;
	IDXGIFactory* m_pLegacyDXGIFactory = nullptr;
	IDXGIAdapter1* m_pDXGIAdapter = nullptr;
	IDXGIAdapter* m_pLegacyDXGIAdapter = nullptr;
	IDXGISwapChain* m_pDXGISwapChain = nullptr;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;

private:
	HWND MainHwnd;
};