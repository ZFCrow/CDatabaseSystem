//! a database system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <remove.c>

struct Fruit {
    char key[20];
    double price;

};

void openfile(){
    //! open file and display it on terminal 
    FILE *fp;
    char ch;
    fp = fopen("FruitPrice.txt", "r");
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
    printf("1. Display all the fruit prices\n");
    printf("2. Display the price of a specific fruit\n");
    printf("3. Change the price of a specific fruit\n");
    printf("4. Add a new fruit\n");
    printf("5. Delete a fruit\n");
    printf("6. Exit\n");
    int choice;
    scanf("%d", &choice);
    getchar(); // to get rid of the \n character

    if (choice == 1){
        openfile();
        
    }
    //for now we just try to add a new fruit with prices with the struct 
    else if (choice == 4){
        struct Fruit fruit;
        printf("Enter the name of the fruit: ");
        scanf("%s", fruit.key);
        printf("Enter the price of the fruit: ");
        scanf("%lf", &fruit.price);

        FILE *fp;
        fp = fopen("FruitPrice.txt", "a"); //append to the file
        if (fp == NULL){
            printf("Error while opening the file.\n");
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "%s %.2lf\n", fruit.key, fruit.price);
        fclose(fp);
        printf("The fruit has been added to the database.\n");
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