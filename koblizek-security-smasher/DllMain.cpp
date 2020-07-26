/*
    - for a complete write up, go to https://medvedpvp.github.io
	- this has to be injected with a LoadLibrary method
*/

#include <Windows.h>
#include <iostream>
#include "CManager.hpp"

CManager g_Manager; // new object not an object ptr

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		if (g_Manager.Setup())
			printf("[KOBLIZEK_SECURITY_SMASHER] initialization succesfull!\n\n");
		else
			printf("[KOBLIZEK_SECURITY_SMASHER] initialization failed.\n\n");
	}

	return TRUE;
}