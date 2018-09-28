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

OSRCODE
IMEngine::InitEngine()
{
	//FAILEDX1(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_GraphBuilder)));

	//_GraphBuilder->QueryInterface(IID_PPV_ARGS(&_MediaEvent));

	return OSR_SUCCESS;
}