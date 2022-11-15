#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// built-in commands:
// exit: call exit(0) system call; accepts no args
// cd: call chdir() system call; accepts exactly 1 arg
// path: sets the shell's path; accepts 0 or more args
// loop: loops a command some times; accepts 2 or more args

int main(int argc, char** argv) {
  // while status is good
  //   prompt for input
  //   parse input
  //   if input is valid, execute command
  //   else, error message
  return 0;
}
