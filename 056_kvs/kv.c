#include "kv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Split_String(char * curr, ssize_t len, char ** pkeyStr, char ** pvalStr) {
  char * pTemp = curr;
  while (*pTemp != '=') {
    ++pTemp;
  }
  *pkeyStr = malloc(len * sizeof(**pkeyStr));
  if (pkeyStr == NULL) {
    exit(EXIT_FAILURE);
  }
  *pvalStr = malloc(len * sizeof(**pvalStr));
  if (pvalStr == NULL) {
    exit(EXIT_FAILURE);
  }

  strncpy(*pkeyStr, curr, pTemp - curr);
  (*pkeyStr)[pTemp - curr] = '\0';
  // strcat(*pkeyStr, "\0");
  ++pTemp;
  strncpy(*pvalStr, pTemp, len - 1 - (pTemp - curr));
  (*pvalStr)[len - 1 - (pTemp - curr)] = '\0';
  // strcat(*pvalStr, "\0");
}

kvarray_t * readKVs(const char * fname) {
  FILE * f = fopen(fname, "r");
  if (f == NULL) {
    perror("Fail to open file!");
    exit(EXIT_FAILURE);
  }
  kvarray_t * kvArr = malloc(1 * sizeof(*kvArr));
  kvArr->size = 0;
  kvArr->pArr = NULL;
  char * curr = NULL;
  size_t sz;
  ssize_t len;
  while ((len = getline(&curr, &sz, f)) >= 0) {
    char * keyStr = NULL;
    char * valStr = NULL;
    Split_String(curr, len, &keyStr, &valStr);
    kvArr->pArr = realloc(kvArr->pArr, sizeof(*kvArr->pArr) * (kvArr->size + 1));
    kvArr->pArr[kvArr->size].key = keyStr;
    kvArr->pArr[kvArr->size].value = valStr;
    ++(kvArr->size);
  }
  free(curr);
  if (fclose(f) != 0) {
    perror("Fail to close the file!");
    exit(EXIT_FAILURE);
  }
  return kvArr;
}

void freeKVs(kvarray_t * pairs) {
  for (size_t i = 0; i < pairs->size; ++i) {
    free(pairs->pArr[i].key);
    free(pairs->pArr[i].value);
  }
  free(pairs->pArr);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  for (size_t i = 0; i < pairs->size; ++i) {
    printf("key = '%s' value = '%s'\n", pairs->pArr[i].key, pairs->pArr[i].value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  char * findValue = NULL;
  for (size_t i = 0; i < pairs->size; ++i) {
    if (strcmp(pairs->pArr[i].key, key) == 0) {
      findValue = pairs->pArr[i].value;
      break;
    }
  }
  return findValue;
}
