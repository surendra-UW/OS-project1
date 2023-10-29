#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define DELIMITER " "
#define TEXT_BOLD "\033[1m"
#define TEXT_ITALIC "\033[3m"
#define TEXT_UNDERLINE "\033[4m"
#define TEXT_RESET "\033[0m"

int validateDate(char *date);
char **validateThHeader(char *line);
char * strUpper(char *str);
int replaceSubstring(char *line, char *search_text, char *replace_text);
char * replaceAllStrings(char *line);

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        printf("Improper number of arguments\nUsage: ./wgroff <file>\n");
    }
    else
    {
        char *input_file = argv[1]; 
        char directoryName[100] = "";
        strcat(directoryName, input_file);
        FILE *file;
        if ((file = fopen(directoryName, "r")) == NULL)
        {
            printf("File doesn't exist\n");
        }
        else
        {
            int line_number = 1;
            char buffer[100];
            fgets(buffer, 100, file);
            char temp[100];
            strcpy(temp, buffer);
            char **arguments = validateThHeader(temp);
            if (arguments == NULL)
                return 0;
            char outputFileName[20] = "";
            sprintf(outputFileName, "%s.%s", arguments[0], arguments[1]);
            FILE *outputFile;
            if ((outputFile = fopen(outputFileName, "w")) == NULL)
            {
                perror("Cannot open the file to write\n");
                return 1;
            }
            //print first line in output 
            char first_line[20];
            sprintf(first_line, "%s(%s)", arguments[0], arguments[1]);
            fprintf(outputFile, "%s", first_line);
            fprintf(outputFile, "%*s\n", 80-((int)strlen(first_line)), first_line);
            while (fgets(buffer, 100, file) != NULL)
            {
                line_number++;
                if (buffer[0] == '#')
                    continue;
                // parse a section 
                char tempBuf[100];
                strcpy(tempBuf, buffer);
                if(buffer[0] == '.' && buffer[1] == 'S' && buffer[2] == 'H') {
                    fprintf(outputFile, "\n");
                    strtok(tempBuf, DELIMITER);
                    char *section_name = strtok(NULL, DELIMITER);
                    section_name[strlen(section_name)-1] = '\0';
                    if(strchr(section_name, '\n') != NULL) {
                        printf("Improper formatting on line %d\n", line_number);
                        return 0;
                    } 
                    fprintf(outputFile, "%s%s%s\n", TEXT_BOLD, strUpper(section_name), TEXT_RESET);
                } else {
                    fprintf(outputFile, "       ");
                    fprintf(outputFile, "%s", replaceAllStrings(buffer));
                }
            }
            fclose(file);
            fprintf(outputFile, "%*s%s%*s\n", 35, "", arguments[2], 35, "");
            fclose(outputFile);
        }
    }

    return 0;
}

char **validateThHeader(char *line)
{
    char *word = strtok(line, DELIMITER);
    char **arguments = malloc(3*sizeof(char *));
    if (strcmp(word, ".TH") == 0)
    {
        // title header
        int count = 0;
        while (word != NULL && count < 3)
        {
            word = strtok(NULL, DELIMITER);
            arguments[count] = word;
            count++;
        }
        word = strtok(NULL, DELIMITER);
        if (word == NULL && count == 3)
        {
            if (strlen(arguments[1]) == 1 && (int)arguments[1][0] >= 49 && (int)arguments[1][0] <= 57 && validateDate(arguments[2]) == 0)
            {
                return arguments;
            }
        }
    }
    printf("Improper formatting on line 1\n");
    return NULL;
}

int validateDate(char *date)
{
    if (strlen(date) < 10)
        return 1;
    else if(strlen(date) > 10 ) {
        date[10] = '\0';
    }
    int day, month, year;
    sscanf(date, "%4d-%2d-%2d", &year, &month, &day);
    if (month < 1 || month > 12) {
        printf("month %d\n", month);
        return 1;
    }
    if (day < 1 || day > 31){
        printf("day %d\n", day);
        return 1;
    }
    return 0;
}

char * strUpper(char *str) {
    int i =0;
    while(str[i]) {
        str[i] = toupper(str[i]);
        i++;
    }
    return str;
}

int replaceSubstring(char *line, char *search_text, char *replace_text) {
    char buffer[150];
    char *start;
    while((start = strstr(line, search_text)) != NULL) {
        strncpy(buffer, line, start-line);
        buffer[start-line] = '\0';
        strcat(buffer, replace_text);
        strcat(buffer, start + strlen(search_text));
        strcpy(line, buffer);
        buffer[0] = '\0';
    }
    return 0;
}

char *replaceAllStrings(char *line) {
    replaceSubstring(line, "/fB", TEXT_BOLD);
    replaceSubstring(line, "/fI", TEXT_ITALIC);
    replaceSubstring(line, "/fU", TEXT_UNDERLINE);
    replaceSubstring(line, "/fP", TEXT_RESET);
    replaceSubstring(line, "//", "/");
    return line;
}