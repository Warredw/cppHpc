#include "utils/Scheduling/Strategies/StaticObjective.h"


StaticObjective::StaticObjective() = default;



void StaticObjective::setObjective(const IloEnv &env, const IloModel &model, const std::vector<std::vector<std::vector<IloNumVar> > > &x, Tournament &tournament) const {



        IloExpr expr(env);

        const int numTeams       = tournament.getNumberTeams();

        //one indexed rounds: 1-4, 8-11, 16-19
        // Loop only over Rounds 36, 37, 38, 39 (i.e. dynamic indices 15–18)
        for (int s = 15; s < 19; ++s) {
            // Optional: you can ramp up weight so that Round 38 matters the most
            for (int i = 0; i < numTeams; ++i) {
                for (int j = i + 1; j < numTeams; ++j) {
                    double diff = std::abs(
                        tournament.getTeamManager()->getTeam(i)->getRating()
                      - tournament.getTeamManager()->getTeam(j)->getRating()
                    );
                    expr += diff * (x[i][j][s] + x[j][i][s]);
                }
            }
        }
        model.add(IloMinimize(env, expr));
        expr.end();

}
