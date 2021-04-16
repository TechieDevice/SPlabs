#include "main.h"

int main(int argc, char *argv[], char *enpv[])
{
	shared shm;
	if (shm.getShm() == 1) exit(1);
	int num = atoi(argv[0]);
	int t;
	int temp;

  	for(int i = 0; i < MAX; i++)
    {
		t = 1;
		
		while (semctl (shm.semid, 0, GETVAL, 0) || shm.map_p->status != MAP_BLOCKED)
			if(shm.map_p->status == MAP_FINISH) 
				break;

		if(shm.map_p->status == MAP_FINISH) 
			break;

		shm.map_p->status = MAP_BLOCKED;
		semctl (shm.semid, 0, SETVAL, 1); 
		
		if (shm.map_p->status == MAP_FINISH)
				break;

		if (i != 0) shm.map_p->map[num][i-1] = temp;
		if (i != (MAX-1)) temp = shm.map_p->map[num][i];
		if (shm.map_p->map[num][i] == 2) t = rand() % 3 + 2;
		shm.map_p->map[num][i] = 1;
		if (i == (MAX-1)) 
		{
			shm.map_p->status = MAP_FINISH;
			break;
		}

		shm.map_p->status = MAP_UNBLOCKED; 
     	semctl (shm.semid, 0, SETVAL, 0);   

		usleep(10000*t*SPEED);
    }
  	shmdt (shm.map_p);      
  	exit (0);
}
