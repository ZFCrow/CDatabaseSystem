#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <conio.h>

#include "functions.h"

// check for existing modulecode and return the current pointer where it matches
struct node *checkExistingModuleCode(struct node *head, char key[])
{
    struct node *current = head;
    while (current != NULL)
    {
        if (strcasecmp(current->module.key, key) == 0)
        {
            return current;
        }
        current = current->next;
    }

    return current;
}


bool checkCode(char key[])
{
    // use isalpha to check for first 3 characters of module code to see if its alpha
    // 4th character will be checked if it is a alpha or digit
    // the remaining characters should be in numeric
    // if not return false
    printf("key: %s\n", key);
    int numofchar = strlen(key);

    // if more than 8 or empty return false
    if (numofchar > 8 | numofchar == 0)
    {
        printf("DETECTED: more than 8 characters or empty\n");
        return false;
    }
    // if contains space return false
    if (containsSpace(key))
    {
        printf("DETECTED: contains space\n");
        return false;
    }
    printf("numofchar: %d\n", numofchar);
    for (int i = 0; i < numofchar; i++)
    {
        if (i < 3)
        {
            if (!isalpha(key[i])) // check if first 3 char is alpha
            {
                printf("DETECTED: character %c is not alpha\n", key[i]);
                return false;
            }
        }

        else if (i == 3)
        {
            if (!isalnum(key[i])) // check if 4th char is alpha or digit
            {
                printf("DETECTED: character %c is not alpha or digit\n", key[i]);
                return false;
            }
        }
        else if (i >= 4)
        {
            if (!isdigit(key[i])) // check if last 4 char is digit
            {
                printf("DETECTED: character %c is not digit\n", key[i]);
                return false;
            }
        }
    }

    return true;
}



// validation functions
int containsSpace(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isspace(str[i]))
        {
            return 1; // String contains a space
        }
    }
    return 0; // String does not contain a space
}



char *filenamevalidations(char *filename, int numoffiles, char *filelist[])
{
    bool flag = true;
    while (flag)
    {
        flag = false;

        int isnum = 1;
        char check[5]; // store first 4 chars in check

        if (strlen(filename) > 4) // use check to check if first 4 chars are 'open' if filename is longer than 4 characters
        {

            for (int i = 0; i < 4; i++)
            {
                // printf("filename[%d]: %c\n", i, filename[i]);
                check[i] = filename[i];
            }

            check[4] = '\0'; // Null-terminate the result string
            // check the first 4 letters of the filename to see if it is 'OPEN'
            if (strcasecmp(check, "open") == 0)
            {
                // if it is, then we need to get rid of the first 5 letters of the filename
                printf("removing open\n");

                for (size_t i = 0; i < strlen(filename); i++)
                {
                    filename[i] = filename[i + 5];
                }
                printf("filename after removing open: %s\n", filename);
            }
        }

        // after removing open, check if filename is a number
        for (size_t i = 0; i < strlen(filename); i++)
        {
            if (!isdigit(filename[i]))
            {
                isnum = 0;
                break;
            }
        }

        if (isnum) // change filename to the actual filename if it is a number
        {
            int fileNumber;
            printf("filename when checking for int: %s\n", filename);
            sscanf(filename, "%d", &fileNumber); // convert string to int and store in fileNumber
            if (fileNumber <= numoffiles && fileNumber != 0)
            {
                strcpy(filename, filelist[fileNumber - 1]);
                printf("%s\n", filename);
            }

            else
            {
                printf("Invalid file mentioned\n");
                // return 1;
            }
        }

        //! =======================================================
        //! this part is to check if the validated filename is in the list of files
        //! =======================================================
        int isnotinlist = 1;

        for (int i = 0; i < numoffiles; i++)
        {
            if (strcmp(filename, filelist[i]) == 0) // check if filename to open exists
            {
                isnotinlist = 0;
                break;
            }
        }

        if (isnotinlist) // break if not in list
        {
            printf("Invalid File name\n");
            // return 1;
            printf("Please enter the File name again: ");
            fgets(filename, 15, stdin);
            filename[strlen(filename) - 1] = '\0'; // get rid of the \n character at the end of the string
            flag = true;
        }
    }

    //! =======================================================
    //! =======================================================

    return filename;
}


