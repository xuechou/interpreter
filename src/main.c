#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static void repl(void)
{
	char line[1024];
	for (;;)
	{
		printf(" >");

		if (!fgets(line, sizeof(line), stdin))
		{
			printf("\n");
			break;
		}
		printf("debug:line%s\n",line);
		interprt(line);
	}
}

static char *readFile(const char *path)
{
	FILE *file = fopen(path, "rb"); //??? rb why not r
	if (file == NULL)
	{
		fprintf(stderr, "Could not open \"%s\".\n", path);
		exit(74);
	}
	fseek(file, 0L, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	char *buffer = malloc(fileSize + 1);
	if (buffer == NULL)
	{
		fprintf(stderr, "Not enough memory to read\"%s\".\n", path);
		exit(74);
	}
	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
	if (bytesRead < fileSize)
	{
		fprintf(stderr, "Could not read file\"%s\".\n", path);
		exit(74);
	}
	buffer[fileSize] = '\0';

	fclose(file);
	return buffer;
}

static void runFile(const char *path)
{
	char *source = readFile(path);
	InterpretResult result = interprt(source);
	free(source);

	if (result == INTERPRET_COMPILE_ERROR)
		exit(64);
	if (result == INTERPRET_RUNTIME_ERROR)
		exit(70);
}

int main(int argc, char const *argv[])
{
	initVM();

	if (argc == 1)
	{
		repl();
	}
	else if (argc == 2)
	{
		runFile(argv[1]);
	}
	else
	{
		fprintf(stderr, "Useage: clox [path] \n");
		exit(64);
	}

	freeVM();
	return 0;
}
