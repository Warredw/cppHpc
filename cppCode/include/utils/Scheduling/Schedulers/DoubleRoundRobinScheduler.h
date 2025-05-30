#ifndef DOUBLEROUNDROBINSCHEDULER_H
#define DOUBLEROUNDROBINSCHEDULER_H

#include "utils/Scheduling/Schedulers/Scheduler.h"
class Tournament;

class DoubleRoundRobinScheduler final : public Scheduler {

    std::vector<std::vector<int>> hapSetSecondHalf;

public:


    void setInitialSchedule(Tournament &tournament, std::mt19937& randomNumberGenerator) override;
    void setSecondSchedule(Tournament &tournament) const override;


    static void addPlayOnceInFirstPartConstraint(const IloEnv& env, const IloModel& model , const std::vector<std::vector<std::vector<IloNumVar>>>&x, int numberRounds, const Tournament& tournament);
    static void addHapConstraint(const IloModel& model, const IloEnv& env, const HapSet& hapSet, const vector<vector<vector<IloNumVar>>>& x, bool firstHalf, const Tournament& tournament);
    static void addHapSecondHalfConstraint(const IloModel& model, const IloEnv& env, const std::vector<std::vector<int>>& hap, const vector<vector<vector<IloNumVar>>>& x, const Tournament& tournament) ;
    static void addOppositeVenueConstraint(const IloModel& model, const IloEnv& env, const Tournament& tournament, const vector<vector<vector<IloNumVar>>>& x, std::vector<std::vector<int>>& firstHalfHomeAway);

    static void addReversedConstraints(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>&x, int numberRounds, Tournament& tournament);
    static void setOptimizationObjective(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x, const Tournament& tournament, std::mt19937& randomNumberGenerator);



};

#endif //DOUBLEROUNDROBINSCHEDULER_H
