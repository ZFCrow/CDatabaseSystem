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

void showAll(struct node *head)
{
    int count = 0;
    struct node *current = head; // Initialize current
    numberofrecords(head);

    // print name of columns
    printf("%-15s\t%-40s\t%-3s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
    while (current != NULL)
    {
        printf("%-15s\t%-40s\t%-3d\n", current->module.key, current->module.name, current->module.credit);
        current = current->next;
    }
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

struct node *insert(struct node *head, char *data)
{
    struct Module newModule;

    // only prompt the next 2 if the data is not empty, if the last data is not a int, we remove the data part
    // and prompt the user to enter again
    // printf("Data: %s\n", data); // print the pointer

    // see if the data has 3 parts, if it does, then we can add the module

    int result = sscanf(data, "%8[^,],%54[^,],%d", newModule.key, newModule.name, &newModule.credit);

    // theres a chance that modulename contains a number, so we need to check if the last part is a number

    // check if the module code is valid if the result ==3 , otherwise dont have to check if code is valid
    if (result == 3)
    {
        if (checkCode(newModule.key) == 0)
        {
            result = 0;
        }
    }

    if (result != 3 || containsSpace(newModule.key)) //* if the result is not 3 or modulecode contain spaces, then we need to prompt the user to enter again
    {
        printf("Invalid input. Please choose to either cancel the operation or we will be prompting you to add values manually now.(enter \'Esc\' to cancel)\n");

        if (cancel())
        {
            return head;
        }

        char buffer[100]; // this is to check for extra input key in by user

        do
        {
            printf("\nEnter the %s: ", PRINTKEY);

            fgets(newModule.key, sizeof(newModule.key), stdin);
            newModule.key[strcspn(newModule.key, "\n")] = '\0'; // get rid of the \n character at the end of the string

            // if the length of the module code is more than 8 or contains spacing OR IT does not contain a verified code, then we need to prompt the user again
        } while (checkCode(newModule.key) == 0);

        //* check if the module code already exists
        struct node *current = head; // Initialize current
    
        if (checkExistingModuleCode(current, newModule.key) != NULL)
        {
            printf("\n%s \"%s\" already exists in database. Please try again.\n", PRINTKEY, newModule.key);
            printf("checked by function\n");
            return head;
        }

        //! name part
        printf("\nEnter the %s: ", PRINTNAME);
        fgets(newModule.name, sizeof(newModule.name), stdin);
        newModule.name[strcspn(newModule.name, "\n")] = '\0'; // get rid of the \n character at the end of the string
        
        if (strlen(newModule.name) == 0)
        {
            strcpy(newModule.name, "NA");
        }

        //! credits part
        bool valid = false;
        while (1)
        {
            printf("\nEnter the %s: ", PRINTCREDIT);
            // USER is allowed to enter \n to store a 0
            fgets(buffer, sizeof(buffer), stdin);
            // only get rid of \n if the buffer is not just \n
            if (buffer[0] != '\n')
                buffer[strcspn(buffer, "\n")] = '\0'; // get rid of the \n character at the end of the string
            
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
        printf("\nA new record of %s=%s, %s=%s, %s=%d is successfully inserted.\n", PRINTKEY, head->module.key, PRINTNAME, head->module.name, PRINTCREDIT, head->module.credit);
        return head;
    }

    else //* if the result is 3, means my module has been created successfully and we can create the node to store the module and link it to the list
    {
        // check if the module code already exists
        struct node *current = head; // Initialize current

        if (checkExistingModuleCode(current, newModule.key) != NULL)
        {
            printf("\nThe record with %s=%s already exists in database. Please try again.\n", PRINTKEY, newModule.key);
            printf("checked by function\n");
            return head;
        }

        // ! ADD node to link list

        head = addNode(head, newModule);
        printf("\nA new record of %s=%s, %s=%s, %s=%d is successfully inserted.\n", PRINTKEY, head->module.key, PRINTNAME, head->module.name, PRINTCREDIT, head->module.credit);
        return head;
    }
}

/* Checks whether the value x is present in linked list */
void query(struct node *head, char *data)
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
                return;
            else
            {
                printf("\nPlease enter query again: ");
                data = inputString(stdin, 10);
            }
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
                attribute[strcspn(attribute, "\n")] = '\0'; // get rid of the \n character at the end of the string

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
            {
                works = 0;
            }
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
                current = checkExistingModuleCode(head, value);

                if (current != NULL)
                {
                    if (count == 0)
                    {
                        printf("\nRecord for %s=%s is found in the database. Below are the details:\n\n", PRINTKEY, value);
                        printf("%-15s\t%-40s\t%-3s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
                    }
                    printf("%-15s\t%-40s\t%-3d\n", current->module.key, current->module.name, current->module.credit);
                    return;
                }
            }
            // check if attribute == module name
            else if (strcasecmp(attribute, PRINTNAME) == 0)
            {
                while (current != NULL)
                {
                    if (strcasecmp(current->module.name, value) == 0)
                    {
                        if (count == 0)
                        {
                            printf("\nRecord for %s=%s is found in the database. Below are the details:\n\n", PRINTNAME, value);
                            printf("%-15s\t%-40s\t%-3s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
                        }
                        printf("%-15s\t%-40s\t%-3d\n", current->module.key, current->module.name, current->module.credit);
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
                        if (count == 0)
                        {
                            printf("\nRecord for %s=%s is found in the database. Below are the details:\n\n", PRINTCREDIT, value);
                            printf("%-15s\t%-40s\t%-3s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
                        }
                        printf("%-15s\t%-40s\t%-3d\n", current->module.key, current->module.name, current->module.credit);
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
    {
        printf("\nThere is no record with %s=%s found in the database.\n", attribute, value);
    }

    return;
}

void update(struct node *head, char *data)
{
    struct node *current = head;
    char key[20];

    if (strlen(data) == 0)
    {
        if (cancel())
        {
            return;
        }

        else
        {
            do
            {
                // Get module code if it was not already specified
                printf("\nPlease type in the module code of the module you want to update:\n");

                fgets(key, sizeof(key), stdin);
                key[strcspn(key, "\n")] = '\0'; // get rid of the \n character at the end of the string

                if (strlen(key) > 20)
                {
                    for (int c; (c = getchar()) != '\n' && c != EOF;)
                    {
                    }
                }

                current = checkExistingModuleCode(head, key); // return current ptr if key found

                data = NULL;
            } while (strlen(key) == 0 || strcmp(key, " ") == 0);
        }
    }

    else
    {
        strcpy(key, data);
        current = checkExistingModuleCode(head, key); // returns  the current ptr if  key found
    }

    if (current != NULL)
    {
        printf("\n%s: %s\n", PRINTKEY, current->module.key);
        printf("%s: %s\n", PRINTNAME, current->module.name);
        printf("%s: %d\n\n", PRINTCREDIT, current->module.credit);

        // ask user which attribute they want to update
        printf("Which attribute do you want to update?\n");
        printf("1. Module Code\n");
        printf("2. Module Name\n");
        printf("3. Module Credit\n\n");

        char choice[3];

        do
        {
            printf("Enter the number here: ");

            fgets(choice, 3, stdin);
            choice[strcspn(choice, "\n")] = '\0'; // get rid of the \n character at the end of the string

            if (atoi(choice) <= 0 || atoi(choice) > 3 || strlen(choice) > 2)
            {
                printf("Invalid choice, please try again.\n\n");

                // clear buffer
                if ((atoi(choice) == 0 && strlen(choice) > 1) || atoi(choice) > 9)
                {
                    for (int c; (c = getchar()) != '\n' && c != EOF;)
                    {
                    }
                }
            }
        } while (atoi(choice) <= 0 || atoi(choice) > 3 || strlen(choice) > 2);

        if (atoi(choice) == 1)
        {
            char newkey[20];

            while (1)
            {
                // ask user to enter the new module code
                printf("\nPlease enter the new module code you want to change to: \n");
                fgets(newkey, sizeof(newkey), stdin);
                newkey[strcspn(newkey, "\n")] = '\0'; // get rid of the \n character at the end of the string

                // check if module code exists
                if (checkExistingModuleCode(head, newkey))
                {
                    printf("Module code already exist.\n");
                }

                // if module code does not exist, update module code
                else if (checkCode(newkey))
                {
                    strcpy(current->module.key, newkey);
                    printf("The value of the module code for the record %s=%s is successfully updated.\n", PRINTKEY, key);
                    return;
                }
            }
        }

        else if (atoi(choice) == 2)
        {
            char newname[55];

            // ask user to enter the new module name
            printf("\nPlease enter the new module name you want to change to: \n");

            fgets(newname, sizeof(newname), stdin);
            newname[strcspn(newname, "\n")] = '\0'; // get rid of the \n character at the end of the string

            if (strlen(newname) == 0) // set name to NA if not specified
            {
                strcpy(newname, "NA");
            }

            if (strlen(newname) > 50)
            {
                for (int c; (c = getchar()) != '\n' && c != EOF;)
                {
                }
            }

            strcpy(current->module.name, newname);
            printf("The value of the module name for the record %s=%s is successfully updated.\n", PRINTKEY, key);
        }

        else if (atoi(choice) == 3)
        {
            char newcredit[255];
            int validint = 1;

            do
            {
                // ask user to enter the new module credit
                printf("\nPlease enter the new module credit you want to change to: \n");

                fgets(newcredit, sizeof(newcredit), stdin);
                newcredit[strcspn(newcredit, "\n")] = '\0'; // get rid of the \n character at the end of the string

                for (int i = 0; i < strlen(newcredit); i++)
                {
                    if (isdigit(newcredit[i]) == 0)
                    {
                        validint = 0;
                        break;
                    }
                }

                if (validint == 1)
                {
                    break;
                }

                else
                {
                    printf("Invalid integer, please try again.\n");
                    validint = 1;
                }

            } while (1);

            current->module.credit = atoi(newcredit);
            printf("The value of the module credit for the record %s=%s is successfully updated.\n", PRINTKEY, key);
        }

        return;
    }

    else
    {
        // exit to main menu if there are no records found
        printf("\nThere is no record with %s=%s found in the database.\n", PRINTKEY, key);
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
            printf("\nPlease type in the module code that you want to delete:\n");
            fgets(deleteData, 20, stdin);

            deleteData[strcspn(deleteData, "\n")] = '\0'; // get rid of the \n character at the end of the string
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

        printf("\nThe record of %s=%s is successfully deleted.\n", PRINTKEY, deleteData);
    }

    else
    {
        printf("\nThere is no record with %s=%s found in the database.\n", PRINTKEY, deleteData);
    }

    return;
}

//!!! SAVING SECTION

void Print_save(struct node *head, FILE *file)
{
    fprintf(file, "%s//%s//%s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);

    while (head != NULL) //traverse through the linked list and write into the file
    {
        fprintf(file, "%s//%s//%d\n", head->module.key, head->module.name, head->module.credit); // Print the current node and the respective details
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
            printf("\nInvalid File. Please save to a .txt file!\n");
            if (cancel()) // option for user to break out of function
            {
                return;
            }

            else
            {
                printf("\nPlease enter filename again: ");
                fgets(filename, 25, stdin);
                filename[strcspn(filename, "\n")] = '\0'; // get rid of the \n character at the end of the string
            }
        }
    }

    FILE *file = fopen(filename, "w"); // Open the file for writing
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    Print_save(head, file); // writing into the file

    fclose(file);

    printf("File Saved: %s\n", filename);

    return;
}
