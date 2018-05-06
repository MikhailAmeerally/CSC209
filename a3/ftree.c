#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include <sys/types.h>
#include "hash.h"
#include <stdbool.h>
#include <dirent.h>
#include <sys/wait.h>


//HELPERS
int check_hash(const char *hash1, const char *hash2)
{
        for(int i = 0; i < 8; i++)
        {
                if(hash1[i] != hash2[i])
                {
                        
                        return -1;
                }
        }
        return 1;
}


int get_size(const char* path)
{
	FILE* f = fopen(path,"rb");
	if(f == NULL)
	{
		perror("Could not open file.");
		exit(EXIT_FAILURE);
	}
	fseek(f,0L,SEEK_END);
	int size = ftell(f);
	fclose(f);
	return size;	
}

bool already_exists(const char* src_path, const char* dest_path)
{
	// So we can get the basename.
	char* src_cpy = strdup(src_path);
	

	DIR* dest_directory;
    struct dirent* check_directory;
    dest_directory = opendir(dest_path);
	if(dest_directory == NULL)
	{
		perror("Could not check directory.");
		exit(EXIT_FAILURE);
	}


	char* base_1 = basename(src_cpy);
	while((check_directory = readdir(dest_directory)) != NULL)
	{
		if((check_directory->d_name[0] != '.'))
		{
			if(strcmp(check_directory->d_name, base_1) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

void overwrite_file(FILE* src, const char* new_path)
{
	rewind(src); // Reset pointer to beginning.
	FILE* new_write = fopen(new_path,"wb");

	if(new_write == NULL)
	{
		perror("Could not overwrite file at destination");
		exit(-1);
	}

	char a;
	while(fread(&a,1,1,src) == 1)
	{
		fwrite(&a, 1, 1, new_write);
	}
	fclose(src); fclose(new_write);
}

void create_file(char* to_be_created, char* at_path, FILE* old_file)
{
	rewind(old_file);
	char* new_path = malloc(strlen(to_be_created) + strlen(at_path) + 2);
	sprintf(new_path, "%s/%s", at_path, to_be_created);

	FILE* f = fopen(new_path, "wb");
	if (f == NULL)
	{
		perror("Could not open file, create_file");
		exit(EXIT_FAILURE);
	}

	char a;
	while(fread(&a,1,1,old_file) == 1)
	{
		fwrite(&a, 1, 1, f);
	}
	fclose(old_file); fclose(f);
}
///////////////

//MAIN
int copy_ftree(const char *src, const char *dest)
{
	if(strcmp(src,dest) == 0)
		return 1;
	

	struct stat s, s2;

	if (lstat(src,&s) == -1) // Get lstat info for src
    {
    	printf("Line 110\n");
        perror("lstat");
        exit(-1);
    }

    if (lstat(dest,&s2) == -1) //Get lstat info for dest
    {
    	printf("Line 116\n");
    	perror("lstat");
    	exit(EXIT_FAILURE);
    }


    if(S_ISREG(s.st_mode)) // FILE TO ...
    {
    	if(S_ISREG(s2.st_mode)) //FILE TO FILE. 
    	{
    			printf("File to file.\n");
	    		char *get_source_base, *get_dest_base, *base1, *base2;
	    		get_source_base = strdup(src); get_dest_base = strdup(dest);
	    		base1 = basename(get_source_base); base2 = basename(get_dest_base);

	    		//If they have the same name.
	    		if(strcmp(base1,base2) == 0)
	    		{
		    		//Get size of src file.
		    		
		    		int size = get_size(src);
		    		int size2 = get_size(dest);

		    		//Get size of dest file.
		    		
		    		FILE* f = fopen(src,"rb"); FILE* f2 = fopen(dest,"rb"); 
			    	if(f == NULL || f2 == NULL )
			    	{
			    		perror("File could not be opened.");
			    		return -1;
			    	}
		    		//If sizes are the same:
		    		if(size == size2)
		    		{
		    			printf("Same size.\n");
		    			// If the hashes are different i.e. they are different files.
		    			if(check_hash(hash(f), hash(f2)) == -1)
		    			{
		    				fclose(f2);
		    				overwrite_file(f,dest);

		    			} //Else, technically, file already exists at source.
		    			return 0;

		    		}

		    		else // SIZES ARE DIFFERENT
		    		{
		    			printf("Different size files. %d, %d\n", size,size2);
		    			fclose(f2);
		    			overwrite_file(f,dest);
		    			return 0;
		    		}
	    		}
	    		else // If the basenames are different, then just overwrite dest file with src file.
	    		{
	    			FILE* f = fopen(src,"rb");
		    		overwrite_file(f,dest);
	    			return 0;
	    			
	    		}
	    }
	    else if(S_ISDIR(s2.st_mode)) // FILE TO DIR
	   	{
	    	printf("FILE TO DIR\n");

			DIR* dest_directory;
			struct dirent* check_directory;
			char* temp_name, *src_basename;
			temp_name = strdup(src);
			src_basename = basename(temp_name);

			dest_directory = opendir(dest);
			if(dest_directory == NULL)
			{
				perror("Could not check directory.");
				return -1;
			}
			while((check_directory = readdir(dest_directory)) != NULL)
			{
				if((check_directory->d_name)[0] != '.')
				{
				
					if(strcmp(check_directory->d_name,src_basename) == 0)//Found file in directory. Need to do checks.
					{
						
						char* new_dest = strdup(dest);
						char* add = malloc(strlen(new_dest) + strlen(src_basename) + 2);
						sprintf(add,"%s/%s", new_dest, src_basename);
			    		
			    		int size = get_size(src);
			    		int size2 = get_size(add);
			    		//If sizes are the same:
			    		if(size == size2)
			    		{
			    			
			    			FILE* f = fopen(src,"rb"); FILE* f2 = fopen(add,"rb"); 
				    		if(f == NULL || f2 == NULL )
				    		{
				    			perror("File could not be opened.");
				    			return -1;
				    		}
					    	
			    			// If the hashes are different i.e. they are different files.
			    			if(check_hash(hash(f), hash(f2)) == 0)
			    			{
			    				fclose(f2);
			    				overwrite_file(f,add);
						
							}
							return 0;
						}
						else
						{
							//SIZES ARE NOT THE SAME.
							//OVERWRITE EXISTING FILE.
							FILE* f = fopen(src,"rb");
							overwrite_file(f,add);
							return 0;
						}	
					}
				}
			}
			

			// The file is not in the directory.
			//Set the new destination to create the file in the destination directory.
			
			char* src_base = strdup(src);
			char* dest_new = strdup(dest);
			char*bn = basename(src_base);
			char* new_file = malloc(strlen(dest_new) + strlen(bn) + 2); // FREE LATER.
			sprintf(new_file,"%s/%s", dest_new, bn);
			
			//Add the new file in the directory
			FILE* old_file = fopen(src,"rb");
			if(old_file == NULL)
			{
				perror("Could not add file to directory. ");
				return -1;
			}
			overwrite_file(old_file,new_file);
			
			return 1;
	    }
	    	
	}
    else if(S_ISDIR(s.st_mode)) // DIR TO ...
	{
	
    	if(S_ISREG(s2.st_mode)) //DIR TO FILE
    	{
    		perror("Cannot copy directory to file.");
    		exit(EXIT_FAILURE);
    	}


    	//DIR TO DIR
    	if(S_ISDIR(s2.st_mode))
    	{
    		int forks = 1;
    		if(already_exists(src,dest))// The directory exists in destination
    		{
    			char* s = strdup(src);
    			char* b = basename(s);
    			char* a = malloc(strlen(dest) + strlen(b)  +2);
    			sprintf(a, "%s/%s", dest,b);

    			
    			DIR* dir = opendir(src);
    			if(dir == NULL)
    			{
    				perror("opendir");
    				return -1;
    			}

    			struct dirent *dir_chk;
    			int forks = 1;
    			while((dir_chk = readdir(dir)) != NULL)
    			{
    				if((dir_chk->d_name)[0] != '.')
    				{
    					//Put on stack.
    					char new_path[strlen(src) + strlen(dir_chk->d_name) + 2];
    					sprintf(new_path, "%s/%s", src, dir_chk->d_name);
    					
    					pid_t pid = fork();
    					if(pid < 0)
    					{
    						perror("fork fail");
    						return -1;
    					}
    					if(pid == 0)
    					{
    						//In child.
    						forks = copy_ftree(new_path,a);
    						exit(forks);

    					}
    				}
    			}
    			//Parent process
    			char q;
				int status, q_value;
				while(wait(&status) != -1)
				{
					q = WEXITSTATUS(status);
					q_value = q;


					if(q_value > -1)
					{
						if(forks > -1)
							forks += q_value;
						else
							forks -= q_value;
					}
					else //ON FAIL
					{
						if (forks > -1)
							forks = -forks+q_value;
						else
							forks += q_value;
					}					
				}
    			return forks;
    		}
    		else // The directory doesn't exist in the destination. 
    			 
    		{

	    		DIR* src_directory;
	    		struct dirent* hello;
	    		struct stat get_subs;
    			char* source_cpy = strdup(src);
    			char* bn = basename(source_cpy);
    			char* new_path = malloc(strlen(dest)+strlen(bn)+2);
    			sprintf(new_path,"%s/%s",dest,bn);

    			//new_path is the path to destination.
    			if(mkdir(new_path,s2.st_mode) == -1)
    			{
    				perror("Could not create directory");
    				return -1;
    			}
    			// open the directory at source. 
    			// We already know this is a directory.
    			if((src_directory = opendir(src)) == NULL)
    			{
    				perror("Could not open source directory.");
    				return -1;
    			}

    			//Read from the directory.

    			while((hello = readdir(src_directory)) != NULL)
    			{
    				/*
					 * if we find a file, then add to new_path directory.
					 * if we find a directory, then create the directory in new_path, and recurse down to that subfolder.
    				 */
    				if((hello->d_name)[0] != '.'){
    					printf("%s\n", hello->d_name);
	    				char* to_stat = malloc(strlen(src) + strlen(hello->d_name) + 2);
	    				sprintf(to_stat, "%s/%s", src, hello->d_name);


	    				if(lstat(to_stat, &get_subs) == -1)
	    				{
	    					perror("Could not get stats.");
	    					exit(EXIT_FAILURE);
	    				}


	    				if(S_ISREG(get_subs.st_mode))
	    				{
	    					FILE* temp1 = fopen(to_stat, "rb");
	    					if(temp1 == NULL)
	    					{
	    						perror("Could not open file");
	    						exit(EXIT_FAILURE);
	    					}
	    					create_file(hello->d_name,new_path, temp1);
	    				}
	    				else if(S_ISDIR(get_subs.st_mode))
	    				{
	    					
	    					pid_t pid = fork();
	    					if(pid < 0)
	    					{
	    						perror("fork fail");
	    						return -1;
	    					}
	    					if(pid == 0)
	    					{
	    						//In child.
	    						forks = copy_ftree(to_stat, new_path);
	    						exit(forks);

	    					}
				
	    				}
	    			}
    			}
    			//Parent process
				char q;
				int status, q_value;
				while(wait(&status) != -1)
				{
					q = WEXITSTATUS(status);
					q_value = q;


					if(q_value > -1)
					{
						if(forks > -1)
							forks += q_value;
						else
							forks -= q_value;
					}
					else //ON FAIL
					{
						if (forks > -1)
							forks = -forks+q_value;
						else
							forks += q_value;
					}					
				}

    			return forks;
    			
    		}
    	}
	}  
	return 1;
}



