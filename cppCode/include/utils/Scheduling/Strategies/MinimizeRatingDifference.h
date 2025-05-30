
#ifndef MINIMIZERATINGDIFFERENCE_H
#define MINIMIZERATINGDIFFERENCE_H

#include "tournament/Tournament.h"
#include "SchedulingStrategy.h"


class MinimizeRatingDifference final : public SchedulingStrategy {

public:
    explicit MinimizeRatingDifference();
    void setObjective(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x,  Tournament& tournament) const override;

};



#endif //MINIMIZERATINGDIFFERENCE_H
