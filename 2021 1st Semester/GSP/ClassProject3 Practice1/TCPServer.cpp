#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

//����ó���Լ� �ΰ���
// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    //������ ���� ����
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0); //���� �ĺ���
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");//��������� ���ε���

    // bind()
    //���ε尡 �� �Ǿ����� ���Ϻ��� üũ
    SOCKADDR_IN serveraddr; //���� ����
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr)); //���ε��Լ�
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN); //�����Լ� SOMAXCONN��ŭ ���
    if (retval == SOCKET_ERROR) err_quit("listen()"); //����ó��

    // ������ ��ſ� ����� ����
    SOCKET client_sock; //Ŭ���̾�Ʈ������ ����θ����� ����
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

        // ������ Ŭ���̾�Ʈ ���� ���
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // Ŭ���̾�Ʈ�� ������ ���
        while (1) {
            // ������ �ޱ�
            retval = recv(client_sock, buf, BUFSIZE, 0);//���ú�
            //-1�̸� ��Ž���, �� �̿��ǰ��̸� �����͸� ����� ����Ʈ�� ����
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            // ���� ������ ���
            buf[retval] = '\0';
            printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port), buf);

            //�ǽ�2
            strcat_s(buf, _sfros); //buf ����ڿ� strcat_s�Լ��� ����Ͽ� _sfros���ڿ��� �Է�
            retval = strlen(buf);

            // ������ ������
            retval = send(client_sock, buf, strlen(buf), 0); //strlen(buf)�������� �����͸� client_sock�� ����
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }

        // closesocket()
        closesocket(client_sock);//���Ŀ� Ŭ���̾�Ʈ�� ���� �ݱ�
        printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    }

    // closesocket()
    closesocket(listen_sock); //������ ���� �ݱ�

    // ���� ����
    WSACleanup();
    return 0;
}
