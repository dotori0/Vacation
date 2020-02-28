#include <Windows.h>
#include <tchar.h>
#include <process.h>

HMODULE g_hMod = NULL;

DWORD ThreadProc(LPVOID lParam)
{
	TCHAR szPath[_MAX_PATH] = { 0, };
	MessageBox(NULL, _T("´Ô ÇØÅ·´çÇÔ ¾ý¤»¤»"), _T("½Â¿ìÀÇ ¸Þ½ÃÁö"), MB_OK);
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpReserved)
{
	HANDLE hThread = NULL;

	g_hMod = (HMODULE)hinstDll;

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		hThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ThreadProc, NULL, 0, NULL);
		CloseHandle(hThread);
		break;
	}

	return TRUE;
}