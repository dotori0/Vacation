#include <Windows.h>
#include <stdio.h>

// Thread Parameter
typedef struct _THREAD_PARAM
{
	FARPROC pFunc[2]; // LoadLibraryA(), GetProcAddress()
	char Buf[4][128]; // "user32.dll", "MessageBoxA", "내가 해킹했지롱~ ㅋㅋ", "승우의 메시지"
} THREAD_PARAM, *PTHREAD_PARAM;

typedef HMODULE (WINAPI* PFLOADLIBRARY) (LPCSTR lpLibFileName);
typedef FARPROC (WINAPI* PFGETPROCADDRESS) (HMODULE hModule, LPCSTR lpProcName);
typedef int (WINAPI* PFMESSAGEBOXA) (HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

DWORD WINAPI ThreadProc(LPVOID lParam)
{
	PTHREAD_PARAM pParam = (PTHREAD_PARAM)lParam;
	HMODULE hMod;
	FARPROC pFunc;

	// hMod = LoadLibraryA ("user32.dll")
	hMod = ((PFLOADLIBRARY)pParam->pFunc[0])(pParam->Buf[0]);

	// pFunc = GetProcAddress (hMod, "MessageBoxA")
	pFunc = (FARPROC)((PFGETPROCADDRESS)pParam->pFunc[1])(hMod, pParam->Buf[1]);

	// pFunc ( NULL, "내가 해킹했지롱~ ㅋㅋ", "승우의 메시지", MB_OK)
	((PFMESSAGEBOXA)pFunc)(NULL, pParam->Buf[2], pParam->Buf[3], MB_OK);
	
	return 0;
}

BOOL InjectCode(DWORD PID)
{
	THREAD_PARAM param = { 0, };
	void* Buf[2] = { 0, };

	// param 세팅
	HMODULE hMod = GetModuleHandleA("kernel32.dll");
	
	param.pFunc[0] = GetProcAddress(hMod, "LoadLibraryA");
	param.pFunc[1] = GetProcAddress(hMod, "GetProcAddress");

	strcpy_s(param.Buf[0], "user32.dll");
	strcpy_s(param.Buf[1], "MessageBoxA");
	strcpy_s(param.Buf[2], "I Hacked You~~ LOL");
	strcpy_s(param.Buf[3], "Message From YSW");

	// Open Process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	// THREAD_PARAM Virtual Allocation
	DWORD Size = sizeof(THREAD_PARAM);
	Buf[0] = VirtualAllocEx(hProcess, NULL, Size, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, Buf[0], (LPVOID)&param, Size, NULL);

	// ThreadProc Virtual Allocation
	Size = (DWORD)InjectCode - (DWORD)ThreadProc;
	Buf[1] = VirtualAllocEx(hProcess, NULL, Size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(hProcess, Buf[1], (LPVOID)ThreadProc, Size, NULL);

	// Create Remote Thread
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)Buf[1], Buf[0], 0, NULL);
	WaitForSingleObject(hThread, INFINITE);

	// Close Handle
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Wrong Usage\n");
		return 1;
	}

	InjectCode(atoi(argv[1]));

	return 0;
}