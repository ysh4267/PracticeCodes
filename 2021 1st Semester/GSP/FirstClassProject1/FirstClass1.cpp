//서버함수는
//서버측 프로세스와 클라이언트측 프로세스 둘다 작동함
//소켓 - 소켓(WSAStartup)
//바인드
//리슨 ( 대기상태 )
//억셉트 - 커넥트
//리시브 - 샌드 (데이터를 주고받는과정)
//샌드 - 리시브 (페어로 이루어짐)
//클로즈 소켓 - 클로즈 소켓(WSACleanUp)

/*
* 바인드 함수
* 소켓상에 있는 정보를
* 넷상에 묶어준다
* ip주소와 port번호를 이용해서
* 소켓을 인터넷 주소에 묶어준다
* 서버포트는 미리 알려줘야한다
* 클라이언트 포트는 임의로 지정된다
* 
* 소켓에대한 디스크립터가 들어감
* 주소에대한 내용이 들어감
* 소켓주소구조체의 길이가 들어감
* 49번째 바인드 함수 ppt 페이지
* 
* htons 호스트 투 네트워크 s
* 
* 
* 소켓 주소 구조체는 다양한 형태로 존재
* 소켓함수를 만들때 SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0)
* TCP/IP 는 SOCKADDR_IN, IrDA 는 SOCKADDR_IRDA로 표현
* SOCKADDR_IN 에는 AF_INET와 포트번호, ip주소 항상 0으로 채우는 공간으로 이루어져있다
* 32비트이다
* 
*/
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#pragma comment(lib,"WS2_32.LIB")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>


int main()
{
    struct hostent* host;
    int ndx;

    host = gethostbyname("naver.com");

    if (!host)
    {
        printf("gethostbyname() 실행 실패/n");
        exit(1);
    }
    for (ndx = 0; NULL != host->h_addr_list[ndx]; ndx++)
        printf("%s\n", inet_ntoa(*(struct in_addr*)host->h_addr_list[ndx]));

    return 0;
}