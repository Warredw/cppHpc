//
// Created by dewae on 12/11/2024.
//

#ifndef MAXIMIZEPOINTSDIFFERENCE_H
#define MAXIMIZEPOINTSDIFFERENCE_H

#include "SchedulingStrategy.h"
#include "tournament/Tournament.h"

class MaximizePointsDifference final : public SchedulingStrategy {

public:
    explicit MaximizePointsDifference();
    void setObjective(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x, Tournament& tournament) const override;
};



#endif //MAXIMIZEPOINTSDIFFERENCE_H
