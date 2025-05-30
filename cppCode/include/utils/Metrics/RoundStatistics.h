#ifndef ROUNDSTATISTICS_H
#define ROUNDSTATISTICS_H

// Some helper functions related to the statistics of a round

#include "entities/Round.h"
#include <unordered_set>

class RoundStatistics {

public:
    static int calculateHighestScore(Round& round);
    static int calculateSecondHighestScore(Round& round);
    static std::unordered_set<std::shared_ptr<Team>> calculateLeadersAfterRound(Round& round);
    static std::unordered_set<std::shared_ptr<Team>> calculateRelegatorsAfterRound(Round& round);

    static std::vector<std::pair<std::shared_ptr<Team>,int>> makeRanking(Round& round);
    static std::unordered_set<std::shared_ptr<Team>> selectTeamPositions(const std::vector<std::pair<std::shared_ptr<Team>,int>>& ranking, int highPosition, int lowPosition);

    static int calculateLowestScore(Round& round);
    static int calculateSecondLowestScore(Round& round);
};



#endif //ROUNDSTATISTICS_H
