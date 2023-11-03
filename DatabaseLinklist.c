#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

struct Module
{
    char key[20];  // module code
    char name[55]; // module name
    char lead[40]; // module lead
};

struct node
{
    struct Module module;
    struct node *next;
};

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

        char *token = strtok(line, "//"); // Split the line into tokens separated by //
        strcpy(newModule.key, token);
        token = strtok(NULL, "//"); // NULL as the first argument tells strtok() to continue splitting the same string
        strcpy(newModule.name, token);
        token = strtok(NULL, "//");
        strcpy(newModule.lead, token);

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

    PrintReverse(head->next);                                                       // Recursively print the rest of the list
    printf("%s \t%s \t%s", head->module.key, head->module.name, head->module.lead); // Print the current node, the first one will run last!
}

// TODO change scanf to fgets as scanf will stop reading when it encounters a space
// todo add a check to see if the module code already exists
struct node *addModule(struct node *head)
{
    struct Module newModule;
    printf("Enter the module code: ");
    scanf("%s", newModule.key);
    printf("Enter the module name: ");
    scanf("%s", newModule.name);
    printf("Enter the module lead: ");
    scanf("%s", newModule.lead);

    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    if (newNode == NULL)
    {
        perror("Memory allocation failed");
        exit(1);
    }

    newNode->module = newModule;
    newNode->next = head; // connect the new node to the current head of the list
    head = newNode;

    return head;
}

/* Checks whether the value x is present in linked list */
bool query(struct node *head, char *data)
{
    // char *value;
    // printf("Enter the module code: ");
    // scanf("%ms", &value);

    // printf("Hi %s\n", value);
    // printf("What's head: %s\t%s\t%s\n", head->module.key, head->module.name, head->module.lead);
    struct node *current = head; // Initialize current
    while (current != NULL)
    {
        if (strcasecmp(current->module.key, data) == 0)
        {
            printf("\nModule code \"%s\" is found in database. Below are the details:\n\n", data);
            printf("Module Code: %s\n", current->module.key);
            printf("Module Name: %s\n", current->module.name);
            printf("Module Lead: %s\n", current->module.lead);

            return true;
        }
        current = current->next;
    }

    printf("\nModule code \"%s\" is not found in database.\n", data);
    free(data);

    return false;
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
    printf("What file do you want to open?\n");
    printf("Available files: \n");
    printf("1. ModuleCode.txt\n");
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
        printf("\nWhat do you want to do?\n");
        printf("1. SHOW_ALL - display all the modules\n\tCommand: 1 or\n\t\t SHOW_ALL\n\n");
        printf("2. INSERT - add a new module\n\tCommand: 2 <key> <values...> or\n\t\t INSERT <key> <values...>\n\n");
        printf("3. QUERY - display a module\n\tCommand: 3 <key> or\n\t\t QUERY <key>\n\n");
        printf("4. UPDATE - change a specific module\n\tCommand: 4 <key> <values...> or\n\t\t UPDATE <key> <values...>\n\n");
        printf("5. DELETE - delete a module\n\tCommand: 5 <key> or\n\t\t DELETE <key>\n\n");
        printf("6. EXIT - close the application\n\tCommand: 6 or\n\t\t EXIT\n\n");
        printf("Enter here: ");
        // scanf("%d", &choice);
        // getchar(); // to get rid of the \n character

        input = inputString(stdin, 10);
        // printf("Input: %s\n", input);
        // printf("Length: %d\n", strlen(input));

        // Get command from input
        int i = 0;
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
        int j = 0;
        char data[strlen(input)];
        for (j; input[j + i + 1] != '\0'; j++)
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
            // INSERT: add a new module
            printf("Data: %s\n", data);
            head = addModule(head);
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
