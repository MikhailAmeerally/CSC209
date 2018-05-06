#include <stdio.h>
// Add your system includes here.

#include "ftree.h"
#include "hash.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

void helper(char name[256]){
}
/*
 * Returns the FTree rooted at the path fname.
 */
struct TreeNode *generate_ftree(const char *fname) {

    DIR d* = opendir(fname);
    struct dirent sd;
    struct dirent temp;
    struct dirent info;
    struct stat st;
    struct TreeNode* contents;
    struct TreeNode* next;
    char* name,hash;


    struct TreeNode tn = malloc(sizeof(struct TreeNode*)*2+sizeof(int)+sizeof(char*));
    tn->hash = NULL; tn->contents = NULL; tn->next = NULL;

 
    sd=readdir(d);
    if(sd==NULL)
    {
        perror("");
        exit(1);
    }    
    //Extract directory name;

    tn.name = sd->d_name;


    if (lstat(tn.name,&st) == -1)
    {
        perror("stat");
        exit(1);
    }
    //Generate Tree Node from Struct stat
    if(S_ISDIR(st)!=0){
        tn.hash=NULL;
        
        while(  (info = readdir(d)) != NULL)
        {
            char* new_name = info->d_name;
            if(new_name =="..")
                continue;
            
            struct stat ss;
            if(lstat(new_name,&ss==-1))
            {
                perror("lstat");
                exit(1);
            }

            if (contents == NULL)
            {
                contents = TreeNode(new_name);
            }
            else if (temp != NULL)
            {
                if (temp.next == NULL)
                {
                    temp.next = TreeNode(info);
                }
            }
        temp = info
    }
    
    //contents equal linked list of files in the directory by recurse 
    }
    else if(S_ISREG!=0)
    {
        tn.contents = NULL;
    //hash of files contents
    }
    else if(S_ISLNK!=0){
        tn.contents = NULL;
    //hash of files contents
    }
    tn.permissions = st.st_mode & 0777;

    return NULL;
}



/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */
void print_ftree(struct TreeNode *root) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    printf("%*s", depth * 2, "");

    // Your implementation here.
}
