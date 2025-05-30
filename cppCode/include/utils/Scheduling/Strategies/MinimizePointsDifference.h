#ifndef MINIMIZEAVERAGEPOINTSSTRATEGY_H
#define MINIMIZEAVERAGEPOINTSSTRATEGY_H

#include "SchedulingStrategy.h"
#include "tournament/Tournament.h"

class MinimizePointsDifference final : public SchedulingStrategy {

    public:
    explicit MinimizePointsDifference();
    void setObjective(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x, Tournament& tournament) const override;
};



#endif //MINIMIZEAVERAGEPOINTSSTRATEGY_H
