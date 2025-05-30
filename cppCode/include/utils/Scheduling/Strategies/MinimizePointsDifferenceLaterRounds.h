#ifndef MINIMIZEPOINTSDIFFERENCELATERROUNDS_H
#define MINIMIZEPOINTSDIFFERENCELATERROUNDS_H

#include "SchedulingStrategy.h"
#include "tournament/Tournament.h"

class MinimizePointsDifferenceLaterRounds final : public SchedulingStrategy {
public:
    explicit MinimizePointsDifferenceLaterRounds();
    void setObjective(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x, Tournament& tournament) const override;
};

#endif // MINIMIZEPOINTSDIFFERENCELATERROUNDS_H