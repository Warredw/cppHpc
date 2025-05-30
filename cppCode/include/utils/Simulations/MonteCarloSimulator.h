
#ifndef MONTECARLOSIMULATOR_H
#define MONTECARLOSIMULATOR_H

#include "utils/Metrics/SecondaryMetrics.h"
#include "utils/Simulations/Simulator.h"
#include <vector>
#include "utils/Metrics/ChampionsLeagueMetrics.h"
#include "utils/Metrics/TeamBounds.h"

class ChampionsLeague;
class DoubleRoundRobin;

class MonteCarloSimulator {

    DoubleRoundRobin* tournament;
    ChampionsLeague* championsLeague;
    std::vector<ChampionsLeagueMetrics>* championsLeagueMetrics;


public:
    // PL constructor
    explicit MonteCarloSimulator(DoubleRoundRobin* tournament);
    // CL constructor
    explicit MonteCarloSimulator(ChampionsLeague* tournament, std::vector<ChampionsLeagueMetrics>* championsLeagueMetrics);

    void runSimulations(int numberSchedules, int numberSimulations, std::mt19937 randomNumberGenerator) const;
};



#endif //MONTECARLOSIMULATOR_H
