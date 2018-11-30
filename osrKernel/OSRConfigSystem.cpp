/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR config
**********************************************************
* OSRConfigSystem.cpp
* Config system implementation
*********************************************************/
#include "stdafx.h"
#include "OSRConfig.h"
 
constexpr size_t SizeOfParametersStruct = sizeof(PARAMS_DATA);

OSRCODE
GetConfigPath(
	LPCWSTR lpPath
)
{
	if (!lpPath) { return FS_OSR_BAD_PTR; }

	WSTRING_PATH szPath = { 0 };
	GetCurrentDirectoryW(sizeof(WSTRING_PATH), szPath);
	_snwprintf_s(szPath, sizeof(WSTRING_PATH), L"%s%s", szPath, L"\\Configs");

	WIN32_FIND_DATAW findData = { 0 };
	HANDLE hFind = FindFirstFileW(szPath, &findData);

	if (!hFind || hFind == INVALID_HANDLE_VALUE)
	{
		if (!CreateDirectoryW(szPath, nullptr))
		{
			return FS_OSR_BAD_PATH;
		}
	}
	else
	{
		bool bDir = ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0);
		FindClose(hFind);

		ASSERT2(bDir, L"The settings path can not be file");
	}

	memcpy_s((LPVOID)lpPath, sizeof(WSTRING_PATH), szPath, sizeof(WSTRING_PATH));

	return OSR_SUCCESS;
}

OSRCODE
ConvertToPath(
	LPCSTR lpConfig,
	LPCWSTR lpPath
)
{
	WSTRING_PATH szConfigPath = { 0 };
	WSTRING64 szConfigName = { 0 };
	LPCSTR lpConfigType = nullptr;

	// find '.ocfg' type
	if (strlen(lpConfig) > 5)
	{
		lpConfigType = (LPCSTR)ptrdiff_t((u64)lpConfig + (strlen(lpConfig) - strlen(".ocfg")));
	}

	// copy to wide char var
	for (size_t i = 0; i < strlen(lpConfig); i++)
	{
		szConfigName[i] = lpConfig[i];
	}

	// get config path
	OSRFAIL2(GetConfigPath(szConfigPath), L"Can't get config path.");

	if (lpConfigType && !strcmp(lpConfigType, ".ocfg"))
	{
		_snwprintf_s(szConfigPath, sizeof(WSTRING_PATH), L"%s%s%s", szConfigPath, L"\\", szConfigName);
	}
	else
	{
		_snwprintf_s(szConfigPath, sizeof(WSTRING_PATH), L"%s%s%s%s", szConfigPath, L"\\", szConfigName, L".ocfg");
	}

	memcpy_s((LPVOID)lpPath, sizeof(WSTRING_PATH), szConfigPath, sizeof(WSTRING_PATH));
	return OSR_SUCCESS;
}

BOOL
IsConfigExist(
	LPCSTR lpConfig
)
{
	bool bDir = false;
	WSTRING_PATH szConfigPath = { 0 };
	WSTRING64 szConfigName = { 0 };

	ConvertToPath(lpConfig, szConfigPath);

	// check for existing file
	WIN32_FIND_DATAW findData = { 0 };
	HANDLE hFind = FindFirstFileW(szConfigPath, &findData);
	DWORD DataFile = 0;

	// if file doesn't exist
	if (!hFind || hFind == INVALID_HANDLE_VALUE)
	{
		SetLastError(0);		// no error. That's OK
		return FALSE;
	}
	else
	{
		// check for dir
		bDir = ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0);

		FindClose(hFind);
	}

	return !bDir;
}

OSRCODE
CreateConfig(
	LPCSTR lpConfigName,
	LPVOID lpConfigData, 
	DWORD DataSize
)
{	
	HANDLE hFile = NULL;
	WSTRING_PATH szConfigPath = { 0 };
	WSTRING64 szConfigName = { 0 };

	if (!IsConfigExist(lpConfigName))
	{
		ConvertToPath(lpConfigName, szConfigPath);

		CONFIG_DATA configData = { 0 };

		// if no config data - set default CONFIF_DATA struct
		if (!lpConfigData)
		{	
			configData.dwMagic = 0x4746434F;	// 'OCFG' DWORD
			memcpy_s(configData.szConfigName, sizeof(STRING64), lpConfigName, strlen(lpConfigName));
			lpConfigData = &configData;
			DataSize = sizeof(CONFIG_DATA);
		}

		OSRFAIL2(CreateNewFileWithData(szConfigPath, lpConfigData, DataSize), L"Can't create file and write data");
	}

	return OSR_SUCCESS;
}

OSRCODE 
WriteConfig(
	LPCSTR lpConfigName,
	LPVOID lpConfigData, 
	DWORD DataSize
)
{
	WSTRING_PATH szConfigPath = { 0 };

	if (!lpConfigData || !DataSize) { return FS_OSR_BAD_PTR; }
	if (!IsConfigExist(lpConfigName)) { return FS_OSR_BAD_PATH; }

	OSRFAIL2(ConvertToPath(lpConfigName, szConfigPath), L"Can't convert string to path");
	OSRFAIL2(CreateNewFileWithData(szConfigPath, lpConfigData, DataSize), L"Can't write data to config");

	return OSR_SUCCESS;
}

OSRCODE
OpenConfig(
	LPCSTR lpConfigName,
	VOID** lpConfigData,
	LPDWORD DataSize
)
{
	WSTRING_PATH szConfigPath = { 0 };

	if (!lpConfigData || !DataSize) { return FS_OSR_BAD_PTR; }
	if (!IsConfigExist(lpConfigName)) { return FS_OSR_BAD_PATH; }

	OSRFAIL2(ConvertToPath(lpConfigName, szConfigPath), L"Can't convert string to path");
	OSRFAIL2(ReadDataFromFile(szConfigPath, lpConfigData, DataSize), L"Can't read data from file");

	return OSR_SUCCESS;
}

OSRCODE 
DeleteConfig(
	LPCSTR lpConfigName
)
{
	WSTRING_PATH szConfigPath = { 0 };

	if (!IsConfigExist(lpConfigName)) { return FS_OSR_BAD_PATH; }

	OSRFAIL2(ConvertToPath(lpConfigName, szConfigPath), L"Can't convert string to path");
	if (!DeleteFileW(szConfigPath)) { return FS_OSR_BAD_PATH; }

	return OSR_SUCCESS;
}

OSRCODE 
CreateParameter(
	LPCSTR lpConfigName,
	LPCSTR lpParamName,
	PARAMS_DATA* ParamsData
)
{
	WSTRING_PATH szConfigPath = { 0 };
	CHAR ConfigName[32] = { 0 };
	LPVOID lpConfigData = nullptr;
	LPVOID lpOutData = nullptr;
	DWORD DataSize = 0;
	DWORD DataSizeOut = 0;

	if (!IsConfigExist(lpConfigName)) { return FS_OSR_BAD_PATH; }

	OSRFAIL2(ConvertToPath(lpConfigName, szConfigPath), L"Can't convert string to path");
	OSRFAIL2(ReadDataFromFile(szConfigPath, &lpConfigData, &DataSize), L"Can't read data from file");

	CONFIG_DATA* pConfigData = reinterpret_cast<CONFIG_DATA*>(lpConfigData);
	if (pConfigData->dwMagic != 0x4746434F)
	{
		FREEKERNELHEAP(lpConfigData);
		return FS_OSR_BAD_PTR;
	}

	pConfigData->dwParamsCount++;

	PARAMS_DATA paramData = { 0 };
	memcpy_s(&paramData, sizeof(PARAMS_DATA), ParamsData, sizeof(PARAMS_DATA));
	strcpy_s(paramData.cbParamName, strlen(lpParamName) + 1, lpParamName);

	DataSizeOut = DataSize + sizeof(PARAMS_DATA) + paramData.dwSharedMemorySize;

	lpOutData = FastAlloc(DataSizeOut + 8);
	memcpy_s(lpOutData, DataSizeOut, lpConfigData, DataSize);

	if (paramData.pSharedMemory || paramData.dwSharedMemorySize)
	{
		memcpy_s(LPVOID(size_t(lpOutData) + DataSize), DataSizeOut - DataSize, &paramData, size_t(sizeof(PARAMS_DATA) - 8));
	}
	else
	{
		memcpy_s(LPVOID(size_t(lpOutData) + DataSize), DataSizeOut, &paramData, size_t(sizeof(PARAMS_DATA)));
	}

	if (paramData.pSharedMemory && paramData.dwSharedMemorySize)
	{
		memcpy(LPVOID(size_t(lpOutData) + DataSize + size_t(sizeof(PARAMS_DATA) - 8)), paramData.pSharedMemory, paramData.dwSharedMemorySize);
	}

	if (OSRFAILED(CreateNewFileWithData(szConfigPath, lpOutData, DataSizeOut)))
	{
		FREEKERNELHEAP(lpConfigData);
		FREEKERNELHEAP(lpOutData);

		return FS_OSR_BAD_PTR;
	}

	FREEKERNELHEAP(lpOutData);
	FREEKERNELHEAP(lpConfigData);

	return OSR_SUCCESS;
}

OSRCODE
WriteParameter(
	LPCSTR lpConfigName,
	LPCSTR lpParamName,
	PARAMS_DATA* ParamsData
)
{
	bool isValid = false;
	WSTRING_PATH szConfigPath = { 0 };
	CHAR ConfigName[32] = { 0 };
	LPVOID lpConfigData = nullptr;
	LPVOID lpOutData = nullptr;
	DWORD CycleSize = 0;
	DWORD DataSize = 0;

	strcpy_s(ConfigName, strlen(lpParamName) + 1, lpParamName);

	if (!IsConfigExist(lpConfigName)) { return FS_OSR_BAD_PATH; }

	OSRFAIL2(ConvertToPath(lpConfigName, szConfigPath), L"Can't convert string to path");
	OSRFAIL2(ReadDataFromFile(szConfigPath, &lpConfigData, &DataSize), L"Can't read data from file");

	CONFIG_DATA* pConfigData = reinterpret_cast<CONFIG_DATA*>(lpConfigData);
	if (pConfigData->dwMagic != 0x4746434F)
	{
		FREEKERNELHEAP(lpConfigData);
		return FS_OSR_BAD_PTR;
	}

	CycleSize = DataSize - sizeof(CONFIG_DATA);

	DWORD ArraySize = CycleSize;
	PARAMS_DATA* pParams = nullptr;

	for (size_t i = 0; i < pConfigData->dwParamsCount; i++)
	{
		pParams = (PARAMS_DATA*)ptrdiff_t(u64(lpConfigData) + (DataSize - ArraySize));
		
		//#TODO: need more safety method
		try
		{
			if (!strcmp(lpParamName, pParams->cbParamName))
			{
				isValid = true;
				break;
			}
		}
		catch (...)
		{
			FREEKERNELHEAP(lpConfigData);

			return FS_OSR_BAD_PTR;
		}

		if (!pParams->pSharedMemory)
		{
			ArraySize = CycleSize - SizeOfParametersStruct;
		}
		else
		{
			ArraySize = CycleSize - SizeOfParametersStruct - pParams->dwSharedMemorySize;
		}

		CycleSize = ArraySize;
	}

	if (isValid)
	{
		lpOutData = FastAlloc(DataSize);

		memcpy(pParams, ParamsData, sizeof(PARAMS_DATA) - 8);
		memcpy(pParams->cbParamName, ConfigName, 32);
		memcpy(&pParams->pSharedMemory, ParamsData->pSharedMemory, ParamsData->dwSharedMemorySize);
		memcpy(lpOutData, lpConfigData, DataSize);

		if (OSRFAILED(CreateNewFileWithData(szConfigPath, lpOutData, DataSize)))
		{
			FREEKERNELHEAP(lpConfigData);
			FREEKERNELHEAP(lpOutData);

			return FS_OSR_BAD_PTR;
		}
	}
	else 
	{
		FREEKERNELHEAP(lpConfigData);

		return FS_OSR_BAD_PTR;
	}

	FREEKERNELHEAP(lpOutData);
	FREEKERNELHEAP(lpConfigData);

	return OSR_SUCCESS;
}

OSRCODE 
OpenParameter(
	LPCSTR lpConfigName,
	LPCSTR lpParamName,
	PARAMS_DATA* ParamsData
)
{
	bool isValid = false;
	WSTRING_PATH szConfigPath = { 0 };
	CHAR ConfigName[32] = { 0 };
	LPVOID lpConfigData = nullptr;
	DWORD CycleSize = 0;
	DWORD DataSize = 0;

	if (!ParamsData) { return FS_OSR_BAD_PTR; }

	strcpy_s(ConfigName, strlen(lpParamName) + 1, lpParamName);

	if (!IsConfigExist(lpConfigName)) { return FS_OSR_BAD_PATH; }

	OSRFAIL2(ConvertToPath(lpConfigName, szConfigPath), L"Can't convert string to path");
	OSRFAIL2(ReadDataFromFile(szConfigPath, &lpConfigData, &DataSize), L"Can't read data from file");

	CONFIG_DATA* pConfigData = reinterpret_cast<CONFIG_DATA*>(lpConfigData);
	if (pConfigData->dwMagic != 0x4746434F)
	{
		FREEKERNELHEAP(lpConfigData);
		return FS_OSR_BAD_PTR;
	}

	CycleSize = DataSize - sizeof(CONFIG_DATA);

	DWORD ArraySize = CycleSize;
	PARAMS_DATA* pParams = nullptr;

	for (size_t i = 0; i < pConfigData->dwParamsCount; i++)
	{
		pParams = (PARAMS_DATA*)ptrdiff_t(u64(lpConfigData) + (DataSize - ArraySize));

		//#TODO: need more safety method
		try
		{
			if (!strcmp(lpParamName, pParams->cbParamName))
			{
				isValid = true;
				break;
			}
		}
		catch (...)
		{
			FREEKERNELHEAP(lpConfigData);

			return FS_OSR_BAD_PTR;
		}

		if (!pParams->pSharedMemory)
		{
			ArraySize = CycleSize - SizeOfParametersStruct;
		}
		else
		{
			ArraySize = CycleSize - SizeOfParametersStruct - pParams->dwSharedMemorySize;
		}

		CycleSize = ArraySize;
	}

	if (isValid)
	{
		memcpy(ParamsData, pParams, sizeof(PARAMS_DATA));
		memcpy(ParamsData->cbParamName, pParams->cbParamName, 32);

		if (pParams->dwSharedMemorySize)
		{
			ParamsData->pSharedMemory = FastAlloc(pParams->dwSharedMemorySize);
		}

		memcpy(ParamsData->pSharedMemory, &pParams->pSharedMemory, pParams->dwSharedMemorySize);
	}
	else
	{
		FREEKERNELHEAP(lpConfigData);

		return FS_OSR_BAD_PTR;
	}

	FREEKERNELHEAP(lpConfigData);

	return OSR_SUCCESS;
}

OSRCODE 
DeleteParameter(
	LPCSTR lpConfigName,
	LPCSTR lpParamName
)
{
	bool isValid = false;
	WSTRING_PATH szConfigPath = { 0 };
	CHAR ConfigName[32] = { 0 };
	LPVOID lpConfigData = nullptr;
	LPVOID lpOutData = nullptr;
	DWORD CycleSize = 0;
	DWORD DataSize = 0;

	strcpy_s(ConfigName, strlen(lpParamName) + 1, lpParamName);

	if (!IsConfigExist(lpConfigName)) { return FS_OSR_BAD_PATH; }

	OSRFAIL2(ConvertToPath(lpConfigName, szConfigPath), L"Can't convert string to path");
	OSRFAIL2(ReadDataFromFile(szConfigPath, &lpConfigData, &DataSize), L"Can't read data from file");

	CONFIG_DATA* pConfigData = reinterpret_cast<CONFIG_DATA*>(lpConfigData);
	if (pConfigData->dwMagic != 0x4746434F)
	{
		FREEKERNELHEAP(lpConfigData);
		return FS_OSR_BAD_PTR;
	}

	CycleSize = DataSize - sizeof(CONFIG_DATA);
	PARAMS_DATA* pParams = nullptr;

	{
		DWORD ArraySize = CycleSize;

		for (size_t i = 0; i < pConfigData->dwParamsCount; i++)
		{
			pParams = (PARAMS_DATA*)ptrdiff_t(u64(lpConfigData) + (DataSize - ArraySize));

			//#TODO: need more safety method
			try
			{
				if (!strcmp(lpParamName, pParams->cbParamName))
				{
					isValid = true;
					break;
				}
			}
			catch (...)
			{
				FREEKERNELHEAP(lpConfigData);

				return FS_OSR_BAD_PTR;
			}

			if (!pParams->pSharedMemory)
			{
				ArraySize = CycleSize - SizeOfParametersStruct;
			}
			else
			{
				ArraySize = CycleSize - SizeOfParametersStruct - pParams->dwSharedMemorySize;
			}

			CycleSize = ArraySize;
		}
	}

	lpOutData = FastAlloc(DataSize);

	try
	{
		DWORD OutSize = 0;
		LPVOID lpConfigPointer = LPVOID(u64(lpOutData) + (sizeof(CONFIG_DATA)));

		pConfigData->dwParamsCount--;

		memcpy(lpOutData, lpConfigData, sizeof(CONFIG_DATA));

		memcpy(
			lpConfigPointer,
			LPVOID(u64(lpConfigData) + (sizeof(CONFIG_DATA))),
			DataSize - CycleSize - sizeof(CONFIG_DATA)
		);

		ptrdiff_t LastSize = ptrdiff_t(
			(u64)LPVOID((u64)lpConfigData + DataSize)
			-
			(u64)LPVOID(u64(lpConfigData) + (sizeof(CONFIG_DATA)) + (DataSize - CycleSize - sizeof(CONFIG_DATA)) + pParams->dwSharedMemorySize + sizeof(PARAMS_DATA))
		);		

		memcpy(
			LPVOID(u64(lpConfigPointer) + (DataSize - CycleSize - sizeof(CONFIG_DATA))),
			LPVOID(u64(lpConfigData) + (sizeof(CONFIG_DATA)) + (DataSize - CycleSize - sizeof(CONFIG_DATA)) + pParams->dwSharedMemorySize + sizeof(PARAMS_DATA)),
			LastSize
		);

		OutSize = DataSize - CycleSize + LastSize;

		if (OSRFAILED(CreateNewFileWithData(szConfigPath, lpOutData, OutSize)))
		{
			FREEKERNELHEAP(lpConfigData);
			FREEKERNELHEAP(lpOutData);

			return FS_OSR_BAD_PTR;
		}
	}
	catch (...)
	{
		FREEKERNELHEAP(lpConfigData);
		FREEKERNELHEAP(lpOutData);

		return FS_OSR_BAD_PTR;
	}

	FREEKERNELHEAP(lpConfigData);
	FREEKERNELHEAP(lpOutData);

	return OSR_SUCCESS;
}
