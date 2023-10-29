
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define NAME_SECTION "\033[1mNAME\033[0m"
#define DESCRIPTION_SECTION "\033[1mDESCRIPTION\033[0m"

char *extractFileName(char *fileName);

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("wapropos what?\n");
    }
    else
    {
        int anyMatch = 0;
        for (int i = 1; i < 10; i++)
        {
            char baseDirectoryPath[20] = "./man_pages/man";
            char section[15];
            sprintf(section, "%d", i);
            strcat(baseDirectoryPath, section);
            DIR *currDirectory;
            if ((currDirectory = opendir(baseDirectoryPath)) == NULL)
            {
                return 1;
            }
            else
            {
                struct dirent *fd;
                while ((fd = readdir(currDirectory)) != NULL)
                {
                    if (strcmp(fd->d_name, ".") == 0 || strcmp(fd->d_name, "..") == 0 || strcmp(fd->d_name, ".gitignore") == 0)
                    {
                        continue;
                    }
                    else
                    {
                        FILE *file;
                        char currentFilePath[20];
                        strcpy(currentFilePath, baseDirectoryPath);
                        strcat(strcat(currentFilePath, "/"), fd->d_name);
                        if ((file = fopen(currentFilePath, "r")) == NULL)
                        {
                            perror("cannot open the file\n");
                            return 1;
                        }
                        char buffer[100];
                        char nameLine[100];
                        int isLookupSection = 0;
                        int copyNameLine = 0;
                        int matchFound = 0;
                        while (fgets(buffer, 100, file) != NULL && matchFound == 0)
                        {

                            if (copyNameLine == 1)
                            {
                                strcpy(nameLine, buffer);
                                copyNameLine = 0;
                            }
                            if (strstr(buffer, NAME_SECTION) && strstr(buffer, NAME_SECTION) == buffer)
                            {
                                isLookupSection = 1;
                                copyNameLine = 1;
                            }
                            else if (strstr(buffer, DESCRIPTION_SECTION) && strstr(buffer, DESCRIPTION_SECTION) == buffer)
                            {
                                isLookupSection = 1;
                            }
                            else if (buffer[0] == '\033')
                            {
                                isLookupSection = 0;
                            }
                            else
                            {
                                if (isLookupSection == 1 && strstr(buffer, argv[1]) != NULL)
                                {
                                    matchFound = 1;
                                }
                            }
                        }

                        if(matchFound == 1) {
                            anyMatch++;
                            char *command = extractFileName(fd->d_name);
                            char *name_one_liner = strstr(nameLine, command) + strlen(command);
                            printf("%s (%d)%s", command, i, name_one_liner);
                        }
                        nameLine[0] = '\0';
                        fclose(file);
                    }
                }
            }
        }
        if(anyMatch == 0) {
            printf("nothing appropriate\n");
        }
    }
    return 0;
}

char * extractFileName(char *fileName) {
    int i= 0;
    char *command = malloc(20*sizeof(char)); 
    strcpy(command, fileName);
    while(command[i] != '\0' && command[i] != '.') i++;
    command[i] = '\0';
    return command;
}