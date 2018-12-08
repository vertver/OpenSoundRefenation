#pragma once
#include "discord-presence/include/discord_register.h"
#include "discord-presence/include/discord_rpc.h"
#include <time.h>

class DiscordNetwork
{
public:

	enum class StatusNumber
	{
		Waiting,
		OpenedAudio,
		PlayingAudio,
		WorkingWithAudio,
		Processing
	};

	void Init()
	{
		DiscordEventHandlers DiscordHandler = { nullptr };
		Discord_Initialize("519951399023280149", &DiscordHandler, (int)true, nullptr);
		bInit = true;
	}

	void Destroy()
	{
		if (bInit)
		{
			Discord_ClearPresence();
			Discord_Shutdown();
			bInit = false;
		}
	}

	void SetStatus(StatusNumber Status)
	{
		if (!bInit) { return; }

		DiscordRichPresence PresenceInfo = { nullptr };
		StatusNumber CStatus = Status;

		PresenceInfo.startTimestamp = time(nullptr);
		PresenceInfo.largeImageText = "Digital Workstation";
		PresenceInfo.details = "Current Track";

		bool bOnLevel = true;

		switch (CStatus)
		{
		case StatusNumber::OpenedAudio: 
			PresenceInfo.largeImageKey = "OPEN";
			break;
		case StatusNumber::PlayingAudio: 
			PresenceInfo.largeImageKey = "PLAY";
			break;
		case StatusNumber::WorkingWithAudio:
			PresenceInfo.largeImageKey = "WORK";
			break;
		case StatusNumber::Processing: 
			PresenceInfo.largeImageKey = "PRCS";
			break;
		case StatusNumber::Waiting: 
			PresenceInfo.largeImageKey = "WAIT";
			PresenceInfo.details = "Waiting for audio";
			break;
		}	

		//STRING64 CurrentTrack 
		
		PresenceInfo.largeImageKey = "OSRLOGO";

		Discord_UpdatePresence(&PresenceInfo);
	}


	~DiscordNetwork()
	{
		Destroy();
	}

private:
	bool bInit;
};

