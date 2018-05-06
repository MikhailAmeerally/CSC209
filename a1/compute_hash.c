#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Hash manipulation functions in hash_functions.c
void hash(char *hash_val, long block_size);
void check_hash(const char *hash1, const char *hash2, long block_size);


#ifndef MAX_BLOCK_SIZE
    #define MAX_BLOCK_SIZE 1024
#endif


int main(int argc, char **argv) {
    //char hash_val[MAX_BLOCK_SIZE];
    char *error_message_1 = "Usage:\n\t compute_hash BLOCK_SIZE [ COMPARISON_HASH ]";
    char *error_message_2 = "Error: The block size should be a positive integer less than ";


    if (argc < 2)
    {
        printf("%s\n", error_message_1);
        return 1;
    }

    else
    {
        long given_size = strtol(argv[1],NULL,10);
        if (given_size > MAX_BLOCK_SIZE || given_size <= 0)
        {
            printf("%s %d\n", error_message_2,MAX_BLOCK_SIZE);
            return 1;
        }

        char hashing[given_size];
        hash(hashing, given_size);

        if (argc ==3)
        {
            char *check = argv[2];
            check_hash(hashing,check,given_size);

        }

    }


    return 0;
}

