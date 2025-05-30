#include "utils/Metrics/ChampionsLeagueMetrics.h"
#include "utils/Metrics/Bounds.h"
#include "tournament/Tournament.h"


// Getters

std::map<int,int> ChampionsLeagueMetrics::getNumberIrrelevantMatches() const {
    return numberIrrelevantMatches;
}

std::map<int,int> ChampionsLeagueMetrics::getNumberHighlyRelevantMatches() const {
    return numberHighlyRelevantMatches;
}

std::map<int,int> ChampionsLeagueMetrics::getNumberPartiallyRelevantMatches() const {
    return numberPartiallyRelevantMatches;
}

std::map<int,int> ChampionsLeagueMetrics::getNumberQualifiersDecided() const {
    return numberQualifiersDecided;
}

std::map<int,int> ChampionsLeagueMetrics::getNumberPlayOffsDecided() const {
    return numberPlayOffsDecided;
}

std::map<int,int> ChampionsLeagueMetrics::getNumberRelegationDecided() const {
    return numberRelegationDecided;
}

std::map<int,int> ChampionsLeagueMetrics::getNumberUndecided() const {
    return numberUndecided;
}



void ChampionsLeagueMetrics::calculateMetrics(Tournament &tournament,  const int numberRoundsBeforeTheEnd) {

    // Get the remainingRounds
    std::map<int, std::vector<Match>> remainingMatches = Bounds::getRemainingMatches(tournament, numberRoundsBeforeTheEnd);

    int counter = 0;

    for(auto it = remainingMatches.begin(); it != remainingMatches.end(); ++it) {

        int roundNumber = tournament.getNumberRounds() - numberRoundsBeforeTheEnd + 1 + counter;
        int numMatches = it->second.size();

        int numberIrrelevantMatchesRound = 0;
        int numberPartiallyRelevantMatchesRound = 0;
        int numberHighlyRelevantMatchesRound = 0;

        int numberQualifiersDecidedRound = 0;
        int numberRelegationDecidedRound = 0;
        int numberPlayOffsDecidedRound = 0;
        int numberUndecidedRound = 0;


        // go over all matches in this round
        for(const auto &match: it->second) {
            std::shared_ptr<Team> homeTeam = match.getHomeTeam();
            std::shared_ptr<Team> awayTeam = match.getAwayTeam();

            std::vector<std::shared_ptr<Team>> teams = {homeTeam, awayTeam};

            const int homeBest = Bounds::solveBestCaseRank(tournament, homeTeam, numberRoundsBeforeTheEnd - counter);
            const int homeWorst = Bounds::solveWorstCaseRank(tournament, homeTeam, numberRoundsBeforeTheEnd - counter);
            const int awayBest = Bounds::solveBestCaseRank(tournament, awayTeam, numberRoundsBeforeTheEnd - counter);
            const int awayWorst = Bounds::solveWorstCaseRank(tournament, awayTeam, numberRoundsBeforeTheEnd - counter);

            const bool homeDecided = Bounds::teamIsDecidedChampionsLeague(homeBest, homeWorst);
            const bool awayDecided = Bounds::teamIsDecidedChampionsLeague(awayBest, awayWorst);

            const bool homeUndecided = !homeDecided;
            const bool awayUndecided = !awayDecided;


            if (!homeDecided) numberUndecidedRound++;
            else if (homeBest <= 8) numberQualifiersDecidedRound++;
            else if (homeWorst <= 24) numberPlayOffsDecidedRound++;
            else if (homeBest >= 25) numberRelegationDecidedRound++;

            if (!awayDecided) numberUndecidedRound++;
            else if (awayBest <= 8) numberQualifiersDecidedRound++;
            else if (awayWorst <= 24) numberPlayOffsDecidedRound++;
            else if (awayBest >= 25) numberRelegationDecidedRound++;

            // Count match-level relevance
            if (homeUndecided && awayUndecided) numberHighlyRelevantMatchesRound++;
            else if (homeUndecided || awayUndecided) numberPartiallyRelevantMatchesRound++;
            else numberIrrelevantMatchesRound++;
        }

        this->numberIrrelevantMatches[roundNumber] = numberIrrelevantMatchesRound;
        this->numberHighlyRelevantMatches[roundNumber] = numberHighlyRelevantMatchesRound;
        this->numberPartiallyRelevantMatches[roundNumber] = numberPartiallyRelevantMatchesRound;

        this->numberQualifiersDecided[roundNumber] = numberQualifiersDecidedRound;
        this->numberPlayOffsDecided[roundNumber] = numberPlayOffsDecidedRound;
        this->numberRelegationDecided[roundNumber] = numberRelegationDecidedRound;
        this->numberUndecided[roundNumber] = numberUndecidedRound;
        counter++;
    }
}
