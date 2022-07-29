#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

int main()
{
	// Initialize the Winsock
	WSADATA ws;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &ws);
	if (wsOK != 0)
	{
		std::cerr << "Can't Initialize Winsock Quiting" << std::endl;
		return -1;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		std::cerr << "Failed To Create Socket Quiting" << std::endl;
		return -2;
	}



	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(4444);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;	

	bind(s, (sockaddr*)&hint, sizeof(hint));

	listen(s, SOMAXCONN);



	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(s, (sockaddr*)&client, &clientSize);

	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Failed To Create Client Socket";
		return -3;
	}

	char host[NI_MAXHOST]; 
	char service[NI_MAXHOST]; 

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXHOST, 0) == 0)
	{
		std::cout << host << " connected on port " << service << std::endl;
	}
	else {
		std::cerr << "Cannot Get The Ip Address For The Client ! " << std::endl;

	}


	closesocket(s);


	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cerr << "Error in recv(). Quitting" << std::endl;
			break;
		}

		if (bytesReceived == 0)
		{
			std::cout << "Client Disconnected" << std::endl;
			break;
		}
		send(clientSocket, buf, bytesReceived + 1, 0);
	}


	closesocket(clientSocket);

	WSACleanup();


	
}
