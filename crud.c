#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <conio.h>

#include "functions.h"

//!!!  TO SHOW EVERYTHING
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

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// add node
struct node *addNode(struct node *head, struct Module newModule)
{
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

struct node *addModule(struct node *head, char *data)
{
    struct Module newModule;

    // only prompt the next 2 if the data is not empty, if the last data is not a int, we remove the data part
    // and prompt the user to enter again
    printf("Data: %s\n", data); // print the pointer

    // see if the data has 3 parts, if it does, then we can add the module

    int result = sscanf(data, "%8[^,],%54[^,],%d", newModule.key, newModule.name, &newModule.credit);

    // theres a chacne that modulename contains a number, so we need to check if the last part is a number
    printf("result: %d\n", result);

    // check if the module code is valid
    if (checkCode(newModule.key) == 0)
    {
        printf("Invalid module code.\nModule Code only contains a total of not more than 8 characters.\nEnsure that your module code has the first 3-4 characters as alpha and the remaining characters as digits.\nPlease try again.\n");
        result = 0;
    }

    if (result != 3 || containsSpace(newModule.key)) //* if the result is not 3 or modulecode contain spaces, then we need to prompt the user to enter again
    {

        printf("Invalid input. please choose to either cancel the operation or we will be prompting you to add values manually now.(enter esc to cancel)\n");
        if (cancel())
        {
            return head;
        }
        char buffer[100]; // this is to check for extra input key in by user

        do
        {
            printf("Enter the %s: ", PRINTKEY);

            fgets(newModule.key, sizeof(newModule.key), stdin);
            newModule.key[strlen(newModule.key) - 1] = '\0'; // get rid of the \n character at the end of the string

            // if the length of the module code is more than 8 or contains spacing OR IT does not contain a verified code, then we need to prompt the user again
        } while (checkCode(newModule.key) == 0);

        //* check if the module code already exists
        struct node *current = head; // Initialize current
        if (checkExistingModuleCode(head, newModule.key) == 1)
        {
            printf("\n%s \"%s\" already exists in database. Please try again.\n", PRINTKEY, newModule.key);
            printf("checked by function\n");
            return head;
        }

        //! name part
        printf("Enter the %s: ", PRINTNAME);
        fgets(newModule.name, sizeof(newModule.name), stdin);
        newModule.name[strlen(newModule.name) - 1] = '\0'; // get rid of the \n character at the end of the string
        if (strlen(newModule.name) == 0)
        {
            strcpy(newModule.name, "NA");
        }

        //! credits part
        bool valid = false;
        while (1)
        {
            printf("Enter the %s: ", PRINTCREDIT);
            // USER is allowed to enter \n to store a 0
            fgets(buffer, sizeof(buffer), stdin);
            // only get rid of \n if the buffer is not just \n
            if (buffer[0] != '\n')
                buffer[strlen(buffer) - 1] = '\0'; // get rid of the \n character at the end of the string
            if (buffer[0] == '\n')
            {
                newModule.credit = 0;
                break;
            }

            else
            {
                // check for isdigit and if it is a number, we store it into the credit
                for (int i = 0; buffer[i] != '\0'; i++)
                {
                    if (isdigit(buffer[i]))
                    {
                        valid = true;
                    }
                    else
                    {
                        printf("Invalid input. Please try again.\n");
                        valid = false;
                        break;
                    }
                }
                // if the buffer is a number, we store it into the credit
                if (valid)
                {
                    newModule.credit = atoi(buffer);
                    break;
                }
            }
        }

        //* ADD node to link list

        head = addNode(head, newModule);
        printf("A new record of Module Code=%s, Module Name=%s, Module Credit=%d is successfully inserted. ", head->module.key, head->module.name, head->module.credit);
        return head;
    }

    else //* if the result is 3, means my module has been created successfully and we can create the node to store the module and link it to the list
    {

        // check if the module code already exists
        struct node *current = head; // Initialize current

        if (checkExistingModuleCode(head, newModule.key) == 1)
        {
            printf("\n%s \"%s\" already exists in database. Please try again.\n", PRINTKEY, newModule.key);
            printf("checked by function\n");
            return head;
        }

        // ! ADD node to link list

        head = addNode(head, newModule);
        printf("A new record of Module Code=%s, Module Name=%s, Module Credit=%d is successfully inserted. ", head->module.key, head->module.name, head->module.credit);
        return head;
    }
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

void print_found(int count, char *value, struct node *current)
{
    if (count == 0)
    {
        printf("\nRecord for %s = %s is found in the database. Below are the details:\n\n", PRINTKEY, value);
        printf("%-15s\t%-40s\t%-3s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
    }
    printf("%-15s\t%-40s\t%-3d\n", current->module.key, current->module.name, current->module.credit);
}

/* Checks whether the value x is present in linked list */
bool query(struct node *head, char *data)
{
    int works = 0; // works = 1 when there is error in the data, 0 when successful
    int count = 0; // count the number of matches
    char *attribute;
    char *value;

    do
    {
        if (works)
        {
            if (cancel())
                return true;
            else
                data = ask_query();
        }
        // check if data is empty
        if (strcmp(data, "") == 0)
        {
            printf("\nQuery data was not found.\n");
            print_query_error();
            works = 1;
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
                works = 1;
            }
            // check if got extra space: " inf1001"
            else if (isspace(value[0]))
            {
                for (int i = 0; value[i] != '\0'; i++)
                    value[i] = value[i + 1];
                works = 0;
            }
            else
                works = 0;
        }
        else
        {
            // if no attribute, set as default module code
            attribute = PRINTKEY;
            value = data;
            works = 0;
        }

        struct node *current = head; // Initialize current
        if (!works)
        {
            // check if attribute == module code
            if (strcasecmp(attribute, PRINTKEY) == 0)
            {
                while (current != NULL)
                {
                    if (strcasecmp(current->module.key, value) == 0)
                    {
                        print_found(count, value, current);
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
                        print_found(count, value, current);
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
                        print_found(count, value, current);
                        count++;
                    }
                    current = current->next;
                }
            }
            else
            {
                printf("\nAttribute name \"%s\" not found.\n", attribute);
                print_query_error();
                works = 1;
            }
        }
    } while (works);

    // count = 0 means no matches found
    if (count == 0)
        printf("\nThere is no record with %s = %s found in the database.\n", attribute, value);

    return true;
}

void update(struct node *head, char *data)
{
    struct node *current = head;
    char *key = (char *)malloc(sizeof(char));

    if (strlen(data) == 0)
    {
        if (cancel())
        {
            return;
        }

        else
        {
            // Get module code
            printf("Please type in the module code of the module you want to update:\n");

            fgets(key, 9, stdin);
            key[strlen(key) - 1] = '\0'; // get rid of the \n character at the end of the string

            printf("%s\n", key);

            current = returnExistingModuleCodeptr(head, key); // return current ptr if key found
        }
    }

    else
    {
        key = strtok(data, " ");
        current = returnExistingModuleCodeptr(head, key); // returns  the current ptr if  key found
    }

    if (current != NULL)
    {
        printf("Key found.\n\n");

        printf("%s: %s\n", PRINTKEY, current->module.key);
        printf("%s: %s\n", PRINTNAME, current->module.name);
        printf("%s: %d\n\n", PRINTCREDIT, current->module.credit);

        printf("Which attribute do you want to update?\n");
        printf("1. Module Code\n");
        printf("2. Module Name\n");
        printf("3. Module Credit\n\n");

        char *choice = (char *)malloc(sizeof(char));

        do
        {
            printf("Enter the number here:\n");

            fgets(choice, sizeof(choice), stdin);
            choice[strlen(choice) - 1] = '\0'; // get rid of the \n character at the end of the string

            if (atoi(choice) < 1 || atoi(choice) > 3)
            {
                printf("Invalid choice, please try again.\n\n");
            }
        } while (atoi(choice) <= 0 || atoi(choice) > 3);

        if (atoi(choice) == 1)
        {
            char newkey[20];

            while (1)
            {
                printf("Please enter the new module code you want to change to:\n");
                fgets(newkey, sizeof(newkey), stdin);
                newkey[strlen(newkey) - 1] = '\0'; // get rid of the \n character at the end of the string

                // check if module code exists
                if (checkExistingModuleCode(head, newkey))
                {
                    printf("Module code already exist.\n");
                    return;
                }

                if (checkCode(newkey))
                {
                    strcpy(current->module.key, newkey);
                    printf("The value for the module code is successfully updated.\n");
                    return;
                }

                else
                {
                    printf("Invalid module code.\nModule Code only contains a total of not more than 8 characters.\nEnsure that your module code has the first 3-4 characters as alpha and the remaining characters as digits.\nPlease try again.\n");
                }
            }
        }

        else if (atoi(choice) == 2)
        {
            char newname[55];
            printf("Please enter the new module name you want to change to:\n");

            fgets(newname, sizeof(newname), stdin);
            newname[strlen(newname) - 1] = '\0'; // get rid of the \n character at the end of the string
            strcpy(current->module.name, newname);
            printf("The value for the module name is successfully updated.\n");
        }

        else if (atoi(choice) == 3)
        {
            char *newcredit = (char *)malloc(sizeof(char));
            do
            {
                printf("Please enter the new module credit you want to change to:\n");

                fgets(newcredit, sizeof(newcredit), stdin);
                newcredit[strlen(newcredit) - 1] = '\0'; // get rid of the \n character at the end of the string

                if (isdigit(*newcredit) == 0)
                {
                    printf("Invalid integer, please try again.\n\n");
                }
            } while (isdigit(*newcredit) == 0);

            current->module.credit = atoi(newcredit);
            printf("The value for the module credit is successfully updated.\n");

            free(newcredit);
        }

        free(choice);
        free(key);

        return;
    }

    else
    {
        printf("There is no record with %s found in the database.\n", key);
        return;
    }
}

void delete(struct node **head, char *deleteData)
{
    struct node *current = *head;
    struct node *prev = NULL;

    if (strlen(deleteData) == 0)
    {
        if (cancel())
        {
            return;
        }

        else
        {
            printf("Please type in the module code that you want to delete:\n");
            fgets(deleteData, sizeof(deleteData), stdin);
            // Remove the newline character at the end of the string
            deleteData[strcspn(deleteData, "\n")] = '\0';

            // clear the stdinput
            for (int c; (c = getchar()) != '\n' && c != EOF;)
            {
            }
        }
    }

    while (current != NULL && strcmp(current->module.key, deleteData) != 0)
    {
        prev = current;
        current = current->next;
    }

    if (current != NULL)
    {
        // If the node to be deleted is the head
        if (prev == NULL)
        {
            *head = current->next;
        }
        else
        {
            prev->next = current->next;
        }

        // Free the memory of the node
        free(current);

        printf("The record of key %s is successfully deleted.\n", deleteData);
    }

    else
    {
        printf("There is no record with key %s found in the database.\n", deleteData);
    }

    return;
}

//!!! SAVING SECTION
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

void save(struct node *head, char *filename)
{
    int check = 1;
    while (check)
    {
        if (strlen(filename) >= 5) // check if filename has at least 5chars
        {
            if (strcasecmp(filename + strlen(filename) - 4, ".txt") == 0) // check if last 4 chars matches .txt
            {
                check = 0; // break out of loop and continue to save into file
            }
        }

        if (check)
        {
            printf("Invalid File. Please save to a .txt file!\n");
            if (cancel())
            {
                return;
            }

            else
            {
                printf("Please enter filename again: ");
                fgets(filename, 25, stdin);
                filename[strlen(filename) - 1] = '\0'; // Remove the newline character from fgets
            }
        }
    }

    printf("Opening File: %s...\n", filename);
    FILE *file = fopen(filename, "w"); // Open the file for writing
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    printf("Saving File: %s...\n", filename);
    // PrintReverse_save(head, file);

    Print_save(head, file); // writing into the file

    printf("Closing File: %s...\n", filename);
    fclose(file);

    printf("File Saved: %s\n", filename);

    return;
}
