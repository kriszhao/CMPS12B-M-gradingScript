//ModelDictionaryTest.c
#define _GNU_SOURCE
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

#define bool _Bool
//#define main main_ignore
#include "IntegerQueue.h"
//#undef main

static uint8_t testsPassed;
static volatile sig_atomic_t testStatus;
static uint8_t disable_exit_handler;
jmp_buf test_crash;
int test_count;

    // static int test_count;

    // static bool verbose;
#define FIRST_TEST isEmpty_test1
#define MAXSCORE 90
#define PNTSPERTEST 10
#define BREAKER_TEST_RANGE 1000
const char origtempstr[20*BREAKER_TEST_RANGE];
enum Test_e {
     
    isEmpty_test1 = 0,
    size_test1,
    size_test2,
    peek_test1,
    peek_test2,
    toString_test1,
    toString_test2,
    equals_test1
    exception_test1,

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

    if (test == isEmpty_test1) return "isEmpty_test1";
    if (test == size_test1) return "size_test1";
    if (test == size_test2) return "size_test2";
    if (test == peek_test1) return "peek_test1";
    if (test == peek_test2) return "peek_test2";
    if (test == toString_test1) return "toString_test1";
    if (test == toString_test2) return "toString_test2";
    if (test == equals_test1) return "equals_test1";
    if (test == exception_test1) return "exception_test1";

    return "";
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
  if (testStatus == 255||totalScore==0) totalScore = 5;
  if (testStatus != 255 && argc == 2) {
    printf("\nYou passed %d out of %d tests\n", testsPassed, NUM_TESTS);
  } else if (argc == 2) {
    printf("\nRecieving charity points for a premature program end\n");
  }

  printf("\nYou will receive %d out of %d possible points on the Hashtable Dictionary\n\n",
      totalScore, MAXSCORE);

  exit(0);
}

// char* newlookup(Dictionary D, char* k) {
//  if (!D) {
//      printErr(NULL_REF_ERR, "lookup()"); 
//      exit(EXIT_FAILURE); 
//  }
    
//  Node N; 
//  for (N=D->head; N!=NULL; N=N->next) {
//      if (strcmp(N->key, k) == 0) { return N->value; }
//  }
//  // associated k-v pair not found 
//  return NULL;
// }

int runTest(int test) {

   IntegerQueue A = newIntegerQueue();
   IntegerQueue B = newIntegerQueue();
   // final int BREAKER_TEST_RANGE = 1000;

   
      if(test == isEmpty_test1) {
        if (length(A)!=0 || length(B)!=0) return 1;
        enqueue(A, 5);
        if (length(A)==0 || length(B)!=0) return 2;
      } else if(test == size_test1) {
        if (length(A) != length(B)) return 1;
        enqueue(A, 5);
        if (length(A) == length(B)) return 2;
        enqueue(B, 5);
        if (length(A) != length(B) ) return 3;
      } else if(test == size_test2) {
        for (int i = 0; i < BREAKER_TEST_RANGE; i++) {
            enqueue(A, i);
        }
        for (int i = 0; i < BREAKER_TEST_RANGE / 10; i++) {
            enqueue(B, i);
        }
        if (length(A)  != BREAKER_TEST_RANGE || length(B)  != BREAKER_TEST_RANGE / 10) return 1;
        for (int i = BREAKER_TEST_RANGE / 10; i < BREAKER_TEST_RANGE; i++) {
            enqueue(B, i);
        }
        if (length(A)  != length(B) ) return 2;
        for (int i = BREAKER_TEST_RANGE * 9 / 10; i < BREAKER_TEST_RANGE; i++) {
          dequeue(A);
        }
        if (length(A)  != BREAKER_TEST_RANGE * 9 / 10 || length(B)  != BREAKER_TEST_RANGE) return 3;
        for (int i = BREAKER_TEST_RANGE * 9 / 10 - 1; i >= 0; i--) {
          dequeue(A);
        }
        dequeueAll(B);
        if (length(A)  != 0 || length(B)  != 0) return 4;
      } else if(test == peek_test1) {
            enqueue(A, 1);
        if (peek(A) != 1) return 1;
            enqueue(B, 123);
        if (peek(B) != 123) return 2;
      } else if(test == peek_test2) {

        for (int i = 0; i < BREAKER_TEST_RANGE; i++) {
            enqueue(A, i);
        }
        for (int i = 0; i < BREAKER_TEST_RANGE / 10; i++) {
          enqueue(B, i);
        }
        for (int i = 0; i < BREAKER_TEST_RANGE / 10; i++) {
        
            dequeue(B);
            if (peek(B) != (i+1)%(BREAKER_TEST_RANGE / 10)) return 1;
            enqueue(B, i);
          if (length(B)!= BREAKER_TEST_RANGE / 10) return 2;
        }

      } else if(test == toString_test1) {
        if (strcmp(IntegerQueueToString(A),IntegerQueueToString(B))!=0) return 1;

      } else if(test == toString_test2) {
        enqueue(A, 5);
        enqueue(A, 3);
        enqueue(A, 9);
        enqueue(A, 7);
        enqueue(A, 8);
        if (strcmp(IntegerQueueToString(A),IntegerQueueToString(B))==0) return 1;
      } else if(test == equals_test1) {
        enqueue(A, 5);
        enqueue(A, 3);
        enqueue(A, 9);
        enqueue(A, 7);
        enqueue(A, 8);
        dequeue(A);
        enqueue(B, 3);
        enqueue(B, 9);
        enqueue(B, 7);
        enqueue(B, 8);
        if (equals(A,B)==0) return 1;
      } else if(test == exception_test1) {
        bool failed = false;
        
        
      
        atexit(exit_attempt_handler);
        signal(SIGSEGV, segfault_handler);
        
        uint8_t fail_type = setjmp(test_crash);
            

        if (fail_type != 0) goto fail_jmp;
        if (fail_type == 0) goto fail_jmp1;
        enqueue(B, 8);
        enqueue(B, 8);
      
        //printf("%d\n",size(A));
        //if(value==0)
        fail_jmp:
        failed = true;
        //printf("111111\n");
        if (failed) return 2;
        fail_jmp1:
        failed = false;     
       
       //printf("2222\n");
        //printf("%d\n",size(A));
        failed = true;
        for (int i = 0; i < BREAKER_TEST_RANGE; i++) {
         enqueue(A, 8);
        }
        //printf("ssss\n");
        // for (int i = BREAKER_TEST_RANGE - 1; i >= 0; i--) {
        uint8_t fail_type1 = setjmp(test_crash);
        if (fail_type1 != 0) goto fail_jmp3;
        if (fail_type1 == 0) goto fail_jmp4;
          dequeue(B);
         
          dequeue(B);
          
        fail_jmp3:
          failed = true;
          if (failed) return 3;
        fail_jmp4:
            failed = false;  
          return 0;        //}
      }
    
    return 0;
  }
  int main(int argc, char* argv[]) {
    test_count = 0;  
        //int test_status = 0;
        disable_exit_handler = 0;
        testsPassed = 0;
        atexit(exit_attempt_handler);
        signal(SIGSEGV, segfault_handler);
        printf("\nList of tests passed/failed:\n");
        for (uint8_t i = FIRST_TEST; i < NUM_TESTS; i++) {

            uint8_t fail_type = setjmp(test_crash);
            if (fail_type != 0) goto fail_jmp;
            testStatus = runTest(i);
            fail_jmp:
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

            if (testStatus == 0) {
                testsPassed++;
            }

        }
        printf("passed %d\n",testsPassed);
        end_program(argc);
        return 0;



  }