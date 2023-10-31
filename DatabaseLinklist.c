#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> 

struct Module {
    char key[20]; // module code
    char name[55]; // module name
    char lead[40]; // module lead
};

struct node {
    struct Module module;
    struct node *next;
};

// instead of reading all these modules from the file in main, create a openfile function and create a linkedlist from there, 
// then return the head of the linked list to main
struct node *openFile(char *filename){
    printf("Reading from the file...\n");
    FILE *file = fopen(filename, "r"); // Open the file for reading

    if (file == NULL) {
        perror("Error opening the file");
        exit(1);
    }

    struct node *head = NULL; // Create an empty linked list
    char line[100]; // Adjust the buffer size based on your file's format

    while (fgets(line, sizeof(line), file) != NULL) {
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
        if (newNode == NULL) {
            perror("Memory allocation failed"); // If malloc() failed, print an error message and exit
            exit(1);
        }

        newNode->module = newModule; // Copy the data into the new node
        newNode->next = head; // connect the new node to the current head of the list, first one will be NULL
        head = newNode; // Set the head to point to the new node 

 

    }

    fclose(file); // Close the file when you're done reading from it

    return head; // return the head of the linked list 
}

void PrintReverse(struct node *head) {

    if (head == NULL) {
        return;
    }

    PrintReverse(head->next); // Recursively print the rest of the list
    printf("%s %s %s\n", head->module.key, head->module.name, head->module.lead); // Print the current node, the first one will run last!
}


int main() {
    //! ask user to open a file first to set them to memory with linkedlist
    printf("What file do you want to open?\n");
    printf("Available files: \n");
    printf("1. ModuleCode.txt\n");
    //scan the choice, user can enter either 'OPEN 1' or 'open ModuleCode.txt' or just '1'
    char filename[25];
    // scanf("%s", filename);
    fgets(filename, sizeof(filename), stdin);
    filename [strlen(filename) - 1] = '\0'; // get rid of the \n character at the end of the string
    printf("filename: %s\n", filename);
    //check if the first 4 words are 'open' or 'OPEN'
    for (int i = 0; i < 4; i++) {
        filename[i] = toupper(filename[i]);
    }
    //check the first 4 letters of the filename to see if it is 'OPEN'
    if (strncmp(filename, "OPEN", 4) == 0) {
        //if it is, then we need to get rid of the first 5 letters of the filename
        printf("removing open\n");

        for (int i = 0; i < strlen(filename); i++) {
            filename[i] = filename[i + 5]; 
        }
        printf("filename after removing open: %s\n", filename);
    }

    //check if user enters a integer or a string 
    int fileNumber;
    if (sscanf(filename, "%d", &fileNumber) == 1) { 
        //if it is a integer, then we need to convert it to a string
        if (fileNumber == 1) {
            strcpy(filename, "ModuleCode.txt");
        }else {
            printf("Invalid file number\n");
            return 1;
        }
    }

    printf("opening file: %s\n", filename);
    struct node *head = openFile(filename);

    struct node *current = head;

    //! ask user what they want to do? 
    printf("What do you want to do?\n");
    printf("1. Display all the modules\n");
    printf("2. Display 1 module\n");
    printf("3. Change 1 specific module\n");
    printf("4. Add a new module\n");
    printf("5. Delete a module\n");
    printf("6. Exit\n");

    int choice;
    scanf("%d", &choice);
    getchar(); // to get rid of the \n character

    if (choice == 1){
        //! print in reverse, so the header will be printed out first! 
        PrintReverse(head); 


    }
    //exit the application
    else{
        printf("Exiting the application\n");
        
        //* free the memory and exit
        //* go through the linked list and free the memory 
        current = head;
        while (current != NULL) {
            struct node *next = current->next;
            free(current);
            current = next;
        }

        //sleep for 5 seconds 
        sleep(5);
    }



    return 0;
}
