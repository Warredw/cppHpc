#include "utils/Metrics/SecondaryMetrics.h"
#include <unordered_set>

#include "tournament/Tournament.h"
#include "utils/Metrics/RoundStatistics.h"

SecondaryMetrics::SecondaryMetrics(): roundWinnerDecided(0), numberLeadChanges(0), numberRelegationChanges(0){}

// Getters
int SecondaryMetrics::getRoundWinnerDecided() const {
    return roundWinnerDecided;
}
int SecondaryMetrics::getNumberLeadChanges() const {
    return numberLeadChanges;
}

int SecondaryMetrics::getNumberRelegationChanges() const {
    return numberRelegationChanges;
}

std::map<int, int> SecondaryMetrics::getAveragePointDifference() const {
    return averagePointDifference;
}


// Calculate Them
void SecondaryMetrics::calculateBasicMetrics(Tournament& tournament) {

    int roundWinnerDecided = 0;
    int currentRound = 0;
    int numberLeadChanges = 0;
    int numberRelegationChanges = 0;
    int numberChanges = 0;

    std::unordered_set<std::shared_ptr<Team>> currentLeaders;
    std::unordered_set<std::shared_ptr<Team>> currentRelegators;


    // Loop over the complete timetable but we only measure the metrics for the second half

    for(auto& round: tournament.getCompleteTimeTable().getRounds()) {

        currentRound += 1;

        if(currentRound < tournament.getNumberTeams()) continue;

        std::vector<std::pair<std::shared_ptr<Team>,int>> ranking = RoundStatistics::makeRanking(round);

        std::unordered_set<std::shared_ptr<Team>> newLeaders = RoundStatistics::calculateLeadersAfterRound(round);
        std::unordered_set<std::shared_ptr<Team>> newRelegators = RoundStatistics::calculateLeadersAfterRound(round);
        if (currentLeaders != newLeaders) {
            numberLeadChanges += 1;
            currentLeaders = newLeaders;
        }

        if (currentRelegators != newRelegators) {
            numberRelegationChanges += 1;
            currentRelegators = newRelegators;
        }

        const int highestScore = RoundStatistics::calculateHighestScore(round);
        const int secondHighestScore = RoundStatistics::calculateSecondHighestScore(round);

        if (roundWinnerDecided == 0 && (highestScore - secondHighestScore > 3*(tournament.getNumberRounds() - currentRound))) {
            roundWinnerDecided = currentRound;
            this->roundWinnerDecided = roundWinnerDecided;
        }
    }



    this->numberLeadChanges = numberLeadChanges;
    if(roundWinnerDecided == 0) this->roundWinnerDecided = currentRound;
    this->numberRelegationChanges = numberRelegationChanges;
}


void SecondaryMetrics::calculateAveragePointDifference(Tournament &tournament) {


    // Go over all the rounds
    for(int round = tournament.getNumberRounds() - 19; round < tournament.getNumberRounds(); round++) {
        Round& currentRound = tournament.getCompleteTimeTable().getRounds()[round];
        double totalPointDifference = 0;
        int matchCount = 0;
        for (const auto& match : currentRound.getMatches()) {
            totalPointDifference += std::abs(match.getPointsHomeTeamBeforeMatch() - match.getPointsAwayTeamBeforeMatch());
            matchCount++;
        }
        this->averagePointDifference[round + 1] = std::round(totalPointDifference / matchCount * 100) / 100;
    }
}





