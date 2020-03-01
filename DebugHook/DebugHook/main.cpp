#include <stdio.h>
#include <Windows.h>

void* g_pfWriteFile = NULL; // WriteFile() 주소
CREATE_PROCESS_DEBUG_INFO g_cpdi; // 디버그 정보
BYTE g_chINT32 = 0xcc, g_chOrgByte = 0; // INT32, 원래의 Byte

BOOL OnCreateProcessDebugEvent(LPDEBUG_EVENT pde)
{
	g_pfWriteFile = GetProcAddress(GetModuleHandleA("kernel32.dll"), "WriteFile");
	memcpy(&g_cpdi, &pde->u.CreateProcessInfo, sizeof(CREATE_PROCESS_DEBUG_INFO)); // 디버그 정보 저장
	
	// WriteFile()의 첫 바이트를 INT32 (0xCC) (Break Point) 로 변경
	ReadProcessMemory(g_cpdi.hProcess, g_pfWriteFile, &g_chOrgByte, sizeof(BYTE), NULL);
	WriteProcessMemory(g_cpdi.hProcess, g_pfWriteFile, &g_chINT32, sizeof(BYTE), NULL);

	return TRUE;
}

BOOL OnExeptionDebugEvent(LPDEBUG_EVENT pde)
{
	CONTEXT ctx;
	PCHAR Buffer = NULL;
	DWORD pToWrite, pBuffer;
	PEXCEPTION_RECORD per = &pde->u.Exception.ExceptionRecord; // 예외 정보
	
	// if BP
	if (per->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		// if WriteFile()
		if (per->ExceptionAddress == g_pfWriteFile)
		{
			// Unhook
			WriteProcessMemory(g_cpdi.hProcess, g_pfWriteFile, &g_chOrgByte, sizeof(BYTE), NULL);

			// Thread Context (Register)
			ctx.ContextFlags = CONTEXT_CONTROL;
			GetThreadContext(g_cpdi.hThread, &ctx);

			// WriteFile Param - 2(Buffer, ESP + 0x8), 3(Size To Write, ESP + 0xC)
			ReadProcessMemory(g_cpdi.hProcess, LPCVOID(ctx.Esp + 0x8), &pBuffer, sizeof(DWORD), NULL);
			ReadProcessMemory(g_cpdi.hProcess, LPCVOID(ctx.Esp + 0xc), &pToWrite, sizeof(DWORD), NULL);

			// Copy Param 2 (Buffer) to Heap
			Buffer = (PCHAR)malloc(pToWrite + 1);
			memset(Buffer, 0, pToWrite + 1);
			ReadProcessMemory(g_cpdi.hProcess, (LPVOID)pBuffer, Buffer, pToWrite, NULL);
			printf("\n --orginal string-- \n%s", Buffer);

			// Lower to Upper
			for (int i = 0; i < pToWrite; i++)
			{
				if (Buffer[i] >= 'a' && Buffer[i] <= 'z')
				{
					Buffer[i] = Buffer[i] - 'a' + 'A';
				}
			}
			printf("\n --converted string-- \n%s", Buffer);

			WriteProcessMemory(g_cpdi.hProcess, (LPVOID)pBuffer, Buffer, pToWrite, NULL);

			// Free Heap
			free(Buffer);

			// Debugee
			ctx.Eip = (DWORD)g_pfWriteFile;
			SetThreadContext(g_cpdi.hThread, &ctx);
			ContinueDebugEvent(pde->dwProcessId, pde->dwThreadId, DBG_CONTINUE);
			Sleep(0);

			// Hook
			WriteProcessMemory(g_cpdi.hProcess, g_pfWriteFile, &g_chINT32, sizeof(BYTE), NULL);

			return TRUE;
		}
	}

	return FALSE;
}

void DebugLoop()
{
	DEBUG_EVENT de;

	// Debug Event 반복
	while (WaitForDebugEvent(&de, INFINITE))
	{
		switch (de.dwDebugEventCode)
		{
		case CREATE_PROCESS_DEBUG_EVENT:
			OnCreateProcessDebugEvent(&de);
			break;
		case EXCEPTION_DEBUG_EVENT:
			if (OnExeptionDebugEvent(&de))
				continue;
			break;
		case EXIT_PROCESS_DEBUG_EVENT:
			goto END;
			break;
		}

		ContinueDebugEvent(de.dwProcessId, de.dwThreadId, DBG_CONTINUE);
	}
END:
	return;
}

int main(int argc, char* argv[])
{
	// PID Input
	DWORD PID;

	if (argc != 2)
	{
		printf("PID : ");
		scanf_s("%d", &PID);
	}
	else
	{
		PID = atoi(argv[1]);
	}


	// Debugee On
	if (!DebugActiveProcess(PID))
	{
		printf("Debug Active Process Failed...\n");
		return 1;
	}

	// Debug
	DebugLoop();

	return 0;
}