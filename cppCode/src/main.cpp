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


int main() {

    std::mt19937 randomNumberGenerator(1234);


    auto* premierLeague = new DoubleRoundRobin(20,19);
    premierLeague->getScheduler().setSchedulingStrategy(new MinimizePointsDifferenceLaterRounds());

    /*
    auto* championsLeague = new ChampionsLeague(36, 2);
    auto* metrics = new ChampionsLeagueMetrics();
    */

    const MonteCarloSimulator simulator(premierLeague);
    simulator.runSimulations(100, 30, randomNumberGenerator);
}

