#include <stdio.h>
#include <Windows.h>
#include <string>

class CManager
{
private:
public:
	auto ModifyImportTable(IMAGE_IMPORT_DESCRIPTOR * iid, void * target, void * replacement)->BOOL;
	auto InstallHook(LPCSTR module, LPCSTR function, void * hook, void ** original)->BOOL;
	auto Setup()->BOOL;
};
extern CManager g_Manager;