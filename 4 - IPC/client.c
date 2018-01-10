#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <pthread.h>

#define SHARE_MEMORY_KEY 4458

#define MESSAGE_QUEUE_KEY_ALL 1818
#define MESSAGE_QUEUE_KEY_NOTALL 2424

#define CL_SIZE 4096

typedef struct{
    long type;
    char mbuff[1024];
} Msq;

Msq sndq,rcvq;
int SMID, MQID, pid, Dest, msqType;
int msgSize = sizeof(sndq) - sizeof(sndq.type);
int rcvSize = 0;

void *rcvT()
{
    while(1)
    {
        rcvSize = msgrcv(MQID, &rcvq, msgSize, pid, NULL);
        if(rcvSize > 0)
        {
            printf("[Received] : ");
            puts(rcvq.mbuff);
        }
    }
}

int main()
{
    pthread_t tid;
    pid = getpid();
    char dpid[10];
    printf("----- Welcome ! Your UserID(PID) is : %d\n\n",pid);
    printf("----- Chatting Program Start -----\n");
    printf("----- Notice : 0 = BroadCast Chat / PID = Send to User who is PID\n");
    printf("----- Enter The User(PID) to send to : ");
    gets(dpid);
    Dest = atoi(dpid);

    char message[1024];
    char logbuff[4096];

    SMID = shmget(SHARE_MEMORY_KEY, CL_SIZE, IPC_CREAT|0666);
    char * logmem;
    logmem = (char *)shmat(SMID, NULL, 0);

    if(Dest == 0)
    {
        sndq.type = 1;
        MQID = msgget(MESSAGE_QUEUE_KEY_ALL, IPC_CREAT|0666);
        while(1)
        {
            printf("[Message] : ");
            gets(message);
            if(!strcmp(message,"EXIT"))
            {
                printf("Exit Chat...\n");
                shmdt(logmem);
                return 0;
            }
            sprintf(logbuff,"[%d] : %s\n", pid,message);
            strcat(logmem,logbuff);
            strcpy(sndq.mbuff, message);
            msgsnd(MQID, &sndq, msgSize, NULL);
        }
    }
    else
    {
        sndq.type = Dest;
        MQID = msgget(MESSAGE_QUEUE_KEY_NOTALL, IPC_CREAT|0666);
        pthread_create(&tid,NULL,rcvT,NULL);
        while(1)
        {
            printf("[Message] : ");
            gets(message);
            if(!strcmp(message,"EXIT"))
            {
                printf("Exit Chat...\n");
                shmdt(logmem);
                return 0;
            }
            strcpy(sndq.mbuff, message);
            msgsnd(MQID, &sndq, msgSize, NULL);
        }
    }

    return 0;

}