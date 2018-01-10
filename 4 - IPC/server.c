#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHARE_MEMORY_KEY 4458
#define MESSAGE_QUEUE_KEY 1818
#define CL_SIZE 4096

typedef struct{
    long type;
    char mbuff[4096];
} Msq;

int main()
{
    int SMID, MQID, CLSIZE;
    int count = 0;
    Msq rcvq,sndq;
    int msgSize = sizeof(rcvq) - sizeof(rcvq.type);
    int rcvSize = 0;
    int msqType;
    char check[5];
    char Y[] = "Y";

    SMID = shmget(SHARE_MEMORY_KEY, CL_SIZE, IPC_CREAT|0666);
    MQID = msgget(MESSAGE_QUEUE_KEY, IPC_CREAT|0666);

    char * logmem;
    logmem = (char *)shmat(SMID, NULL, 0);

    while(1)
    {
        rcvSize = msgrcv(MQID, &rcvq, msgSize, msqType, NULL);
        if(rcvSize > 0)
        {
            printf("%s",logmem);
            count++;
        }

        if(count > 100)
        {
            printf("Log Data Fulled ! Please TYPE 'EXIT' At Client!\n");
            shmdt(logmem);
            printf("Have U Done it ??? TYPE : Y/N\n ANS : ");
            gets(check);

            if(!strcmp(check,Y))
            {
                shmctl(SMID,IPC_RMID, NULL);
                printf("Memory Wipe Done !\nPlease Restart Server For New CHAT !\n");
                return 0;
            }
        }
    }
}