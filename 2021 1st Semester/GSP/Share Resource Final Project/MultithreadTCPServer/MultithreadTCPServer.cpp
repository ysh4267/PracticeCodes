#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

int share;
HANDLE hEvent;

class ServerClass {
private:
    WSADATA wsa;
    SOCKET listen_sock;
    SOCKADDR_IN serveraddr;
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread;

public:
    ServerClass();
    ~ServerClass();
    void err_quit(char*);
    void err_display(char*);
    void ReadyServer();
    void Connect();
    static DWORD WINAPI ProcessClient(LPVOID arg);
};

//생성자
ServerClass::ServerClass()
{
    share = 0;
    // 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return;
}

//소멸자
ServerClass::~ServerClass()
{
    // closesocket()
    closesocket(listen_sock);

    // 윈속 종료
    WSACleanup();
}

void UpdateShare(char* buf) {
    SetEvent(hEvent);
    share = atoi(buf);
    return;
}

// 소켓 함수 오류 출력 후 종료
void ServerClass::err_quit(char* msg)
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
void ServerClass::err_display(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

// 소켓 준비
void ServerClass::ReadyServer()
{
    int retval;

    // socket()
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");
}

// 소켓 통신
void ServerClass::Connect()
{
    hEvent = CreateEvent( // event object 생성.
        NULL,  // 상속 불가.
        TRUE,  // manual-reset mode로 생성.
        FALSE,  // non-signaled 상태로 생성.
        NULL  // 이름 없는 event.
    );

    SetEvent(hEvent); // event의 state를 signaled 상태로 변경.

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // 접속한 클라이언트 정보 출력
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // 스레드 생성
        hThread = CreateThread(NULL, 0, ProcessClient,
            (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }
}

// 클라이언트와 데이터 통신
DWORD WINAPI ServerClass::ProcessClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];
    char Tbuf[BUFSIZE + 1];

    // 클라이언트 정보 얻기
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    while (1) {
        // share 안에 들어갈 숫자 데이터 받기
        retval = recv(client_sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            //err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // 받은 share 숫자 데이터 출력
        buf[retval] = '\0';
        printf("[TCP/%s:%d] [Share] %s\n", inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port), buf);

        // 변경전 share정보 삽입
        sprintf(Tbuf, "%d", share);

        // 변경전 share 데이터 보내기
        retval = send(client_sock, Tbuf, retval, 0);
        if (retval == SOCKET_ERROR) {
            //err_display("send()");
            break;
        }

        // share 변경
        WaitForSingleObject(hEvent, INFINITE);
        UpdateShare(buf);
        
        // buf 초기화
        for (auto& ch : buf) ch = '\0';

        // share정보 삽입
        sprintf(buf, "%d", share);

        // 변경된 share 데이터 보내기
        retval = send(client_sock, buf, retval, 0);
        if (retval == SOCKET_ERROR) {
            //err_display("send()");
            break;
        }

    }

    // closesocket()
    closesocket(client_sock);
    printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    return 0;
}


int main(int argc, char* argv[])
{
    ServerClass Server;
    Server.ReadyServer();
    Server.Connect();

    return 0;
}