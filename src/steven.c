#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const char* PROMPT = "steven> ";

char* path;

void parseInput(char*, char*, char**, int*);
int executeCommand(char*, char**, int*);
int spawnProgram(char*, char**);

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
  // initialize path
  path = (char*)malloc(1024);
  if (path == NULL) {
    printf("error: unable to allocate memory for path\n");
    exit(1);
  }
  strcpy(path, "/bin");

  // run interactive mode
  if (argc == 1) {
    char* input;
    ssize_t inputLength = 0;
    size_t inputBuffer = 0;

    char* command = (char*)malloc(1024);
    if (command == NULL) {
      perror("error: unable to allocate memory for command\n");
      exit(1);
    }

    int argCount = 0;
    char** args = (char**)malloc(sizeof(char*) * 512);
    if (args == NULL) {
      perror("error: unable to allocate memory for args\n");
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
      parseInput(input, command, args, &argCount);

      // only changes to 0 if exit command is entered
      status = executeCommand(command, args, &argCount);
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
    printf("error: too many arguments.\n");
    exit(1);
  }

  return 0;
}

void parseInput(char* input, char* command, char** args, int* argCount) {
  char* t1;

  t1 = strsep(&input, " ");
  t1[strcspn(t1, "\r\n")] = 0;
  strcpy(command, t1);

  *argCount = 0;

  args = (char**)realloc(args, sizeof(char*) * 512);
  if (args == NULL) {
    printf("error: unable to allocate memory for args\n");
    exit(1);
  }

  int i = 0;
  while (input != NULL) {
    char* t2 = strsep(&input, " ");
    if (strcmp(t2, "\0") == 0 || strcmp(t2, "\n") == 0 ||
        strcmp(t2, " ") == 0) {
      continue;
    }
    else {
      args[i] = t2;
      *argCount += 1;
    }
    i++;
  }
}

int executeCommand(char* command, char** args, int* argCount) {
  if (strcmp(command, "exit") == 0) {
    if (*argCount != 0) {
      printf("exit: takes no arguments\n");
      return 1;
    }
    return 0;
  }

  else if (strcmp(command, "cd") == 0) {
    if (*argCount != 1) {
      printf("cd: takes 1 argument\n");
      return 1;
    }
    printf("%s\n", args[0]);
    int status = chdir(args[0]);
    if (status == -1) {
      printf("error: chdir failed\n");
      exit(1);
    }
  }

  else if (strcmp(command, "path") == 0) {
    strcpy(path, "");
    if (*argCount == 0) {
      return 1;
    }
    else {
      int i;
      for (i = 0; i < *argCount; i++) {
        strcat(path, args[i]);
        if (i != *argCount - 1) {
          strcat(path, ":");
        }
      }
    }
  }

  else if (strcmp(command, "loop") == 0) {
    printf("loop not yet implemented\n");
  }

  // else if (external command) { ... }

  else {
    printf("command '%s' not found\n", command);
  }
  return 1;
}

int spawnProgram(char* program, char** argList) {
  pid_t pid;
  pid = fork();

  if (pid != 0) {
    // parent process
    return pid;
  }
  else {
    // execute program
    execvp(program, argList);
    // error in executing program
    fprintf(stderr, "error occurred in execvp\n");
    abort();
  }
}
