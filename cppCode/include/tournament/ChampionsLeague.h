//
// Created by dewae on 31/03/2025.
//

#ifndef CHAMPIONSLEAGUE_H
#define CHAMPIONSLEAGUE_H
#include "Tournament.h"
#include "utils/Scheduling/Schedulers/ChampionsLeagueScheduler.h"


class ChampionsLeague final :  public Tournament {

private:
    ChampionsLeagueScheduler scheduler;

public:
    ChampionsLeague(int numberTeams, int numberDynamicRounds); // constructor



    // Override the pure virtual functions
    void makeInitialSchedule(std::mt19937& randomNumberGenerator) override;
    void makeSecondSchedule() override;


};



#endif //CHAMPIONSLEAGUE_H
