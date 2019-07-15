#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define _GNU_SOURCE
#define bool _Bool
#define main main_ignore
#include "Queens.c"
#undef main

static uint8_t testsPassed;
static volatile sig_atomic_t testStatus;
static uint8_t disable_exit_handler;
jmp_buf test_crash;
int test_count;

    // static int test_count;

    // static boolean verbose;
#define FIRST_TEST printBoard_test
#define MAXSCORE 25
#define PNTSPERTEST 5
#define BREAKER_TEST_RANGE 20000
const char origtempstr[20*BREAKER_TEST_RANGE];
enum Test_e {
    printBoard_test = 0,
    placeQueen_test,
    removeQueen_test,
    findSolution_test,
    baseCase_test,

  NUM_TESTS,
};

char* strtrim(char *str) {
   // printf("4444:%s\n",str);
    char *ptr = malloc((strlen(str)+1)*sizeof(char));
    int i,j=0;
    for(i=0;str[i]!='\0';i++)
    {
        if (!isspace(str[i]) && str[i] != '\t'&&str[i] != '\n') 
        ptr[j++]=str[i];
    } 
    ptr[j]='\0';
   
     return ptr;
 
}
char *testName(int test) {

    if(test == printBoard_test)
        return "printBoard_test";
    if(test == placeQueen_test)
        return "placeQueen_test";
    if(test == removeQueen_test)
        return "removeQueen_test";
    if(test == findSolution_test)
        return "findSolution_test";
    if(test == baseCase_test)
        return "baseCase_test";

    return "";
}

    // static boolean strcmp(int[] a, String b) {
    //     return (Queens.setToString(a)).compareTo(b) == 0;
    // }
// bool strcmpn(int ss[], char* s,int n) {
//         //char* temp = NULL;
//         //itoa()
       
//         int SIZE = 500;

//         char buffer[SIZE];
//         memset( buffer, '\0', sizeof(char)*SIZE );
//         //char *buffer = NULL;
//         //fflush(stdout);
//         // freopen("/dev/null", "a", stdout);
//         // setbuf(stdout, buffer);
//         // printSet(ss,  n);
//         // freopen ("/dev/tty", "a", stdout);
//         //printSet(ss,  n);
//         int stdout_save = dup(STDOUT_FILENO); //save the stdout state
//         freopen("NUL", "a", stdout); //redirect stdout to null pointer
//         setvbuf(stdout, buffer, _IOFBF, 1024); //set buffer to stdout
//         printSet(ss,  n);
//         freopen("NUL", "a", stdout); //redirect stdout to null again
//         dup2(stdout_save, STDOUT_FILENO); //restore the previous state of stdout
//         setvbuf(stdout, NULL, _IONBF, 1024); //disable buffer to print to screen instantly
//            //printf("22222\n");
//         //fflush(stdout);
//        char *buffer_trim =strtrim(buffer);
//        //printf("333 %s:%s\n", buffer,s);
//        char *s_trim = strtrim(s);
//        // printf("333 %s:%s\n", buffer_trim,s_trim );
//         if(strcmp(buffer_trim,s_trim) == 0)
//             return 1;
//          //printf("your result: %s",buffer);
//          //printf("correct: %s",s);
//         return 0;

// }

void printArrayNew(int** B,int n) {
       // char* result;
        printf("{");
        for(int i=0;i<=n;i++) {
            printf("{");
            for(int j=0;j<=n;j++) {
                if(j!=n)
                    printf("%d,",B[i][j]);
                else {
                    printf("%d",B[i][j]);
                }
            }
            printf("}");
            if(i!=n)
                printf(",");
        }
        printf("}\n");
        //return result;
}

  

int compareFiles(FILE *fp1, FILE *fp2) 
{ 
    char * line1 = NULL;
    size_t len1 = 0;
    char * line2 = NULL;
    size_t len2 = 0;
    ssize_t read1;
    ssize_t read2;
    if (fp1 == NULL)
        exit(EXIT_FAILURE);
    if (fp2 == NULL)
        exit(EXIT_FAILURE);

    while ((read1 = getline(&line1, &len1, fp1)) != -1&&(read2 = getline(&line2, &len2, fp2)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        char *trim1 =strtrim(line1);
       //printf("333 %s:%s\n", buffer,s);
       char *trim2 = strtrim(line2);
       // printf("333 %s:%s\n", buffer_trim,s_trim );
        if(strcmp(trim1,trim2) != 0)
            return 1;
      
        
    }

    while ((read1 = getline(&line1, &len1, fp1)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
            return 1;
         //printf("your result: %s",buffer);
         //printf("correct: %s",s);     
        
    }
    while ((read2 = getline(&line2, &len2, fp2)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
            return 1;
         //printf("your result: %s",buffer);
         //printf("correct: %s",s);     
        
    }
    free(line1);
    free(line2);
    return 0;
} 
int CheckResult(char* file1, char* file2) {
    FILE *fp1 = fopen(file1, "r"); 
    FILE *fp2 = fopen(file2, "r"); 
  
    if (fp1 == NULL || fp2 == NULL) 
    { 
       printf("Error : Files not open\n"); 
       return 255;
    }
   int status =  compareFiles(fp1, fp2);
    fclose(fp1);
    fclose(fp2);

    return status;

}
void segfault_handler(int signal) { // everyone knows what this is
  testStatus = 255;
  longjmp(test_crash, 1);
}

void exit_attempt_handler(void) { // only I decide when you are done
  if (disable_exit_handler) return; // allow this to be disabled
  testStatus = 255;
  longjmp(test_crash, 2);
}

void abrupt_termination_handler(int signal) { // program killed externally
  testStatus = 255;
  longjmp(test_crash, 3);
}
//at least I got rid of the goto
void end_program(uint8_t argc) {
  disable_exit_handler = 1;

  uint8_t totalScore = (MAXSCORE - NUM_TESTS * PNTSPERTEST) +
    testsPassed * PNTSPERTEST;
  if (testStatus == 255||totalScore==0) totalScore = 10;
  if (testStatus != 255 && argc == 2) {
    printf("\nYou passed %d out of %d tests\n", testsPassed, NUM_TESTS);
  } else if (argc == 2) {
    printf("\nRecieving charity points for a premature program end\n");
  }

  printf("\nYou will receive %d out of %d possible points on the Hashtable Dictionary\n\n",
      totalScore, MAXSCORE);

  exit(0);
}

int runTest(int test) {

    if (test == printBoard_test) {
                //test1 print board
        int test_status = -1;
        int A1[5][5]= {{0,0,0,0,0},{2,0,1,0,0},{4,-1,-1,-1,1},{1,1,-1,-1,-2},{3,-1,-3,1,-1}};
        int *B1[5];
        for(int i=0;i<5;i++)              
                B1[i] = (int *)A1[i]; 
        char *name = "unit-out1.txt";
        // int fd = open(name, O_RDWR | O_CREAT, 0666);//S_IRUSR | S_IWUSR
        // int stdout_fd = dup(1);//STDOUT_FILENO)
        // dup2(fd, 1);
        // printBoard(B1,4);
        // fflush(stdout);
        // dup2(stdout_fd, 1);
        // close(fd);
        // //close(stdout_fd);
        // clearerr(stdout);
        freopen(name,"a",stdout);
        printBoard(B1,4);
        freopen("/dev/tty", "a", stdout);


                //check if outputs are the same
        test_status = CheckResult("modelunit-out1.txt", "unit-out1.txt");
        return test_status;

    } else if (test == placeQueen_test) {
                //test2 place queen
                //two possible result since students may update all the board, in the instruction Pat only update cells below the current rows
        int test_status = -1;
        int  A2[5][5]= {{0,0,0,0,0},{2,0,1,0,0},{4,-1,-1,-1,1},{0,0,-1,-1,-2},{0,0,-2,0,-1}};
        int *B2[5];
          for(int i=0;i<5;i++)              
                B2[i] = (int *)A2[i];          
                // PrintStream o2 = new PrintStream(new File("unit-out2.txt")); 
                // System.setOut(o2);
                // Queens.placeQueen(B2,3,1);   
                // System.out.println(printArray(B2));
                // o2.close();
                // System.setOut(new PrintStream(new FileOutputStream(FileDescriptor.out)));

        char *name = "unit-out2.txt";
        // int fd1 = open(name, O_RDWR | O_CREAT, 0666);//S_IRUSR | S_IWUSR
        // int stdout_fd1 = dup(1);//STDOUT_FILENO)
        // dup2(fd1, 1);

        // placeQueen(B2,4,3,1);
        // printArrayNew(B2,4);

        // fflush(stdout);
        // dup2(stdout_fd1, 1);
        // close(fd1);
        // //close(stdout_fd1);
        // clearerr(stdout);
        freopen(name,"a",stdout);
        placeQueen(B2,4,3,1);
        printArrayNew(B2,4);
        freopen("/dev/tty", "a", stdout);

        test_status = CheckResult("modelunit-out2-1.txt", "unit-out2.txt");
        if (test_status == 1) {
            test_status = CheckResult("modelunit-out2-2.txt", "unit-out2.txt");
        } 

            return test_status;

    } else if (test == removeQueen_test) {
                //test3 remove queen
        int test_status = -1;
        int  A3[5][5]= {{0,0,0,0,0},{2,0,1,0,0},{4,-1,-1,-1,1},{1,1,-1,-1,-2},{3,-1,-3,1,-1}};
        int *B3[5];
          for(int i=0;i<5;i++)              
                B3[i] = (int *)A3[i];       
                // PrintStream o3 = new PrintStream(new File("unit-out3.txt")); 
                // System.setOut(o3);
                // Queens.removeQueen( B3, 4 , 3);
                // System.out.println(B3[4][3]+":"+B3[4][0]);
                // o3.close();
                // System.setOut(new PrintStream(new FileOutputStream(FileDescriptor.out)));

        char *name = "unit-out3.txt";
        // int fd2 = open(name, O_RDWR | O_CREAT, 0666);//S_IRUSR | S_IWUSR
        // int stdout_fd2 = dup(1);//STDOUT_FILENO)
        // dup2(fd2, 1);

        // removeQueen( B3, 4, 4 , 3);
        // printf("%d:%d\n",B3[4][3],B3[4][0]);

        // fflush(stdout);
        // dup2(stdout_fd2, 1);
        // close(fd2);
        // //close(stdout_fd2);
        // clearerr(stdout);

        freopen(name,"a",stdout);
        removeQueen( B3, 4, 4 , 3);
        printf("%d:%d\n",B3[4][3],B3[4][0]);

        freopen("/dev/tty", "a", stdout);

        test_status = CheckResult("modelunit-out3.txt", "unit-out3.txt");
        return test_status;

    } else if (test == findSolution_test) {
                //test4 findsolution 
        int test_status = -1;
        int  A4[5][5]= {{0,0,0,0,0},{2,0,1,0,0},{4,-1,-1,-1,1},{1,1,-1,-1,-2},{0,-1,-3,0,-1}};
        int *B4[5] ;
          for(int i=0;i<5;i++)              
                B4[i] = (int *)A4[i]; 
                // PrintStream o4 = new PrintStream(new File("unit-out4.txt")); 
                // System.setOut(o4);
                
                // System.out.println(Queens.findSolutions( B4, 4 , "")); //don't use verbose since some student will use -v some use verbose
                // o4.close();
                // System.setOut(new PrintStream(new FileOutputStream(FileDescriptor.out)));

        char *name = "unit-out4.txt";
        // int fd3 = open(name, O_RDWR | O_CREAT, 0666);//S_IRUSR | S_IWUSR
        // int stdout_fd3 = dup(1);//STDOUT_FILENO)
        // dup2(fd3, 1);

        // printf("%d\n",findSolutions( B4, 4, 4 , "")); //don't use verbose since some student will use -v some use verbose

        // fflush(stdout);
        // dup2(stdout_fd3, 1);
        // close(fd3);
        // //close(stdout_fd3);
        // clearerr(stdout);
        freopen(name,"a",stdout);
        printf("%d\n",findSolutions( B4, 4, 4 , ""));

        freopen("/dev/tty", "a", stdout);


        test_status = CheckResult("modelunit-out4.txt", "unit-out4.txt");
        return test_status;

    } else if (test == baseCase_test) {
                //test5 base case  print nothing
        int test_status = -1;
        int  A5[5][5]= {{0,0,0,0,0},{2,0,1,0,0},{0,-1,-1,-1,0},{0,0,-1,0,-1},{0,0,-1,0,0}};
        int *B5[5] ;
        for(int i=0;i<5;i++)              
                B5[i] = (int *)A5[i]; 
                // PrintStream o5 = new PrintStream(new File("unit-out5.txt"));
                // System.setOut(o5);
                // System.out.println(Queens.findSolutions( B5, 2 , ""));
                // o5.close(); 
                // System.setOut(new PrintStream(new FileOutputStream(FileDescriptor.out)));

        char *name = "unit-out5.txt";
        // int fd4 = open(name, O_RDWR | O_CREAT, 0666);//S_IRUSR | S_IWUSR
        // int stdout_fd4 = dup(1);//STDOUT_FILENO)
        // dup2(fd4, 1);

        // printf("%d\n",findSolutions( B5, 4, 2 , ""));

        // fflush(stdout);
        // dup2(stdout_fd4, 1);
        // close(fd4);
        // //close(stdout_fd4);
        // clearerr(stdout);

        freopen(name,"a",stdout);
        printf("%d\n",findSolutions( B5, 4, 2 , ""));

        freopen("/dev/tty", "a", stdout);
        test_status = CheckResult("modelunit-out5.txt", "unit-out5.txt");
        return test_status;
    }
        return 0;
}   

int main(int argc, char* argv[]) {

        if (argc > 2 || (argc == 2 && strcmp(argv[1],"-v")!=0)) {
            printf("Usage: %s [-v]", (argc > 0 ? argv[0] : "./ModelQueensTest"));
            exit(1);
        }
        // printf("\n"); // more spacing
        // if (argc == 2) printf("\n"); 
        test_count = 0;  
        //int test_status = 0;
        disable_exit_handler = 0;
        testsPassed = 0;
        atexit(exit_attempt_handler);
        signal(SIGSEGV, segfault_handler);

        for (uint8_t i = FIRST_TEST; i < NUM_TESTS; i++) {

            uint8_t fail_type = setjmp(test_crash);
            if (fail_type != 0) goto fail_jmp;
            testStatus = runTest(i);
            fail_jmp:
            if (argc == 2) { // it's verbose mode
                // char *name = "QueensTest-out.txt";
                // int fd = open(name, O_CREAT |O_APPEND, 0666);
                // int stdout_fd = dup(1);//STDOUT_FILENO)
                // dup2(fd, 1);
                 printf("Test %s %s", testName(i),testStatus == 0 ? "PASSED" : "FAILED");
                if (testStatus == 255) {
                    printf(": due to a %s\n", fail_type == 1 ? "segfault" : fail_type == 2 ?"program exit" : "program interruption");
                    printf("\nWARNING: Program will now stop running tests\n\n");
                    end_program(argc);

                } else if (testStatus == 254) {
                    printf(": undefined test\n");
                } else if (testStatus != 0) {
                    printf(": test %d\n", testStatus);
                } else {
                    printf("\n");
                }
            }


            if (testStatus == 0) {
                testsPassed++;
            }
        }

        end_program(argc);
        return 0;
}
