#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <conio.h>

#include "functions.h"

void print_query_error()
{
    printf("Available attributes: %s , %s , %s\n", PRINTKEY, PRINTNAME, PRINTCREDIT);
    printf("Example of a query: %s=ict1101\n", PRINTKEY);
}

void numberofrecords(struct node *head)
{
    int count = 0;
    struct node *current = head; // Initialize current
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    printf("\nThere are in total %d records found:\n\n", count);
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

void declaration()
{
    printf("\"\nDeclaration\n");
    printf("SIT\'s policy on copying does not allow the students to copy source code as well as assessment solutions\nfrom another person or other places. It is the students\' responsibility to guarantee that their assessment\nsolutions are their own work. Meanwhile, the students must also ensure that their work is not accessible\nby others. Where such plagiarism is detected, both of the assessments involved will receive ZERO mark.\n");
    printf("\nWe hereby declare that:\n");
    printf("- We fully understand and agree to the abovementioned plagiarism policy.\n");
    printf("- We did not copy any code from others or from other places.\n");
    printf("- We did not share our codes with others or upload to any other places for public access.\n");
    printf("- We agree that our project will receive Zero mark if there is any plagiarism detected.\n");
    printf("\nDeclared by: P5_4\n");
    printf("Team members:\n");
    printf("1. Chua Shing Ying\n");
    printf("2. Lim Mei Tian\n");
    printf("3. Lee Zhiqi, Charlene\n");
    printf("4. Cheah Zheng Feng\n");
    printf("5. Phang Zi Heng\n");
    printf("Date: 23 November 2023\n\"\n\n");
}