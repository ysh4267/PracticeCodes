#include <iostream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib,"WS2_32.LIB")

int main()
{
	WSADATA wsaData;
	BYTE	nMajor = 2, nMinor = 2;
	WORD	wVersionRequested = MAKEWORD(nMinor, nMajor);
	int		nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet == SOCKET_ERROR)
	{
		cout << "������ ������ �ʱ�ȭ �� �� �����ϴ�." << endl;
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != nMajor ||
		HIBYTE(wsaData.wVersion) != nMinor)
	{
		cout << "������ ���� " << nMajor << "." << nMinor << " �� ����� �� �����ϴ�." << endl;
		WSACleanup();
		return -1;
	}

	cout << "������ ������ �ʱ�ȭ �Ͽ����ϴ�." << endl;

	WSACleanup();

	return 0;
}