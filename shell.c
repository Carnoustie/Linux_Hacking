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
int pipeFileDescriptors[2];
int wstatus1;
int wstatus2;
int wstatus3;


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
  printf("\n\n\nTo exit, press ctrl-c\n\n\n");


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
    // printArray("tokens", tokens, numTokens);
    // inspectString(tokens[1]);
    // cleanString(tokens[1]);


    // printf("\n\n\nTime to tokenize!");
    // printf("\n\n\nnumTokens: %d", numTokens);
    for(int i=0; i<numTokens; i++){
        cleanString(tokens[i]);
    }

    char* firstArgs[2];
    firstArgs[0] = argv[0];
    firstArgs[1] = tokens[1];
    firstArgs[2] =  NULL;

    if(pipe(pipeFileDescriptors)==-1){
      perror("pipe");
    }

    int fork1 =  fork();

    if(fork1==0){
      close(pipeFileDescriptors[0]);
      dup2(pipeFileDescriptors[1], STDOUT_FILENO);
      close(pipeFileDescriptors[1]);
      execv("/usr/bin/which", firstArgs);
    }else{
      waitReturnVal = wait(&wstatus1);
      int fork2 = fork();
      if(fork2==0){
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
