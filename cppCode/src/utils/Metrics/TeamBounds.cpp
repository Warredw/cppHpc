#include "utils/Metrics/TeamBounds.h"
#include "utils/Metrics/Bounds.h"


std::map<int, std::map<std::shared_ptr<Team>, std::pair<int, int>>> TeamBounds::getTeamBounds() const {
    return teamBounds;
}


void TeamBounds::setTeamBounds(Tournament &tournament, const int n) {

    const int numberRoundsTournament = tournament.getNumberRounds();

    // For all the last n rounds we need to calculate the highest and lowest position for each team
    for(int i = n; i > 0; i--) {

        // check the current round number
        const int currentRound = numberRoundsTournament - i;

        // go over all the teams
        for(const auto& team: tournament.getTeamManager()->getTeams()) {
            const int highest = Bounds::solveBestCaseRank(tournament, team, i);
            const int lowest = Bounds::solveWorstCaseRank(tournament, team, i);

            // set in the dataStructure
            teamBounds[currentRound][team] = std::make_pair(highest, lowest);
        }
    }
}
