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


int main()
{
    struct hostent* host;
    int ndx;

    host = gethostbyname("naver.com");

    if (!host)
    {
        printf("gethostbyname() ���� ����/n");
        exit(1);
    }
    for (ndx = 0; NULL != host->h_addr_list[ndx]; ndx++)
        printf("%s\n", inet_ntoa(*(struct in_addr*)host->h_addr_list[ndx]));

    return 0;
}