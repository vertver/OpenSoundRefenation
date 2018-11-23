/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR DirectX
**********************************************************
* osrUI.cpp
* Main User Interface implementation
*********************************************************/
#include "stdafx.h"
#include "VUMeter.h"

DX11Render dx11Renderer;
VUMeter vMeter;
bool Window_Flag_Resizeing = false;

OSR::Mixer OutMixer;

VOID
OSR::UserInterface::CreateApplicationMenu()
{
}

VOID
CycleFunc()
{
	bool bDemo = true;
	static float mnmm = 0;
	static ImVec4 clear_color = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
	float L = 1.0*abs(sin(mnmm) *sin(mnmm*0.1)); //
	float R = 0.8*abs(cos(mnmm + 1.0)*sin(mnmm*0.1));
	static float LS = 0.0f;
	static float RS = 0.0f;
	LS = max(LS, L);
	RS = max(RS, R);
	static bool peakdetectL = false;
	if (L > 0.98f) peakdetectL = true;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImVec2 RegionAvail = ImGui::GetContentRegionAvail();
	vMeter.DrawLevels(RegionAvail.x - 30, RegionAvail.y - 50.0, L, R, LS, RS, peakdetectL, false);

	mnmm += 0.1;
	LS *= 0.995f;
	RS *= 0.995f;

	if (bDemo)
	{
		float f = 0.0f;
		int counter = 0;

		ImGui::Begin("Test Window");

		ImGui::Text("File dialog");

		ImGui::SliderFloat("Track Position", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Clear color", (float*)&clear_color);

		if (ImGui::Button("Button"))
		{
			OutMixer.PlaySample();
		}

		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	dx11Renderer.m_pContext->OMSetRenderTargets(1, &dx11Renderer.m_pRenderTargetView, nullptr);
	dx11Renderer.m_pContext->ClearRenderTargetView(dx11Renderer.m_pRenderTargetView, (float*)&clear_color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	dx11Renderer.m_pDXGISwapChain->Present(1, 0); // Present with vsync
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_DROPFILES:
	{
		DWORD dwFileCount = DragQueryFileW((HDROP)wParam, 0xFFFFFFFF, nullptr, 0);
		WSTRING_PATH szFilePaths = { NULL };

		if (dwFileCount > 1) { break; }

		for (DWORD i = 0; i < dwFileCount; i++)
		{
			DWORD dwPathSize = DragQueryFileW((HDROP)wParam, i, nullptr, 0);

			if (dwPathSize > 0 && dwPathSize < MAX_PATH)
			{
				if (DragQueryFileW((HDROP)wParam, i, szFilePaths, dwPathSize + 1) > 0)
				{
					WIN32_FIND_DATAW findData = { NULL };

					HANDLE hFind = FindFirstFileW(szFilePaths, &findData);

					// open file here
					OutMixer.LoadSample(szFilePaths);

					FindClose(hFind);
				}
			}
		}

		DragFinish((HDROP)wParam);
	}
	return 0;
	case WM_COPY:
		break;
	case WM_ENTERSIZEMOVE:
		SetTimer(hWnd, 2, 16, nullptr);
		Window_Flag_Resizeing = true;
		return 0;
	case WM_EXITSIZEMOVE:
		KillTimer(hWnd, 2);
		Window_Flag_Resizeing = false;
		return 0;
	case WM_TIMER:
		RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_INVALIDATE);
		return 0;
	case WM_PAINT:
		if (Window_Flag_Resizeing)
		{
			CycleFunc();
		}
		break;
	case WM_SIZE:
		if (dx11Renderer.m_pDevice != nullptr && wParam != SIZE_MINIMIZED)
		{
			_RELEASE(dx11Renderer.m_pRenderTargetView);
			dx11Renderer.m_pDXGISwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);

			ID3D11Texture2D* pBackBuffer = nullptr;
			dx11Renderer.m_pDXGISwapChain->GetBuffer(NULL, IID_PPV_ARGS(&pBackBuffer));
			dx11Renderer.m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &dx11Renderer.m_pRenderTargetView);
			_RELEASE(pBackBuffer);
		}
		return 0;
	case WM_SYSCOMMAND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

OSRCODE
OSR::UserInterface::CreateMainWindow()
{
	// create window class
	WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"OSR_DAW", nullptr };
	RegisterClassExW(&wc);

	RECT rec = { 0, 0, 640, 360 };
	AdjustWindowRectEx(&rec, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW);

	MainHwnd = CreateWindowW(
		L"OSR_DAW",
		L"Open Sound Refenation 0.4A",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rec.right - rec.left,
		rec.bottom - rec.top,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	if (OSRFAILED(dx11Renderer.CreateRenderWindow(MainHwnd)))
	{
		THROW2(L"Can't create render window");
	}

	ShowWindow(MainHwnd, SW_SHOWDEFAULT);
	UpdateWindow(MainHwnd);
	DragAcceptFiles(MainHwnd, TRUE);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();

	STRING_PATH szPath = { NULL };
	GetCurrentDirectoryA(sizeof(STRING_PATH), szPath);
	snprintf(szPath, sizeof(STRING_PATH), "%s%s", szPath, "\\arimo_reg.ttf");

	ImFont* font = io.Fonts->AddFontFromFileTTF(szPath, 16.0f);
	if (font != NULL) 
	{
		io.FontDefault = font; 
	}
	else 
	{ 
		io.Fonts->AddFontDefault();
	}
	io.Fonts->Build();

	OutMixer.CreateMixer(MainHwnd);

	ImGui_ImplWin32_Init(MainHwnd);
	ImGui_ImplDX11_Init(dx11Renderer.m_pDevice, dx11Renderer.m_pContext);

	// Setup style
	ImGui::StyleColorsDark();

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		CycleFunc();
		//g_pSwapChain->Present(0, 0); // Present without vsync
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return OSR_SUCCESS;
}