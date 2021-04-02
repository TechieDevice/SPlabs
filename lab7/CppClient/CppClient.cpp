#include "CppClient.h"

Client::Client() {}

Client::connection_status Client::ClientSet(char* hostIP, unsigned short hostPort, unsigned short new_con_type)
{
#ifdef WIN32
    WSAStartup(MAKEWORD(2, 2), &wData);
#endif
    con_type = new_con_type;
    address.SIN_ADDR = HOST_ADDR(hostIP);
    address.sin_port = htons(hostPort);
    address.sin_family = AF_INET;
    if (con_type == 0) {
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        if (_socket == -1) return status = connection_status::socket_err;
        if (connect(_socket, (struct sockaddr*)&address, sizeof(address)) < 0) return status = connection_status::socket_err;
    }
    else if (con_type == 1) {
        _socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (_socket == -1) return status = connection_status::socket_err;
    }
    printf("Socket configured successfully\n");

    status = connection_status::conn;
    return status;
}

Client::~Client()
{
    status = connection_status::off;
    CLOSESOCKET(_socket);
#ifdef WIN32
    WSACleanup();
#endif
}

bool Client::sendData(char* data_string, size_t size)
{
    if (con_type == 0)
    {
        if (send(_socket, data_string, size, 0) < 0) return false;
        return true;
    }
    else if (con_type == 1)
    {
        if (sendto(_socket, data_string, size, 0, (const struct sockaddr*)&address, sizeof(address)) < 0) return false;
        return true;
    }
}

