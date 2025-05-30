#include "utils/Scheduling/Strategies/MinimizePointsDifference.h"



MinimizePointsDifference::MinimizePointsDifference() = default;


void MinimizePointsDifference::setObjective(const IloEnv& env, const IloModel& model,  const std::vector<std::vector<std::vector<IloNumVar>>>& x, Tournament& tournament) const {
    IloExpr expr(env);

    for (int i = 0; i < tournament.getNumberTeams(); ++i) {
        for (int j = 0; j < tournament.getNumberTeams(); ++j) {
            if (i != j) {
                const int pointsDifference = std::abs(tournament.getTeamManager()->getTeam(i)->getPoints() - tournament.getTeamManager()->getTeam(j)->getPoints());
                for (int s = 0; s < tournament.getNumberDynamicRounds(); ++s) {
                    expr += pointsDifference * x[i][j][s];
                }
            }
        }
    }

    model.add(IloMinimize(env, expr));
}