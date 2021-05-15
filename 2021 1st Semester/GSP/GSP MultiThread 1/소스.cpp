/*
	ThreadAdderOne.cpp
	프로그램 설명: 프로세스 유사형태 쓰레드 생성.
*/

#include <stdio.h>
#include <windows.h>
#include <tchar.h>

#define NUM_OF_GATE		3
DWORD pCounter = 0;
CRITICAL_SECTION   hCriticalSection;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	DWORD* nPtr = (DWORD*)lpParam;
	DWORD total = 0;
	EnterCriticalSection(&hCriticalSection);
	for (DWORD i = 0; i < 4; i++)
	{
		printf("%d ", nPtr[pCounter]);
		total += nPtr[pCounter];
		pCounter++;
	}
	LeaveCriticalSection(&hCriticalSection);
	return total;
}

int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadID[NUM_OF_GATE];
	HANDLE hThread[NUM_OF_GATE];

	InitializeCriticalSection(&hCriticalSection);

	DWORD paramThread[12] = { 0 };
	DWORD total = 0;
	DWORD result = 0;

	for (DWORD i = 0; i < 12; i++)
	{
		scanf_s("%d", &paramThread[i]);
		printf("param %d is %d\n", i, paramThread[i]);
	}

	for (DWORD i = 0; i < NUM_OF_GATE; i++)
	{
		hThread[i] =
			CreateThread(
				NULL, 0,
				ThreadProc,
				(LPVOID)(paramThread),
				0, &dwThreadID[i]
			);
	}

	for (DWORD i = 0; i < NUM_OF_GATE; i++)
	{
		ResumeThread(hThread[i]);
	}

	WaitForMultipleObjects(NUM_OF_GATE, hThread, TRUE, INFINITE);

	GetExitCodeThread(hThread[0], &result);
	total += result;

	GetExitCodeThread(hThread[1], &result);
	total += result;

	GetExitCodeThread(hThread[2], &result);
	total += result;

	_tprintf(_T("\ntotal (param[0] ~ param[11]): %d \n"), total);

	for (DWORD i = 0; i < NUM_OF_GATE; i++)
	{
		CloseHandle(hThread[i]);
	}

	DeleteCriticalSection(&hCriticalSection);

	return 0;
}

