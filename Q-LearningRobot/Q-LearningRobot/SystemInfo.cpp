#include <iostream>
#include <string>
#include <windows.h>
#pragma warning(disable: 28159)//avoid GetVersionEx to be warned

using namespace std;

#define  GBYTES  1073741824
#define  MBYTES  1048576
#define  KBYTES  1024
#define  DKBYTES 1024.0

static const int k_max_info_buffer = 256;

void get_os_info()
{
	OSVERSIONINFO os_version_info = { sizeof(OSVERSIONINFO) };
	GetVersionEx(&os_version_info);//C/C++ > General > SDL checks > No
	string os_name;

	if (os_version_info.dwMajorVersion == 5 && os_version_info.dwMinorVersion == 0) os_name = "windows 2000";
	else if (os_version_info.dwMajorVersion == 5 && os_version_info.dwMinorVersion == 1) os_name = "windows XP";
	else if (os_version_info.dwMajorVersion == 6 && os_version_info.dwMinorVersion == 0) os_name = "windows 2003";
	else if (os_version_info.dwMajorVersion == 5 && os_version_info.dwMinorVersion == 2) os_name = "windows vista";
	else if (os_version_info.dwMajorVersion == 6 && os_version_info.dwMinorVersion == 1) os_name = "windows 7";
	else if (os_version_info.dwMajorVersion == 6 && os_version_info.dwMinorVersion == 2) os_name = "windows 10";
	else os_name = "unknown";

	cout << ">>  OS: " << os_name << endl;
}

DWORD deax;
DWORD debx;
DWORD decx;
DWORD dedx;

void init_cpu(DWORD veax)
{
	__asm
	{
		mov eax, veax
		cpuid
		mov deax, eax
		mov debx, ebx
		mov decx, ecx
		mov dedx, edx
	}
}

long get_cpu_freq()
{
	int start, over;

	_asm
	{
		RDTSC
		mov start, eax
	}

	Sleep(50);

	_asm
	{
		RDTSC
		mov over, eax
	}

	return (over - start) / 50000;
}

string get_manufacture_id()
{
	char manu_id[25] = { 0 };
	init_cpu(0);
	memcpy(manu_id + 0, &debx, 4);
	memcpy(manu_id + 4, &dedx, 4);
	memcpy(manu_id + 8, &decx, 4);

	return manu_id;
}

string get_cpu_type()
{
	const DWORD id = 0x80000002;//start 0x80000002 end to 0x80000004  
	char cpu_type[49] = { 0 };

	for (DWORD t = 0; t < 3; t++)
	{
		init_cpu(id + t);
		memcpy(cpu_type + 16 * t + 0, &deax, 4);
		memcpy(cpu_type + 16 * t + 4, &debx, 4);
		memcpy(cpu_type + 16 * t + 8, &decx, 4);
		memcpy(cpu_type + 16 * t + 12, &dedx, 4);
	}

	return cpu_type;
}

void get_cpu_info()
{
	cout << ">> CPU: " << get_cpu_type() << " [current: " << get_cpu_freq() << "MHz]" << endl;
}

void get_memory_info()
{
	string memory_info;
	MEMORYSTATUSEX statusex;
	statusex.dwLength = sizeof(statusex);

	if (GlobalMemoryStatusEx(&statusex))
	{
		unsigned long long total = 0, remain_total = 0, avl = 0, remain_avl = 0;
		double decimal_total = 0, decimal_avl = 0;
		remain_total = statusex.ullTotalPhys % GBYTES;
		total = statusex.ullTotalPhys / GBYTES;
		avl = statusex.ullAvailPhys / GBYTES;
		remain_avl = statusex.ullAvailPhys % GBYTES;

		if (remain_total > 0) decimal_total = (remain_total / MBYTES) / DKBYTES;
		if (remain_avl > 0) decimal_avl = (remain_avl / MBYTES) / DKBYTES;

		decimal_total += (double)total;
		decimal_avl += (double)avl;
		char buffer[k_max_info_buffer];
		sprintf_s(buffer, k_max_info_buffer, ">> MEM: %.2f GB [%.2f GB available]", decimal_total, decimal_avl);
		memory_info.append(buffer);
	}
	else memory_info.append(">> MEM: can't get any information.");

	cout << memory_info << endl;
}

void get_system_info()
{
	get_cpu_info();
	get_memory_info();
	get_os_info();
}
