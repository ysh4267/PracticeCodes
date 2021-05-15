#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TESTNAME "www.naver.com"

// 소켓 함수 오류 출력

void err_display(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
    return;
}

// 도메인 이름 -> IPv4 주소
BOOL GetIPAddr(char* name, IN_ADDR* addr)
{
    //이름으로 호스트 아이피를 얻어내는 함수
    HOSTENT* ptr = gethostbyname(name);
    if (ptr == NULL) {
        err_display("gethostbyname()");
        return FALSE;
    }
    if (ptr->h_addrtype != AF_INET)
        return FALSE;
    memcpy(addr, ptr->h_addr, ptr->h_length);
    return TRUE;
}

// IPv4 주소 -> 도메인 이름
BOOL GetDomainName(IN_ADDR addr, char* name, int namelen)
{
    HOSTENT* ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
    if (ptr == NULL) {
        err_display("gethostbyaddr()");
        return FALSE;
    }
    if (ptr->h_addrtype != AF_INET)
        return FALSE;
    strncpy(name, ptr->h_name, namelen);
    return TRUE;
}

int main(int argc, char* argv[])
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    printf("도메인 이름(변환 전) = %s\n", TESTNAME);

    // 도메인 이름 -> IP 주소
    IN_ADDR addr;
    if (GetIPAddr(TESTNAME, &addr)) {
        // 성공이면 결과 출력
        printf("IP 주소(변환 후) = %s\n", inet_ntoa(addr));

        // IP 주소 -> 도메인 이름
        char name[256];
        if (GetDomainName(addr, name, sizeof(name))) {
            // 성공이면 결과 출력
            printf("도메인 이름(다시 변환 후) = %s\n", name);
        }
    }

    WSACleanup();
    
    //실습 2
	time_t current_time;
	struct tm* struct_time;
    time(&current_time);
    struct_time = localtime(&current_time);
    printf("시간은 %4d 년 %2d 월 %2d 일 %2d시 %2d분 %2d초",
        struct_time->tm_year + 1900,
        struct_time->tm_mon + 1,
        struct_time->tm_mday,
        struct_time->tm_hour,
        struct_time->tm_min,
        struct_time->tm_sec);

    return 0;
}