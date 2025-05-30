#ifndef DRR_H
#define DRR_H

#include "tournament/Tournament.h"
#include "utils/Scheduling/Schedulers/DoubleRoundRobinScheduler.h"


class DoubleRoundRobin: public Tournament {


private:
    DoubleRoundRobinScheduler scheduler;

public:

    // Getters
    DoubleRoundRobinScheduler& getScheduler();


    DoubleRoundRobin(int numberTeams, int dynamicRounds);


    // Override the virtual functions
    void makeInitialSchedule(std::mt19937& randomNumberGenerator) override;
    void makeSecondSchedule() override;


};



#endif //DRR_H
