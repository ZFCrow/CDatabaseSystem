#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <conio.h>


//! testing compiling multiple files
// #include "testing.h"

//functions prototype that contains functions from all the files 
#include "functions.h" 


bool checkCode(char key[])
{
    // use isalpha to check for first 3 characters of module code to see if its alpha 
    // 4th character will not be checked as it can be a letter or number
    // the remaining characters should be in numeric 
    // if not return false
    printf("key: %s\n", key);
    int numofchar = strlen(key);
    // if more than 8 return false 
    if (numofchar > 8)
    {
        printf("DETECTED: more than 8 characters\n");
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
            if (!isalpha(key[i]))
            {
                printf("DETECTED: character %c is not alpha\n", key[i]);
                return false;
            }
        }
        else if (i >= 4)
        {
            if (!isdigit(key[i]))
            {
                printf("DETECTED: character %c is not digit\n", key[i]);
                return false;
            }
        }
    }

    
    return true;

}
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
        // strcpy(newModule.credit, token);

        // Create a new node
        struct node *newNode = (struct node *)malloc(sizeof(struct node)); // Allocate memory for the new node
        if (newNode == NULL)
        {
            perror("Memory allocation failed"); // If malloc() failed, print an error message and exit
            exit(1);
        }

        newNode->module = newModule; // Copy the data into the new node
        newNode->next = head;        // connect the new node to the current head of the list, first one will be NULL
        head = newNode;              // Set the head to point to the new node
    }

    fclose(file); // Close the file when you're done reading from it
    printf("linked list created!\n");
    return head; // return the head of the linked list
}








void print_query_error()
{
    printf("Available attributes: %s , %s , %s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
    printf("Example of a query: %s=ict1101\n", PRINTKEY);
}

char *ask_query()
{
    char *value;
    printf("\nPlease enter query again: ");
    value = inputString(stdin, 10);
    return value;
}





void addfile(char *filelist[], int *numoffiles, char *filename)
{
    char *newtxtfilename = strdup(filename); //
    if (newtxtfilename != NULL)
    {
        filelist[*numoffiles] = newtxtfilename; //
        *numoffiles += 1;
    }

    else
    {
        perror("Memory allocation error");
        exit(1);
    }
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
            if (fileNumber <= numoffiles)
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
            fgets(filename, sizeof(char[15]), stdin);
            filename[strlen(filename) - 1] = '\0'; // get rid of the \n character at the end of the string
            flag = true;
        }
    }

    //! =======================================================
    //! =======================================================

    return filename;
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
            //printf ("looking at next key %s \n", current->module.key);
            index = current->next;

            while (index != NULL)
            {
                // if 1 then sort by module code
                if (sortchoice == 1)
                {
                    if (strcasecmp(current->module.key, index->module.key) > 0)
                    {
                        //printf("looking at current key %s \n", current->module.key);
                        // if the current module code is greater than the index module code, then we need to swap the modules
                        //once swap, the next iteration will compare the newly swapped module with the next module
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



int main()
{

    //    addingtime(1, 2);

    int choice = 1;

    do
    {
        char *filelist[255];            // array of pointers to store filename
        int numoffiles = 0;             // number of files in filelist
        int *pnumoffiles = &numoffiles; // pointer to numoffiles
        //! print the openfile menu and get the command from user
        char *filename = filemenu(filelist, pnumoffiles);
        printf("filename: %s\n", filename);

        // validate the command/filename entered by user
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
