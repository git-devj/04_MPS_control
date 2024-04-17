// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#pragma once
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>>
#include <WinSock2.h>
#include "MPS.h"

#define BUFFERSIZE 1024

using namespace std;

void client(SOCKET clientSock, SOCKADDR_IN clientAddr, vector<thread*>* clientlist)
{
	int nFunction = 0;
	byte recvData[256] = { 0, };
	byte sendData[256] = { 0, };

	cout << "Client connected IP address = " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;
	vector<wchar_t> buffer;
	char x[256];

	while (1)
	{
		MpsUpdate();
		if (recv(clientSock, (char*)&x, 256, 0) == SOCKET_ERROR)
		{
			cout << "error" << endl;
			break;
		}

		for (int i = 0; i < 256; i++)
		{
			recvData[i] = x[i];
		}

		nFunction = recvData[7];

		for (int i = 0; i < 8; i++)
			sendData[i] = recvData[i];

		switch (nFunction)
		{
		case 4:
			for (int i = 0; i < 80; i++)
			{
				sendData[9 + (i * 2)] = (byte)(output[i] / 256);
				sendData[10 + (i * 2)] = (byte)(output[i] % 256);
			}
			send(clientSock, (char*)sendData, 169, 0);
			break;

		case 16:
			for (int i = 0; i < 4; i++)
				sendData[8 + i] = recvData[8 + i];
			send(clientSock, (char*)sendData, 12, 0);
			for (int i = 0; i < 80; i++)
			{
				input[i] = (WORD)((recvData[13 + (i * 2)] << 8) | recvData[14 + (i * 2)]);
			}
			break;

		default:
			break;
		}

		continue;
	}
	closesocket(clientSock);
}

int main()
{
	vector<thread*> clientlist;

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return 1;
	}
	SOCKET serverSock = socket(PF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(502);

	if (bind(serverSock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		cout << "error" << endl;
		return 1;
	}

	if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "error" << endl;
		return 1;
	}
	cout << "Server Start" << endl;

	while (1)
	{
		int len = sizeof(SOCKADDR_IN);
		SOCKADDR_IN clientAddr;
		SOCKET clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &len);
		clientlist.push_back(new thread(client, clientSock, clientAddr, &clientlist));
	}

	if (clientlist.size() > 0)
	{
		for (auto ptr = clientlist.begin(); ptr < clientlist.end(); ptr++)
		{
			(*ptr)->join();
		}
	}
	closesocket(serverSock);
	WSACleanup();
	return 0;
}