#include "counts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
counts_t * createCounts(void) {
  counts_t * c = calloc(1, sizeof(*c));
  if (c == NULL) {
    exit(EXIT_FAILURE);
  }
  c->oneCountArr = NULL;
  c->size = 0;
  return c;
}
void addCount(counts_t * c, const char * name) {
  char * name2 = calloc(10, sizeof(*name2));
  if (name != NULL) {
    name2 = realloc(name2, sizeof(*name2) * (strlen(name) + 1));
    strcpy(name2, name);
  }
  else {
    strcpy(name2, "<unknown>");  //??
  }
  int flag = 0;
  for (size_t i = 0; i < c->size; ++i) {
    if (strcmp(name2, c->oneCountArr[i].str) == 0) {
      ++c->oneCountArr[i].seeTimes;
      flag = 1;
    }
  }
  if (flag == 0) {
    c->oneCountArr = realloc(c->oneCountArr, (c->size + 1) * sizeof(*c->oneCountArr));
    c->oneCountArr[c->size].seeTimes = 1;
    c->oneCountArr[c->size].str = calloc(strlen(name2) + 1, sizeof(*c->oneCountArr[c->size].str));
    strcpy(c->oneCountArr[c->size].str, name2);
    ++c->size;
  }
  free(name2);
}
void printCounts(counts_t * c, FILE * outFile) {
  FILE * f = outFile;
  if (f == NULL) {
    exit(EXIT_FAILURE);
  }
  int unknownIdx = -1;
  for (size_t i = 0; i < c->size; ++i) {
    if (strcmp(c->oneCountArr[i].str, "<unknown>") == 0) {
      unknownIdx = i;
      continue;
    }
    else {
      fprintf(f, "%s: %d\n", c->oneCountArr[i].str, c->oneCountArr[i].seeTimes);
    }
  }
  if (unknownIdx != -1) {
    fprintf(f, "%s : %d\n", c->oneCountArr[unknownIdx].str, c->oneCountArr[unknownIdx].seeTimes);
  }
  if (fclose(f) != 0) {
    exit(EXIT_FAILURE);
  }
}

void freeCounts(counts_t * c) {
  for (size_t i = 0; i < c->size; ++i) {
    free(c->oneCountArr[i].str);
  }
  free(c->oneCountArr);
  free(c);
}
