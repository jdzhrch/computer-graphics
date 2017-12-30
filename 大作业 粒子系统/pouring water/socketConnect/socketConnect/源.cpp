#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <Windows.h>

#pragma comment(lib,"ws2_32.lib")

int main(int argc, char *argv[])
{

	short port;

	port = 9400;
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("socket init error");
		exit(-1);
	}

	SOCKET serverSocket;

	if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("创建套接字失败！");
		exit(-1);
	}

	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);

	if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("socket port error:%d", port);
		exit(-1);
	}


	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("listen error");
		exit(-1);
	}
	printf("Server 9400 is listening......\n");
	SOCKET clientSocket;
	struct sockaddr_in clientAddress;

	memset(&clientAddress, 0, sizeof(clientAddress));
	int addrlen = sizeof(clientAddress);

	if ((clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &addrlen)) == INVALID_SOCKET)
	{
		printf("link error");
		exit(-1);
	}

	char buf[4096];

	while (1)
	{
		int bytes = recv(clientSocket, buf, sizeof(buf), 0);

		/*    if ((bytes=recv(clientSocket,buf,sizeof(buf),0))==SOCKET_ERROR)
		{
		printf("data error");
		exit(-1);
		}*/

		buf[bytes] = '\0';
		printf("Message from %s:\n %s\n", inet_ntoa(clientAddress.sin_addr), buf);
		//    if(send(clientSocket,buf,bytes,0)==SOCKET_ERROR)
		//    {
		//        printf("发送数据失败！");
		////        exit(-1);
		//    }

	}

	WSACleanup();
	exit(-1);

}