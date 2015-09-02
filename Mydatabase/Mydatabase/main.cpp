#include <stdio.h>

void main(void)
{
	FILE *stream;
	char list[30];
	int  i, numread, numwritten;

	/* Open file in text mode: */
	if ((stream = fopen("fread.out", "w+t")) != NULL)
	{
		for (i = 0; i < 25; i++)
			list[i] = (char)('z' - i);
		/* Write 25 characters to stream */
		numwritten = fwrite(list, sizeof(char), 25, stream);
		printf("Wrote %d items\n", numwritten);
		fclose(stream);

	}
	else
		printf("Problem opening the file\n");

	if ((stream = fopen("fread.out", "r+t")) != NULL)
	{
		/* Attempt to read in 25 characters */
		numread = fread(list, sizeof(char), 25, stream);
		printf("Number of items read = %d\n", numread);
		printf("Contents of buffer = %.25s\n", list);
		fclose(stream);
	}
	else
		printf("File could not be opened\n");
}
