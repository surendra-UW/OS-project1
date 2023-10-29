
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int findFileInDirectory(char *directoryNum, char *command);
int compareFiles(char *sourceFileName, char *destinationFileName);

int main(int argc, char *argv[])
{

	if (argc == 1)
	{
		printf("What manual page do you want?\nFor example, try 'wman wman'\n");
	}
	else if (argc == 2)
	{
		char *command = argv[1];
		char directoryName[100] = "man_pages/man";
		char curreDirectory[100];
		int result =0;
		for(int i=1;i<=9;i++) {
			strcpy(curreDirectory, directoryName);
			curreDirectory[strlen(curreDirectory)] = i + '0';
			result = findFileInDirectory(curreDirectory, command);
			if(result == 2) break;
		}
		if (result == 0)
		{
			printf("No manual entry for %s\n", command);
		}
	}
	else
	{
		char *page = argv[1];
		char *command = argv[2];
		if ( strlen(page) == 1 && (int)page[0] >= 49 && (int)page[0] <= 57)
		{
			char directoryName[100] = "man_pages/man";
			int result = findFileInDirectory(strcat(directoryName, page), command);
			if (result == 0)
			{
				printf("No manual entry for %s in section %s\n", command, page);
			}
		}
		else
		{
			printf("invalid section\n");
			return 1;
		}
	}
	return 0;
}

int findFileInDirectory(char *directoryName, char *command)
{
	DIR *currDirectory;
	if ((currDirectory = opendir(directoryName)) == NULL)
	{
		return 1;
	}
	struct dirent *fd;
	while ((fd = readdir(currDirectory)) != NULL)
	{
		if (strcmp(fd->d_name, ".") == 0 || strcmp(fd->d_name, "..") == 0 || strcmp(fd->d_name, ".gitignore") == 0)
		{
			continue;
		}
		if (fd->d_type == DT_DIR)
		{
			char *currDirectoryName = (char *)malloc(strlen(directoryName));
			strcpy(currDirectoryName, directoryName);
			strcat(strcat(currDirectoryName, "/"), fd->d_name);
			int result = findFileInDirectory(currDirectoryName, command);
			if (result == 2)
				return 2;
		}
		else
		{
			if (compareFiles(fd->d_name, command) == 0)
			{
				strcat(strcat(directoryName, "/"), fd->d_name);
				FILE *file;
				if ((file = fopen(directoryName, "r")) == NULL)
				{
					perror("cannot open file\n");
					return 1;
				}
				else
				{
					char buffer[100];
					while (fgets(buffer, 100, file) != NULL)
					{
						printf("%s", buffer);
					}
					fclose(file);
					return 2;
				}
			}
		}
	}
	closedir(currDirectory);
	return 0;
}

int compareFiles(char *sourceFileName, char *destinationFileName)
{
	int i = 0;
	while (sourceFileName[i] && destinationFileName[i])
	{
		if (sourceFileName[i] == destinationFileName[i])
			i++;
		else
			return 1;
	}

	if (destinationFileName[i] == (char)'\0' && (sourceFileName[i] == (char)'.'))
		return 0;
	return 1;
}