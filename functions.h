// #ifndef MAINFUNCTIONS_H
// #define MAINFUNCTIONS_H
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define PRINTKEY "Module Code"
#define PRINTNAME "Module Name"
#define PRINTCREDIT "Module Credit"

#define INITIAL_CAPACITY 5

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
struct node *sort(struct node *head, int sortchoice);
char *inputString(FILE *fp, size_t size);
struct node *openFile(char *filename); //! trigger by main function after filemenu function  and filenamevalidations to determine if file is valid
void addfile(char *filelist[], int *numoffiles, char *filename, int *max_capacity);
void declaration();

// functions in validations.c
int containsSpace(const char *str);
char *filenamevalidations(char *filename, int numoffiles, char *filelist[]);
bool checkCode(char key[]);
struct node *checkExistingModuleCode(struct node *head, char key[]);

// functions in filemenu.c
char *filemenu(char *filelist[], int *numoffiles, int *max_capacity);
int mainmenu(struct node **head, struct node **current);

// functions in crud.c
void showAll(struct node *head);
void numberofrecords(struct node *head);
void PrintReverse(struct node *head);
struct node *insert(struct node *head, char *data);
struct node *addNode(struct node *head, struct Module newModule);
void print_query_error();
char *ask_query();
void print_found(int count, char *value, struct node *current);
void query(struct node *head, char *inputData);
void update(struct node *head, char *data);
void delete(struct node **head, char *data);
void save(struct node *head, char *filename);
void PrintReverse_save(struct node *head, FILE *file);
void Print_save(struct node *head, FILE *file);



#endif
