#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>

BOOL Injection(int PID, TCHAR* DllPath)
{
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	HMODULE hMod = NULL;

	void* Buf = NULL;
	DWORD PathLen = (DWORD)(_tcslen(DllPath) + 1) * sizeof(TCHAR);
	LPTHREAD_START_ROUTINE hThreadProc;

	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID)))
	{
		_tprintf(_T("프로세스를 열 수 없습니다.\n"));
		return FALSE;
	}

	Buf = VirtualAllocEx(hProcess, NULL, PathLen, MEM_COMMIT, PAGE_READWRITE);

	WriteProcessMemory(hProcess, Buf, (LPVOID)DllPath, PathLen, NULL);

	hMod = GetModuleHandle(_T("kernel32.dll"));
	hThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");

	hThread = CreateRemoteThread(hProcess, NULL, 0, hThreadProc, Buf, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;
}

BOOL Ejection(DWORD PID, TCHAR* DllName)
{
	HANDLE hProcess, hThread;
	HMODULE hMod;
	LPTHREAD_START_ROUTINE pThreadProc = NULL;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	BOOL bMore = FALSE, bFound = FALSE;
	MODULEENTRY32 me = { sizeof(me) };

	bMore = Module32First(hSnapshot, &me);
	for (; bMore; bMore = Module32Next(hSnapshot, &me))
	{
		if (!_tcsicmp(me.szModule, DllName) || !_tcsicmp(me.szExePath, DllName))
		{
			bFound = TRUE;
			break;
		}
	}

	if (!bFound)
	{
		_tprintf(_T("Can't Find The DLL\n"));
		CloseHandle(hSnapshot);
		return FALSE;
	}

	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID)))
	{
		_tprintf(_T("Can't Open The Process.\n"));
		return FALSE;
	}

	hMod = GetModuleHandle(_T("kernel32.dll"));
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "FreeLibrary");

	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, me.modBaseAddr, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);
	CloseHandle(hSnapshot);

	return TRUE;
}

int _tmain(int argc, TCHAR* argv[])
{
	if (argc != 3)
	{
		_tprintf(_T("잘못된 사용 입니다.\n"));
		return 1;
	}

	// Injection
	if (Injection(_tstol(argv[1]), argv[2]))
		_tprintf(_T("Injection Succeed\n"));
	else
	{
		_tprintf(_T("Injection Failed\n"));
		return 0;
	}

	// Ejection
	if (Ejection(_tstol(argv[1]), _tcsrchr(argv[2], '\\') + 1))
		_tprintf(_T("Ejection Succeed\n"));
	else
		_tprintf(_T("Ejection Failed\n"));

	return 0;
}