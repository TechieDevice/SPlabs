#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
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

#define SEM_ID	2001      /* ключ массива семафоров */
#define SHM_ID	2002      /* ключ разделяемой памяти */
#define PERMS	0666      /* права доступа */
#define MAX 50
#define RND 10
#define SPEED 30

/* коды сообщений */

#define MAP_BLOCKED  0 /* пустое сообщение */
#define MAP_UNBLOCKED 1 /* тип сообщения о том, что передана непустая строка */
#define MAP_FINISH 2 /* тип сообщения о том, что пора завершать обмен */

/* структура сообщения, помещаемого в разделяемую память */
typedef struct
{
  int status;
  int map[4][MAX];
} map_t;

class shared{
  public:
    shared();
    ~shared();
    int freeShm();
    int getShm();
    void* getShmptr(int id);
    int createShm();
    int getMemId();
    int semid;
    int shmid;
    map_t *map_p;
};