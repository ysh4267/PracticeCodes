#include <iostream>
//윈속 헤더파일
#include <winsock2.h>

using namespace std;

//윈속 라이브러리 파일
#pragma comment(lib,"WS2_32.LIB")

int main()
{
	//윈도우기본 소켓데이터타입을 사용
	WSADATA wsaData;
	//윈도우 소켓을 사용하기 위한변수들
	BYTE	nMajor = 2, nMinor = 2;
	WORD	wVersionRequested = MAKEWORD(nMinor, nMajor);
	//윈도우 소켓 시작함수
	int	 nRet = WSAStartup(wVersionRequested, &wsaData);
	//윈속버전및 기타에러처리
	if (nRet == SOCKET_ERROR)
	{
		cout << "윈도우 소켓을 초기화 할 수 없습니다." << endl;
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != nMajor ||
		HIBYTE(wsaData.wVersion) != nMinor)
	{
		cout << "윈도우 소켓 " << nMajor << "." << nMinor << " 을 사용할 수 없습니다." << endl;
		WSACleanup();
		return -1;
	}

	cout << "윈도우 소켓을 초기화 하였습니다." << endl;
	//윈도우 소켓 종료함수
	WSACleanup();

	return 0;
}