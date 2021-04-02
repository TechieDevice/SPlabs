#include "CppClient.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <signal.h>

static volatile int keepRunning = 1;

void getDataString(char** p_data_string)
{
	*p_data_string = new char[MAX_LENGTH + 1];

	printf("enter data\n");

	char* data = *p_data_string;
	int len = 0;
	while (true)
	{
		*data = getchar();
		if (*data == '\n')
		{
			*data = '\0';
			return;
		}
		data++;
		len++;
		if (len == MAX_LENGTH)
		{
			*data = '\0';

			return;
		}
	}
}

void dataReading(char* &buf, std::condition_variable &cv, std::mutex &ready, bool &write_ready, bool &read_ready)
{
	char* data_string;
	printf("reader start\n");

	while (true)
	{
		getDataString(&data_string);

		{
			std::unique_lock<std::mutex> locker(ready);
			if (!write_ready) cv.wait(locker, [&] {return write_ready; });
		}
		
		buf = data_string;

		write_ready = false;
		read_ready = true;

		cv.notify_one();
	}
}

void signal_callback_handler(int signum) 
{
	printf("Caught signal %d\n", signum);
	keepRunning = 0;
	exit(signum);
}

int main()
{
	Client client;
	std::condition_variable cv;
	std::mutex ready;
	bool write_ready = true;
	bool read_ready = false;
	char* buf;
	char* data_string;
	signal(SIGINT, signal_callback_handler);
	printf("settings done\n");

	printf("ented host ip and port\n");
	char hostIP[15];
	unsigned short hostPort = 0;
	scanf("%s", hostIP);
	scanf("%hd", &hostPort);
	getchar();
	printf("Enter tcp(0) or udp(1) ");
	int con_type;
	scanf("%d", &con_type);
	getchar();
	
	if (client.ClientSet(&hostIP[0], hostPort, con_type) == Client::connection_status::conn) {
		printf("Connected to server\n");
	}
	else {
		printf("Connection error!\n");
		return -1;
	}

	std::thread reader(dataReading, std::ref(buf), ref(cv), ref(ready), std::ref(write_ready), std::ref(read_ready));
	reader.detach();
	printf("reader detach\n");
	
	while (keepRunning == 1)
	{
		{
			std::unique_lock<std::mutex> locker(ready);
			cv.wait(locker, [&] {return read_ready; });
		}

		data_string = buf;
		read_ready = false;
		write_ready = true;

		cv.notify_one();

		printf("sending data - %s\n", data_string);

		int size = strlen(data_string);
		if (!(client.sendData(data_string, size))) printf("send error");
	}
	reader.~thread();
	client.~Client();

    return 0;   
}