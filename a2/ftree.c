#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ftree.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

/*
 * Returns the FTree rooted at the path fname.
 */
struct TreeNode *generate_ftree(const char *fname) 
{
    

    //Variables Needed
    DIR* directory;
    struct dirent *reading;
    struct stat current_stats;
    struct TreeNode* tn = malloc(sizeof(struct TreeNode));
    struct TreeNode* temp;
    

    //Set tree to NULL
    tn->hash = NULL; tn->next = NULL; tn->contents = NULL;

    
    tn->fname = malloc(strlen(fname)+1);
    if(tn->fname == NULL)
    {
    	perror("malloc");
    	exit(1);
    }
    strcpy(tn->fname,fname);
    

    if (lstat(tn->fname,&current_stats) == -1)
    {
        perror("lstat");
        exit(1);
    }

    tn->permissions = current_stats.st_mode & 0777;
    
    


    
    
	if (S_ISREG(current_stats.st_mode) || S_ISLNK(current_stats.st_mode))
	{
		tn->contents = NULL;

		FILE* file = fopen(fname,"r");
		if(file == NULL)
		{
			perror("fopen");
			exit(1);
		}

		tn->hash = hash(file); // Add file.
		
		if(fclose(file) == EOF)
		{
			perror("fclose");
			exit(1);
		}

	}
	
	
	else
	{
		directory = opendir(fname);
		if(directory == NULL)
		{
			perror("Error");
			exit(1);
		}


		while(  (reading = readdir(directory)) != NULL) // DIR NOT EMPTY
        {
        	
        	if((reading->d_name)[0] == '.')
                continue;

            // Get the new path name.
            char* new_name = malloc(sizeof(char) * (strlen(fname) + strlen(reading->d_name) + 2));
	        strcpy(new_name,fname);
	        strcat(new_name,"/");
	        strcat(new_name,reading->d_name);
            
        
            if (tn->contents == NULL)
            {
                tn->contents = generate_ftree(new_name);
                temp = tn->contents;
            }
            else if (temp != NULL)
            {
                temp->next = generate_ftree(new_name);
                temp = temp->next;
            }
        }
        closedir(directory);
        
	}

    return tn;

}


/*
 * Returns the basename of the path of a file, so that it can be printed
 * in the print_ftree function.
 */

char* grab_basename(char *current_path)
{
    char *base = malloc(strlen(current_path)+1);
    base = strrchr(current_path, '/');

    if (base == NULL)
        return strdup(current_path); // Duplicate the path.
    else
        return strdup(base + 1); // Add one to index so that we don't get the '/'
}

/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */
void print_ftree(struct TreeNode *root) 
{

    char* basename = malloc(strlen(root->fname)+1);
    basename = grab_basename(root->fname); 
    
    static int depth = 0;
    printf("%*s", depth * 2, "");
    if (root->hash == NULL) //It is a directory.
    {
        printf("===== %s (%o) =====\n", basename,root->permissions);
        if (root->contents != NULL) // Go deeper
        {
            depth += 1;
            print_ftree(root->contents);
            depth -=1;
        }

        if (root->next != NULL)
        {
            print_ftree(root->next);
        }
    } 
    else //It is a file or link (a leaf)
    {
        printf("%s (%o)\n", basename,root->permissions);
        if (root->next != NULL)
        {
            print_ftree(root->next);
        }
    }
    
    
    


}


