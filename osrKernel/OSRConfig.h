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

typedef struct  
{
	BYTE Type;

	f16 Float16Array[64];		// 1
	f32 Float32Array[32];		// 2
	f64 Float64Array[16];		// 3

	CHAR AnsiString[128];		// 4
	WCHAR WideString[64];		// 5

	u8 ByteArray[128];			// 6
	u16 WordArray[64];			// 7
	u24 ThreeByteArray[42];		// 8
	u32 DwordArray[32];			// 9
	u64 QwordArray[16];			// 10

	BIT_BYTE BitBiteArray[128];	// 11

} SETTINGS_DATA;

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

class ConfigSystem
{
private:
	OSRCODE WriteParam(LPCSTR Param, LPVOID Data, BYTE Type, size_t Size)
	{
		PARAMS_DATA paramData = { 0 };
		OSRCODE sCode = OSR_SUCCESS;

		thsys.EnterSection();
		switch (Type)
		{
		case 1:
			memcpy(paramData.pData, Data, Size * sizeof(f16));

			sCode = WriteParameter(ConfigData.szConfigName, Param, &paramData);
			break;

		case 2:
			memcpy(paramData.pData, Data, Size * sizeof(f32));

			sCode = WriteParameter(ConfigData.szConfigName, Param, &paramData);
			break;

		case 3:
			memcpy(paramData.pData, Data, Size * sizeof(f64));

			sCode = WriteParameter(ConfigData.szConfigName, Param, &paramData);
			break;

		case 4:
			memcpy(paramData.pData, Data, Size * sizeof(CHAR));

			sCode = WriteParameter(ConfigData.szConfigName, Param, &paramData);
			break;

		case 5:
			memcpy(paramData.pData, Data, Size * sizeof(WCHAR));

			sCode = WriteParameter(ConfigData.szConfigName, Param, &paramData);
			break;

		case 6:
			memcpy(paramData.pData, Data, Size * sizeof(u8));

			sCode = WriteParameter(ConfigData.szConfigName, Param, &paramData);
			break;

		case 7:
			memcpy(paramData.pData, Data, Size * sizeof(u16));

			sCode = WriteParameter(ConfigData.szConfigName, Param, &paramData);
			break;

		case 8:
			memcpy(paramData.pData, Data, Size * sizeof(u24));

			sCode = WriteParameter(ConfigData.szConfigName, Param, &paramData);
			break;

		case 9:
			memcpy(paramData.pData, Data, Size * sizeof(u32));

			sCode = WriteParameter(ConfigData.szConfigName, Param, &paramData);
			break;


		case 10:
			memcpy(paramData.pData, Data, Size * sizeof(u64));

			sCode = WriteParameter(ConfigData.szConfigName, Param, &paramData);
			break;

		case 11:
			memcpy(paramData.pData, Data, Size * sizeof(BIT_BYTE));

			sCode = WriteParameter(ConfigData.szConfigName, Param, &paramData);
			break;
		}

		thsys.LeaveSection();

		return sCode;
	}

public:
	ConfigSystem()
	{
		memset((LPVOID)ConfigData.szConfigName, 0, sizeof(STRING64));
	}

	ConfigSystem(LPCSTR Config)
	{
		memset((LPVOID)ConfigData.szConfigName, 0, sizeof(STRING64));
		memcpy(ConfigData.szConfigName, Config, strlen(Config));

		CreateConfig(Config);
	}

	OSRCODE LoadConfig(LPCSTR Config)
	{
		try
		{
			memcpy(ConfigData.szConfigName, Config, strlen(Config));
			CreateConfig(Config);

			return OSR_SUCCESS;
		}
		catch (const std::exception& exc)
		{
			STRING256 szLog = { 0 };

			SuspendMainThread();
			_snprintf_s(szLog, 256, "The application throw exception with info: %s \n%s", exc.what(), "Please, open minidump file and send on our GitHub.");
			MessageBoxA(NULL, szLog, "Application throw exception", MB_OK | MB_ICONERROR);
			ResumeMainThread();
		}
	}

	OSRCODE create()
	{
		return CreateConfig(ConfigData.szConfigName);
	}

	OSRCODE new_param(LPCSTR ParamName, f16 Float)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, &Float, sizeof(f16));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, f32 Float)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, &Float, sizeof(f32));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, f64 Float)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, &Float, sizeof(f64));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, f16* Float, size_t Size)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, Float, Size * sizeof(f16));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, f32* Float, size_t Size)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, Float, Size * sizeof(f32));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, f64* Float, size_t Size)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, Float, Size * sizeof(f64));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, u8 Byte)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, &Byte, sizeof(u8));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, u16 Word)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, &Word, sizeof(u16));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, u24 Data)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, &Data, sizeof(u24));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, u32 Dword)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, &Dword, sizeof(u32));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, u64 Qword)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, &Qword, sizeof(u64));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, u8* Byte, size_t Size)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, Byte, Size * sizeof(u8));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, u16* Word, size_t Size)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, Word, Size * sizeof(u16));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, u32* Dword, size_t Size)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, Dword, Size * sizeof(u32));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, u64* Qword, size_t Size)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, Qword, Size * sizeof(u64));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, CHAR* Data, size_t Size)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, Data, Size * sizeof(CHAR));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, WCHAR* Data, size_t Size)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, Data, Size * sizeof(WCHAR));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, BIT_BYTE Data)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, &Data, sizeof(BIT_BYTE));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE new_param(LPCSTR ParamName, BIT_BYTE* Data, size_t Size)
	{
		PARAMS_DATA Param = { 0 };
		memcpy(Param.pData, Data, Size * sizeof(BIT_BYTE));

		OSRFAIL2(CreateParameter(ConfigData.szConfigName, ParamName, &Param), L"Can't create parameter");

		return OSR_SUCCESS;
	}

	OSRCODE edit_param(LPCSTR Param, void* Data, size_t Size, BYTE Type)
	{
		return WriteParam(Param, Data, Type, Size);
	}

	OSRCODE open_param(LPCSTR Param, void* Data, size_t DataSizeInBytes)
	{
		PARAMS_DATA Params = { 0 };
		OSRCODE sCode = OSR_SUCCESS;

		if (OSRFAILED((sCode = OpenParameter(ConfigData.szConfigName, Param, &Params))))
		{
			return sCode;
		}

		memcpy(Data, Params.pData, DataSizeInBytes);

		return OSR_SUCCESS;
	}

private:
	CONFIG_DATA ConfigData;
	ThreadSystem thsys;
};
