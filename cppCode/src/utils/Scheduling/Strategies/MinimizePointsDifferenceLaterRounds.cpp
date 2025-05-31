#include "utils/Scheduling/Strategies/MinimizePointsDifferenceLaterRounds.h"

MinimizePointsDifferenceLaterRounds::MinimizePointsDifferenceLaterRounds() = default;

void MinimizePointsDifferenceLaterRounds::setObjective(const IloEnv &env, const IloModel &model, const std::vector<std::vector<std::vector<IloNumVar> > > &x, Tournament &tournament) const {


    if(tournament.getNumberDynamicRounds() > 0) {
        IloExpr expr(env);

        const int numTeams       = tournament.getNumberTeams();
        const int numDynamic     = tournament.getNumberDynamicRounds(); // should be 19
        const int K              = 4;                                  // last 4 rounds
        const int startRound     = numDynamic - K;                     // zero-based index

        // Loop only over Rounds 36, 37, 38, 39 (i.e. dynamic indices 15–18)
        for (int s = startRound; s < numDynamic; ++s) {
            // Optional: you can ramp up weight so that Round 38 matters the most
            double w = 1.0 + 0.2 * (s - startRound);
            for (int i = 0; i < numTeams; ++i) {
                for (int j = i + 1; j < numTeams; ++j) {
                    double gap = std::abs(
                        tournament.getTeamManager()->getTeam(i)->getPoints()
                      - tournament.getTeamManager()->getTeam(j)->getPoints()
                    );
                    expr += w * gap * (x[i][j][s] + x[j][i][s]);
                }
            }
        }

        model.add(IloMinimize(env, expr));
        expr.end();
    }
}
