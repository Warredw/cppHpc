// Created by dewae on 31/05/2025.
//

#ifndef STATICOBJECTIVE_H
#define STATICOBJECTIVE_H



#include "tournament/Tournament.h"
#include "SchedulingStrategy.h"


class StaticObjective final : public SchedulingStrategy {

public:
    explicit StaticObjective();
    void setObjective(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x,  Tournament& tournament) const override;

};



#endif //STATICOBJECTIVE_H
