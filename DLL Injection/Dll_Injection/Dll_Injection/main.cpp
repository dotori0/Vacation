#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>

enum { INJECTION_MODE, EJECTION_MODE };

DWORD nMode;

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

BOOL InjectAll(PTCHAR DllPath)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pe;

	pe.dwSize = sizeof(PROCESSENTRY32);

	Process32First(hSnapShot, &pe);
	do 
	{
		DWORD PID = pe.th32ProcessID;

		if (PID < 100)
			continue;

		if (nMode == INJECTION_MODE)
			Injection(PID, DllPath);
		else
			Ejection(PID, DllPath);
	} while (Process32Next(hSnapShot, &pe));

	CloseHandle(hSnapShot);
	return TRUE;
}

int _tmain(int argc, TCHAR* argv[])
{
	if (argc != 4 && argc != 3)
	{
		_tprintf(_T("잘못된 사용 입니다.\n"));
		return 1;
	}

	if (!_tcsicmp(argv[1], _T("i")))
	{
		// Injection
		if (Injection(_tstol(argv[2]), argv[3]))
			_tprintf(_T("Injection Succeed\n"));
		else
		{
			_tprintf(_T("Injection Failed\n"));
			return 0;
		}
	}
	else if (!_tcsicmp(argv[1], _T("e")))
	{
		// Ejection
		if (Ejection(_tstol(argv[2]), _tcsrchr(argv[3], '\\') + 1))
			_tprintf(_T("Ejection Succeed\n"));
		else
			_tprintf(_T("Ejection Failed\n"));
	}
	else
	{
		if (!_tcsicmp(argv[1], _T("ia")))
			nMode = INJECTION_MODE;
		else if (!_tcsicmp(argv[1], _T("ie")))
			nMode = EJECTION_MODE;
		else
			return 1;

		if (InjectAll(argv[2]))
			_tprintf(_T("Succeed\n"));
		else
			_tprintf(_T("Failed\n"));
	}

	return 0;
}	