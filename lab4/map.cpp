#include "main.h"

void mapSet(shared* shm) 
{
	memset(shm->map_p->map, 0, sizeof (map_t));
	int n = rand() % RND + (MAX-12-RND);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < n; j++) 
		{
			int p = rand() % (MAX-3) +2;
			shm->map_p->map[i][p] = 2;
		}
	}
}

void draw(int map[4][MAX])
{
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

int main(int argc, char *argv[], char *enpv[])
{
    pid_t pid;
    shared shm;
	srand(time(0));
	if (shm.createShm() == 1) exit(1);

	mapSet(&shm);
	draw(shm.map_p->map);
	char num[2];
    
	for (int i = 0; i < 4; i++)
	{
		sprintf(num, "%d", i);
		pid=fork();
		if(pid == -1) 
		{
			perror("fork");
			exit(1);
		}
		if(pid == 0)
		{
			pid = execl("runner", num, NULL);
            printf("runner start error\n");
            exit(1);
		}
	}

	semctl (shm.semid, 0, SETVAL, 0);
	shm.map_p->status = MAP_UNBLOCKED;

	while (1)
	{
		if (shm.map_p->status != MAP_BLOCKED)
		{
			if (semctl (shm.semid, 0, GETVAL, 0))    
				continue;

			shm.map_p->status = MAP_BLOCKED;
			semctl (shm.semid, 0, SETVAL, 1); 

			draw(shm.map_p->map);
			if (shm.map_p->status == MAP_FINISH)
				break;

			shm.map_p->status = MAP_UNBLOCKED; 
			semctl (shm.semid, 0, SETVAL, 0); 

			usleep(3000*SPEED);
		}
	}

	shm.~shared();
    exit (0);
}