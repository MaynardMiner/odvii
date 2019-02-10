#include <stdio.h>
#include <tchar.h>

///
///  Copyright (c) 2008 - 2016 Advanced Micro Devices, Inc.

///  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
///  EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.

/// \file Source.cpp

#include <windows.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "adl_sdk.h"
#include "adl_structures.h"

using namespace std;

#include <stdio.h>

// Comment out one of the two lines below to allow or supress diagnostic messages
// #define PRINTF
#define PRINTF printf

typedef int (*ADL2_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int, ADL_CONTEXT_HANDLE *);
typedef int (*ADL2_MAIN_CONTROL_DESTROY)(ADL_CONTEXT_HANDLE);
typedef int (*ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int);
typedef int (*ADL_MAIN_CONTROL_DESTROY)();
typedef int (*ADL2_FLUSH_DRIVER_DATA)(int);
typedef int (*ADL2_ADAPTER_ACTIVE_GET)(ADL_CONTEXT_HANDLE, int, int *);

typedef int (*ADL_ADAPTER_NUMBEROFADAPTERS_GET)(int *);
typedef int (*ADL_ADAPTER_ADAPTERINFO_GET)(LPAdapterInfo, int);
typedef int (*ADL2_ADAPTERX2_CAPS)(int, int *);
typedef int (*ADL2_OVERDRIVEN_CAPABILITIES_GET)(ADL_CONTEXT_HANDLE, int, ADLODNCapabilities *);
typedef int (*ADL2_OVERDRIVEN_SYSTEMCLOCKS_GET)(ADL_CONTEXT_HANDLE, int, ADLODNPerformanceLevels *);
typedef int (*ADL2_OVERDRIVEN_SYSTEMCLOCKS_SET)(ADL_CONTEXT_HANDLE, int, ADLODNPerformanceLevels *);
typedef int (*ADL2_OVERDRIVEN_MEMORYCLOCKS_GET)(ADL_CONTEXT_HANDLE, int, ADLODNPerformanceLevels *);
typedef int (*ADL2_OVERDRIVEN_MEMORYCLOCKS_SET)(ADL_CONTEXT_HANDLE, int, ADLODNPerformanceLevels *);
typedef int (*ADL2_OVERDRIVEN_PERFORMANCESTATUS_GET)(ADL_CONTEXT_HANDLE, int, ADLODNPerformanceStatus *);
typedef int (*ADL2_OVERDRIVEN_FANCONTROL_GET)(ADL_CONTEXT_HANDLE, int, ADLODNFanControl *);
typedef int (*ADL2_OVERDRIVEN_FANCONTROL_SET)(ADL_CONTEXT_HANDLE, int, ADLODNFanControl *);
typedef int (*ADL2_OVERDRIVEN_POWERLIMIT_GET)(ADL_CONTEXT_HANDLE, int, ADLODNPowerLimitSetting *);
typedef int (*ADL2_OVERDRIVEN_POWERLIMIT_SET)(ADL_CONTEXT_HANDLE, int, ADLODNPowerLimitSetting *);
typedef int (*ADL2_OVERDRIVEN_TEMPERATURE_GET)(ADL_CONTEXT_HANDLE, int, int, int *);
typedef int (*ADL2_OVERDRIVE6_CURRENTPOWER_GET)(ADL_CONTEXT_HANDLE, int iAdapterIndex, int iPowerType, int *lpCurrentValue);
typedef int (*ADL2_OVERDRIVE5_FANSPEED_GET)(ADL_CONTEXT_HANDLE, int, int, ADLFanSpeedValue *);
typedef int (*ADL_OVERDRIVE5_FANSPEEDINFO_GET)(int, int, ADLFanSpeedInfo *);
typedef int (*ADL_OVERDRIVE5_FANSPEED_SET)(int, int, ADLFanSpeedValue *lpFanSpeedValue);
typedef int(*ADL2_OVERDRIVEN_SETTINGSEXT_GET) (ADL_CONTEXT_HANDLE, int, int64_t*, int64_t*, int64_t*, int64_t*);
typedef int(*ADL2_OVERDRIVEN_SETTINGSEXT_SET) (ADL_CONTEXT_HANDLE, int, int, ADLODNExtSettings*, ADLODNExtSettings*);

HINSTANCE hDLL;

ADL2_MAIN_CONTROL_CREATE ADL2_Main_Control_Create = NULL;
ADL2_MAIN_CONTROL_DESTROY ADL2_Main_Control_Destroy = NULL;
ADL_MAIN_CONTROL_CREATE ADL_Main_Control_Create = NULL;
ADL_MAIN_CONTROL_DESTROY ADL_Main_Control_Destroy = NULL;
ADL_ADAPTER_NUMBEROFADAPTERS_GET ADL_Adapter_NumberOfAdapters_Get = NULL;
ADL_ADAPTER_ADAPTERINFO_GET ADL_Adapter_AdapterInfo_Get = NULL;
ADL2_ADAPTERX2_CAPS ADL2_AdapterX2_Caps = NULL;
ADL2_ADAPTER_ACTIVE_GET ADL2_Adapter_Active_Get = NULL;
ADL2_OVERDRIVEN_CAPABILITIES_GET ADL2_OverdriveN_Capabilities_Get = NULL;
ADL2_OVERDRIVEN_SYSTEMCLOCKS_GET ADL2_OverdriveN_SystemClocks_Get = NULL;
ADL2_OVERDRIVEN_SYSTEMCLOCKS_SET ADL2_OverdriveN_SystemClocks_Set = NULL;
ADL2_OVERDRIVEN_PERFORMANCESTATUS_GET ADL2_OverdriveN_PerformanceStatus_Get = NULL;
ADL2_OVERDRIVEN_FANCONTROL_GET ADL2_OverdriveN_FanControl_Get = NULL;
ADL2_OVERDRIVEN_FANCONTROL_SET ADL2_OverdriveN_FanControl_Set = NULL;
ADL2_OVERDRIVEN_POWERLIMIT_GET ADL2_OverdriveN_PowerLimit_Get = NULL;
ADL2_OVERDRIVEN_POWERLIMIT_SET ADL2_OverdriveN_PowerLimit_Set = NULL;
ADL2_OVERDRIVEN_MEMORYCLOCKS_GET ADL2_OverdriveN_MemoryClocks_Get = NULL;
ADL2_OVERDRIVEN_MEMORYCLOCKS_GET ADL2_OverdriveN_MemoryClocks_Set = NULL;
ADL2_OVERDRIVEN_TEMPERATURE_GET ADL2_OverdriveN_Temperature_Get = NULL;
ADL2_OVERDRIVE6_CURRENTPOWER_GET ADL2_Overdrive6_CurrentPower_Get = NULL;
ADL2_OVERDRIVE5_FANSPEED_GET ADL2_Overdrive5_FanSpeed_Get = NULL;
ADL_OVERDRIVE5_FANSPEEDINFO_GET ADL_Overdrive5_FanSpeedInfo_Get = NULL;
ADL_OVERDRIVE5_FANSPEED_SET ADL_Overdrive5_FanSpeed_Set = NULL;
ADL2_OVERDRIVEN_SETTINGSEXT_GET ADL2_OverdriveN_SettingsExt_Get = NULL;
ADL2_OVERDRIVEN_SETTINGSEXT_SET ADL2_OverdriveN_SettingsExt_Set = NULL;

// Memory allocation function
void *__stdcall ADL_Main_Memory_Alloc(int iSize)
{
	void *lpBuffer = malloc(iSize);
	return lpBuffer;
}

// Optional Memory de-allocation function
void __stdcall ADL_Main_Memory_Free(void **lpBuffer)
{
	if (NULL != *lpBuffer)
	{
		free(*lpBuffer);
		*lpBuffer = NULL;
	}
}

ADL_CONTEXT_HANDLE context = NULL;

LPAdapterInfo lpAdapterInfo = NULL;
int iNumberAdapters;

int initializeADL()
{

	// Load the ADL dll
	HINSTANCE hDLL = LoadLibrary(L"atiadlxx.dll");

	if (NULL == hDLL)
	{
		PRINTF("Failed to load ADL library\n");
		return FALSE;
	}
	ADL2_Main_Control_Create = (ADL2_MAIN_CONTROL_CREATE)GetProcAddress(hDLL, "ADL2_Main_Control_Create");
	ADL2_Main_Control_Destroy = (ADL2_MAIN_CONTROL_DESTROY)GetProcAddress(hDLL, "ADL2_Main_Control_Destroy");
	ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE)GetProcAddress(hDLL, "ADL_Main_Control_Create");
	ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY)GetProcAddress(hDLL, "ADL_Main_Control_Destroy");
	ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET)GetProcAddress(hDLL, "ADL_Adapter_NumberOfAdapters_Get");
	ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET)GetProcAddress(hDLL, "ADL_Adapter_AdapterInfo_Get");
	ADL2_AdapterX2_Caps = (ADL2_ADAPTERX2_CAPS)GetProcAddress(hDLL, "ADL2_AdapterX2_Caps");
	ADL2_Adapter_Active_Get = (ADL2_ADAPTER_ACTIVE_GET)GetProcAddress(hDLL, "ADL2_Adapter_Active_Get");
	ADL2_OverdriveN_Capabilities_Get = (ADL2_OVERDRIVEN_CAPABILITIES_GET)GetProcAddress(hDLL, "ADL2_OverdriveN_Capabilities_Get");
	ADL2_OverdriveN_SystemClocks_Get = (ADL2_OVERDRIVEN_SYSTEMCLOCKS_GET)GetProcAddress(hDLL, "ADL2_OverdriveN_SystemClocks_Get");
	ADL2_OverdriveN_SystemClocks_Set = (ADL2_OVERDRIVEN_SYSTEMCLOCKS_SET)GetProcAddress(hDLL, "ADL2_OverdriveN_SystemClocks_Set");
	ADL2_OverdriveN_MemoryClocks_Get = (ADL2_OVERDRIVEN_MEMORYCLOCKS_GET)GetProcAddress(hDLL, "ADL2_OverdriveN_MemoryClocks_Get");
	ADL2_OverdriveN_MemoryClocks_Set = (ADL2_OVERDRIVEN_MEMORYCLOCKS_SET)GetProcAddress(hDLL, "ADL2_OverdriveN_MemoryClocks_Set");
	ADL2_OverdriveN_PerformanceStatus_Get = (ADL2_OVERDRIVEN_PERFORMANCESTATUS_GET)GetProcAddress(hDLL, "ADL2_OverdriveN_PerformanceStatus_Get");
	ADL2_OverdriveN_FanControl_Get = (ADL2_OVERDRIVEN_FANCONTROL_GET)GetProcAddress(hDLL, "ADL2_OverdriveN_FanControl_Get");
	ADL2_OverdriveN_FanControl_Set = (ADL2_OVERDRIVEN_FANCONTROL_SET)GetProcAddress(hDLL, "ADL2_OverdriveN_FanControl_Set");
	ADL2_OverdriveN_PowerLimit_Get = (ADL2_OVERDRIVEN_POWERLIMIT_GET)GetProcAddress(hDLL, "ADL2_OverdriveN_PowerLimit_Get");
	ADL2_OverdriveN_PowerLimit_Set = (ADL2_OVERDRIVEN_POWERLIMIT_SET)GetProcAddress(hDLL, "ADL2_OverdriveN_PowerLimit_Set");
	ADL2_OverdriveN_Temperature_Get = (ADL2_OVERDRIVEN_TEMPERATURE_GET)GetProcAddress(hDLL, "ADL2_OverdriveN_Temperature_Get");
	ADL2_Overdrive6_CurrentPower_Get = (ADL2_OVERDRIVE6_CURRENTPOWER_GET)GetProcAddress(hDLL, "ADL2_Overdrive6_CurrentPower_Get");
	ADL2_Overdrive5_FanSpeed_Get = (ADL2_OVERDRIVE5_FANSPEED_GET)GetProcAddress(hDLL, "ADL2_Overdrive5_FanSpeed_Get");
	ADL_Overdrive5_FanSpeedInfo_Get = (ADL_OVERDRIVE5_FANSPEEDINFO_GET)GetProcAddress(hDLL, "ADL_Overdrive5_FanSpeedInfo_Get");
	ADL_Overdrive5_FanSpeed_Set = (ADL_OVERDRIVE5_FANSPEED_SET)GetProcAddress(hDLL, "ADL_Overdrive5_FanSpeed_Set");
	ADL2_OverdriveN_SettingsExt_Get = (ADL2_OVERDRIVEN_SETTINGSEXT_GET)GetProcAddress(hDLL, "ADL2_OverdriveN_SettingsExt_Get");
	ADL2_OverdriveN_SettingsExt_Set = (ADL2_OVERDRIVEN_SETTINGSEXT_SET)GetProcAddress(hDLL, "ADL2_OverdriveN_SettingsExt_Set");

	if (NULL == ADL2_Main_Control_Create ||
		NULL == ADL2_Main_Control_Destroy ||
		NULL == ADL_Main_Control_Create ||
		NULL == ADL_Main_Control_Destroy ||
		NULL == ADL_Adapter_NumberOfAdapters_Get ||
		NULL == ADL_Adapter_AdapterInfo_Get ||
		NULL == ADL2_AdapterX2_Caps ||
		NULL == ADL2_Adapter_Active_Get ||
		NULL == ADL2_OverdriveN_Capabilities_Get ||
		NULL == ADL2_OverdriveN_SystemClocks_Get ||
		NULL == ADL2_OverdriveN_SystemClocks_Set ||
		NULL == ADL2_OverdriveN_MemoryClocks_Get ||
		NULL == ADL2_OverdriveN_MemoryClocks_Set ||
		NULL == ADL2_OverdriveN_PerformanceStatus_Get ||
		NULL == ADL2_OverdriveN_FanControl_Get ||
		NULL == ADL2_OverdriveN_FanControl_Set ||
		NULL == ADL2_AdapterX2_Caps ||
		NULL == ADL2_Overdrive6_CurrentPower_Get ||
		NULL == ADL2_Overdrive5_FanSpeed_Get ||
		NULL == ADL_Overdrive5_FanSpeedInfo_Get ||
		NULL == ADL_Overdrive5_FanSpeed_Set ||
		NULL == ADL2_OverdriveN_SettingsExt_Get ||
		NULL == ADL2_OverdriveN_SettingsExt_Set)
	{
		PRINTF("Failed to get ADL function pointers\n");
		return FALSE;
	}

	if (ADL_OK != ADL_Main_Control_Create(ADL_Main_Memory_Alloc, 1))
	{
		printf("Failed to initialize nested ADL context");
		return ADL_ERR;
	}

	if (ADL_OK != ADL2_Main_Control_Create(ADL_Main_Memory_Alloc, 1, &context))
	{
		printf("Failed to initialize nested ADL context");
		return ADL_ERR;
	}

	return TRUE;
}

void deinitializeADL()
{

	ADL_Main_Control_Destroy();

	FreeLibrary(hDLL);
}

int printODStats(int type, int maxrpm)
{
	int i;
	int active = 0;
	int cnt = 0;
	std::vector<int> bus;
	std::vector<int> adapter;
	std::vector<string> lpadapter;
	int stat = -1;

	// Repeat for all available adapters in the system
	for (i = 0; i < iNumberAdapters; i++)
	{
		if (lpAdapterInfo[i].iBusNumber > -1)
		{
			if (lpAdapterInfo[i].iVendorID == 1002)
			{
				if (std::find(bus.begin(), bus.end(), lpAdapterInfo[i].iBusNumber) == bus.end())
				{
					bus.push_back(lpAdapterInfo[i].iBusNumber);
				}
			}
		}
	}

	sort(bus.begin(), bus.end());

	if (type == -1)
	{
		for (int j = 0; j < bus.size(); j++)
		{
			for (i = 0; i < iNumberAdapters; i++)
			{
				if (lpAdapterInfo[i].iBusNumber == bus[j])
				{
					adapter.push_back(lpAdapterInfo[i].iAdapterIndex);
					lpadapter.push_back(lpAdapterInfo[i].strAdapterName);
					break;
				}
			}
		}
	}
	else
	{
		stat = bus[type];
		for (int j = 0; j < iNumberAdapters; j++)
		{
			if (lpAdapterInfo[j].iBusNumber == stat)
			{
				adapter.push_back(lpAdapterInfo[j].iAdapterIndex);
				lpadapter.push_back(lpAdapterInfo[j].strAdapterName);
				break;
			}
		}
	}

	for (i = 0; i < adapter.size(); i++)
	{
		int Watts = 0;
		int Temp = 0;
		double fanmin = 0;
		double fanmax = 0;
		double diff = 0;
		double xcalc = 0;
		double x = 0;
		double current = 0;
		int fanspeed = 0;
		int index = adapter[i];
		string name = lpadapter[i];


		ADLODNExtSettings *ExtCurrent = new ADLODNExtSettings;
		memset(ExtCurrent, 0, sizeof(ADLODNExtSettings));
		ADLODNExtSettingsInfo *ExtInfo = new ADLODNExtSettingsInfo;
		memset(ExtCurrent, 0, sizeof(ADLODNExtSettings));
		int64_t Unknown = 0;
		int64_t sSize = 0x0F;  // (points x 2)+5  sizeof(ADLODNExtSettings)/sizeof(int)
		int64_t ADLODNExtSettingsInfo_ptr = 0;
		int64_t ADLODNExtSettings_ptr = 0;  //0x23   //0x0F

		if (ADL_OK == ADL2_OverdriveN_SettingsExt_Get(context, index, &Unknown, &sSize, &ADLODNExtSettingsInfo_ptr, &ADLODNExtSettings_ptr))
		{
		 memcpy(ExtCurrent, (ADLODNExtSettings*)ADLODNExtSettings_ptr, sizeof(ADLODNExtSettings));
		 memcpy(ExtInfo, (ADLODNExtSettingsInfo*)ADLODNExtSettingsInfo_ptr, sizeof(ADLODNExtSettingsInfo));
		 free((ADLODNExtSettingsInfo*)ADLODNExtSettingsInfo_ptr);
		 free((ADLODNExtSettings*)ADLODNExtSettings_ptr);
		}
		else
		{
		 std::cout << "Gpu " << cnt << " Errors=" << "ADL2_OverdriveN_SettingsExt_Get is failed. Are drivers at least 18.12.1 or greater?" << std::endl;
		}

		ADLODNCapabilities overdriveCapabilities;
		memset(&overdriveCapabilities, 0, sizeof(ADLODNCapabilities));

		if (ADL_OK != ADL2_OverdriveN_Capabilities_Get(context, index, &overdriveCapabilities))
		{
			std::cout << "Gpu " << cnt << " Errors="
					  << "ADL2_OverdriveN_Capabilities_Get is failed" << std::endl;
		}

		if (ADL_OK != ADL2_OverdriveN_Temperature_Get(context, index, 1, &Temp))
		{
			std::cout << "Gpu " << cnt << " Errors="
					  << "ADL2_OverdriveN_Temperature_Get has failed" << std::endl;
		}

		if (ADL_OK != ADL2_Overdrive6_CurrentPower_Get(context, index, 0, &Watts))
		{
			std::cout << "Gpu " << cnt << " Errors="
					  << "ADL2_Overdrive6_CurrentPower_Get has failed" << std::endl;
		}

		ADLODNPerformanceLevels *odClockPerformanceLevels;

		int clocksize = sizeof(ADLODNPerformanceLevels) + sizeof(ADLODNPerformanceLevel) * (overdriveCapabilities.iMaximumNumberOfPerformanceLevels - 1);
		void *clockperformanceLevelsBuffer = new char[clocksize];
		memset(clockperformanceLevelsBuffer, 0, clocksize);
		odClockPerformanceLevels = (ADLODNPerformanceLevels *)clockperformanceLevelsBuffer;
		odClockPerformanceLevels->iSize = clocksize;
		odClockPerformanceLevels->iNumberOfPerformanceLevels = overdriveCapabilities.iMaximumNumberOfPerformanceLevels;
		//get GPU clocks
		if (ADL_OK != ADL2_OverdriveN_SystemClocks_Get(context, index, odClockPerformanceLevels))
		{
			std::cout << "Gpu " << cnt << " Errors="
					  << "ADL2_OverdriveN_SystemClocks_Get has failed" << std::endl;
		}

		ADLODNPerformanceLevels *odMemPerformanceLevels;

		int memsize = sizeof(ADLODNPerformanceLevels) + sizeof(ADLODNPerformanceLevel) * (overdriveCapabilities.iMaximumNumberOfPerformanceLevels - 1);
		void *memperformanceLevelsBuffer = new char[memsize];
		memset(memperformanceLevelsBuffer, 0, memsize);
		odMemPerformanceLevels = (ADLODNPerformanceLevels *)memperformanceLevelsBuffer;
		odMemPerformanceLevels->iSize = memsize;
		odMemPerformanceLevels->iNumberOfPerformanceLevels = overdriveCapabilities.iMaximumNumberOfPerformanceLevels;

		if (ADL_OK != ADL2_OverdriveN_MemoryClocks_Get(context, index, odMemPerformanceLevels))
		{
			std::cout << "Gpu " << cnt << " Errors="<< "ADL2_OverdriveN_MemoryClocks_Get has failed" << std::endl;
		}

		ADLFanSpeedValue FanSpeedValue = {0};
		FanSpeedValue.iSize = sizeof(ADLFanSpeedValue);
		FanSpeedValue.iSpeedType = ADL_DL_FANCTRL_SPEED_TYPE_RPM;

		if (ADL_OK != ADL2_Overdrive5_FanSpeed_Get(context, index, 0, &FanSpeedValue))
		{
			std::cout << "Gpu " << cnt << " Errors="
					  << "ADL_Overdrive5_FanSpeed_Get has failed" << std::endl;
		}

		ADLFanSpeedInfo FanSpeedInfo = {0};
		FanSpeedInfo.iSize = sizeof(ADLFanSpeedInfo);
		if (ADL_OK != ADL_Overdrive5_FanSpeedInfo_Get(index, 0, &FanSpeedInfo))
		{
			std::cout << "Gpu " << cnt << " Errors="
					  << "ADL_Overdrive5_FanSpeedInfo_Get has failed" << std::endl;
		}

		int level = 4;

		std::cout << "Gpu " << cnt << " Model= " << name << std::endl;
		std::cout << "Gpu " << cnt << " P5 Voltage= " << odClockPerformanceLevels->aLevels[5].iVddc << std::endl;
		string str2("Vega");
		if (name.find(str2) != string::npos)
		{
			std::cout << "Gpu " << cnt << " LastMemClock= " << odMemPerformanceLevels->aLevels[3].iClock / 100 << std::endl;
			std::cout << "Gpu " << cnt << " LastMemVDDC= " << odMemPerformanceLevels->aLevels[3].iVddc << std::endl;
		}
		else
		{
			std::cout << "Gpu " << cnt << " LastMemClock= " << odMemPerformanceLevels->aLevels[2].iClock / 100 << std::endl;
			std::cout << "Gpu " << cnt << " LastMemVDDC= " << odMemPerformanceLevels->aLevels[2].iVddc << std::endl;
		}

		std::cout << "Gpu " << cnt << " RPM=" << FanSpeedValue.iFanSpeed << std::endl;
		std::cout << "Gpu " << cnt << " Fan=" << ExtCurrent->Point[level].Percentage << std::endl;
		std::cout << "Gpu " << cnt << " Watts=" << Watts / 256 << std::endl;
		std::cout << "Gpu " << cnt << " Temp=" << Temp / 1000 << std::endl;
		std::cout << "" << std::endl;

		cnt++;

		// From Sample
		//  PRINTF("-----------------------------------------\n");
		//  PRINTF("ADL2_OverdriveN_FanControlX2_Get Data\n");
		//  PRINTF("-----------------------------------------\n");
		//  PRINTF("Min : %d, Max : %d, Step : %d\n" ,overdriveCapabilities.fanSpeed.iMin,overdriveCapabilities.fanSpeed.iMax, overdriveCapabilities.fanSpeed.iStep);
		//	PRINTF("odNFanControl.iFanControlMode : %d\n" , odNFanControl.iFanControlMode);
		//	PRINTF("odNFanControl.iMinFanLimit : %d\n" , odNFanControl.iMinFanLimit);
		//	PRINTF("odNFanControl.iTargetFanSpeed : %d\n" , odNFanControl.iTargetFanSpeed);
		//
		//	PRINTF ("-------- FAN (Min performance) ---------\n");
		//	PRINTF("Min : %d, Max : %d, Step : %d\n" ,overdriveCapabilities.minimumPerformanceClock.iMin,overdriveCapabilities.minimumPerformanceClock.iMax, overdriveCapabilities.minimumPerformanceClock.iStep);
		//	PRINTF("odNFanControl.iMinPerformanceClock : %d\n" , odNFanControl.iMinPerformanceClock);
		//
		//	PRINTF ("--------- FAN (Target Temp) -----------\n");
		//	PRINTF("Min : %d, Max : %d, Step : %d\n" ,overdriveCapabilities.fanTemperature.iMin,overdriveCapabilities.fanTemperature.iMax, overdriveCapabilities.fanTemperature.iStep);
		//	PRINTF("odNFanControl.iTargetTemperature : %d\n" , odNFanControl.iTargetTemperature);
		//	PRINTF("CURRENT WATTS : %d\n", Watts/256);
		//
		//	PRINTF("---------Fan Current Speed--------------\n");
		//	PRINTF("odNFanControl.iCurrentFanSpeed : %d\n" , odNFanControl.iCurrentFanSpeed);
		//	PRINTF("odNFanControl.iCurrentFanSpeedMode : %d\n" , odNFanControl.iCurrentFanSpeedMode);
		//	PRINTF("-----------------------------------------\n");
	}
	return 0;
}

int setODNFANParameters(int device, int value)
{
	int i;
	int j;
	int gpu = -1;
	int index = -1;
	std::vector<int> bus;
	std::vector<int> adapter;
	int selected = device;
	int setting = value;
	double fanmax = 0;
	double diff = 0;
	double xcalc = 0;
	double x = 0;
	int fanspeed = 0;

	// Repeat for all available adapters in the system
	for (i = 0; i < iNumberAdapters; i++)
	{
		if (lpAdapterInfo[i].iBusNumber > -1)
		{
			if (lpAdapterInfo[i].iVendorID == 1002)
			{
				if (std::find(bus.begin(), bus.end(), lpAdapterInfo[i].iBusNumber) == bus.end())
				{
					bus.push_back(lpAdapterInfo[i].iBusNumber);
				}
			}
		}
	}

	sort(bus.begin(), bus.end());
	gpu = bus[selected];

	for (j = 0; j < iNumberAdapters; j++)
	{
		if (lpAdapterInfo[j].iBusNumber == gpu)
		{
			index = lpAdapterInfo[j].iAdapterIndex;
		}
	}

	if (index == -1)
	{
		std::cout << "Error: Could Not Find GPU " << selected << std::endl;
		return (0);
	}

	return (0);
}

int main(int argc, char *argv[])
{
	//	Sleep(30000);

	if (initializeADL())
	{
		if (argc > 1)
		{

			// Obtain the number of adapters for the system
			if (ADL_OK != ADL_Adapter_NumberOfAdapters_Get(&iNumberAdapters))
			{
				PRINTF("Cannot get the number of adapters!\n");
				return 0;
			}

			if (0 < iNumberAdapters)
			{
				lpAdapterInfo = (LPAdapterInfo)malloc(sizeof(AdapterInfo) * iNumberAdapters);
				memset(lpAdapterInfo, '\0', sizeof(AdapterInfo) * iNumberAdapters);

				// Get the AdapterInfo structure for all adapters in the system
				ADL_Adapter_AdapterInfo_Get(lpAdapterInfo, sizeof(AdapterInfo) * iNumberAdapters);
			}

			int norpm = -1;
			int GPU = argc - 2;
			int GPUVar = -1;

			switch (*(argv[1]))
			{
			case 's':
				if (!argv[2])
					printODStats(norpm, GPUVar);
				else
				{
					for (int g = 0; g < GPU; g++)
					{
						int check = g + 2;
						int device = atoi(argv[check]);
						printODStats(g, device);
					}
				}
				break;
			case 'f':
				setODNFANParameters(atoi(argv[2]), atoi(argv[3]));
				break;
			}
		}

		ADL_Main_Memory_Free((void **)&lpAdapterInfo);

		deinitializeADL();
	}

	exit(0);
}
