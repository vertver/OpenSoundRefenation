/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* main.cpp
* entry-point
*********************************************************/
#include "OSRClasses.h"

int argc;
char* argv[MAX_NUM_ARGVS];

BOOL
WINAPI
WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	if (strstr(lpCmdLine, "-admin") || strstr(lpCmdLine, "-a") || strstr(lpCmdLine, "-adm")) { RunWithAdminPrivilege(); }

	// critical section
	InitApplication();


	//OpenFileDialog(&lpPath);
	//if (lpPath)
	//{
	//	engine.LoadSoundFile(lpPath, FALSE);
	//	engine.CreateXEngine();
	//	engine.CreateSourceBuffer();
	//	mixer.InitMixer(engine);
	//	mixer.PlaySimpleWave();
	//}
	//else
	//{
	//	DestroyApplication();
	//}

	//QApplication a(argc, argv);
	//OSR w;
	//w.show();
	//return a.exec();	

	while (true)
	{

	}
}
