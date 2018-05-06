#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	int random_number;
	FILE* f;
	f = fopen("bin_nums.bin", "ab");

	if(f == NULL)
	{
		perror("File");
		exit(1);
	}

	for(int i = 0; i < 100; i++)
	{
		random_number = rand() % 100;
		fwrite(&random_number, sizeof(random_number), 1, f);
		fputc('\n',f);
	}

	fclose(f);

	return 0;
}
