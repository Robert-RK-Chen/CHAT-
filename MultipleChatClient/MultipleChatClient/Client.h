#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <WS2tcpip.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

void sendMsg(string userInput, SOCKET sock);
void recvMsg(char* buf, SOCKET sock);

class Client
{
private:
	string ipAddress;
	int port;
	SOCKET sock;
	sockaddr_in hint;

public:
	void setipAddress(string IPADDRESS)
	{
		ipAddress = IPADDRESS;
	}

	void setPort(int PORT)
	{
		port = PORT;
	}

	void InitWinsock()
	{
		WSADATA data;
		WORD ver = MAKEWORD(2, 2);
		int wsResult = WSAStartup(ver, &data);
		if (wsResult != 0)
		{
			exception e("δ������ winsock���������ͻ��ˣ�", 1);
			throw e;
		}
	}

	void CreateSock()
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			exception e("δ�ܴ��� socket �˿ڣ��������ͻ��ˣ�", 1);
			throw e;
			WSACleanup();
		}
	}

	void HintStructure()
	{
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	}

	void ConnServer()
	{
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			exception e ("�������ӵ�������򿪷���ˣ�", 1);
			throw e;
			closesocket(sock);
			WSACleanup();
		}
	}

	void Send(string userInput)
	{
		sendMsg(userInput, sock);
	}

	void GetMsg()
	{
		char buf[4096];
		recvMsg(buf, sock);
	}

	void Close()
	{
		closesocket(sock);
		WSACleanup();
	}
};

void sendMsg(string userInput, SOCKET sock)
{
	if (userInput.size() > 0)		// Make sure the user has typed in something
	{
		// Send the text
		send(sock, userInput.c_str(), userInput.size() + 1, 0);		// string end with '\0'
	}
}

void recvMsg(char* buf, SOCKET sock)
{
	while (true)
	{
		ZeroMemory(buf, 4096);
		int bytesReceieved = recv(sock, buf, 4096, 0);
		if (bytesReceieved > 0)
		{
			// �� string ������ʾ�������ڵ� MSGBOX ��
			::SendMessage(AfxGetMainWnd()->GetDlgItem(IDC_MSGBOX)->m_hWnd, EM_REPLACESEL, 0, (LPARAM)string(buf, 0, bytesReceieved).c_str());
		}
	}
}