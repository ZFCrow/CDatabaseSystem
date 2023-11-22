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

bool cancel() //allow user to exit the current operation that they are in
{
    printf("\nPress any key to continue or \'Esc\' to exit\n"); 
    char escape = _getch(); //accept 'esc' key to exit the function

    if (escape == 27) //check if 'esc' key is pressed
    {
        printf("Exiting Function... \n");
        return true;
    }

    else
    {
        return false;
    }
}

// instead of reading all these modules from the file in main, create a openfile function and create a linkedlist from there,
// then return the head of the linked list to main
struct node *openFile(char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Error opening the file");
        exit(1);
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

    char *newtxtfilename = strdup(filename); //copy filename to newtextfilename
    if (newtxtfilename != NULL)
    {
        filelist[*numoffiles] = newtxtfilename; //add the index of the filelist add the newtxtfilename
      
        *numoffiles += 1;
    }

    else
    {
        perror("Memory allocation error");
        exit(1);
    }
}

// sorting nodes 
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
            index = current->next;

            while (index != NULL)
            {
                // if 1 then sort by module code
                if (sortchoice == 1)
                {
                    if (strcasecmp(current->module.key, index->module.key) > 0)
                    {
                        //  if the current module code is greater than the index module code, then we need to swap the modules
                        // once swap, the next iteration will compare the newly swapped module with the next module
                        
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

int main()
{
    declaration();
    printf("Welcome to EzDB!\n");
    
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
       
        //! validate the command/filename entered by user
        filename = filenamevalidations(filename, numoffiles, filelist);
    
        // free everything before opening a new file
        for (int i = 0; i < numoffiles; i++)
        {
            free(filelist[i]);
        }

        // open the file
        struct node *head = openFile(filename);

        struct node *current = head;

        while (choice)
        {
            //! ask user what they want to do?
            printf("\nWhat do you want to do with %s?\n", filename);

            //! mainmenu
            choice = mainmenu(&head, &current); // this is pointer to pointer, so we can change the head pointer in the function
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
