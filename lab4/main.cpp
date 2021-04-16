#include "main.h"

shared::shared() {}

shared::~shared() { shared::freeShm(); }
 
int shared::createShm()
{
    /* создание массива семафоров из одного элемента */
    if ((semid = semget (SEM_ID, 1, PERMS | IPC_CREAT)) < 0) {
        printf("server: can not create semaphore"); 
        return 1; 
    }

    /* создание сегмента разделяемой памяти */
    if ((shmid = shmget (SHM_ID, sizeof (map_t), PERMS | IPC_CREAT)) < 0) {
        printf("server: can not create shared memory segment");
        return 1; 
    }
        
    /* подключение сегмента к адресному пространству процесса */
    if ((map_p = (map_t *) shmat (shmid, 0, 0)) == NULL) {
        printf("server: shared memory attach error");
        return 1; 
    }
    return 0;
}

int shared::getShm()
{
    /* получение доступа к массиву семафоров */
    if ((semid = semget (SEM_ID, 1, 0)) < 0) {
        printf("client: can not get semaphore");
        return 1; 
    }

    /* получение доступа к сегменту разделяемой памяти */
    if ((shmid = shmget (SHM_ID, sizeof (map_t), 0)) < 0) {
        printf("client: can not get shared memory segment");
        return 1; 
    }

    /* получение адреса сегмента */
    if ((map_p = (map_t *) shmat (shmid, 0, 0)) == NULL) {
        printf("client: shared memory attach error");
        return 1; 
    }

    return 0;
}
 
int shared::getMemId()
{
    return shmid;
}
 
void* shared::getShmptr(int id)
{
    map_p = (map_t *) shmat (shmid, 0, 0);
    shmid = id;
    return map_p;
}
 
int shared::freeShm()
{
    if (semctl (semid, 0, IPC_RMID, (struct semid_ds *) 0) < 0) {
        printf("server: semaphore remove error");
        return 1; 
    }

    /* удаление сегмента разделяемой памяти */
    shmdt (map_p);
    if (shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0) < 0) {
        printf("server: shared memory remove error");
        return 1; 
    }

    return 0;
}