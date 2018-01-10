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

#define MESSAGE_QUEUE_CLIENT_ONE 1818
#define MESSAGE_QUEUE_CLIENT_TWO 1819
#define MESSAGE_QUEUE_CLIENT_THREE 1820
#define CL_SIZE 105004
#define SHARE_TEMP_KEY 4500
#define TEMP_SIZE 1050
#define GLOBAL_TYPE 100000

char * logmem;
char * tmpmem;
int SMID, SMID_C;
int count = 0;
typedef struct {
   long type;
   long pid;
   char mbuff[1024];
} Msq;

typedef struct {
   int MQID;
   long pid;
} Client;

Client client[4];

void *receiver1() {

   Msq rcvq;
   int msgSize = sizeof(rcvq) - sizeof(rcvq.type);
   
   while (1) {

      //if (msgrcv(client_id.MQID, &rcvq, msgSize, client_id.pid, MSG_EXCEPT) > 0) {
      if (msgrcv(client[1].MQID, &rcvq, msgSize, client[1].pid, MSG_EXCEPT) > 0) {
         
         if (!strcmp(rcvq.mbuff, "EXIT")) {
            pthread_exit(NULL);
         }
         if (rcvq.type >= GLOBAL_TYPE) {
            printf("%s", tmpmem);
            memcpy(logmem + (count % 100) * 1050, tmpmem, TEMP_SIZE);
            count++;
            memcpy(logmem + 105000, &count, 4);
         }
         else {
            if (rcvq.type == client[1].pid) {

               msgsnd(client[1].MQID, &rcvq, msgSize, 0);

            }

            else if (rcvq.type == client[2].pid) {

               msgsnd(client[2].MQID, &rcvq, msgSize, 0);
            }
            else if (rcvq.type == client[3].pid) {

               msgsnd(client[3].MQID, &rcvq, msgSize, 0);
            }
            else
               printf("fatal err\n");

         }
      }
   }


}
void *receiver2() {

   Msq rcvq;
   int msgSize = sizeof(rcvq) - sizeof(rcvq.type);

   while (1) {

      //if (msgrcv(client_id.MQID, &rcvq, msgSize, client_id.pid, MSG_EXCEPT) > 0) {
      if (msgrcv(client[2].MQID, &rcvq, msgSize, client[2].pid, MSG_EXCEPT) > 0) {
         
         if (!strcmp(rcvq.mbuff, "EXIT")) {
            pthread_exit(NULL);
         }
         if (rcvq.type >= GLOBAL_TYPE) {
            printf("%s", tmpmem);
            memcpy(logmem + (count % 100) * 1050, tmpmem, TEMP_SIZE);
            count++;
            memcpy(logmem + 105000, &count, 4);
         }
         else {
            if (rcvq.type == client[1].pid) {

               msgsnd(client[1].MQID, &rcvq, msgSize, 0);

            }

            else if (rcvq.type == client[2].pid) {

               msgsnd(client[2].MQID, &rcvq, msgSize, 0);
            }
            else if (rcvq.type == client[3].pid) {

               msgsnd(client[3].MQID, &rcvq, msgSize, 0);
            }
            else
               printf("fatal err\n");

         }
      }
   }


}
void *receiver3() {

   Msq rcvq;
   int msgSize = sizeof(rcvq) - sizeof(rcvq.type);

   while (1) {

      //if (msgrcv(client_id.MQID, &rcvq, msgSize, client_id.pid, MSG_EXCEPT) > 0) {
      if (msgrcv(client[3].MQID, &rcvq, msgSize, client[3].pid, MSG_EXCEPT) > 0) {
         //printf("rcv success3 its type is %ld\n", rcvq.type);
         if (!strcmp(rcvq.mbuff, "EXIT")) {
            pthread_exit(NULL);
         }
         if (rcvq.type >= GLOBAL_TYPE) {
            
            printf("%s", tmpmem);
            memcpy(logmem + (count % 100) * 1050, tmpmem, TEMP_SIZE);
            count++;
            memcpy(logmem + 105000, &count, 4);
         }
         else {
            if (rcvq.type == client[1].pid) {

               msgsnd(client[1].MQID, &rcvq, msgSize, 0);
                  
            }

            else if (rcvq.type == client[2].pid) {

               msgsnd(client[2].MQID, &rcvq, msgSize, 0);
            }
            else if (rcvq.type == client[3].pid) {

               msgsnd(client[3].MQID, &rcvq, msgSize, 0);
            }
            else
               printf("fatal err\n");
         }
      }
   }


}
int main()
{
    int count = 0;
    
   Msq rcvq;

    int rcvSize = 0;
   int msgSize = sizeof(rcvq) - sizeof(rcvq.type);
    

    SMID = shmget(SHARE_MEMORY_KEY, CL_SIZE, IPC_CREAT|0666);
   SMID_C = shmget(SHARE_TEMP_KEY, TEMP_SIZE, IPC_CREAT | 0666);
    client[1].MQID = msgget(MESSAGE_QUEUE_CLIENT_ONE, IPC_CREAT|0666);
   client[2].MQID = msgget(MESSAGE_QUEUE_CLIENT_TWO, IPC_CREAT | 0666);
   client[3].MQID = msgget(MESSAGE_QUEUE_CLIENT_THREE, IPC_CREAT | 0666);
    
    logmem = (char *)shmat(SMID, NULL, 0);
   tmpmem = (char *)shmat(SMID_C, NULL, 0);
   printf("register client 1\n");
   msgrcv(client[1].MQID, &rcvq, msgSize, 0, 0);
   client[1].pid = rcvq.pid;
   printf("register client 2\n");
   msgrcv(client[2].MQID, &rcvq, msgSize, 0, 0);
   client[2].pid = rcvq.pid;
   printf("register client 3\n");
   msgrcv(client[3].MQID, &rcvq, msgSize, 0, 0);
   client[3].pid = rcvq.pid;
   
   
   pthread_t tid[3];
   
   pthread_create(&tid[0], NULL, receiver1, NULL);
   pthread_create(&tid[1], NULL, receiver2, NULL);
   pthread_create(&tid[2], NULL, receiver3, NULL);
   printf("Now start chat program\n1 : %ld 2 : %ld 3 : %ld\n", client[1].pid, client[2].pid, client[3].pid);
   for (int i = 0; i < 3; i++)
      pthread_join(tid[i], NULL);
   shmdt(logmem);
   shmctl(SMID, IPC_RMID, NULL);
   printf("Now exit chat program\n");
   return 0;

}