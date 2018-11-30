/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR config
**********************************************************
* OSRConfig.h
* Config system implementation
*********************************************************/
#pragma once
#include "OSR.h"

typedef struct  
{
	DWORD dwMagic;				// magic 'OCFG' dword (0x4746434F)
	DWORD dwParamsCount;		// count of parameters
	DWORD dwParamsTreeCount;	// reserved
	STRING64 szConfigName;		// 64 - 5 (size of '.ocfg' string) = 59 max symbols for name
} CONFIG_DATA;

typedef struct  
{
	CHAR cbParamName[32];		// parameter name
	BYTE pData[128];			// static 128 byte data array
	DWORD dwSharedMemorySize;	// size of shared memory (if 0 - without shared memory)
	LPVOID pSharedMemory;		// pointer to shared memory (in file - first 8 bytes of data)
} PARAMS_DATA;

DLL_API OSRCODE GetConfigPath(LPCWSTR lpPath);
DLL_API OSRCODE ConvertToPath(LPCSTR lpConfig, LPCWSTR lpPath);
DLL_API BOOL IsConfigExist(LPCSTR lpConfig);

DLL_API OSRCODE CreateConfig(LPCSTR lpConfigName, LPVOID lpConfigData = NULL, DWORD DataSize = 0);
DLL_API OSRCODE WriteConfig(LPCSTR lpConfigName, LPVOID lpConfigData, DWORD DataSize);
DLL_API OSRCODE OpenConfig(LPCSTR lpConfigName, VOID** lpConfigData, LPDWORD DataSize);
DLL_API OSRCODE DeleteConfig(LPCSTR lpConfigName);

DLL_API OSRCODE CreateParameter(LPCSTR lpConfigName, LPCSTR lpParamName, PARAMS_DATA* ParamsData);
DLL_API OSRCODE WriteParameter(LPCSTR lpConfigName, LPCSTR lpParamName, PARAMS_DATA* ParamsData);
DLL_API OSRCODE OpenParameter(LPCSTR lpConfigName, LPCSTR lpParamName, PARAMS_DATA* ParamsData);
DLL_API OSRCODE DeleteParameter(LPCSTR lpConfigName, LPCSTR lpParamName);
