#include "utils/Scheduling/Strategies/MinimizeRatingDifference.h"


MinimizeRatingDifference::MinimizeRatingDifference() = default;


void MinimizeRatingDifference::setObjective(const IloEnv& env, const IloModel& model,  const std::vector<std::vector<std::vector<IloNumVar>>>& x, Tournament& tournament) const{
    IloExpr expr(env);

    for (int i = 0; i < tournament.getNumberTeams(); ++i) {
        for (int j = 0; j < tournament.getNumberTeams(); ++j) {
            if (i != j) {
                const double ratingDifference = std::abs(tournament.getTeamManager()->getTeam(i)->getRating() - tournament.getTeamManager()->getTeam(j)->getRating());
                for (int s = 0; s < tournament.getNumberDynamicRounds(); ++s) {
                    expr += ratingDifference * x[i][j][s];
                }
            }
        }
    }
    model.add(IloMinimize(env, expr));
}
