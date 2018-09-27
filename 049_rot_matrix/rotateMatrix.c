#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Read_data(FILE * f, int mat[][10]) {
  int c;
  int counter = 0;
  int enterCounter = 0;
  while ((c = fgetc(f)) != EOF) {
    if (c != '\n') {
      if (counter >= 100) {
        perror("Too much lines");
        exit(EXIT_FAILURE);
      }
      mat[counter / 10][counter % 10] = c;
      ++counter;
    }
    else if (counter == 0 || counter % 10 != 0) {
      perror("Should be 10 chars one line");
      exit(EXIT_FAILURE);
    }
    else {
      ++enterCounter;
      if (enterCounter > 10) {
        perror("Too much lines");
        exit(EXIT_FAILURE);
      }
    }
  }
}
void Rotate(int mat[][10], int rmat[][10]) {
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      rmat[j][9 - i] = mat[i][j];
    }
  }
}

void Print_matrix(int mat[][10]) {
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      printf("%c", mat[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: rotateMatrix inputFileName\n");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  int matrix[10][10] = {0};
  Read_data(f, matrix);
  int rotateMatrix[10][10] = {0};
  Rotate(matrix, rotateMatrix);
  Print_matrix(rotateMatrix);
  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
