#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define UPPERBOUND 1000
#define MAXARGS 10


int main(int argc, char* const argv[]){
  char readBuff[10024];
  printf("\n\n\nWelcome to my very limited shell!\n\n\n");
  int proceed = 2;
  int parentPid = getpid();
  int wstatus;
  pid_t waitreturn;

  int pipefd[2];

  char * argArray[MAXARGS];
  argArray[0] = argv[0];
  // argArray[1] = "ls";
  argArray[2] = NULL;
  // argArray[3] = argv[3];
  // argArray[4] = argv[4];
  // argArray[5] = argv[5];
  // argArray[6] = argv[6];
  // argArray[7] = argv[7];





  // char * const *argArray;

  while(proceed==2 || proceed==3){


    if(pipe(pipefd)==-1){
      perror("pipe");
    }


    printf("\n\n\nPrompt>> ");
    char command[MAXARGS];
    // fgets(command, sizeof(command), stdin);
    scanf("%s", command);
    argArray[1] = command;

    // printf("\n\n\n");
    // if( (int) *command== 9){break;}
    pid_t child = fork();
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


    if( (newPid = getpid()) != parentPid){
      proceed=0;
      // printf("\n\n\nPid inside: %d\n\n\n\n\n\n\n\n\n", newPid);
      // execv("/usr/bin/ls", argv);
      close(pipefd[0]);
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[1]);
      // printf("\n\n\nargArray before:");
      // printf("\n\n\nargArray[0]:   %s", argArray[0]);
      // printf("\n\n\nargArray[1]:   %s", argArray[1]);
      // printf("\n\n\nargArray[2]:   %s", argArray[2]);
      execv("/usr/bin/which", argArray);

    }else{
      waitreturn =  wait(&wstatus);
      close(pipefd[1]);
      int child2 = fork();
      if(getpid()!=parentPid){
        int wordLen = read(pipefd[0], readBuff, sizeof(readBuff));
        close(pipefd[0]);
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

        char *ce = "/usr/bin/man";
        argArray[1] = NULL;
        printf("\n\n\n");
        if(execv(readBuff, argArray)==-1){
          perror("execv");
        };
      }
      waitreturn =  wait(&wstatus);



    }
    waitreturn =  wait(&wstatus);
    // int pid = getpid();
    // printf("\n\n\nLatest Pid: %d", pid);
    // printf("\n\n\nParent Pid: %d", parentPid);
    // proceed++;

  }

}
