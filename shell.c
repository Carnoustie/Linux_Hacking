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


int tokenize(char** tokens, char *input){
  char delim[] = " ";
  char *nextToken = strtok(input,delim);;
  tokens[0] = nextToken;

  int h=1;
  while( (tokens[h]=strtok(NULL,delim))!=NULL ){
    h++;
  }
  return h;
}


void printArray(char* arrayname, char* arrayToPrint, int arrayLength){
  for(int i=0; i<arrayLength; i++){
    printf("\n%s[%d] = %d", arrayname, i, arrayToPrint[i]);
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
    int numTokens = tokenize(tokens, command);
    // printf("\n\n\nTime to tokenize!");
    for(int i=0; i<numTokens; i++){
        printf("\n\n\ntoken %d: %s", i,  tokens[i]);
    }

    pid_t forkResult = fork(); //execv will be executed by children

  }

  //ensuring that parent doesnt enter this section
  if(forkResult==0){
    //redirecting output of "which"
    close(pipeFileDescriptors[0]);
    dup2(pipeFileDescriptors[1], STDOUT_FILENO);
    close(pipeFileDescriptors[1]);
    // printf("\n\n\nargArray[0]:   %s", argArray[0]);
    // printf("\n\n\nargArray[1]:   %s", argArray[1]);
    // printf("\n\n\nargArray[2]:   %s", argArray[2]);
    execv("/usr/bin/which", tokens);

  }
  else{

    waitReturnVal =  wait(&wstatus);

    int forkResult2 = fork();
    if(forkResult2==0){
      close(pipeFileDescriptors[1]);
      int wordLen = read(pipeFileDescriptors[0], readBuff, sizeof(readBuff));
      close(pipeFileDescriptors[0]);
      readBuff[wordLen-1] = '\0';
      printf("read: %s", readBuff);
    }
    //read output of which

    // printf("\n\n\n");
    // for(int k = 0; k<=wordLen; k++){
    //   printf(" , %c", readBuff[k]);
    // }
    // printf("\n\n\n");
    //
    // char *cek = "/usr/bin/man";
    // printf("\n\n\n");
    // for(int k = 0; k<=wordLen; k++){
    //   printf(" , %c", cek[k]);
    // }
    // printf("\n\n\n");


    // printf("read the following:   %s", readBuff);
    // argArray[0] = readBuff;
    // argArray[1] = NULL;
    //
    // printf("argArray[0] after:   %s", argArray[0]);
    // printf("      argArray[1] after:   %s", argArray[1]);

    // argArray[0]= readBuff;
    // printf("\n\n\nargArray after:");
    // argArray[3] = argArray[2];
    // argArray[2] = "ls";

    // printf("\n\n\nargArray[0]:   %s", argArray[0]);
    // printf("\n\n\nargArray[1]:   %s", argArray[1]);
    // printf("\n\n\nargArray[2]:   %s", argArray[2]);
    // printf("\n\n\nargArray[3]:   %s", argArray[3]);
    //
    // printf("\n\n\n\n\n\ninput arg array: ");
    // printf("\n\n\nargArray[0]:   %s", argv[0]);
    // printf("\n\n\nargArray[1]:   %s", argv[1]);
    // printf("\n\n\nargArray[2]:   %s", argv[2]);
    // printf("\n\n\nargArray[3]:   %s", argv[3]);

    // execv("/usr/bin/man", argArray);

    // argArray[1] = NULL; //set to NULL by convection, see man-page of execv()
    // printf("\n\n\n");
    // char* r[2];

    if(execv(readBuff, tokens)==-1){ // execute the program chosen by the user
      // perror("execv");
    };

    // wait for prior command to finish before prompting new command
    waitReturnVal =  wait(&wstatus);

  }
  // wait for prior command to finish before prompting new command
  waitReturnVal =  wait(&wstatus);
  // int pid = getpid();
  // printf("\n\n\nLatest Pid: %d", pid);
  // printf("\n\n\nParent Pid: %d", parentPid);
  // proceed++;

}
