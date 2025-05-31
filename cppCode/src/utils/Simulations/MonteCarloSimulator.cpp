#include "utils/Simulations/MonteCarloSimulator.h"

#include <utils/Metrics/RandomForestFeatures.h>

#include "tournament/ChampionsLeague.h"
#include "utils/Scheduling/Strategies/SchedulingStrategy.h"
#include "tournament/DoubleRoundRobin.h"
#include "utils/Writers/Writer.h"
#include "utils/Writers/TeamBoundsWriter.h"

#include "utils/Metrics/Bounds.h"
#include "utils/Scheduling/Schedulers/ChampionsLeagueScheduler.h"
#include "utils/Scheduling/Schedulers/DoubleRoundRobinScheduler.h"


MonteCarloSimulator::MonteCarloSimulator(DoubleRoundRobin* tournament)
    : tournament(tournament),championsLeague(nullptr) {
}

MonteCarloSimulator::MonteCarloSimulator(ChampionsLeague* tournament)
    : tournament(nullptr), championsLeague(tournament) {
}

void MonteCarloSimulator::runSimulations(const int numberSchedules, const int numberSimulations, std::mt19937 randomNumberGenerator) const {

    TeamBounds currentBound;
    RandomForestFeatures rfFeatures;
    Rankings ranking;

    const int total = numberSchedules * numberSimulations;

    std::vector<TeamBounds> teamBounds;
    std::vector<RandomForestFeatures> rfFeaturesList;
    std::vector<std::map<int, std::vector<Match>>> lastRoundsMatches(total);
    std::vector<Rankings> rankingInfo;


    // = premier league competition
    if(tournament != nullptr) {

        int runCounter = 0;
        for(int i = 0; i < numberSchedules; i++) {
            //runCounter++;
            tournament->resetTournament();

            //tournament->getTeamManager()->changeTeamRatings("equallyDistanced",20);

            tournament->makeInitialSchedule(randomNumberGenerator);

            std::cout<<"currently running schedule " << i << std::endl;

            for(int j = 0; j < numberSimulations; ++j) {

                std::cout<<"running"<<i<<" "<<j<<std::endl;


                Simulator::simulateCompetition(tournament->getFirstTimeTable(), randomNumberGenerator);

                tournament->makeSecondSchedule();
                Simulator::simulateCompetition(tournament->getSecondTimeTable(), randomNumberGenerator);

                tournament->setCompleteTimeTable();

                currentBound.setTeamBounds(*tournament, 5);
                teamBounds.push_back(currentBound);

                rfFeatures.setRandomForestFeatures(*tournament);
                rfFeaturesList.push_back(rfFeatures);

                lastRoundsMatches[runCounter]  = Bounds::getRemainingMatches(*tournament,5);

                ranking.setRanking(*tournament);
                rankingInfo.push_back(ranking);

                tournament->getTeamManager()->resetTeams();
                runCounter++;

            }

        }
    }



    // for the champions league format
    else if (championsLeague!= nullptr) {

        //ChampionsLeagueMetrics metric;


        for(int i = 0; i < numberSchedules; i++) {

            std::cout<<"currently running simulation " << i << std::endl;

            championsLeague->resetTournament();
            championsLeague->makeInitialSchedule(randomNumberGenerator);


            for(int j = 0; j < numberSimulations; ++j) {

                if(championsLeague->getNumberDynamicRounds() != 0) {


                    Simulator::simulateCompetition(championsLeague->getFirstTimeTable(), randomNumberGenerator);

                    championsLeague->makeSecondSchedule();

                    Simulator::simulateCompetition(championsLeague->getSecondTimeTable(), randomNumberGenerator);
                    championsLeague->setCompleteTimeTable();

                    championsLeague->getTeamManager()->resetTeams();
                }


                else {
                    Simulator::simulateCompetition(championsLeague->getCompleteTimeTable(), randomNumberGenerator);
                    championsLeague->getTeamManager()->resetTeams();
                }

            }

        }
    }
    Writer::writeTeamBounds(R"(C:\Users\dewae\Desktop\cpp\hpc\cppCode\good_output\bounds_dynamic_points_difference.txt)", teamBounds);
    Writer::writeRandomForestFeatures(R"(C:\Users\dewae\Desktop\cpp\hpc\cppCode\good_output\rf_features_dynamic_points_difference.txt)", rfFeaturesList);
    Writer::writeLastRounds(R"(C:\Users\dewae\Desktop\cpp\hpc\cppCode\good_output\last_rounds_dynamic_points_difference.txt)", lastRoundsMatches);
    Writer::writeRankings(R"(C:\Users\dewae\Desktop\cpp\hpc\cppCode\good_output\ranking_dynamic_points_difference.txt)", rankingInfo);
}


