// #ifndef MAINFUNCTIONS_H
// #define MAINFUNCTIONS_H
#ifndef FUNCTIONS_H
#define FUNCTIONS_H


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


// functions in main.c
bool cancel();
int containsSpace(const char *str);
char *inputString(FILE *fp, size_t size);
struct node *openFile(char *filename); //! trigger by main function after filemenu function  and filenamevalidations to determine if file is valid
void print_query_error();
char *ask_query();
void addfile(char *filelist[], int *numoffiles, char *filename, int *max_capacity);
char *filenamevalidations(char *filename, int numoffiles, char *filelist[]);
struct node *sort(struct node *head, int sortchoice);
bool checkCode(char key[]);


// functions in filemenu.c
char *filemenu(char *filelist[], int *numoffiles, int *max_capacity);
int menu2(struct node **head, struct node **current);




// functions in crud.c
void printall(struct node *head);
void PrintReverse(struct node *head);
struct node *addModule(struct node *head, char *data);
bool query(struct node *head, char *inputData);
void update(struct node *head, char *data);
void delete(struct node **head, char *data);
void save(struct node *head, char *filename);
void PrintReverse_save(struct node *head, FILE *file);
void Print_save(struct node *head, FILE *file);
#endif
