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

void inspectString(char* s){
  char nextChar;
  int charToInt;
  int i=0;
  while(nextChar=s[i]!='\0'){
    printf(" %d", (int) nextChar);
  }
}


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


void printArray(char* arrayname, char** arrayToPrint, int arrayLength){
  for(int i=0; i<arrayLength; i++){
    printf("\n%s[%d] = %s", arrayname, i, arrayToPrint[i]);
  }
}



int main(int argc, char* const argv[]){




  printf("\n\n\nWelcome to my very limited shell!\n\n\n");

  int parentPid = getpid(); //Process id of parent


  while(1){
    int pipeFileDescriptors[2]; // file descriptors for redirecting output of "which"

    //create pipe for redirecting output
    if(pipe(pipeFileDescriptors)==-1){
      perror("pipe");
    }


    printf("\n\n\nPrompt>> ");
    char buffer[1000];

    char *command = fgets(buffer, sizeof(buffer), stdin);

    // printf("\n\n\nRead the following:   %s", command);
    char* tokens[MAXARGS];
    tokens[0] = argv[0];
    int numTokens = tokenize(tokens, command);
    printArray("tokens", tokens, numTokens);
    // inspectString(tokens[1]);
    // cleanString(tokens[1]);



    // printf("\n\n\nTime to tokenize!");
    // printf("\n\n\nnumTokens: %d", numTokens);
    for(int i=0; i<numTokens; i++){
        cleanString(tokens[i]);
    }





    //Uncomment...
    printf("\n\n\n\n\n\nchecking character by character.\n\n\n");
    //
    // for(int i=0; i<4; i++){
    //   printf("\nchar in argv: %c", argv[1][i]);
    //   printf("\nchar in tokens: %c", tokens[1][i]);
    // }

    // char* test[2];
    // test[0] = argv[0];
    // test[1] = "man\0";
    // test[2] = NULL;
    //
    // cleanString(tokens[1]);
    // int w = strcmp(tokens[1], argv[1]);
    // printf("\n\n\ncompare: %d", w);
    // tokens[1] = argv[1];
    // tokens[0] = argv[0];



    execv("/usr/bin/which", tokens);
    //Uncomment...

    // pid_t forkResult = fork(); //execv will be executed by children

  }

  //ensuring that parent doesnt enter this section
  // if(forkResult==0){
  //   //redirecting output of "which"
  //   close(pipeFileDescriptors[0]);
  //   dup2(pipeFileDescriptors[1], STDOUT_FILENO);
  //   close(pipeFileDescriptors[1]);
  //   // printf("\n\n\nargArray[0]:   %s", argArray[0]);
  //   // printf("\n\n\nargArray[1]:   %s", argArray[1]);
  //   // printf("\n\n\nargArray[2]:   %s", argArray[2]);
  //   execv("/usr/bin/which", tokens);
  //
  // }
  //
  //
  //   if(execv(readBuff, tokens)==-1){ // execute the program chosen by the user
  //     // perror("execv");
  //   };
  //
  //   // wait for prior command to finish before prompting new command
  //   waitReturnVal =  wait(&wstatus);
  //
  // }
  // // wait for prior command to finish before prompting new command
  // waitReturnVal =  wait(&wstatus);
  // // int pid = getpid();
  // printf("\n\n\nLatest Pid: %d", pid);
  // printf("\n\n\nParent Pid: %d", parentPid);
  // proceed++;

}
