#include "tournament/ChampionsLeague.h"
#include "utils/Scheduling/Schedulers/ChampionsLeagueScheduler.h"
#include "utils/TeamManager/ChampionsLeagueTeamManager.h"

ChampionsLeague::ChampionsLeague(const int numberTeams, const int numberDynamicRounds) :Tournament(numberTeams, 8, numberDynamicRounds) {
    scheduler = ChampionsLeagueScheduler();
    teamManager = new ChampionsLeagueTeamManager;
}


void ChampionsLeague::makeInitialSchedule(std::mt19937 &randomNumberGenerator) {
    scheduler.setInitialSchedule(*this, randomNumberGenerator);
}

void ChampionsLeague::makeSecondSchedule() {
    scheduler.setSecondSchedule(*this);
}

