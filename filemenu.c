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

char *filemenu(char *filelist[], int *numoffiles, int *max_capacity)
{
    char *currentdir = ".";
    // get the filename from user
    printf("\nOpening Directory...\n");
    DIR *directory = opendir(currentdir);

    if (directory == NULL)
    {
        perror("Error opening Directory"); // if there is nothing in dir
    }

    //! ask user to open a file first to set them to memory with linkedlist
    printf("What file do you want to open?\n\n");
    printf("Available files: \n");

    struct dirent *file;

    while ((file = readdir(directory)) != NULL) // read file in directory
    {
        char *txtfilename = file->d_name; // get name of file

        if (txtfilename != NULL && strcasecmp(txtfilename + strlen(txtfilename) - 4, ".txt") == 0) // check if file is text file
        {
            addfile(filelist, numoffiles, txtfilename, max_capacity); //add filename to filelist if it is a text file
        }
    }

    for (int i = 0; i < *numoffiles; i++)
    {
        printf("%d. %s\n", i + 1, filelist[i]); //print available files
    }

    closedir(directory); // closed directory

    printf("\nEnter here: ");

    // scan the choice, user can enter either 'OPEN #N' or 'open <filename>' or just '#N'
    char filename[25];
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0'; // get rid of the \n character at the end of the string

    return strdup(filename);
}

int mainmenu(struct node **head, struct node **current)
{
    char *input;
    printf("1. SHOW_ALL - display all the modules\n\tCommand: SHOW_ALL or\n\t\t 1\n\n");
    printf("2. INSERT - add a new module\n\tCommand: INSERT <key>,<value 1>,<value 2> or\n\t\t 2 <key>,<value 1>,<value 2>\n\n");
    printf("3. QUERY - display a module\n\tCommand: QUERY <key> or\n\t\t QUERY <key/value name>=<key/value>\n\t\t 3 <key>\n\t\t 3 <key/value name>=<key/value>\n\n");
    printf("4. UPDATE - change a specific module\n\tCommand: UPDATE <key> or\n\t\t 4 <key>\n\n");
    printf("5. DELETE - delete a module\n\tCommand: DELETE <key> or\n\t\t 5 <key>\n\n");
    printf("6. SAVE - save all the latest records in memory into the database file\n\tCommand: SAVE <filename> or\n\t\t 6 <filename>\n\n");
    printf("7. OPEN - open another file\n\tCommand: OPEN or\n\t\t 7\n\n");
    printf("8. EXIT - close the application\n\tCommand: EXIT or\n\t\t 8\n\n");
    printf("Enter here: ");

    input = inputString(stdin, 10);

    // Get command from input
    int i;
    char command[strlen(input)];
    for (i = 0; input[i] != '\0'; i++)
    {
        if (input[i] == ' ')
            break;
        command[i] = input[i];
    }
    command[i] = '\0';

    // Get data from input
    char data[strlen(input)];
    if (i == strlen(input))
    {
        data[0] = '\0';
    }
    else
    {
        int j;
        for (j = 0; input[j + i + 1] != '\0'; j++)
        {
            data[j] = input[i + j + 1];
        }

        data[j] = '\0';
    }

    int choice = 1;

    if (strcasecmp(command, "show_all") == 0 || strcasecmp(command, "1") == 0)
    {
        // SHOW_ALL: display all the modules
        char inputChoice[10];
        int sortchoice;
        do
        {
            // ask user which one they want to sort by
            printf("\nHow do you want to sort the modules?\n");

            printf("1. %s\n", PRINTKEY);
            printf("2. %s\n", PRINTNAME);
            printf("3. %s\n\n", PRINTCREDIT);
            printf("Enter the number here: ");

            // fgets the sortchoice
            fgets(inputChoice, sizeof(inputChoice), stdin);
            inputChoice[strcspn(inputChoice, "\n")] = '\0'; // get rid of the \n character at the end of the string
            sortchoice = atoi(inputChoice);

            if (sortchoice < 1 || sortchoice > 3)
            {
                printf("Invalid choice. Please try again.\n");
            }
        } while (sortchoice < 1 || sortchoice > 3);
        //! sort the linked list first
        *head = sort(*head, sortchoice);

        // print normally
        showAll(*head);
        printf("\n");
    }
    else if (strcasecmp(command, "insert") == 0 || strcasecmp(command, "2") == 0)
    {
        // todo, pass in the data as a string, then split it into tokens, if the data contains anything
        //  INSERT: add a new module
        *head = insert(*head, data);
    }
    else if (strcasecmp(command, "query") == 0 || strcasecmp(command, "3") == 0)
    {
        // QUERY: display a module
        query(*head, data);
    }
    else if (strcasecmp(command, "update") == 0 || strcasecmp(command, "4") == 0)
    {
        // UPDATE: change a specific module
        update(*head, data);
    }
    else if (strcasecmp(command, "delete") == 0 || strcasecmp(command, "5") == 0)
    {
        // DELETE: delete a module
        delete (head, data);
        // check if the the std is empty
    }
    else if (strcasecmp(command, "save") == 0 || strcasecmp(command, "6") == 0)
    {
        // SAVE: SAVE into File
        sort(*head, 1); //sort file to be in order before saving the changes made to the specified file

        save(*head, data);
    }
    else if (strcasecmp(command, "open") == 0 || strcasecmp(command, "7") == 0)
    {
        // OPEN: open another file
        // exit the while loop
        choice = 7;
        return choice;
    }
    else if (strcasecmp(command, "exit") == 0 || strcasecmp(command, "8") == 0)
    {
        // EXIT: close the application
        // exit the while loop
        choice = 0;
        {
            // exit the application
            printf("\nExiting the application\n");

            //* free the memory and exit
            //* go through the linked list and free the memory
            *current = *head;
            while (*current != NULL)
            {
                struct node *next = (*current)->next;
                free(*current);
                *current = next;
            }
        }
        return choice;
    }
    else
    {
        // If command not found
        printf("Command not found. Please try again.\n");
    }

    // wait for user to press enter to continue
    printf("\nPress Enter to continue...");
    getchar();

    return choice;
}