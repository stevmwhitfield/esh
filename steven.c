#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const char* PROMPT = "steven> ";

void parseInput(char* input, char* command, char** args);
int executeCommand(char*, char**);

// shell runs in interactive mode by default
// shell supports batch mode by running shell with arg: text file

// shell creates a new process for each external command
// shell uses internal implementations for built-in commands

// built-in commands:
// exit: call exit(0) system call; accepts no args
// cd: call chdir() system call; accepts exactly 1 arg
// path: sets the shell's path; accepts 0 or more args
// loop: loops a command some times; accepts 2 or more args

// interactive mode: read input using getline()
// batch mode: read input from text file

// if EOF, exit(0)

// tokenize input with strsep()

// execute commands with fork(), exec(), and wait()/waitpid()

// execv()

// shell uses a path variable; initially set to /bin

int main(int argc, char** argv) {
  // run interactive mode
  if (argc == 1) {
    // initialize path to /bin
    char* path = (char*)malloc(1024);
    if (path == NULL) {
      perror("Error: unable to allocate memory for path");
      exit(1);
    }
    strcpy(path, "/bin");

    char* input;
    ssize_t inputLength = 0;
    size_t inputBuffer = 0;

    char* command = (char*)malloc(1024);
    if (command == NULL) {
      perror("Error: unable to allocate memory for command");
      exit(1);
    }

    char** args = (char**)malloc(sizeof(char*) * 512);
    if (args == NULL) {
      perror("Error: unable to allocate memory for args");
      exit(1);
    }

    int status = 1;

    while (status) {
      // prompt user for input
      // exit shell if input is EOF
      printf("%s", PROMPT);
      inputLength = getline(&input, &inputBuffer, stdin);
      if (inputLength == -1) {
        break;
      }

      // prompt for input again if user enters nothing
      if (inputLength == 1) {
        continue;
      }

      // separate input into command and args
      parseInput(input, command, args);

      // only changes to 0 if exit command is entered
      status = executeCommand(command, args);
    }

    if (path) free(path);
    if (input) free(input);
    if (command) free(command);
    if (args) free(args);

    exit(0);
  }
  // run batch mode
  else if (argc == 2) {
    exit(0);
  }
  else {
    printf("Error: too many arguments.\n");
    exit(1);
  }

  return 0;
}

void parseInput(char* input, char* command, char** args) {
  char* t1;

  t1 = strsep(&input, " ");
  strcpy(command, t1);

  int i = 0;
  while (input != NULL) {
    args[i] = strsep(&input, " ");
    i++;
  }
}

int executeCommand(char* command, char** args) {
  return 1;
}