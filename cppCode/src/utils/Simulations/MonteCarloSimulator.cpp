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

#include <chrono>
#include <iomanip>
#include <ctime>


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
        auto startTime = std::chrono::steady_clock::now();

        for(int i = 0; i < numberSchedules; i++) {
            auto currentTime = std::chrono::system_clock::now();
            std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
    
            auto now = std::chrono::steady_clock::now();
            auto elapsed = now - startTime;
            double avgTimePerSchedule = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() / static_cast<double>(i + 1);
            int remainingSchedules = numberSchedules - i - 1;
            int estimatedRemainingSeconds = static_cast<int>(avgTimePerSchedule * remainingSchedules);
    
            int minutes = estimatedRemainingSeconds / 60;
            int seconds = estimatedRemainingSeconds % 60;
    
            std::cout << "currently running schedule " << i
                      << " at " << std::put_time(std::localtime(&currentTimeT), "%H:%M:%S")
                      << " | estimated time remaining: " << minutes << "m " << seconds << "s" << std::endl;

            //runCounter++;
            tournament->resetTournament();

            //tournament->getTeamManager()->changeTeamRatings("equallyDistanced",20);

            tournament->makeInitialSchedule(randomNumberGenerator);

            for(int j = 0; j < numberSimulations; ++j) {

                std::cout<<"running ["<<i<<"->"<<j<<"]"<<std::endl;


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
        
        auto startTime = std::chrono::steady_clock::now();

        for(int i = 0; i < numberSchedules; i++) {
            auto currentTime = std::chrono::system_clock::now();
            std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
    
            auto now = std::chrono::steady_clock::now();
            auto elapsed = now - startTime;
            double avgTimePerSchedule = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() / static_cast<double>(i + 1);
            int remainingSchedules = numberSchedules - i - 1;
            int estimatedRemainingSeconds = static_cast<int>(avgTimePerSchedule * remainingSchedules);
    
            int minutes = estimatedRemainingSeconds / 60;
            int seconds = estimatedRemainingSeconds % 60;
    
            std::cout << "currently running schedule " << i
                      << " at " << std::put_time(std::localtime(&currentTimeT), "%H:%M:%S")
                      << " | estimated time remaining: " << minutes << "m " << seconds << "s" << std::endl;
            
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

    Writer::writeTeamBounds(R"(bounds_dynamic_pointsdifference.txt)", teamBounds);
    Writer::writeRandomForestFeatures(R"(rf_features_dynamic_pointsdifference.txt)", rfFeaturesList);
    Writer::writeLastRounds(R"(last_rounds_dynamic_pointsdifference.txt)", lastRoundsMatches);
    Writer::writeRankings(R"(ranking_dynamic_pointsdifference.txt)", rankingInfo);
}


