#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

int time_count;
int execute;

void handler(int handle)
{
	printf("Recieved Sig Handler\n");
	printf("Time: %d\n",time_count);
	printf("Executions: %d\n", execute);
	exit(1);
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Needs 2 args.\n");
		exit(1);
	}

	time_count = atoi(argv[2]);

	char* path = argv[1];

	FILE* f = fopen(path,"rb");
	
	if(f == NULL)
	{
		perror("File");
		exit(1);
	}

	struct itimerval timer;
	struct sigaction listen;
	memset( &listen, 0, sizeof(listen));

	timer.it_value.tv_sec = (atoi(argv[2]));
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
	
	listen.sa_handler = handler;
	listen.sa_flags = 0;
	sigemptyset(&listen.sa_mask);
	
	sigaction(SIGALRM,&listen,NULL);
	setitimer(ITIMER_REAL, &timer, NULL);


	int random, current;
	for(;;)
	{
		random = rand() % 100;
		fseek(f, random, SEEK_SET);
		fread(&current, sizeof(int), 1, f);
		execute ++;
		//printf("%d\n", current);
		//sleep(5);
	}
	fclose(f);
	return 0;


}
