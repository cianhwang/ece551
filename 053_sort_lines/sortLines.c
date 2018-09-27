#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

char ** Load_file(size_t * plen, int argc, char * argv) {
  char ** line = NULL;
  if (argc == 1) {
    char * curr = NULL;
    size_t sz;
    while ((getline(&curr, &sz, stdin) >= 0)) {
      line = realloc(line, (*plen + 1) * sizeof(*line));
      line[*plen] = curr;
      curr = NULL;
      ++(*plen);
    }
    free(curr);
  }
  else {
    FILE * f = fopen(argv, "r");
    if (f == NULL) {
      perror("Could not open file.");
      exit(EXIT_FAILURE);
    }
    char * curr = NULL;
    size_t sz;
    while ((getline(&curr, &sz, f) >= 0)) {
      line = realloc(line, (*plen + 1) * sizeof(*line));
      line[*plen] = curr;
      curr = NULL;
      ++(*plen);
    }
    free(curr);

    if (fclose(f) != 0) {
      perror("Fail to close file.");
      exit(EXIT_FAILURE);
    }
  }
  return line;
}

int main(int argc, char ** argv) {
  if (argc < 1) {
    perror("Invalid input.");
    return EXIT_FAILURE;
  }
  if (argc == 1) {
    char ** line = NULL;
    size_t length = 0;
    line = Load_file(&length, argc, argv[0]);
    if (line == NULL) {
      perror("Invalid.");
      return EXIT_FAILURE;
    }
    sortData(line, length);
    for (size_t i = 0; i < length; ++i) {
      printf("%s", line[i]);
      free(line[i]);
    }
    free(line);
  }
  else {
    for (int i = 1; i < argc; ++i) {
      char ** line = NULL;
      size_t length = 0;

      line = Load_file(&length, argc, argv[i]);
      if (line == NULL) {
        perror("Invalid.");
        return EXIT_FAILURE;
      }
      sortData(line, length);
      for (size_t i = 0; i < length; ++i) {
        printf("%s", line[i]);
        free(line[i]);
      }
      free(line);
    }
  }
  return EXIT_SUCCESS;
}
