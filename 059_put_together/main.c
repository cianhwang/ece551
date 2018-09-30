#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counts.h"
#include "kv.h"
#include "outname.h"

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("fail to open file");
    exit(EXIT_FAILURE);
  }
  counts_t * c = createCounts();
  char * curr = NULL;
  size_t sz = 0;
  ssize_t len = 0;
  while ((len = getline(&curr, &sz, f)) >= 0) {
    char * line = calloc(len, sizeof(*line));  // curr contains /n.
    strncpy(line, curr, len - 1);

    char * findVal = lookupValue(kvPairs, line);
    addCount(c, findVal);
    free(line);
  }
  free(curr);

  if (fclose(f) != 0) {
    perror("fail to close file");
    exit(EXIT_FAILURE);
  }

  return c;
}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)
  if (argc <= 2) {
    return EXIT_FAILURE;
  }
  //read the key/value pairs from the file named by argv[1] (call the result kv)
  kvarray_t * kvPairs = readKVs(argv[1]);

  //count from 2 to argc (call the number you count i)
  //count the values that appear in the file named by argv[i], using kv as the key/value pair
  //   (call this result c)
  for (int i = 2; i < argc; ++i) {
    counts_t * c = countFile(argv[i], kvPairs);
    char * outName = computeOutputFileName(argv[i]);

    //compute the output file name from argv[i] (call this outName)
    //open the file named by outName (call that f)
    FILE * f = fopen(outName, "w");
    //print the counts from c into the FILE f
    printCounts(c, f);
    //close f
    if (fclose(f) != 0) {
      perror("fail to close file");
      return EXIT_FAILURE;
    }
    //free the memory for outName and c
    free(outName);
    freeCounts(c);
  }
  //free the memory for kv
  freeKVs(kvPairs);
  return EXIT_SUCCESS;
}
