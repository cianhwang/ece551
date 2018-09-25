#include "election.h"

#include <stdio.h>
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
  uint64_t voteTemp = 0;
  for (size_t i = 0; i < nStates; ++i) {
    if (voteCounts[i] * 2 > stateData[i].population) {
      voteTemp += stateData[i].electoralVotes;
    }
  }
  return voteTemp;
}

void printRecounts(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  for (size_t i = 0; i < nStates; ++i) {
    double ratio = (double)(voteCounts[i]) / (double)(stateData[i].population);
    if (ratio < 0.505 && ratio > 0.495) {
      printf("%s requires a recount (Candidate A has %.2f%% of the vote)\n",
             stateData[i].name,
             ratio * 100);
    }
  }
  return;
}

void printLargestWin(state_t * stateData, uint64_t * voteCounts, size_t nStates) {}
