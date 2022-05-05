#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000                                 //change maximum character/line here
#define USER_INPUT "Student.inp"                 //rename user input file here
#define EXAMPLE_INPUT "Student-example.inp"      //rename example input file here
#define USER_OUTPUT "Student.out"                //rename output file here

FILE *fi, *fo;                       //input file and output file
int q = 0;
char tmp[MAX], ss[MAX], choice, custom_input[MAX];

typedef struct node
{
    char name[MAX], id[MAX];
    int age;
    float math, phys, chem, elec;
    struct node *next;
} student;
student *profile = NULL;             //initial dynamic stack
student *tmp_profile = NULL;

void print_welcome_screen();         //show welcome and guide
void regenerate_example_input();     //in case of missing, this will regenerate an example input file
void process_file();                 //handle in/out file and fix error such as corrupt, missing,..
void process_data();                 //handle in/out data and request from program
int isEND(char *s);                  //check end of input file
void push(student x);                //push new student to dynamic stack
student pop();                       //pop out top student of stack
student peek();                      //get info of top student of stack
int empty();                         //check if stack is empty or not
char* process_name(char *s);         //post-processing naming by remove redundant space or missed capital
int check_age(int x);                //check if age is valid
int check_score(float x);            //check if score is valid
void classify_score(float x);        //print "!, P, F" to output file depend on validity of data

int main()
{
    print_welcome_screen();
    process_file();
    return 0;
}

void process_data()
{
    student tt;
    fprintf(fo, "SYMBOL EXPLANATION:\n(!): Data is not valid\n(P): Pass\n(F): Fail\n\n");
    while(1 > 0)
    {
        memset(tmp, 0, strlen(tmp) + 1);    //reset tmp
        memset(ss, 0, strlen(ss) + 1);      //reset ss
        fgets(tmp, sizeof(tmp), fi);        //get command
        strcpy(tmp, process_name(tmp));
        tmp[strcspn(tmp, "\n")] = 0;
        if(isEND(tmp)) break;
        else if(strcmp(tmp, "PUSH")==0 || strcmp(tmp, "Push")==0)
        {
            //get name
            fgets(tmp, sizeof(tmp), fi);
            tmp[strcspn(tmp, "\n")] = 0;
            strcpy(tt.name, process_name(tmp));
            //get id
            fgets(tmp, sizeof(tmp), fi);
            tmp[strcspn(tmp, "\n")] = 0;
            strcpy(tt.id, process_name(tmp));
            //get age
            fscanf(fi, "%d", &tt.age);
            //get score
            fscanf(fi, "%f%f%f%f", &tt.math, &tt.phys, &tt.chem, &tt.elec);
            push(tt);
            q++;
            //clear remaining \n
            fgets(ss, sizeof(ss), fi);
            //print log to output
            fprintf(fo, "-> A student info was PUSHED to stack\n");
            fprintf(fo, "      Name : %s\n", tt.name);
            fprintf(fo, "      ID   : %s\n", tt.id);
            if(check_age(tt.age)) fprintf(fo, "      Age  : %d\n", tt.age);
            else fprintf(fo, "      Age  : %d (NOT VALID!)\n", tt.age);
            fprintf(fo, "      Math: %0.2f", tt.math);
            classify_score(tt.math);
            fprintf(fo, ", Physics: %0.2f", tt.phys);
            classify_score(tt.phys);
            fprintf(fo, ", Chemistry: %0.2f", tt.chem);
            classify_score(tt.chem);
            fprintf(fo, ", Electronic: %0.2f", tt.elec);
            classify_score(tt.elec);
            fprintf(fo, "\n\n");

        }
        else if(strcmp(tmp, "POP")==0 || strcmp(tmp, "Pop")==0)
        {
            tt = pop();
            if(!empty())
            {
                fprintf(fo, "<- A student info was POP out of stack\n");
                fprintf(fo, "      Name : %s\n", tt.name);
                fprintf(fo, "      ID   : %s\n", tt.id);
                if(check_age(tt.age)) fprintf(fo, "      Age  : %d\n", tt.age);
                else fprintf(fo, "      Age  : %d (NOT VALID!)\n", tt.age);
                fprintf(fo, "      Math: %0.2f", tt.math);
                classify_score(tt.math);
                fprintf(fo, ", Physics: %0.2f", tt.phys);
                classify_score(tt.phys);
                fprintf(fo, ", Chemistry: %0.2f", tt.chem);
                classify_score(tt.chem);
                fprintf(fo, ", Electronic: %0.2f", tt.elec);
                classify_score(tt.elec);
                fprintf(fo, "\n\n");
                q--;
            }
        }
        else if(strcmp(tmp, "PEEK")==0 || strcmp(tmp, "Peek")==0)
        {
            tt = peek();
            if(!empty())
            {
                fprintf(fo, "-- Info of TOP student on stack\n");
                fprintf(fo, "      Name : %s\n", tt.name);
                fprintf(fo, "      ID   : %s\n", tt.id);
                if(check_age(tt.age)) fprintf(fo, "      Age  : %d\n", tt.age);
                else fprintf(fo, "      Age  : %d (NOT VALID!)\n", tt.age);
                fprintf(fo, "      Math: %0.2f", tt.math);
                classify_score(tt.math);
                fprintf(fo, ", Physics: %0.2f", tt.phys);
                classify_score(tt.phys);
                fprintf(fo, ", Chemistry: %0.2f", tt.chem);
                classify_score(tt.chem);
                fprintf(fo, ", Electronic: %0.2f", tt.elec);
                classify_score(tt.elec);
                fprintf(fo, "\n\n");
            }
        }
        else if(strcmp(tmp, "EMPTY")==0 || strcmp(tmp, "Empty")==0)
        {
            if(empty()) fprintf(fo, "The stack is currently EMPTY\n\n");
            else if (q == 1) fprintf(fo, "There is currently 1 student left in stack\n\n");
            else fprintf(fo, "There are currently %d students left in stack\n\n", q);
        }
        else if(strstr(tmp, "SEARCH") !=NULL || strstr(tmp, "Search") != NULL)
        {
            int start_pos, found = 0;
            tmp_profile = profile;
            if(strstr(tmp, "NAME") !=NULL || strstr(tmp, "Name") != NULL)
            {
                start_pos = 12;
                strncpy(ss, tmp + start_pos, strlen(tmp) - start_pos);
                while(tmp_profile != NULL)
                {
                    if(strcmp(ss, tmp_profile->name) == 0)
                    {
                        found = 1;
                        fprintf(fo, "** Found \"%s\" in stack\n", tmp_profile->name);
                        fprintf(fo, "      Related info: id %s, age %d", tmp_profile->id, tmp_profile->age);
                        if(!check_age(tmp_profile->age)) fprintf(fo, " (NOT VALID)");
                        fprintf(fo, "\n      Math: %0.2f", tt.math);
                        classify_score(tt.math);
                        fprintf(fo, ", Physics: %0.2f", tt.phys);
                        classify_score(tt.phys);
                        fprintf(fo, ", Chemistry: %0.2f", tt.chem);
                        classify_score(tt.chem);
                        fprintf(fo, ", Electronic: %0.2f", tt.elec);
                        classify_score(tt.elec);
                        fprintf(fo, "\n\n");
                        break;
                    }
                    tmp_profile = tmp_profile->next;
                }
                if(!found) fprintf(fo, "** Currently in stack, there are no students name \"%s\"\n\n", ss);
            }
            else if(strstr(tmp, "ID") !=NULL || strstr(tmp, "Id") != NULL)
            {
                start_pos = 10;
                strncpy(ss, tmp + start_pos, strlen(tmp) - start_pos);
                while(tmp_profile != NULL)
                {
                    if(strcmp(ss, tmp_profile->id) == 0)
                    {
                        found = 1;
                        fprintf(fo, "** Found id \"%s\" in stack\n", tmp_profile->id);
                        fprintf(fo, "      Related info: name %s, age %d", tmp_profile->name, tmp_profile->age);
                        if(!check_age(tmp_profile->age)) fprintf(fo, " (NOT VALID)");
                        fprintf(fo, "\n      Math: %0.2f", tt.math);
                        classify_score(tt.math);
                        fprintf(fo, ", Physics: %0.2f", tt.phys);
                        classify_score(tt.phys);
                        fprintf(fo, ", Chemistry: %0.2f", tt.chem);
                        classify_score(tt.chem);
                        fprintf(fo, ", Electronic: %0.2f", tt.elec);
                        classify_score(tt.elec);
                        fprintf(fo, "\n\n");
                        break;
                    }
                    tmp_profile = tmp_profile->next;
                }
                if(!found) fprintf(fo, "** Currently in stack, there are no students with id \"%s\"\n\n", ss);
            }
            else fprintf(fo, "Unknown SEARCH command option!\n\n");
        }
        else fprintf(fo, "Invalid/Unknown command: \"%s\"\n\n", tmp);
    }
}

int isEND(char *s)
{
    if(strcmp(s, "end")==0 || strcmp(s, "END")==0 || s==NULL || feof(fi)) return 1;
    return 0;
}

int check_age(int x)
{
    if(0<=x && x<=122) return 1;
    else return 0;
}

int check_score(float x)
{
    if(0<=x && x<=10) return 1;
    else return 0;
}

void classify_score(float x)
{
    if (!check_score(x)) fprintf(fo, " (!)");
    else if(x < 5) fprintf(fo, " (F)");
    else fprintf(fo, " (P)");
}

void push(student x)
{
    student *tmp = (student*)calloc(1, sizeof(student));
    //clone data
    strcpy(tmp->name, x.name);
    strcpy(tmp->id, x.id);
    tmp->age = x.age;
    tmp->math = x.math;
    tmp->phys = x.phys;
    tmp->chem = x.chem;
    tmp->elec = x.elec;
    //push
    tmp->next = profile;
    profile = tmp;
}

student pop()
{
    student tmp_ret;
    if(profile == NULL || q==0)
        fprintf(fo, "Cannot POP! Dynamic stack is empty!\n");
    else
    {
        student *tmp;
        //pop
        tmp = profile;
        profile = profile->next;
        //clone data, pop node
        strcpy(tmp_ret.name, tmp->name);
        strcpy(tmp_ret.id, tmp->id);
        tmp_ret.age = tmp->age;
        tmp_ret.math = tmp->math;
        tmp_ret.phys = tmp->phys;
        tmp_ret.chem = tmp->chem;
        tmp_ret.elec = tmp->elec;
        tmp_ret.next = NULL;
        return tmp_ret;
    }
    return tmp_ret;
}

student peek()
{
    student tmp_ret;
    if(profile == NULL || q==0)
        fprintf(fo, "Cannot PEEK! Dynamic stack is empty!\n");
    else
    {
        student *tmp;
        tmp = profile;
        //clone data, peek node
        strcpy(tmp_ret.name, tmp->name);
        strcpy(tmp_ret.id, tmp->id);
        tmp_ret.age = tmp->age;
        tmp_ret.math = tmp->math;
        tmp_ret.phys = tmp->phys;
        tmp_ret.chem = tmp->chem;
        tmp_ret.elec = tmp->elec;
        tmp_ret.next = NULL;
        return tmp_ret;
    }
    return tmp_ret;
}

int empty()
{
    if(q == 0) return 1;
    else return 0;
}

char* process_name(char *s)
{
    int i, j, head, tail;
    head = 0; tail = strlen(s) - 1;
    char *name_tmp = (char*)calloc(tail + 1, sizeof(char));
    while(s[head]==' ') head++;
    while(s[tail]==' ') tail--;
    if(head >= tail) return name_tmp;
    i = head; j = 0;
    while(i <= tail)
    {
        name_tmp[j] = s[i];
        if(s[i] == ' ')
        {
            i++;
            while(s[i] == ' ') i++;
            if(s[i] >= 'a' && s[i] <= 'z') s[i] -= 32;
            i--;
        }
        i++;j++;
    }
    if(name_tmp[0] >= 'a' && name_tmp[0] <= 'z') name_tmp[0] -= 32;
    return name_tmp;
}

void print_welcome_screen()
{
    printf("|\n|    Name : Pham Thanh Nhan\n");
    printf("|    ID   : 1951083\n");
    printf("|    Class: TT01, EE2415\n|\n");
    printf("|*************************************************************|\n");
    printf("|      PROGRAM MANAGES LIST OF STUDENTS BY DYNAMIC STACK      |\n");
    printf("|                                                             |\n");
    printf("|      Support operations: POP, PUSH, SEARCH, PEEK, EMPTY     |\n");
    printf("|                                                             |\n");
    printf("|   Program will be stopped by END string on INPUT file       |\n");
    printf("|   Data and commands will be loaded from .INP file           |\n");
    printf("|   Result will be written to .OUT file                       |\n");
    printf("|_____________________________________________________________|\n");
    printf("|          OPERATION USAGE AND FORMAT ON .INP FILE            |\n");
    printf("|                                                             |\n");
    printf("|   POP   : just type \"pop\" on new line to pop                |\n");
    printf("|                                                             |\n");
    printf("|   PUSH  : type PUSH and provide next 4 lines of student     |\n");
    printf("|   |       information to push to stack                      |\n");
    printf("|   |                                                         |\n");
    printf("|   ->syntax: PUSH               //operation                  |\n");
    printf("|   |         Nguyen van    A    //name                       |\n");
    printf("|   |         1951234            //id                         |\n");
    printf("|   |         20                 //age                        |\n");
    printf("|   |         8.3 7.5 6.4 7.3    //score                      |\n");
    printf("|   ->Math, Physics, Chemistry, Electronic                    |\n");
    printf("|                                                             |\n");
    printf("|   SEARCH: search for a student from input name of id        |\n");
    printf("|   |                                                         |\n");
    printf("|   ->syntax: SEARCH name Nguyen Van A                        |\n");
    printf("|             SEARCH id 1951234                               |\n");
    printf("|                                                             |\n");
    printf("|   PEEK  : return top student of stack                       |\n");
    printf("|                                                             |\n");
    printf("|   EMPTY : return result if list of student is empty or not  |\n");
    printf("|_____________________________________________________________|\n");
    printf("\n  To load EXAMPLE INPUT,     ENTER 0 (default option)\n");
    printf("  To load USER INPUT,        ENTER 1\n");
    printf("  To load your CUSTOM INPUT, ENTER 2\n");
    printf("  To reset EXAMPLE INPUT,    ENTER r\n");
}

void process_file()
{
    printf("\n    -> Your choice: ");
    scanf("%c", &choice); fgets(tmp, sizeof(tmp), stdin);
    if(choice == '1') strcpy(tmp, USER_INPUT);
    else if(choice == '2')
    {
        printf("\n  Example of custom input name: Abc.txt or D:\\Test\\myinput.txt\n\n");
        do
        {
            printf("    -> Enter your custom input name or location: ");
            fgets(custom_input, sizeof(custom_input), stdin);
            custom_input[strcspn(custom_input, "\n")] = 0;
            strcpy(tmp, custom_input);
            if(strcmp(tmp, "r")==0) goto reset_input;
            fi = fopen(tmp, "r");
            if(fi == NULL)
            {
                printf("\n    !! Cannot find your custom input file.\n");
                printf("       Please check your spelling and directory.\n");
                printf("       You can enter \"r\" to load default file.\n\n\n");
            }
        } while(fi == NULL);


    }
    else if(choice == 'r' || choice == 'R')
    {
        reset_input:
        regenerate_example_input();
        printf("\n    -> EXAMPLE INPUT file regenerated! EXAMPLE INPUT was loaded by default.\n");
        strcpy(tmp, EXAMPLE_INPUT);
    }
    else
    {
        if(choice != '0') printf("\n    -> Invalid input! EXAMPLE INPUT was loaded by default.\n");
        strcpy(tmp, EXAMPLE_INPUT);
    }
    //---------------------------------------------------------------------------------------------
    fi = fopen(tmp, "r");
    if(fi == NULL)
    {
        printf("\n    -> No INPUT file was found!");
        strcpy(tmp, USER_INPUT);
        fi = fopen(tmp, "w");
        fclose(fi);
        regenerate_example_input();
        printf("\n    -> An EXAMPLE INPUT was generated and loaded by default.\n");
        strcpy(tmp, EXAMPLE_INPUT);
        fi = fopen(tmp, "r");
    }
    printf("_______________________________________________________________\n");
    printf("\n  Program is reading from \"%s\" file.\n", tmp);
    //---------------------------------------------------------------------------------------------
    strcpy(tmp, USER_OUTPUT);
    fo = fopen(tmp, "w");
    process_data();              //start processing input and output data
    //---------------------------------------------------------------------------------------------
    fclose(fi);
    fclose(fo);
    free(profile);
    free(tmp_profile);
    strcpy(tmp, USER_OUTPUT);
    printf("\n  Finished writing to file.\n    -> Please check \"%s\" for detailed result.\n", tmp);
}

void regenerate_example_input()
{
    strcpy(tmp, EXAMPLE_INPUT);
    fi = fopen(tmp, "w");
    fputs("PUSH\n", fi);
    fputs("Nguyen van   A\n", fi);
    fputs("1951234\n", fi);
    fputs("20\n", fi);
    fputs("8.3 7.5 6.4 7.3\n", fi);
    fputs("PUSH\n", fi);
    fputs("Pham   Thi huong\n", fi);
    fputs("1851096\n", fi);
    fputs("21\n", fi);
    fputs("7.9 9.3 5.3 6.5\n", fi);
    fputs("PUSH\n", fi);
    fputs("La   thanh    nghia\n", fi);
    fputs("1951009\n", fi);
    fputs("20\n", fi);
    fputs("8.9 5.3 8.3 5.8\n", fi);
    fputs("PUSH\n", fi);
    fputs("   Tran Thanh        tam\n", fi);
    fputs("1752005\n", fi);
    fputs("22\n", fi);
    fputs("7.2 4.6 9.2 8.2\n", fi);
    fputs("PEEK\n", fi);
    fputs("SEARCH name pham thi huong\n", fi);
    fputs("SEARCH id 1752005\n", fi);
    fputs("POP\n", fi);
    fputs("EMPTY\n", fi);
    fputs("POP\n", fi);
    fputs("POP\n", fi);
    fputs("EMPTY\n", fi);
    fputs("END", fi);
    fclose(fi);
}
