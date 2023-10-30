//! a database system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


struct Module {
    char key[20]; //module code
    char name[55]; //module name
    char lead[20]; //module lead

};

void openfile(){
    //! open file and display it on terminal 
    FILE *fp;
    char ch;
    fp = fopen("ModuleCode.txt", "r");
    if (fp == NULL){
        printf("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    printf("The contents of %s file are:\n", "test.txt");

    while((ch = fgetc(fp)) != EOF){ //read the file character by character till the end of file EOF meaning end of file
        printf("%c", ch);
    }
    fclose(fp);
    printf("\n");


}


int main(){
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
        openfile();
    
    }


    //for now we just try to add a new fruit with prices with the struct 
    else if (choice == 4){

        struct Module module;
        printf("Enter the module code: ");
        scanf("%s", module.key);
        getchar(); // Consume the newline character left in the input buffer by scanf

        printf("Enter the module name: ");
        //scanf("%s", module.name); // spacing is not allowed
        //USE fgets instead
        fgets(module.name, 55, stdin);
        module.name[strcspn(module.name, "\n")] = 0;  //remove the \n character

        printf("Enter the module lead: ");
        scanf("%s", module.lead);


        FILE *fp;
        fp = fopen("ModuleCode.txt", "a"); //append to the file

        if (fp == NULL){
            printf("Error while opening the file.\n");
            exit(EXIT_FAILURE);
        }
        //IF FILE EMPTY, CREATE A FIRST ROW OF HEADERS , MODULECODE, MODULENAME, MODULELEAD
        fseek(fp, 0, SEEK_END);
        if (ftell(fp) == 0){
            fprintf(fp, "%s %s %s\n", "ModuleCode", "ModuleName", "ModuleLead");
        }

        fprintf(fp, "%s %s %s\n", module.key, module.name, module.lead);
        fclose(fp);
        printf("The modules has been added to the database.\n");
    }

    else{
        //exit 
        printf("Thank you for using the database.\n");
        //delay 3seconds before closing 
        _sleep(3000);
    }

    //stays on terminal 
    if (choice != 6){
        getchar();
    }

    return 0;
}