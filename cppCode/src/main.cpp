#include <iostream>
#include <utils/Scheduling/Strategies/StaticObjective.h>

#include "tournament/DoubleRoundRobin.h"
#include "tournament/Tournament.h"
#include "utils/Scheduling/Schedulers/Scheduler.h"
#include "utils/Writers/Writer.h"
#include "utils/Simulations/MonteCarloSimulator.h"
#include "utils/Simulations/Simulator.h"
#include "utils/Scheduling/Strategies/MaximizePointsDifference.h"
#include "utils/Scheduling/Strategies/MinimizePointsDifference.h"
#include "utils/Scheduling/Strategies/MinimizePointsDifferenceLaterRounds.h"
#include "utils/Scheduling/Strategies/MinimizeIrrelevanceStaticSchedule.h"
#include "utils/Scheduling/Strategies/SchedulingStrategy.h"
#include "utils/Metrics/Bounds.h"

#include "utils/TeamManager/ChampionsLeagueTeamManager.h"
#include "utils/Scheduling/Schedulers/ChampionsLeagueScheduler.h"
#include "tournament/ChampionsLeague.h"
#include "utils/Metrics/ChampionsLeagueMetrics.h"
#include "utils/Scheduling/Strategies/MinimizeRatingDifference.h"
#include "config/CplexConfig.h"


int main(int argc, char** argv) {
    int threads = 1;
    if (argc > 1) {
        threads = std::stoi(argv[1]);
    }
    CplexConfig::setNumThreads(threads);
    std::cout << "Using " << CplexConfig::getNumThreads() << " threads for CPLEX." << std::endl;
    
    std::mt19937 randomNumberGenerator(1234);

    /*
    auto* premierLeague = new DoubleRoundRobin(20,19);
    premierLeague->getScheduler().setSchedulingStrategy(new MinimizePointsDifferenceLaterRounds());
    */

    auto* championsLeague = new ChampionsLeague(36, 1);
    auto* metrics = new ChampionsLeagueMetrics();


    const MonteCarloSimulator simulator(championsLeague);
    simulator.runSimulations(100, 30, randomNumberGenerator);
}
