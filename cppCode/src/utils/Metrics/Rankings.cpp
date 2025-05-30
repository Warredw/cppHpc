#include "utils/Metrics/Rankings.h"
#include <stdexcept>


void Rankings::setRanking(Tournament &tournament) {

    int roundCounter = 0;

    // go over all the rounds
    for (auto &round : tournament.getCompleteTimeTable().getRounds()) {

        // go over all the matches
        for (const auto &match : round.getMatches()) {

            // get the teams
            const auto homeTeam = match.getHomeTeam();
            const auto awayTeam = match.getAwayTeam();

            // ensure teams are not null
            if (!homeTeam || !awayTeam) {
                throw std::runtime_error("Home or away team is null");
            }

            // get the ratings
            const double homeTeamRating = match.getRatingHomeTeamAfterMatch();
            const double awayTeamRating = match.getRatingAwayTeamAfterMatch();

            // get the points after the match
            const double homeTeamPoints = match.getPointsHomeTeamAfterMatch();
            const double awayTeamPoints = match.getPointsAwayTeamAfterMatch();

            // get the goal saldo for both teams (problem because it takes the one at the end)
            const int saldoHomeTeam = match.getTiebreakerHomeTeamAfterMatch();
            const int saldoAwayTeam = match.getTieBreakerAwayTeamAfterMatch();

            // now store it
            rankingInfo[roundCounter][homeTeam] = std::make_tuple(homeTeamRating, homeTeamPoints, saldoHomeTeam);
            rankingInfo[roundCounter][awayTeam] = std::make_tuple(awayTeamRating, awayTeamPoints, saldoAwayTeam);
        }

        // increment the round counter
        roundCounter++;
    }
}

std::map<int, std::map<std::shared_ptr<Team>, std::tuple<double, int, int>>> Rankings::getRankingInfo() const {
    return rankingInfo;
}