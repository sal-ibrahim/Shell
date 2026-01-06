#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define BUFFER_SIZE 1024

int handle_echo(char *command);


int main(int argc, char *argv[]) {
    //flush after every printf
    setbuf(stdout, NULL);

    char command[BUFFER_SIZE];

    while (1){
        printf("$ ");

        fgets(command, sizeof(command), stdin);

        //remove the newline character
        command[strcspn(command, "\n")] = '\0';

        //handle exit command
        if (strcmp(command, "exit") == 0){
        return 0;
        }

        //handle echo command
        if (handle_echo(command)){
        continue;
        }
    }

    return 0;
}



int handle_echo(char *command){
  /*
  (char *) -> int
  Function checks whether the given command is an echo command. If so, prints the text following echo to standard output and returns 1. 
  If the command is not an echo command, returns 0.
  */

  if (strncmp(command, "echo", 4) != 0 || (command[4] != ' ' && command[4] != '\0')){
    return 0;
  }

  if (command[4] == ' '){
    printf("%s\n", command + 5);
  } else{
    printf("\n");
  }

  return 1;
}
