#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <signal.h>
#include <string.h>
#include <cstdlib>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX 50
#define RND 10
#define SPEED 30

#define MAP_BLOCKED  0
#define MAP_UNBLOCKED 1 
#define MAP_FINISH 2

typedef struct
{
  int status;
  int map[4][MAX];
  int id;
} map_t;

pthread_mutex_t mutex;

void mapSet(map_t* map_p) 
{
	memset(map_p->map, 0, sizeof (map_t));
	int n = rand() % RND + (MAX-12-RND);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < n; j++) 
		{
			int p = rand() % (MAX-3) +2;
			map_p->map[i][p] = 2;
		}
	}
}

void draw(int map[4][MAX])
{
	system("export TERM=xterm");
	system("clear");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			if (map[i][j] == 1) printf("*");
			else if (map[i][j] == 2) printf("|");
			else printf("_");
		}
		printf("\n");
	}
}

void* runner(void* map_p_void)
{
	int t;
	map_t* map_p = (map_t*)map_p_void;
	int num = map_p->id;
	int temp;
	printf("da %d\n", num);

  	for(int i = 0; i < MAX; i++)
    {
		t = 1;
		
		while (map_p->status != MAP_BLOCKED)
			if(map_p->status == MAP_FINISH) 
				break;

		if(map_p->status == MAP_FINISH) 
			break;

		map_p->status = MAP_BLOCKED;
		pthread_mutex_lock(&mutex);

		if(map_p->status == MAP_FINISH) { 
			pthread_mutex_unlock(&mutex);
			break;
		}

		if (i != 0) map_p->map[num][i-1] = temp;
		if (i != (MAX-1)) temp = map_p->map[num][i];
		if (map_p->map[num][i] == 2) t = rand() % 3 + 2;
		map_p->map[num][i] = 1;
		if (i == (MAX-1)) 
		{
			pthread_mutex_unlock(&mutex);
			map_p->status = MAP_FINISH;
			break;
		}

		pthread_mutex_unlock(&mutex);	
		map_p->status = MAP_UNBLOCKED; 

		usleep(10000*t*SPEED);
    }
	return NULL;
}

int main(int argc, char *argv[], char *enpv[])
{
    pthread_t tid[4];
    map_t map_p;
	srand(time(0));
	printf("da1");

	mapSet(&map_p);
	printf("da2");
	draw(map_p.map);

	printf("da3");
	for (int i = 0; i < 4; i++)
	{
		map_p.id = i;
		int status = pthread_create(&tid[i], NULL, runner, (void*)&map_p);
		if (status != 0) perror("pthread_create"), exit(EXIT_FAILURE);
		usleep(5000);
	}

	pthread_mutex_unlock(&mutex);
	map_p.status = MAP_UNBLOCKED;

	while (1)
	{
		if (map_p.status != MAP_BLOCKED)
		{
			map_p.status = MAP_BLOCKED;
			pthread_mutex_lock(&mutex); 

			draw(map_p.map);
			if (map_p.status == MAP_FINISH)
				break;

			pthread_mutex_unlock(&mutex);
			map_p.status = MAP_UNBLOCKED;

			usleep(3000*SPEED);
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		pthread_join(tid[i],NULL);
	}
	
    exit (0);
}