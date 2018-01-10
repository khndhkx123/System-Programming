#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

int fileopen(void)
{
	int filedes;
	if((filedes = open("./sample.txt", O_RDWR|O_CREAT|O_TRUNC, 0666)) == -1)
		printf("File Open ERROR!\n");
	
	return filedes;
}

int CnW(int filedes, char *buff, int count)
{
	count++;
	lseek(filedes, 0, SEEK_SET);
	sprintf(buff, "%d", count);
	write(filedes, buff, strlen(buff));
	return count;
}

void myfunc(int signo) {}

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Please Enter : %s <number> <sample.txt>\n",argv[0]);
		return 0;
	}
	struct timeval startT, endT;
	
	struct sigaction mysig;
	mysig.sa_handler = myfunc;
	sigemptyset(&mysig.sa_mask);
	mysig.sa_flags = 0;

	double Timeuse = 0;
	gettimeofday(&startT,NULL);

	int fd;
	int p1,p2,p3,cp;
	char filecount[1000000];
	int count = 0;
	int deadline = atoi(argv[1]);

	if(deadline < 0)
	{
		printf("You have enter the -# of test, Please Restart and Enter over 0 !\n");
		return 0;
	}

	pid_t pid;

	fd = fileopen();
	sprintf(filecount, "%d", count);
	lseek(fd, 0, SEEK_SET);
	write(fd, filecount, strlen(filecount));

	if((pid = fork()) < 0)
		printf("Process Fork 1 ERROR!\n");

	if(pid == 0)
	{
		p1 = getppid();

		if((pid = fork()) < 0)
			printf("Process Fork 2 ERROR!\n");

		if(pid == 0)
		{
			p3 = getpid();
		}
		else
		{
			p2 = getpid();
		}
	}
	else
	{
		p1 = getpid();
	}

	cp = getpid();
	sigaction(SIGUSR1, &mysig, 0);

	if(cp == p2 || cp == p3)
	{
		pause();

		if(cp == p2)
		{
			while(deadline != count)
			{
				count = CnW(fd, filecount, count);
				kill(pid, SIGUSR1);
				pause();
			}
			gettimeofday(&endT,NULL);
			Timeuse = 1000000*(endT.tv_sec - startT.tv_sec) + (endT.tv_usec - startT.tv_usec);
			Timeuse /= 1000000;
			printf("Program Run Time : %f sec\n",Timeuse-1);

			return 0;
		}
		else
		{
			while(deadline != count)
			{
				count = CnW(fd, filecount, count);
				kill(p1, SIGUSR1);
				pause();
			}
			gettimeofday(&endT,NULL);
			Timeuse = 1000000*(endT.tv_sec - startT.tv_sec) + (endT.tv_usec - startT.tv_usec);
			Timeuse /= 1000000;
			printf("Program Run Time : %f sec\n",Timeuse-1);

			return 0;
		}

	}
	else
	{
		printf("Test over 100000 will take long time. Please take a break !\n");
		sleep(1);

		while(deadline != count)
		{
			count = CnW(fd, filecount, count);
			kill(pid, SIGUSR1);
			pause();
		}
		gettimeofday(&endT,NULL);
		Timeuse = 1000000*(endT.tv_sec - startT.tv_sec) + (endT.tv_usec - startT.tv_usec);
		Timeuse /= 1000000;
		printf("Program Run Time : %f sec\n",Timeuse-1);

		return 0;
	}

	return 0;
}
