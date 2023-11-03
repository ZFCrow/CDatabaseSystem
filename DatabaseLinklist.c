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

    PrintReverse(head->next);  // Recursively print the rest of the list
    // check if module.name contains a \n character, if it does not,add 1 behind
    char *space = strchr(head->module.name, ' ');
    // if (space == NULL)
    // {
    //     printf("%s \t%s\t\t%s\n", head->module.key, head->module.name, head->module.lead); // Print the current node, the first one will run last!
    // }
    // else
    // {
    //     printf("%s \t%s \t%s\n", head->module.key, head->module.name, head->module.lead); // Print the current node, the first one will run last!
    // }
    printf("%-15s\t%-25s\t%-30s\n", head->module.key, head->module.name, head->module.lead);

}

// TODO change scanf to fgets as scanf will stop reading when it encounters a space
// todo add a check to see if the module code already exists
struct node *addModule(struct node *head)
{
    struct Module newModule;
    printf("Enter the module code: ");
    scanf("%s", newModule.key);
    //remove the \n character from the buffer
    getchar();
    printf("Enter the module name: ");
    // scanf("%s", newModule.name);
    //use fgets instead 
    fgets(newModule.name, sizeof(newModule.name), stdin);
    newModule.name[strlen(newModule.name) - 1] = '\0'; // get rid of the \n character at the end of the string
    printf("Enter the module lead: ");
    // scanf("%s", newModule.lead);
    //use fgets instead
    fgets(newModule.lead, sizeof(newModule.lead), stdin);
    newModule.lead[strlen(newModule.lead) - 1] = '\0'; // get rid of the \n character at the end of the string


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
bool query(struct node *head)
{
    printf("in query");
    char *value = NULL;
    //char value[50];
    printf("Enter the module code: ");
    scanf("%ms", &value);
    //scanf("%s", value);

    printf("Hi %s\n", value);
    // printf("What's head: %s\t%s\t%s\n", head->module.key, head->module.name, head->module.lead);
    struct node *current = head; // Initialize current
    while (current != NULL)
    {
        if (strcasecmp(current->module.key, value) == 0)
        {
            printf("\nModule code \"%s\" is found in database. Below are the details:\n\n", value);
            printf("Module Code: %s\n", current->module.key);
            printf("Module Name: %s\n", current->module.name);
            printf("Module Lead: %s\n", current->module.lead);

            return true;
        }
        current = current->next;
    }

    printf("\nModule code \"%s\" is not found in database.\n", value);
    free(value);

    return false;
}

int main()
{
    //! ask user to open a file first to set them to memory with linkedlist
    printf("What file do you want to open?\n");
    printf("Available files: \n");
    printf("1. ModuleCode.txt\n");
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
    int choice = 0;

    while (choice != 6)
    {

        //! ask user what they want to do?
        printf("\nWhat do you want to do?\n");
        printf("1. Display all the modules\n");
        printf("2. Display 1 module\n");
        printf("3. Change 1 specific module\n");
        printf("4. Add a new module\n");
        printf("5. Delete a module\n");
        printf("6. Exit\n");

        scanf("%d", &choice);
        getchar(); // to get rid of the \n character

        if (choice == 1)
        {
            //! print in reverse, so the header will be printed out first!
            PrintReverse(head);
        }
        else if (choice == 2)
        {
            printf("going query");
            query(head);
        }
        // add function
        else if (choice == 4)
        {
            head = addModule(head);
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
