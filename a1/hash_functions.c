#include <stdio.h>

/*
 * Compute an xor-based hash of the data provided on STDIN. The result
 * should be placed in the array of length block_size pointed to by
 * hash_val.
 */
void hash(char *hash_val, long block_size) 
{
	int i = 0;
	char to_be_hashed;
	for (int i = 0; i < block_size; i++)
	{
		hash_val[i] = '\0';
	}
	
	while (scanf("%c",&to_be_hashed) != EOF)
	{
		hash_val[i] = hash_val[i] ^ to_be_hashed;
		i++;
		if( i == block_size)
			i = 0;

	}

}


/* 
 * Print the message "Hashes match" if the two hashes of size 
 * block_size contain the same value. Print "Hash mismatch" otherwise.
 */
void check_hash(const char *hash1, const char *hash2, long block_size) 
{
	char* match = "Hash match";
	char* mismatch = "Hash mismatch";
	for(int i = 0; i < block_size; i++)
	{
		if(hash1[i] != hash2[i])
		{
			printf("%s\n", mismatch);
			return;
		}

	}
	printf("%s\n", match);
	return;
}
		

