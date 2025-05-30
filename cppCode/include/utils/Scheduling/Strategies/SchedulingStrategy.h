#ifndef SCHEDULINGSTRATEGY_H
#define SCHEDULINGSTRATEGY_H

#include <ilcplex/ilocplex.h>
#include <vector>

class Tournament;
class SchedulingStrategy {
public:
    virtual ~SchedulingStrategy() = default;
    virtual void setObjective(const IloEnv& env, const IloModel& model,  const std::vector<std::vector<std::vector<IloNumVar>>>& x,  Tournament& tournament) const = 0;

};



#endif //SCHEDULINGSTRATEGY_H
