#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

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
    FILE *file = fopen(filename, "r"); // Open the file for reading

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

    return head; // return the head of the linked list
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

struct node *addModule(struct node *head, char *data)
{
    struct Module newModule;

    // only prompt the next 2 if the data is not empty, if the last data is not a int, we remove the data part
    // and prompt the user to enter again
    printf("Data: %s\n", data);
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

        printf("Enter the %s: ", PRINTKEY);
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
    else
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

/* Checks whether the value x is present in linked list */
bool query(struct node *head, char *data)
{
    // char *value;
    // printf("Enter the module code: ");
    // scanf("%ms", &value);

    // printf("Hi %s\n", value);
    // printf("What's head: %s\t%s\t%d\n", head->module.key, head->module.name, head->module.credit);
    // printf("Data: %s\n", data);

    int count = 0;
    char *attribute;
    char *value;

    if (strchr(data, '=') != NULL)
    {
        attribute = strtok(data, "=");
        // printf("Attribute: %s\n", attribute);
        value = strtok(NULL, "=");
        // printf("Value: %s\n", value);
    }
    else
    {
        attribute = PRINTKEY;
        value = data;
    }

    struct node *current = head; // Initialize current

    if (strcasecmp(attribute, PRINTKEY) == 0)
    {
        while (current != NULL)
        {
            if (strcasecmp(current->module.key, value) == 0)
            {
                printf("\nA record for %s(key) = %s is found in the database. Below are the details:\n", PRINTKEY, value);
                printf("%s: %s\n", PRINTKEY, current->module.key);
                printf("%s: %s\n", PRINTNAME, current->module.name);
                printf("%s: %d\n", PRINTCREDIT, current->module.credit);

                count++;
            }
            current = current->next;
        }
    }
    else if (strcasecmp(attribute, PRINTNAME) == 0)
    {
        while (current != NULL)
        {
            if (strcasecmp(current->module.name, value) == 0)
            {
                printf("\nA record for %s(value) = %s is found in the database. Below are the details:\n", PRINTNAME, value);
                printf("%s: %s\n", PRINTKEY, current->module.key);
                printf("%s: %s\n", PRINTNAME, current->module.name);
                printf("%s: %d\n", PRINTCREDIT, current->module.credit);

                count++;
            }
            current = current->next;
        }
    }
    else if (strcasecmp(attribute, PRINTCREDIT) == 0)
    {
        while (current != NULL)
        {
            if (current->module.credit == atoi(value))
            {
                printf("\nA record for %s(value) = %s is found in the database. Below are the details:\n", PRINTCREDIT, value);
                printf("%s: %s\n", PRINTKEY, current->module.key);
                printf("%s: %s\n", PRINTNAME, current->module.name);
                printf("%s: %d\n", PRINTCREDIT, current->module.credit);

                count++;
            }
            current = current->next;
        }
    }
    else
    {
        printf("Attribute name %s not found.\n", attribute);
        printf("Available attributes: %s , %s , %s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
        printf("E.g. %s=ict1101\n", PRINTKEY);
        return true;
    }

    if (count == 0)
    {
        printf("\nThere is no record with %s=%s found in the database.\n", attribute, value);
    }
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
    PrintReverse_save(head, file);

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

int main()
{
    //! ask user to open a file first to set them to memory with linkedlist
    printf("What file do you want to open?\n\n");
    printf("Available files: \n");
    printf("1. ModuleCode.txt\n\n");
    printf("Enter here: ");

    // scan the choice, user can enter either 'OPEN 1' or 'open ModuleCode.txt' or just '1'
    char filename[25];
    // scanf("%s", filename);
    fgets(filename, sizeof(filename), stdin);
    filename[strlen(filename) - 1] = '\0'; // get rid of the \n character at the end of the string
    printf("filename: %s\n", filename);
    // check if the first 4 words are 'open' or 'OPEN'
    for (int i = 0; i < 4; i++)
    {
        filename[i] = toupper(filename[i]);
    }
    // check the first 4 letters of the filename to see if it is 'OPEN'
    if (strncmp(filename, "OPEN", 4) == 0)
    {
        // if it is, then we need to get rid of the first 5 letters of the filename
        printf("removing open\n");

        for (int i = 0; i < strlen(filename); i++)
        {
            filename[i] = filename[i + 5];
        }
        printf("filename after removing open: %s\n", filename);
    }

    // check if user enters a integer or a string
    int fileNumber;
    if (sscanf(filename, "%d", &fileNumber) == 1)
    {
        // if it is a integer, then we need to convert it to a string
        if (fileNumber == 1)
        {
            strcpy(filename, "ModuleCode.txt");
        }
        else
        {
            printf("Invalid file number\n");
            return 1;
        }
    }

    printf("opening file: %s\n", filename);
    struct node *head = openFile(filename);

    struct node *current = head;
    int choice = 1;

    char *input;

    while (choice)
    {

        //! ask user what they want to do?
        printf("\nWhat do you want to do with %s?\n", filename);
        printf("1. SHOW_ALL - display all the modules\n\tCommand: SHOW_ALL or\n\t\t 1\n\n");
        printf("2. INSERT - add a new module\n\tCommand: INSERT <key>,<value 1>,<value 2>,...,<value n> or\n\t\t 2 <key>,<value 1>,<value 2>,...,<value n>\n\n");
        printf("3. QUERY - display a module\n\tCommand: QUERY <key> or\n\t\t 3 <key>\n\n");
        printf("4. UPDATE - change a specific module\n\tCommand: UPDATE <key> <values...> or\n\t\t 4 <key> <values...>\n\n");
        printf("5. DELETE - delete a module\n\tCommand: DELETE <key> or\n\t\t 5 <key>\n\n");
        printf("6. EXIT - close the application\n\tCommand: EXIT or\n\t\t 6\n\n");
        printf("7. SAVE - save all the latest records in memory into the database file\n\tCommand: SAVE <filename> or\n\t\t 7 <filename>\n\n");
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
        int j;
        char data[strlen(input)];
        for (j = 0; input[j + i + 1] != '\0'; j++)
        {
            data[j] = input[i + j + 1];
        }
        // printf("j = %d\n", j);
        data[j] = '\0';

        // printf("Command: %s\n", command);
        // printf("Data: %s\n", data);

        if (strcasecmp(command, "show_all") == 0 || strcasecmp(command, "1") == 0)
        {
            // SHOW_ALL: display all the modules
            printf("\n");
            //! print in reverse, so the header will be printed out first!
            PrintReverse(head);
            printf("\n");
        }
        else if (strcasecmp(command, "insert") == 0 || strcasecmp(command, "2") == 0)
        {
            // todo, pass in the data as a string, then split it into tokens, if the data contains anything
            //  INSERT: add a new module
            // printf("Data: %s\n", data);
            head = addModule(head, data);
        }
        else if (strcasecmp(command, "query") == 0 || strcasecmp(command, "3") == 0)
        {
            // QUERY: display a module
            query(head, data);
        }
        else if (strcasecmp(command, "update") == 0 || strcasecmp(command, "4") == 0)
        {
            // UPDATE: change a specific module
        }
        else if (strcasecmp(command, "delete") == 0 || strcasecmp(command, "5") == 0)
        {
            // DELETE: delete a module
        }
        else if (strcasecmp(command, "exit") == 0 || strcasecmp(command, "6") == 0)
        {
            // EXIT: close the application
            // exit the while loop
            choice = 0;
        }
        else if (strcasecmp(command, "save") == 0 || strcasecmp(command, "7") == 0)
        {
            // SAVE: SAVE into File
            save(head, data);
        }
        else
        {
            // If command not found
            printf("Command not found. Please try again.\n");
        }

        sleep(2);
    }

    // exit the application
    printf("Exiting the application\n");

    //* free the memory and exit
    //* go through the linked list and free the memory
    current = head;
    while (current != NULL)
    {
        struct node *next = current->next;
        free(current);
        current = next;
    }

    // sleep for 5 seconds
    sleep(5);

    return 0;
}