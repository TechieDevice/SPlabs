#include "CppServer.h"

using namespace std;

//UDP SERVER

UdpServer::UdpServer() {}

UdpServer::~UdpServer() { if (status == server_status::up) stop(); }

void UdpServer::setServer(unsigned short new_port)
{ 
	port = new_port;
	if (status == server_status::up)
		stop();
}

UdpServer::server_status UdpServer::start()
{
#ifdef WIN32
	WSAStartup(MAKEWORD(2, 2), &wData);
#endif
	address.SIN_ADDR = INADDR_ANY;
	address.sin_port = htons(port);
	address.sin_family = AF_INET;
	serverS = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (serverS == -1) return status = server_status::socket_err;
	printf("Server configured successfully\n");
	
	if (bind(serverS, (struct sockaddr*)&address, sizeof(address)) < 0) return status = server_status::socket_err;
	printf("Socket binded successfully\n");
	
	status = server_status::up;
	return status;
}

void UdpServer::handle()
{
	while (status == server_status::up)
	{
		int size = 0;
		size = loadData();
		char* data = getData();
		data[size] = 0;
		printf("from: %u.%u.%u.%u \n",
				(unsigned char)(&from_address.SIN_ADDR)[0],
				(unsigned char)(&from_address.SIN_ADDR)[1],
				(unsigned char)(&from_address.SIN_ADDR)[2],
				(unsigned char)(&from_address.SIN_ADDR)[3]
				);
		printf("size: %d bytes\n", size);
		printf("received data: %s\n", data);
	}
}

int UdpServer::loadData() { return recvfrom(serverS, buffer, sizeof(buffer), 0, (struct sockaddr *)&from_address, &from_lenght); }

char* UdpServer::getData() { return buffer; }

void UdpServer::stop()
{
	status = server_status::off;
	CLOSESOCKET(serverS);
#ifdef WIN32
	WSACleanup();
#endif
	printf("Server was stoped\n");
}

//TCP SERVER

TcpServer::TcpServer() {}

TcpServer::~TcpServer() { if (status == server_status::up) stop(); }

void TcpServer::setServer(unsigned short new_port)
{ 
	port = new_port;
	if (status == server_status::up)
		stop();
}

TcpServer::server_status TcpServer::start()
{
#ifdef WIN32
	WSAStartup(MAKEWORD(2, 2), &wData);
#endif
	address.SIN_ADDR = INADDR_ANY;
	address.sin_port = htons(port);
	address.sin_family = AF_INET;
	
	serverS = socket(AF_INET, SOCK_STREAM, 0);
	
	if (serverS == -1) return status = server_status::socket_err;
	printf("Server configured successfully\n");
	
	if (bind(serverS, (struct sockaddr*)&address, sizeof(address)) < 0) return status = server_status::socket_err;
	printf("Socket binded successfully\n");
	
	if (listen(serverS, SOMAXCONN) < 0) return status = server_status::socket_err;
	printf("Start listenin at port%u\n", ntohs(address.sin_port));
	
	status = server_status::up;
	return status;
}

void TcpServer::handle()
{
	while (status == server_status::up)
	{
		WIN_AND_UNIX_SOCKET acceptS;
		WIN_AND_UNIX_SOCKADDR_IN clientAddr;
		ADDR addrlen = sizeof(clientAddr);
		if ((acceptS = accept(serverS, (struct sockaddr*)&clientAddr, &addrlen)) != 0)
		{
			unsigned int ip = clientAddr.SIN_ADDR;
			printf("Sended Client connected from 0  %u.%u.%u.%u:%u\n",
				(unsigned char)(&ip)[0],
				(unsigned char)(&ip)[1],
				(unsigned char)(&ip)[2],
				(unsigned char)(&ip)[3],
				ntohs(clientAddr.sin_port));
			TcpClient* client = new TcpClient(acceptS, clientAddr);
			client->~TcpClient();
		}
		SLEEP(30);
	}
}

void TcpServer::stop()
{
	status = server_status::off;
	CLOSESOCKET(serverS);
#ifdef WIN32
	WSACleanup();
#endif
	printf("Server was stoped\n");
}

//CLIENT

TcpClient::TcpClient(WIN_AND_UNIX_SOCKET socket, WIN_AND_UNIX_SOCKADDR_IN c_address)
{
	clientS = socket;
	clientAddress = c_address;
	receive();
}

TcpClient::~TcpClient()
{
	shutdown(clientS, 0);
	CLOSESOCKET(clientS);
	printf("Client disconnect\n");
}

int TcpClient::loadData() { return recv(clientS, buffer, sizeof(buffer), 0); }

char* TcpClient::getData() { return buffer; }

void TcpClient::receive()
{
	int size = 0;
	do {
		size = loadData();
		char* data = getData();
		data[size] = 0;
		cout
			<< "size: " << size << " bytes" << endl
			<< "received data: " << data << endl;
	} while (size > 0);
}
