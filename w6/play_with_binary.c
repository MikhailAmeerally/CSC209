#include <stdio.h>
#include <string.h>

int main()
{
	int i = 12;
	FILE * nameFile = fopen("nameFile", "w" );
	
	fprintf(nameFile, "Mikhail Ameerally\n");

	fclose(nameFile);

	FILE* opening = fopen("nameFile","rb");
	char buffer[100];
	fread(buffer,4,1,opening);
	fclose(opening);

	printf("%s\n", buffer);

	return 0;

}