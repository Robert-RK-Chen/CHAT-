#include <iostream>
#include <WS2tcpip.h>
#include <sstream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main()
{
	// Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		cerr << "Can't Initialize winsock! QuitTing." << endl;
		return 0;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a server socket! Quitting." << endl;
		return 0;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	//string ipAddress = "172.16.180.48";
	//hint.sin_port = htons(4999);
	//inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);		// Could also use inet_pton

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	fd_set master;		// 用于替代多线程
	FD_ZERO(&master);

	FD_SET(listening, &master);

	// create a running server handle multiple connection
	while (true)
	{
		fd_set copy = master;
		// The select function determines the status of one or more sockets, waiting if necessary,
		// to perform synchronous I/O.
		// Read more please go to https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-select
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];
			if (sock == listening)
			{
				// Accept a new connection
				SOCKET client = accept(listening, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &master);

				// Send a welcome message to the connected clients
				//string welcomeMsg = "Welcome to the AWESOME Chat Server!\r\n";
				//send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
			}
			else
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				// receieve message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					// Drop the client
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else
				{
					// Send message to other clients, and definiately NOT the listening socket
					for (int i = 0; i < master.fd_count; i++)
					{
						SOCKET outSock = master.fd_array[i];
						if (outSock != listening && outSock != sock)
						{
							ostringstream ss;
							ss <<  buf ;
							string strOut = ss.str();
							send(outSock, strOut.c_str(), strOut.size() + 1, 0);
						}
					}
				}
			}
		}
	}

	// Cleanup winsock
	WSACleanup();
	system("Pause");
	return 0;
}