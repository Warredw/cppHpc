#ifndef MINIMIZEIRRELEVANCESTATICSCHEDULE_H
#define MINIMIZEIRRELEVANCESTATICSCHEDULE_H

#include <ilcplex/ilocplex.h>
#include <vector>
#include <memory>

#include "SchedulingStrategy.h"

class Tournament;
class Team;

class MinimizeIrrelevanceStaticSchedule final: public SchedulingStrategy{
public:
    MinimizeIrrelevanceStaticSchedule();
    void setObjective(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x,  Tournament& tournament) const override;

    static double getMatchWeight(int r1, int r2);

    static double getMatchPenalty(int rankI, int rankJ);
};

#endif // MINIMIZEIRRELEVANCESTATICSCHEDULE_H
