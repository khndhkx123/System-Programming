#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define THRNUM 300
#define count 4000/THRNUM

int **M1;
int **M2;
int blk[THRNUM+1];

void *mcc(void *arg)
{
    long long sum = 0;
    long long total = 0;

    int l = (int)arg;
    int i=0,j=0,k=0;
    int start = blk[l];
    int end = blk[l+1];

    for(i=start; i<end; i++)
    {
        for(j=0; j<4000; j++)
        {
            sum = 0;
            for(k=0; k<4000; k++)
            {
                sum += M1[i][j] * M2[j][k];
            }
            total += sum;
        }
    }
    pthread_exit((void*)total);
}

int main()
{
    printf("\n----- Attention !\n\n");
    printf("----- 1. Please Rename Your Samplefiles NAME AS BELOW\n");
    printf("----- m1.txt AND m2.txt\n");
    printf("----- Each Of .txt Should End With <Tab>\n----- Please Add <Tab> To Final Number In .txt\n\n");
    printf("----- 2. The Thread # is define in Code Definition of THRNUM <Line 8>\n");
    printf("----- Normally The Best = # Of Your CPU Cores <NOW : Thread # is %d>\n\n",THRNUM);
    printf("----- It is Done ? [Y/N] Insert Capital : ");

    char set[5];
    char Y[] = "Y";
    char N[] = "N";
    gets(set);

    if(!strcmp(set,Y))
    {
        printf("\n----- Program Start Running !\n\n");
    }
    else
    {
        printf("\n----- Please Rename & Restart !\n\n");
        return 0;
    }

    struct timeval startT, endT;
    double Timeuse = 0;

    FILE *fp1,*fp2;
    int i,j,k;
    long long totalsum = 0;
    long long *total = (long long*)malloc(sizeof(long long)*THRNUM);

    pthread_t tids[THRNUM];

    fp1 = fopen("./m1.txt","r");
    fp2 = fopen("./m2.txt","r");
    
    if(fp1 == NULL)
    {
        printf("File m1.txt Read ERROR ! \n");
    }
    if(fp2 == NULL)
    {
        printf("File m2.txt Read ERROR ! \n");
    }

////////////////////////////// MALLOC //////////////////////////////
    printf("----- Memory Allocation Running ...\n");

    M1 = (int**)malloc(sizeof(int*)*4000);
    for(i=0; i<4000; i++)
    {
        M1[i] = (int*)malloc(sizeof(int)*4000);
    }

    M2 = (int**)malloc(sizeof(int*)*4000);
    for(i=0; i<4000; i++)
    {
        M2[i] = (int*)malloc(sizeof(int)*4000);
    }

    for(i=0; i<THRNUM+1; i++)
    {
        blk[i] = i*count;
    }

    printf("----- Memory Allocation Done !\n");
////////////////////////////// MALLOC //////////////////////////////

////////////////////////////// READ TO ARRAY //////////////////////////////
    printf("----- Reading Files To Array ...\n");

    for(i=0; i<4000; i++)
    {
        for(j=0; j<4000; j++)
        {
            fscanf(fp1,"%d  ,",&M1[i][j]);
        }
    }

    for(i=0; i<4000; i++)
    {
        for(j=0; j<4000; j++)
        {
            fscanf(fp2,"%d  ,",&M2[i][j]);
        }
    }

    printf("----- Reading Files Done !\n");
////////////////////////////// READ TO ARRAY //////////////////////////////
    fclose(fp1);
    fclose(fp2);
////////////////////////////// THREAD CREATION //////////////////////////////
    printf("----- Multi-Thread Matrix Calculation Start !\n");
    gettimeofday(&startT,NULL);

    for(i=0; i<THRNUM; i++)
    {
        if(pthread_create(&tids[i],NULL,mcc,(void*)i) < 0)
        {
            perror("Thread Creation ERROR!\n");
            exit(0);
        }
    }

    for(i=0; i<THRNUM; i++)
    {
        pthread_join(tids[i],(void*)&total[i]);
    }

    gettimeofday(&endT,NULL); //Timer End
    Timeuse = 1000000*(endT.tv_sec - startT.tv_sec) + (endT.tv_usec - startT.tv_usec);
	Timeuse /= 1000000;
    
    printf("----- Matrix Calculation END !\n\n");
////////////////////////////// THREAD CREATION //////////////////////////////   

    for(i=0; i<THRNUM; i++)
    {
        totalsum += total[i];
        printf("----- Thread #%d = %lld\n",i+1,total[i]);
    }
    printf("\n----- Total sum : %lld\n",totalsum);
    printf("\n----- Program Run Time : %f sec\n",Timeuse);

    return 0;
}