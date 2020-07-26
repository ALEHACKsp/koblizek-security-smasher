#include "CManager.hpp"
#include <Dbghelp.h>

auto CManager::ModifyImportTable(IMAGE_IMPORT_DESCRIPTOR * iid, void * target, void * replacement) -> BOOL
{
	IMAGE_THUNK_DATA * itd = ( IMAGE_THUNK_DATA * ) ( ( ( char * ) GetModuleHandle(NULL) ) + iid->FirstThunk );

	while (itd->u1.Function)
	{
		if (( ( void * ) itd->u1.Function ) == target)
		{
			// Temporary change access to memory area to R/W
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery(itd, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
			VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);

			// Replace entry!!
			*( ( void ** ) itd ) = replacement;

			// Restore memory permissions
			VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);

			return TRUE;
		}

		itd += 1;
	}
	return FALSE;
}

auto CManager::InstallHook(LPCSTR module, LPCSTR function, void * hook, void ** original) ->BOOL
{
	HMODULE process = GetModuleHandle(NULL);

	// Save original address to function
	*original = ( void * ) GetProcAddress(GetModuleHandleA(module), function);

	ULONG entrySize;

	IMAGE_IMPORT_DESCRIPTOR * iid = ( IMAGE_IMPORT_DESCRIPTOR * ) ImageDirectoryEntryToData(process, 1, IMAGE_DIRECTORY_ENTRY_IMPORT, &entrySize);

	// Search for module
	while (iid->Name)
	{
		const char * name = ( ( char * ) process ) + iid->Name;

		if (_stricmp(name, module) == 0)
		{
			return this->ModifyImportTable(iid, *original, hook);
		}
		iid += 1;
	}

	return FALSE;
}

int(__stdcall * RealSend)( SOCKET,
						  const char *, 
						  int, 
						  int );

int __stdcall HookedSend(SOCKET     s,
						 const char * buf,
						 int        len,
						 int        flags)
{
	printf("[KOBLIZEK_SECURITY_SMASHER] Loader zkousi poslat message '%s'.\n\nPovolit (y/n)? ", buf);
	char choice;
	scanf_s("%c", &choice);
	if (choice == 'y')
	{
		return RealSend(s, buf, len, flags);
	}
	else
	{
		printf("\n \n stopped a message.");
		return RealSend(s, "0", len, flags);

		return 0;
	}
}

auto CManager::Setup() -> BOOL
{
	if (this->InstallHook("Ws2_32.dll", "send", ( void * ) HookedSend, ( void ** ) ( &RealSend )))
		return TRUE;
	else
		return FALSE;
}