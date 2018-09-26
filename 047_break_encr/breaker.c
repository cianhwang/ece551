#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int count_maxfreq(FILE * f) {
  int arr[26] = {0};
  int c;
  int idx = 0;
  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) {
      c = tolower(c);
      idx = c - 'a';
      ++arr[idx];
    }
  }
  int maxIdx = 0;
  int maxTemp = 0;
  for (int i = 0; i < 26; ++i) {
    if (arr[i] > maxTemp) {
      maxIdx = i;
      maxTemp = arr[i];
    }
  }
  return (maxIdx - 4) % 26;
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: breaker inputFileName\n");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  int key = count_maxfreq(f);
  printf("%d\n", key);

  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
