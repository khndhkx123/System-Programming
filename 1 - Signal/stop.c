#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void run(void *no)
{
	printf("Program Running......\n");
	sleep(1);
}

void sig_fn(int signo)
{
	printf("Ctrl-C is pressed. Try Again\n");
}

int main()
{
	struct sigaction mysig;
	mysig.sa_handler = sig_fn;
	sigemptyset(&mysig.sa_mask);
	mysig.sa_flags = 0;

	while(1)
	{
		sigaction(SIGINT, &mysig, 0);
		run(NULL);
	}
	return 0;
}
