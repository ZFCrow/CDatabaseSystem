#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <conio.h>

// functions prototype that contains functions from all the files
#include "functions.h"

bool cancel()
{
    printf("\nPress 'ESC' to exit or any key to continue\n");
    char escape = _getch();

    if (escape == 27)
    {
        printf("Exiting Function... \n");
        return true;
    }
    else
    {
        return false;
    }
}

char *inputString(FILE *fp, size_t size)
{
    // The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(*str) * size); // size is start size
    if (!str)
        return str;
    while (EOF != (ch = fgetc(fp)) && ch != '\n')
    {
        str[len++] = ch;
        if (len == size)
        {
            str = realloc(str, sizeof(*str) * (size += 16));
            if (!str)
                return str;
        }
    }
    str[len++] = '\0';
    return realloc(str, sizeof(*str) * len);
}

// instead of reading all these modules from the file in main, create a openfile function and create a linkedlist from there,
// then return the head of the linked list to main
struct node *openFile(char *filename)
{
    printf("Reading from the file...\n");
    printf("Filename: %s\n", filename);
    // replace the filename with moduletime.txt
    // strcpy(filename, "a.txt");
    // check if i got space to open file
    int availableFileHandles = _getmaxstdio();
    if (availableFileHandles == -1)
    {
        perror("_getmaxstdio");
        exit(1);
    }

    printf("Maximum number of open file handles: %d\n", availableFileHandles);
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Error opening the file");
        exit(1);
    }
    else
    {
        printf("%s opened successfully!\n", filename);
    }

    struct node *head = NULL; // Create an empty linked list
    char line[100];           // Adjust the buffer size based on your file's format

    while (fgets(line, sizeof(line), file) != NULL)
    {
        struct Module newModule;
        // Parse the line and populate newModule with data from the file
        // You can use strtok() to split the line into tokens
        // You can use strcpy() to copy the tokens into the newModule structure
        // ignore the first line
        if (strcmp(line, "Module Code//Module Name//Module Credit\n") == 0)
        {
            printf("Ignoring the first line\n");
            continue;
        }
        char *token = strtok(line, "//"); // Split the line into tokens separated by //
        strcpy(newModule.key, token);
        token = strtok(NULL, "//"); // NULL as the first argument tells strtok() to continue splitting the same string
        strcpy(newModule.name, token);
        token = strtok(NULL, "//");
        newModule.credit = atoi(token);

        // Add the newModule to the linked list
        head = addNode(head, newModule);
    }
    fclose(file); // Close the file when you're done reading from it
    printf("linked list created!\n");
    return head; // return the head of the linked list
}

void addfile(char *filelist[], int *numoffiles, char *filename, int *max_capacity)
{
    if (*numoffiles >= *max_capacity)
    {
        printf("Filellist is full, increasing size\n");
        // If the array is full, reallocate with increased capacity
        int new_capacity = *numoffiles * 2; // Double the capacity
        *max_capacity = new_capacity;
        *filelist = realloc(*filelist, new_capacity * sizeof(char *));

        if (filelist == NULL)
        {
            perror("Memory reallocation error");
            exit(1);
        }
    }

    char *newtxtfilename = strdup(filename); //
    if (newtxtfilename != NULL)
    {
        filelist[*numoffiles] = newtxtfilename; //
        printf("filename added: %s\n", filelist[*numoffiles]);
        *numoffiles += 1;
    }

    else
    {
        perror("Memory allocation error");
        exit(1);
    }
}

// sorting nodes base on module code
struct node *sort(struct node *head, int sortchoice)
{
    struct node *current = head;
    struct node *index = NULL;
    struct Module temp; // temporary variable to store the module

    if (head == NULL)
    {
        return head;
    }
    else
    {
        while (current != NULL)
        {
            // printf ("looking at next key %s \n", current->module.key);
            index = current->next;

            while (index != NULL)
            {
                // if 1 then sort by module code
                if (sortchoice == 1)
                {
                    if (strcasecmp(current->module.key, index->module.key) > 0)
                    {
                        // printf("looking at current key %s \n", current->module.key);
                        //  if the current module code is greater than the index module code, then we need to swap the modules
                        // once swap, the next iteration will compare the newly swapped module with the next module
                        // printf("swapping %s and %s\n", current->module.key, index->module.key);
                        temp = current->module;
                        current->module = index->module;
                        index->module = temp;
                    }
                }
                // if 2 then sort by module name
                else if (sortchoice == 2)
                {
                    if (strcasecmp(current->module.name, index->module.name) > 0)
                    {
                        // if the current module name is greater than the index module name, then we need to swap the modules
                        temp = current->module;
                        current->module = index->module;
                        index->module = temp;
                    }
                }
                // if 3 then sort by module credit
                else if (sortchoice == 3)
                {
                    if (current->module.credit > index->module.credit)
                    {
                        // if the current module credit is greater than the index module credit, then we need to swap the modules
                        temp = current->module;
                        current->module = index->module;
                        index->module = temp;
                    }
                }
                index = index->next;
            }
            current = current->next;
        }
    }
    return head;
}

void declaration()
{
    printf("\"\nDeclaration\n");
    printf("SIT\'s policy on copying does not allow the students to copy source code as well as assessment solutions\nfrom another person or other places. It is the students\' responsibility to guarantee that their assessment\nsolutions are their own work. Meanwhile, the students must also ensure that their work is not accessible\nby others. Where such plagiarism is detected, both of the assessments involved will receive ZERO mark.\n");
    printf("\nWe hereby declare that:\n");
    printf("- We fully understand and agree to the abovementioned plagiarism policy.\n");
    printf("- We did not copy any code from others or from other places.\n");
    printf("- We did not share our codes with others or upload to any other places for public access.\n");
    printf("- We agree that our project will receive Zero mark if there is any plagiarism detected.\n");
    printf("\nDeclared by: P5_4\n");
    printf("Team members:\n");
    printf("1. Chua Shing Ying\n");
    printf("2. Lim Mei Tian\n");
    printf("3. Lee Zhiqi, Charlene\n");
    printf("4. Cheah Zheng Feng\n");
    printf("5. Phang Zi Heng\n");
    printf("Date: 23 November 2023\n\"\n\n");
}

int main()
{
    declaration();
    //    addingtime(1, 2);

    int choice = 1;
    int max_capacity = INITIAL_CAPACITY;

    do
    {
        char *filelist[max_capacity];       // array of pointers to store filename
        int numoffiles = 0;                 // number of files in filelist
        int *pnumoffiles = &numoffiles;     // pointer to numoffiles
        int *pmax_capacity = &max_capacity; // pointer to max capacity

        //! print the openfile menu and get the command from user
        char *filename = filemenu(filelist, pnumoffiles, pmax_capacity);
        printf("filename: %s\n", filename);

        //! validate the command/filename entered by user
        filename = filenamevalidations(filename, numoffiles, filelist);
        printf("filename after validations: %s\n", filename);

        // free everything before opening a new file
        for (int i = 0; i < numoffiles; i++)
        {
            free(filelist[i]);
        }

        // free(pnumoffiles);

        // open the file
        struct node *head = openFile(filename);

        struct node *current = head;

        while (choice)
        {

            //! ask user what they want to do?
            printf("\nWhat do you want to do with %s?\n", filename);

            //! menu2
            choice = menu2(&head, &current); // this is pointer to pointer, so we can change the head pointer in the function
            // UPDATE MY HEAD
            if (choice == 7)
            {
                // free the memory
                current = head;
                while (current != NULL)
                {
                    struct node *next = current->next;
                    free(current);
                    current = next;
                }
                break;
            }
        }

        // the loop for do while
    } while (choice == 7); // means user wants to open another file
    // sleep for 5 seconds
    sleep(5);

    return 0;
}
