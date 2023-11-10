#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>

//! testing compiling multiple files
// #include "testing.h"

// #define MKEY "ModuleCode"
// #define MNAME "ModuleName"
// #define MCREDIT "Credit"

#define PRINTKEY "Module Code"
#define PRINTNAME "Module Name"
#define PRINTCREDIT "Module Credit"

struct Module
{
    char key[20];  // module code
    char name[55]; // module name
    // char credit[40]; // module credit
    int credit; // module credit
};

struct node
{
    struct Module module;
    struct node *next;
};

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

void printall(struct node *head)
{
    struct node *current = head; // Initialize current
    // print name of columns
    printf("%-15s\t%-40s\t%-3s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
    while (current != NULL)
    {
        printf("%-15s\t%-40s\t%-3d\n", current->module.key, current->module.name, current->module.credit);
        current = current->next;
    }
}

void PrintReverse(struct node *head)
{

    if (head == NULL)
    {
        return;
    }
    // print the header once only
    if (head->next == NULL)
    {
        printf("%-15s\t%-40s\t%-3s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
    }
    PrintReverse(head->next); // Recursively print the rest of the list
    printf("%-15s\t%-40s\t%-3d\n", head->module.key, head->module.name, head->module.credit);
}

void PrintReverse_save(struct node *head, FILE *file)
{

    if (head == NULL)
    {
        return;
    }
    // print the header once only
    if (head->next == NULL)
    {
        fprintf(file, "%s//%s//%s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
    }
    PrintReverse_save(head->next, file);
    fprintf(file, "%s//%s//%d\n", head->module.key, head->module.name, head->module.credit); // Print the current node, the first one will run last!
}

void Print_save(struct node *head, FILE *file)
{
    fprintf(file, "%s//%s//%s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);

    while (head != NULL)
    {
        fprintf(file, "%s//%s//%d\n", head->module.key, head->module.name, head->module.credit); // Print the current node, the first one will run last!
        head = head->next;
    }
}

struct node *addModule(struct node *head, char *data)
{
    struct Module newModule;

    // only prompt the next 2 if the data is not empty, if the last data is not a int, we remove the data part
    // and prompt the user to enter again
    printf("Data: %s\n", data); // print the pointer
    // printf("Data deref: %s\n", *data); // wrong way to deref, it will print the first char of the string

    // see if the data has 3 parts, if it does, then we can add the module

    int result = sscanf(data, "%8[^,],%54[^,],%d", newModule.key, newModule.name, &newModule.credit);
    // theres a chacne that modulename contains a number, so we need to check if the last part is a number
    printf("result: %d\n", result);

    if (result != 3 || containsSpace(newModule.key)) // if the result is not 3 or modulecode contain spaces, then we need to prompt the user to enter again
    {

        printf("Invalid input. will be prompting you to add values manually now.\n");
        char buffer[100]; // this is to check for extra input key in by user

        do
        {
            printf("Enter the %s: ", PRINTKEY);

            fgets(newModule.key, sizeof(newModule.key), stdin);
            newModule.key[strlen(newModule.key) - 1] = '\0';                      // get rid of the \n character at the end of the string
        } while (strlen(newModule.key) > 8 || containsSpace(newModule.key) == 1); // if the length of the module code is more than 8 or contains spacing, then we need to prompt the user again

        //* check if the module code already exists
        struct node *current = head; // Initialize current
        while (current != NULL)
        {
            if (strcasecmp(current->module.key, newModule.key) == 0)
            {
                printf("\n%s \"%s\" already exists in database. Please try again.\n", PRINTKEY, newModule.key);
                return head;
            }
            current = current->next;
        }

        printf("Enter the %s: ", PRINTNAME);
        fgets(newModule.name, sizeof(newModule.name), stdin);
        newModule.name[strlen(newModule.name) - 1] = '\0'; // get rid of the \n character at the end of the string

        while (1)
        {
            printf("Enter the %s: ", PRINTCREDIT);
            if (scanf("%d", &newModule.credit) == 1)
            {
                // check if user enter extra input
                fgets(buffer, sizeof(buffer), stdin);
                if (buffer[0] != '\n')
                {
                    printf("Invalid input. Please try again.\n");
                    continue;
                }
                else
                {
                    break;
                }
            }
            else // if user enter a non-integer
            {

                printf("Invalid input. Please try again.\n");
                scanf("%*[^\n]"); // clear input buffer
                continue;
            }
        }

        struct node *newNode = (struct node *)malloc(sizeof(struct node));
        if (newNode == NULL)
        {
            perror("Memory allocation failed");
            exit(1);
        }

        newNode->module = newModule;
        newNode->next = head; // connect the new node to the current head of the list
        head = newNode;

        printf("\nData is inserted into the database.\n");
        printf("%s: %s\n", PRINTKEY, head->module.key);
        printf("%s: %s\n", PRINTNAME, head->module.name);
        printf("%s: %d\n", PRINTCREDIT, head->module.credit);

        return head;
    }

    else // if the result is 3, means my module has been created successfully and we can create the node to store the module and link it to the list
    {

        // check if the module code already exists
        struct node *current = head; // Initialize current
        while (current != NULL)
        {
            if (strcasecmp(current->module.key, newModule.key) == 0)
            {
                printf("\n%s \"%s\" already exists in database. Please try again.\n", PRINTKEY, newModule.key);
                return head;
            }
            current = current->next;
        }

        struct node *newNode = (struct node *)malloc(sizeof(struct node));
        if (newNode == NULL)
        {
            perror("Memory allocation failed");
            exit(1);
        }

        newNode->module = newModule;
        newNode->next = head; // connect the new node to the current head of the list
        head = newNode;

        printf("\nData is inserted into the database.\n");
        printf("%s: %s\n", PRINTKEY, head->module.key);
        printf("%s: %s\n", PRINTNAME, head->module.name);
        printf("%s: %d\n", PRINTCREDIT, head->module.credit);

        return head;
    }
}

void print_query(struct node *current, char *value, char *attribute)
{
    printf("\nA record for %s(key) = %s is found in the database. Below are the details:\n", attribute, value);
    printf("%s: %s\n", PRINTKEY, current->module.key);
    printf("%s: %s\n", PRINTNAME, current->module.name);
    printf("%s: %d\n", PRINTCREDIT, current->module.credit);
}

void print_query_error()
{
    printf("Available attributes: %s , %s , %s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
    printf("Example of a query: query %s=ict1101\n", PRINTKEY);
    printf("Please try again.\n");
}

/* Checks whether the value x is present in linked list */
bool query(struct node *head, char *data)
{
    // printf("What's head: %s\t%s\t%d\n", head->module.key, head->module.name, head->module.credit);
    // printf("Data: %s\n", data);

    int count = 0;
    char *attribute;
    char *value;

    // check if data is empty
    if (strcmp(data, "") == 0)
    {
        printf("\nQuery data was not found.\n");
        print_query_error();
        return true;
    }
    else if (strchr(data, '=') != NULL)
    {
        attribute = strtok(data, "=");
        // printf("Attribute: %s\n", attribute);
        value = strtok(NULL, "=");
        // printf("Value: %s\n", value);

        // check if got extra space: "module code "
        if (isspace(attribute[strlen(attribute) - 1]))
            attribute[strlen(attribute) - 1] = '\0';

        // check if value null
        if (value == NULL)
        {
            printf("\nQuery data for \"%s\" attribute was not found.\n", attribute);
            print_query_error();
            return true;
        }
        // check if got extra space: " inf1001"
        else if (isspace(value[0]))
        {
            for (int i = 0; value[i] != '\0'; i++)
                value[i] = value[i + 1];
        }
    }
    else
    {
        // if no attribute, set as default module code
        attribute = PRINTKEY;
        value = data;
    }

    struct node *current = head; // Initialize current

    // check if attribute == module code
    if (strcasecmp(attribute, PRINTKEY) == 0)
    {
        while (current != NULL)
        {
            if (strcasecmp(current->module.key, value) == 0)
            {
                print_query(current, value, PRINTKEY);
                count++;
            }
            current = current->next;
        }
    }
    // check if attribute == module name
    else if (strcasecmp(attribute, PRINTNAME) == 0)
    {
        while (current != NULL)
        {
            if (strcasecmp(current->module.name, value) == 0)
            {
                print_query(current, value, PRINTNAME);
                count++;
            }
            current = current->next;
        }
    }
    // check if attribute == module credit
    else if (strcasecmp(attribute, PRINTCREDIT) == 0)
    {
        while (current != NULL)
        {
            if (current->module.credit == atoi(value))
            {
                print_query(current, value, PRINTCREDIT);
                count++;
            }
            current = current->next;
        }
    }
    else
    {
        printf("\nAttribute name \"%s\" not found.\n", attribute);
        print_query_error();
        return true;
    }

    if (count == 0)
        printf("\nThere is no record with %s = %s found in the database.\n", attribute, value);
    return true;
}

void save(struct node *head, char *filename)
{
    int check = 1;
    while (check)
    {
        if (strlen(filename) >= 5) // check if filename has at least 5chars
        {
            char filetype[4]; // Include space for the null
            strcpy(filetype, filename + strlen(filename) - 3);

            if (strncmp(filetype, "txt", 3) == 0) // check if last 3 chars matches txt
            {
                check = 0; // break out of loop and continue to save into file
            }
        }

        if (check)
        {
            printf("Invalid File. Please save to a .txt file!\n");
            printf("Please enter filename again: ");
            fgets(filename, 25, stdin);
            filename[strcspn(filename, "\n")] = '\0'; // Remove the newline character from fgets
        }
    }

    FILE *file = fopen(filename, "w"); // Open the file for writing

    printf("Saving File...\n");
    // PrintReverse_save(head, file);
    Print_save(head, file);

    printf("Closing File...\n");
    fclose(file);
    printf("File Saved!\n");

    return;
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

char *filemenu(char *filelist[], int *numoffiles)
{
    char *currentdir = ".";
    // get the filename from user
    printf("Opening Directory...\n");
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
        char *txtfilename = file->d_name;                // get name of file
        char *file_extension = strchr(txtfilename, '.'); // get file extension

        if (file_extension != NULL && strcmp(file_extension, ".txt") == 0) // check if file is text file
        {
            addfile(filelist, numoffiles, txtfilename);
        }
    }

    for (int i = 0; i < *numoffiles; i++)
    {
        printf("%d. %s\n", i + 1, filelist[i]);
    }

    closedir(directory); // closed directory

    printf("\nEnter here: ");

    // scan the choice, user can enter either 'OPEN #N' or 'open <filename>' or just '#N'
    char filename[25];
    fgets(filename, sizeof(filename), stdin);
    filename[strlen(filename) - 1] = '\0'; // get rid of the \n character at the end of the string

    // printf("filename: %s\n", filename);
    return strdup(filename);
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
struct node *sort(struct node *head)
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
                if (strcasecmp(current->module.key, index->module.key) > 0)
                {
                    // if the current module code is greater than the index module code, then we need to swap the modules
                    temp = current->module;
                    current->module = index->module;
                    index->module = temp;
                }
                index = index->next;
            }
            current = current->next;
        }
    }
    return head;
}

int menu2(struct node **head, struct node **current)
{
    char *input;
    printf("1. SHOW_ALL - display all the modules\n\tCommand: SHOW_ALL or\n\t\t 1\n\n");
    printf("2. INSERT - add a new module\n\tCommand: INSERT <key>,<value 1>,<value 2>,...,<value n> or\n\t\t 2 <key>,<value 1>,<value 2>,...,<value n>\n\n");
    printf("3. QUERY - display a module\n\tCommand: QUERY <key> or\n\t\t QUERY <key/value name>=<key/value>\n\t\t 3 <key>\n\t\t 3 <key/value name>=<key/value>\n\n");
    printf("4. UPDATE - change a specific module\n\tCommand: UPDATE <key> <values...> or\n\t\t 4 <key> <values...>\n\n");
    printf("5. DELETE - delete a module\n\tCommand: DELETE <key> or\n\t\t 5 <key>\n\n");
    printf("6. SAVE - save all the latest records in memory into the database file\n\tCommand: SAVE <filename> or\n\t\t 6 <filename>\n\n");
    printf("7. OPEN - open another file\n\tCommand: OPEN or\n\t\t 7\n\n");
    printf("8. EXIT - close the application\n\tCommand: EXIT or\n\t\t 8\n\n");
    printf("Enter here: ");
    // scanf("%d", &choice);
    // getchar(); // to get rid of the \n character

    input = inputString(stdin, 10);
    // printf("Input: %s\n", input);
    // printf("Length: %d\n", strlen(input));

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
    // printf("i = %d\n", i);

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
        // printf("j = %d\n", j);
        data[j] = '\0';
    }

    int choice = 1;
    // printf("Command: %s\n", command);
    // printf("Data: %s\n", data);

    if (strcasecmp(command, "show_all") == 0 || strcasecmp(command, "1") == 0)
    {
        // SHOW_ALL: display all the modules
        printf("\n");
        //! sort the linked list first
        *head = sort(*head);

        // //! print in reverse, so the header will be printed out first!
        // PrintReverse(*head);

        // print normally
        printall(*head);
        printf("\n");
    }
    else if (strcasecmp(command, "insert") == 0 || strcasecmp(command, "2") == 0)
    {
        // todo, pass in the data as a string, then split it into tokens, if the data contains anything
        //  INSERT: add a new module
        // printf("Data: %s\n", data);
        *head = addModule(*head, data);
    }
    else if (strcasecmp(command, "query") == 0 || strcasecmp(command, "3") == 0)
    {
        // QUERY: display a module
        // printf("data: %s", data);
        query(*head, data);
    }
    else if (strcasecmp(command, "update") == 0 || strcasecmp(command, "4") == 0)
    {
        // UPDATE: change a specific module
    }
    else if (strcasecmp(command, "delete") == 0 || strcasecmp(command, "5") == 0)
    {
        // DELETE: delete a module
    }
    else if (strcasecmp(command, "save") == 0 || strcasecmp(command, "6") == 0)
    {
        // SAVE: SAVE into File
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
            printf("Exiting the application\n");

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

    // sleep(2);
    // wait for user to press enter to continue
    printf("Press Enter to continue...");
    getchar();

    return choice;
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