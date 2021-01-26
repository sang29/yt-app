#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
//#include <string.h>

#define INPUT_SIZE 100

char* getCommandFromInput();
void getUserInput();


// add static variable and do an offset to add to array and count variables
// while loop

int main(int argc, char **argv) {
  getUserInput();
  getUserInput();
  getUserInput();

  return 0;
}

char* getInput() {
  char *inputBuffer = (char *) malloc(INPUT_SIZE);
  //char inputBuffer[INPUT_SIZE];
  int numBytes = read(STDIN_FILENO, inputBuffer, INPUT_SIZE);
  static int counter = 0;

  counter = counter + numBytes;
  printf("Number of bytes read so far: %d\n", counter);
  write(STDOUT_FILENO, "Program is done!!\n", 18);
  return inputBuffer;
}

void getUserInput() {
  char command[INPUT_SIZE];
  char* inputBuffer = getInput();

  int i = 0;
  for (i=0; i < INPUT_SIZE; i++) {
    command[i] = toupper(inputBuffer[i]);
    if (command[i] == '\n') {
      break;
    }
  }
  write(STDOUT_FILENO, command, i+1);
  free(inputBuffer);
}






