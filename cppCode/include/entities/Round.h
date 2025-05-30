#ifndef ROUND_H
#define ROUND_H

#include <vector>
#include "entities/Match.h"

class Round {
  std::vector<Match> matches;

public:
  void addMatch(const Match& match);
  void dropMatch(); // drops the last match
  std::vector<Match>& getMatches();

};

#endif //ROUND_H
