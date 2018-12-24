#include "outname.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * computeOutputFileName(const char * inputName) {
  char * outName = NULL;
  outName = calloc(strlen(inputName) + 8, sizeof(*outName));
  if (outName == NULL) {
    exit(EXIT_FAILURE);
  }
  strcpy(outName, inputName);
  strcat(outName, ".counts");
  return outName;
}
