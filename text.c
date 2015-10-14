#include <stdlib.h>
#include <stdio.h>

char* load_file(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if(file == NULL)
	{
		return NULL;
	}

	const int CHUNK = 512;
	char* text = (char*)malloc(CHUNK);
	int length = 0;
	int length_change;
	do	
	{
		text = (char*)realloc(text, length + CHUNK);
		length_change = fread(text, 1, CHUNK, file);
		length += length_change;
	} while(length_change == CHUNK);
	text[length] = '\0';
	length += 1;
	text = (char*)realloc(text, length + CHUNK);
	return text;
}
