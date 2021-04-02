#include "CppServer.h"

int main()
{
    int port, con_type;
    printf("Enter port ");
    scanf("%d", &port);
    printf("Enter tcp(0) or udp(1) ");
    scanf("%d", &con_type);
    if (con_type == 0) {
        TcpServer server;
	    server.setServer(port);
        if (server.start() == TcpServer::server_status::up) {
            printf("Server is up!\n");
            server.handle();
        } 
        else {
            printf("Server start error!\n");
            return -1;
        }
    } 
    else if (con_type == 1) {
        UdpServer server;
	    server.setServer(port);
        if (server.start() == UdpServer::server_status::up) {
            printf("Server is up!\n");
            server.handle();
        } 
        else {
            printf("Server start error!\n");
            return -1;
        }
    } 
    else {
        printf("Incorrect connection type\n");
        return -1;
    }
    
	return 0;
}