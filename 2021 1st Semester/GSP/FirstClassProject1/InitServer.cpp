//�����Լ���
//������ ���μ����� Ŭ���̾�Ʈ�� ���μ��� �Ѵ� �۵���
//���� - ����(WSAStartup)
//���ε�
//���� ( ������ )
//���Ʈ - Ŀ��Ʈ
//���ú� - ���� (�����͸� �ְ�޴°���)
//���� - ���ú� (���� �̷����)
//Ŭ���� ���� - Ŭ���� ����(WSACleanUp)

/*
* ���ε� �Լ�
* ���ϻ� �ִ� ������
* �ݻ� �����ش�
* ip�ּҿ� port��ȣ�� �̿��ؼ�
* ������ ���ͳ� �ּҿ� �����ش�
* ������Ʈ�� �̸� �˷�����Ѵ�
* Ŭ���̾�Ʈ ��Ʈ�� ���Ƿ� �����ȴ�
* 
* ���Ͽ����� ��ũ���Ͱ� ��
* �ּҿ����� ������ ��
* �����ּұ���ü�� ���̰� ��
* 49��° ���ε� �Լ� ppt ������
* 
* htons ȣ��Ʈ �� ��Ʈ��ũ s
* 
* 
* ���� �ּ� ����ü�� �پ��� ���·� ����
* �����Լ��� ���鶧 SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0)
* TCP/IP �� SOCKADDR_IN, IrDA �� SOCKADDR_IRDA�� ǥ��
* SOCKADDR_IN ���� AF_INET�� ��Ʈ��ȣ, ip�ּ� �׻� 0���� ä��� �������� �̷�����ִ�
* 32��Ʈ�̴�
* 
*/
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#pragma comment(lib,"WS2_32.LIB")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg)
{
    LPVOID lpMsgBuf;
    //�������� ������ �����ϰ� ������ ������ �޼��� �ڽ��� �����ϴ� �Լ�
    //FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, = �ɼ�, 
    //NULL
    //WSAGetLastError(), = �����ڵ�
    //MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), = ���
    //(LPTSTR)&lpMsgBuf = ���� ���ڿ� �����ּ�
    //0, NULL����
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    //�޼����ڽ��� �Ҵ�� ���۸�
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    //���������� ������
    LocalFree(lpMsgBuf);
    exit(1);
}

int main(int argc, char* argv[])
{
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;
    //������ ������ �ش� �޼����ڽ� ����
    MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

    // socket()
    SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    //������ ������ INVALID_SOCKET�̸� �޼����ڽ��� ������
    if (tcp_sock == INVALID_SOCKET) err_quit("socket()");
    MessageBox(NULL, "TCP ���� ���� ����", "�˸�", MB_OK);

    // closesocket()�Լ��� �޸� ����
    closesocket(tcp_sock);
    // ���� ����
    WSACleanup();
    return 0;
}