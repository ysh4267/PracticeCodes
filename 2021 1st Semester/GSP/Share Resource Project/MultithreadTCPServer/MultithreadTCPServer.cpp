#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

int share;

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

//������
ServerClass::ServerClass()
{
    share = 0;
    // ���� �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return;
}

//�Ҹ���
ServerClass::~ServerClass()
{
    // closesocket()
    closesocket(listen_sock);

    // ���� ����
    WSACleanup();
}

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

// ���� �غ�
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

// ���� ���
void ServerClass::Connect()
{
    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // ������ Ŭ���̾�Ʈ ���� ���
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // ������ ����
        hThread = CreateThread(NULL, 0, ProcessClient,
            (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }
}

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ServerClass::ProcessClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    // Ŭ���̾�Ʈ ���� ���
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    while (1) {
        // ������ �ޱ�
        retval = recv(client_sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            //err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // ���� ������ ���
        buf[retval] = '\0';
        printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port), buf);

        // ������ ������
        retval = send(client_sock, buf, retval, 0);
        if (retval == SOCKET_ERROR) {
            //err_display("send()");
            break;
        }

        // share���� ����
        sprintf(buf, "%d", share);

        // share ������ ������
        retval = send(client_sock, buf, retval, 0);
        if (retval == SOCKET_ERROR) {
            //err_display("send()");
            break;
        }

        // share ������ �ޱ�
        retval = recv(client_sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            //err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        share = atoi(buf);

        // ���� ������ ���
        buf[retval] = '\0';
        printf("[Share] %s\n", buf);
    }

    // closesocket()
    closesocket(client_sock);
    printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
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