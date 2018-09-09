/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* XAudio2Mixer.cpp
* XAudio2 mixer implementation
*********************************************************/
#include "stdafx.h"

VOID
XMixer::InitMixer(
	XEngine engine
)
{
	_DEB(engine.pXAudio->QueryInterface(IID_PPV_ARGS(&pXAudio)));
	_DEB(pXAudio->CreateSubmixVoice(&pSubmixVoice, 1, SAMPLE_RATE));
	audioEngine = engine;
}

VOID 
XMixer::PlaySimpleWave()
{
	if (audioEngine.audioBuffer.AudioBytes)
	{
		HRESULT hr = audioEngine.pSourceVoice->Start();

		XAUDIO2_VOICE_STATE voiceState;
		BOOL isRunning = TRUE;
		while (SUCCEEDED(hr) && isRunning)
		{
			audioEngine.pSourceVoice->GetState(&voiceState);
			if (!voiceState.BuffersQueued) { break; }

			isRunning = (voiceState.BuffersQueued > 0) != 0;

			if (GetAsyncKeyState(VK_SPACE)) { break; }
			Sleep(10);
		}
	}
}
