/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR WASAPI
**********************************************************
* IMWASAPI.cpp
* WASAPI kernel implementation
*********************************************************/
#pragma once
#include "stdafx.h"
#include <dshow.h>
#include <audiopolicy.h>

class IMEngine : public IAudioSessionEvents
{
public:
	OSRCODE InitEngine();
private:
	//STDMETHOD(OnDisplayNameChanged) (LPCWSTR NewDisplayName, LPCGUID EventContext) { return S_OK; };
	//STDMETHOD(OnIconPathChanged) (LPCWSTR NewIconPath, LPCGUID EventContext) { return S_OK; };
	//STDMETHOD(OnSimpleVolumeChanged) (float NewSimpleVolume, BOOL NewMute, LPCGUID EventContext);
	//STDMETHOD(OnChannelVolumeChanged) (DWORD ChannelCount, float NewChannelVolumesVolume[], DWORD ChangedChannel, LPCGUID EventContext) { return S_OK; };
	//STDMETHOD(OnGroupingParamChanged) (LPCGUID NewGroupingParam, LPCGUID EventContext) { return S_OK; };
	//STDMETHOD(OnStateChanged) (AudioSessionState NewState) { return S_OK; };
	//STDMETHOD(OnSessionDisconnected) (AudioSessionDisconnectReason DisconnectReason) { return S_OK; };

	IGraphBuilder *_GraphBuilder;
	IMediaSeeking *_MediaSeeking;
	IMediaEventEx *_MediaEvent;

	IAudioSessionManager2 *_SessionManager2;
	ISimpleAudioVolume *_SimpleVolume;
	IAudioSessionControl2 *_SessionControl2;
};

class IMMixer
{
public:

private:
	IMEngine* engine;
};