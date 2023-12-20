#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define UPPERBOUND 1000
#define MAXARGS 100


int wstatus; //allocating an int neccesary for wait()
pid_t waitReturnVal; //For storing the return of wait()
char readBuff[1000]; //Storing the output of "which"-system call
char * argArray[MAXARGS]; //arguments into execv()
int pipeFileDescriptors[2]; //for pipe
int wstatus1; // waiting for children
int wstatus2;  // waiting for children


// String processing to comply with execv()
//Arg1: string to clean, most likely argument into program that the user wants to execute
// Return: String in a format that execv() expects
void cleanString(char* input){
  int j=0;
  while(j<100){
    if(((int) input[j])==10){
      input[j] = 0;
      return;
    }
    j++;
  }
  return;
}

//Inspect String
// Arg1: String that user wants to inspect and see true ascii values one-by-one
// return: void
void inspectString(char* s){
  char nextChar;
  int charToInt;
  int i=0;
  while(nextChar=s[i]!='\0'){
    printf(" %d", (int) nextChar);
  }
}


//Tokenize
//Arg1: Array of pointers into which the tokens are stored
// Arg2: String provided by the user
// Return: The number of arguments provided by the user, int.
int tokenize(char** tokens, char *input){
  char delim[] = " ";
  char *nextToken = strtok(input,delim);
  tokens[1] = nextToken;

  int h=2;
  while( (tokens[h]=strtok(NULL,delim))!=NULL ){
    cleanString(tokens[h]);
    h++;
  }
  return h;
}

//Print content of string array
// Arg1: Name of array, string
//Arg2: Actual array of strings
// Arg3: Length of provided array
void printArray(char* arrayname, char** arrayToPrint, int arrayLength){
  for(int i=0; i<arrayLength; i++){
    printf("\n%s[%d] = %s", arrayname, i, arrayToPrint[i]);
  }
}



int main(int argc, char* const argv[]){

  printf("\n\n\nWelcome to my very limited shell!\n\n\n");
  printf("\n\n\nTo exit, press ctrl-c\n\n\n");


  int parentPid = getpid(); //Process id of parent

  while(1){
    int pipeFileDescriptors[2]; // file descriptors for redirecting output of "which"

    printf("\n\n\nPrompt>> ");

    char buffer[1000]; // For storing user input

    char *command = fgets(buffer, sizeof(buffer), stdin);

    char* tokens[MAXARGS]; //Pass into tokenize
    tokens[0] = argv[0]; //To comply with execv()
    int numTokens = tokenize(tokens, command);

    //Make all tokens compatible with execv()
    for(int i=0; i<numTokens; i++){
        cleanString(tokens[i]);
    }

    //Arg into "which" syscall
    char* firstArgs[2];
    firstArgs[0] = argv[0];
    firstArgs[1] = tokens[1];
    firstArgs[2] =  NULL;

    //create pipe for redirecting output
    if(pipe(pipeFileDescriptors)==-1){
      perror("pipe");
    }

    // First child
    int fork1 =  fork();


    if(fork1==0){
      //Pass output of which to second child that then executes program
      close(pipeFileDescriptors[0]);
      dup2(pipeFileDescriptors[1], STDOUT_FILENO);
      close(pipeFileDescriptors[1]);
      execv("/usr/bin/which", firstArgs);
    }else{
      waitReturnVal = wait(&wstatus1); // Wait until which returns
      int fork2 = fork();
      if(fork2==0){
        //Execute actual program with user arguments
        close(pipeFileDescriptors[1]);
        char buff[1000];
        int wordLen = read(pipeFileDescriptors[0], buff, sizeof(buff));
        buff[wordLen-1] = '\0';
        close(pipeFileDescriptors[0]);
        char* args2[numTokens+1];
        args2[0] = argv[0];
        for(int j=2; j<=numTokens;j++){
          args2[j-1] = tokens[j];
        }
        // printArray("args2", args2, numTokens);
        if(execv(buff,args2)==-1){
          perror("execv");
        }
      }
      int waitReturnVal = wait(&wstatus2);
    }
    //Continue to loop
  }
}
