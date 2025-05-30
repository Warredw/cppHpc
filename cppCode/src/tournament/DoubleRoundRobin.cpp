#include "tournament/DoubleRoundRobin.h"
#include "utils/Scheduling/Schedulers/DoubleRoundRobinScheduler.h"
#include "utils/TeamManager/PremierLeague.h"


DoubleRoundRobin::DoubleRoundRobin(const int numberTeams, const int dynamicRounds)
    : Tournament(numberTeams, (numberTeams-1) * 2, dynamicRounds) {

    firstTimeTable.resizeRounds(numberTeams-1);
    secondTimeTable.resizeRounds(numberTeams-1);
    scheduler = DoubleRoundRobinScheduler();
    teamManager = new PremierLeague();
}



DoubleRoundRobinScheduler& DoubleRoundRobin::getScheduler() {
    return scheduler;
}


void DoubleRoundRobin::makeInitialSchedule(std::mt19937& randomNumberGenerator) {
    scheduler.setInitialSchedule(*this, randomNumberGenerator);
}

void DoubleRoundRobin::makeSecondSchedule() {
    scheduler.setSecondSchedule(*this);
}


