#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "entities/TimeTable.h"
#include <random>
#include "utils/Simulations/OutcomeProbabilities.h"

class Simulator {
public:
    static void simulateCompetition(TimeTable& timeTable, std::mt19937& randomNumberGenerator);
    static void simulateCompetition(TimeTable& timeTable, std::mt19937& randomNumberGenerator, int numberRoundsBeforeTheEnd);
    static void simulateMatches(const std::vector<Match>& matches);
};

#endif //SIMULATOR_H
