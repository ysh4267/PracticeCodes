#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

//오류처리함수 두가지
// 소켓 함수 오류 출력 후 종료
void err_quit(const char *msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char *msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char *)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

int main(int argc, char *argv[])
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    //서버측 소켓 생성
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0); //소켓 식별자
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");//만든소켓을 바인드함

    // bind()
    //바인드가 잘 되었는지 리턴벨류 체크
    SOCKADDR_IN serveraddr; //소켓 생성
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr)); //바인드함수
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN); //리슨함수 SOMAXCONN만큼 듣기
    if (retval == SOCKET_ERROR) err_quit("listen()"); //예외처리

    // 데이터 통신에 사용할 변수
    SOCKET client_sock; //클라이언트측과의 논리통로를위한 소켓
    SOCKADDR_IN clientaddr;
    int addrlen;

    char buf[BUFSIZE + 1];
    char _sfros[] = " from Server\0";

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // 접속한 클라이언트 정보 출력
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // 클라이언트와 데이터 통신
        while (1) {
            // 데이터 받기
            retval = recv(client_sock, buf, BUFSIZE, 0);//리시브
            //-1이면 통신실패, 그 이외의값이면 데이터를 통신함 바이트의 숫자
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            // 받은 데이터 출력
            buf[retval] = '\0';
            printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port), buf);

            //실습2
            strcat_s(buf, _sfros); //buf 문장뒤에 strcat_s함수를 사용하여 _sfros문자열을 입력
            retval = strlen(buf);

            // 데이터 보내기
            retval = send(client_sock, buf, strlen(buf), 0); //strlen(buf)사이즈의 데이터를 client_sock에 전송
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }

        // closesocket()
        closesocket(client_sock);//이후에 클라이언트측 소켓 닫기
        printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    }

    // closesocket()
    closesocket(listen_sock); //서버측 소켓 닫기

    // 윈속 종료
    WSACleanup();
    return 0;
}
