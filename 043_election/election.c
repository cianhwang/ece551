#include "election.h"
//include any other headers you need here...

state_t parseLine(const char * line) {
  state_t tempState;
  const char * tempLine = line;
  int flag = 0;
  int idx = 0;
  uint64_t tempPopulation = 0;
  unsigned int tempVote = 0;
  while (*line != '\0') {
    if (*line == ':') {
      switch (flag) {
        case 0:
          while (tempLine < line) {
            tempState.name[idx] = *tempLine;
            ++tempLine;
            ++idx;
          }
          tempState.name[idx] = '\0';
          flag = 1;
          ++tempLine;
          ++line;
          break;
        case 1:
          while (tempLine < line) {
            tempPopulation = 10 * tempPopulation + *tempLine - '0';
            ++tempLine;
          }
          tempState.population = tempPopulation;
          ++tempLine;
          ++line;
          flag = 2;
          break;
        default:
          continue;
      }
    }
    ++line;
  }
  if (flag == 2) {
    while (tempLine < line) {
      tempVote = 10 * tempVote + *tempLine - '0';
      ++tempLine;
    }
    tempState.electoralVotes = tempVote;
    flag = 3;
  }
  if (flag != 3) {
    exit(EXIT_FAILURE);
  }
  else {
    return tempState;
  }
}

unsigned int countElectoralVotes(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 2: write me
  return 0;
}

void printRecounts(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 3: write me
}

void printLargestWin(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 4: write me
}
