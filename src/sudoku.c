#include <stdio.h>
#include <stdlib.h>

#include "../include/sudoku.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    PrintHelp();
    return EXIT_FAILURE;
  }

  return 0;
}

void PrintHelp(void) {
  printf("sudoku - A program that solves 9x9 sudoku puzzles.\n");
  printf("\n");
  printf("Usage:\n");
  printf("  sudoku [OPTIONS] <FILE>\n");
  printf("\n");
  printf("Options:\n");
  printf("  %-20s %s\n", "-h, --help", "Show this message and exit.");
}