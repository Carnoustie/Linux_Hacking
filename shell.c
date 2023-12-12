#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define UPPERBOUND 1000
#define MAXARGS 10


int wstatus; //allocating an int neccesary for wait()
pid_t waitReturnVal; //For storing the return of wait()
char readBuff[1000]; //Storing the output of "which"-system call


int main(int argc, char* const argv[]){
  printf("\n\n\nWelcome to my very limited shell!\n\n\n");

  int parentPid = getpid(); //Process id of parent
  int proceed = 1; //for later implementing user option to quit out of shell
  int pipeFileDescriptors[2]; // file descriptors for redirecting output of "which"

  char * argArray[MAXARGS]; //arguments into execv()
  argArray[0] = argv[0]; // by convention, needs to hold filename of running program, i.e "shell.c"


  while(proceed){

    //create pipe for redirecting output
    if(pipe(pipeFileDescriptors)==-1){
      perror("pipe");
    }


    printf("\n\n\nPrompt>> ");
    char command[MAXARGS];
    scanf("%s", command);
    //filling up argumnents into upcoming execv()
    argArray[1] = command;
    argArray[2] = NULL;

    // printf("\n\n\n");
    // if( (int) *command== 9){break;}
    pid_t child = fork(); //execv will be executed by children
    int newPid;


    // printf("\n\n\nargv[1]:   %s", argv[1]);
    // printf("\n\n\nargArray[0]:   %s", argArray[0]);
    // printf("\n\n\nargArray[1]:   %s", argArray[1]);
    // printf("\n\n\nargArray[2]:   %s", argArray[2]);

    // printf("\n\n\nargArray[0]:   %s", argv[0]);
    // printf("\n\n\nargArray[1]:   %s", argv[1]);
    // printf("\n\n\nargArray[2]:   %s", argv[2]);
    // printf("\n\n\nargArray[3]:   %s", argArray[3]);
    // printf("\n\n\nargArray[4]:   %s", argArray[4]);
    // printf("\n\n\nargArray[5]:   %s", argArray[5]);
    // printf("\n\n\nargArray[6]:   %s", argArray[6]);
    // printf("\n\n\nargArray[7]:   %s", argArray[7]);


    //ensuring that parent doesnt enter this section
    if( (newPid = getpid()) != parentPid){
      proceed=0;
      // printf("\n\n\nPid inside: %d\n\n\n\n\n\n\n\n\n", newPid);
      //redirecting output of which
      close(pipeFileDescriptors[0]);
      dup2(pipeFileDescriptors[1], STDOUT_FILENO);
      close(pipeFileDescriptors[1]);
      // printf("\n\n\nargArray before:");
      // printf("\n\n\nargArray[0]:   %s", argArray[0]);
      // printf("\n\n\nargArray[1]:   %s", argArray[1]);
      // printf("\n\n\nargArray[2]:   %s", argArray[2]);
      execv("/usr/bin/which", argArray);

    }else{
      waitreturn =  wait(&wstatus);
      close(pipeFileDescriptors[1]);
      int child2 = fork();
      if(getpid()!=parentPid){
        //read output of which
        int wordLen = read(pipeFileDescriptors[0], readBuff, sizeof(readBuff));
        close(pipeFileDescriptors[0]);
        readBuff[wordLen-1] = '\0';
        printf("\n\n\n");
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
        printf("\n\n\n");


        // printf("read the following:   %s", readBuff);
        // argArray[0] = readBuff;
        // argArray[1] = NULL;
        //
        // printf("argArray[0] after:   %s", argArray[0]);
        // printf("      argArray[1] after:   %s", argArray[1]);

        // argArray[0]= readBuff;
        // printf("\n\n\nargArray after:");
        // printf("\n\n\nargArray[0]:   %s", argArray[0]);
        // printf("\n\n\nargArray[1]:   %s", argArray[1]);
        // printf("\n\n\nargArray[2]:   %s", argArray[2]);
        // execv("/usr/bin/man", argArray);

        argArray[1] = NULL; //set to NULL by convection, see man-page of execv()
        printf("\n\n\n");
        if(execv(readBuff, argArray)==-1){ // execute the program chosen by the user
          perror("execv");
        };
      }

      // wait for prior command to finish before prompting new command
      waitreturn =  wait(&wstatus);

    }
    // wait for prior command to finish before prompting new command
    waitreturn =  wait(&wstatus);
    // int pid = getpid();
    // printf("\n\n\nLatest Pid: %d", pid);
    // printf("\n\n\nParent Pid: %d", parentPid);
    // proceed++;
  }

}
