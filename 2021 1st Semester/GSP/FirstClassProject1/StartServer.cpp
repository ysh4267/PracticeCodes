#include <iostream>
//���� �������
#include <winsock2.h>

using namespace std;

//���� ���̺귯�� ����
#pragma comment(lib,"WS2_32.LIB")

int main()
{
	//������⺻ ���ϵ�����Ÿ���� ���
	WSADATA wsaData;
	//������ ������ ����ϱ� ���Ѻ�����
	BYTE	nMajor = 2, nMinor = 2;
	WORD	wVersionRequested = MAKEWORD(nMinor, nMajor);
	//������ ���� �����Լ�
	int	 nRet = WSAStartup(wVersionRequested, &wsaData);
	//���ӹ����� ��Ÿ����ó��
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
	//������ ���� �����Լ�
	WSACleanup();

	return 0;
}