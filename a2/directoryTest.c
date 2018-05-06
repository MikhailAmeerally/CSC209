#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>

int main(int argc, char* argv[])
{
	if (argc !=2)
	{
		return -1;
	}
	DIR* directory;
	struct stat buf;
	struct dirent* info;


	

	char* path = argv[1]; 

	if(lstat(path,&buf) == -1)
	{
		perror("lstat");
		exit(1);
	}
	if( S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode) )
	{
		printf("Regular or Link file.\n");
	}
	else
	{
		directory = opendir(path);
		if(directory == NULL)
		{
			perror("");
			exit(1);
		}
		while((info = readdir(directory)) != NULL)
		{
			if ((info->d_name)[0] == '.')
			{
				continue;
			}
			printf("%s\n", info->d_name);
		}
	}
	
	

	

	return 0;
}