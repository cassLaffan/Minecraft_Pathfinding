#include "Metrics.h"

/*
* Memory functions.
* 0 = virtual
* 1 = physical
*/
DWORDLONG getCurrent(int option) {
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	if (option) {
		//physical
		return memInfo.ullTotalPhys;
	}
	else {
		//virtual
		return memInfo.ullTotalPageFile;
	}
	
}

DWORDLONG getTotal(int option) {
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	if (option) {
		//physical
		return memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	}
	else {	
		//virtual
		return memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
	}
}

size_t getCurrentProcess(int option) {
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	if (option) {
		//physical
		return pmc.WorkingSetSize;
	}
	else {
		//virtual
		return pmc.PrivateUsage;
	}
}

/*
* CPU Usage

double getCurrentCPU() {
	static PDH_HQUERY cpuQuery;
	static PDH_HCOUNTER cpuTotal;

	PdhOpenQuery(NULL, NULL, &cpuQuery);

	PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);

	PDH_FMT_COUNTERVALUE counterVal;

	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
	return counterVal.doubleValue;
}
*/
