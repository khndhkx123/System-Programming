#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/msg.h>
#include <stddef.h>

#define SHARE_MEMORY_KEY 4458
#define SHARE_TEMP_KEY 4500
#define TEMP_SIZE 1050
#define MESSAGE_QUEUE_KEY 1818

#define CL_SIZE 105004
#define GLOBAL_TYPE 100000
typedef struct{
    long type;
   long pid;
    char mbuff[1024];
} Msq;

Msq sndq,rcvq;
int SMID_C, SMID, MQID, pid, Dest, msqType;
int msgSize = sizeof(sndq) - sizeof(sndq.type);
int rcvSize = 0;

void *rcvT()
{
    while(1)
    {   
      
        if(msgrcv(MQID, &rcvq, msgSize, pid, IPC_NOWAIT) > 0)
        {
         //printf("receive type is %ld\n", pid);
         printf("\n[%ld] : %s\n", rcvq.pid, rcvq.mbuff);
         printf("----- Enter The User(PID) to send to : ");
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
   printf("----- Send \"EXIT\" when you want exit chat-----\n");
   int * count;
    char message[1024];
    char logbuff[1050];
   SMID_C = shmget(SHARE_TEMP_KEY, TEMP_SIZE, IPC_CREAT | 0666);
   char * tmpmem;
   tmpmem = (char *)shmat(SMID_C, NULL, 0);
    SMID = shmget(SHARE_MEMORY_KEY, CL_SIZE, IPC_CREAT|0666);
    char * logmem;
    logmem = (char *)shmat(SMID, NULL, 0);
   sndq.pid = pid;
   sndq.type = 1;
   strcpy(sndq.mbuff, "regist");
   MQID = msgget(MESSAGE_QUEUE_KEY, IPC_CREAT | 0666);
   msgsnd(MQID, &sndq, msgSize, IPC_NOWAIT);
   pthread_create(&tid, NULL, rcvT, NULL);
   while (1) {
      printf("----- Notice : 0 = BroadCast Chat / PID = Send to User who is PID\n");
      printf("----- Enter The User(PID) to send to : ");
      gets(dpid);
      Dest = atoi(dpid);
      if (Dest == 0)
      {   
         sndq.type = GLOBAL_TYPE;
         printf("[Message] : ");
         gets(message);
         if (!strcmp(message, "EXIT"))
         {
            printf("Exit Chat...\n");
            strcpy(sndq.mbuff, message);
            msgsnd(MQID, &sndq, msgSize, 0);
            shmdt(logmem);
            return 0;
         }
         sprintf(logbuff, "[%ld] : %s\n",pid, message);
         memcpy(tmpmem, logbuff, TEMP_SIZE);
         strcpy(sndq.mbuff, message);
         msgsnd(MQID, &sndq, msgSize, 0);
         
         
      }
      else if (Dest == -1) {
         char log[1050];
         memcpy(count, logmem + 105000, 4);
         int i;
         if (*count <= 100) {
            for (i = 0; i < *count; i++) {
               strcpy(log, logmem + i*(*count));
               printf("%s\n", log);
            }
         }
         else if (*count > 100) {
            for (i = *count % 100; i < 100; i++) {
               strcpy(log, logmem + i*(*count));
               printf("%s\n", log);
            }
            for (i = 0; i < *count % 100; i++) {
               strcpy(log, logmem + i*(*count));
               printf("%s\n", log);
            }
         }
      }
      else
      {
         sndq.type = Dest;
         
         
         printf("[Message] : ");
         gets(message);
         if (!strcmp(message, "EXIT"))
         {
            printf("Exit Chat...\n");
            strcpy(sndq.mbuff, message);
            msgsnd(MQID, &sndq, msgSize, 0);
            shmdt(logmem);
            return 0;
         }
         strcpy(sndq.mbuff, message);
         msgsnd(MQID, &sndq, msgSize, 0);
         
      }
   }
    return 0;

}