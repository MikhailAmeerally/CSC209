#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Read a user id and password from standard input, 
   - create a new process to run the validate program
   - send 'validate' the user id and password on a pipe, 
   - print a message 
        "Password verified" if the user id and password matched, 
        "Invalid password", or 
        "No such user"
     depending on the return value of 'validate'.
*/


int main(void) {
    /* Read a user id and password from stdin */
    int status;
    char userid[10];
    char password[10];
    printf("User id:\n");
    scanf("%s", userid);
    printf("Password:\n");
    scanf("%s", password);


    /* TODO: Create a pipe. */
    int fd[2];
    
    if (pipe(fd) == -1)
    {
    	perror("pipe");
    	exit(1);
    }

    int ret_val;
    if ((ret_val = fork()) == -1 ) {
        perror("fork");
        exit(1);
    } else if (ret_val == 0) {
        /* TODO: The child. Change stdin so that it is actually the pipe, 
           then exec the validate program. */
        close(STDOUT_FILENO);
        dup2(fd[0],STDIN_FILENO);
        execl("/Users/Mikhail/repo_ameeral2/w8/validate","validate",NULL);


    } else {
        /* TODO: The parent. Write the userid and password to the pipe, 
           then wait for validate to exit. Print an appropriate message
           based on the exit status of the process. */
        close(STDIN_FILENO);
        write(fd[1],userid, 10);
        write(fd[1],password,10);
    	dup2(fd[1],STDOUT_FILENO);
    	wait(&status);
        if(!(WIFEXITED(status)) )
        {
            perror("WIFEXITED");
            exit(1);
        }
        else
        {
            if(WEXITSTATUS(status) == 0)
            {
                printf("Password verified");
            }
            else if(WEXITSTATUS(status) == 1)
            {
                printf("Invalid password");
            }
            else if(WEXITSTATUS(status) == 2)
            {
                printf("No such user");
            }
            else
            {
                printf("Error in exit.\n");
                perror("WEXITSTATUS FAIL");
                exit(1);
            }
        }
	   	

    }
    return 0;
}
