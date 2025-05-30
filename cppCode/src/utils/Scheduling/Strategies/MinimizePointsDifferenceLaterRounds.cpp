#include "utils/Scheduling/Strategies/MinimizePointsDifferenceLaterRounds.h"

MinimizePointsDifferenceLaterRounds::MinimizePointsDifferenceLaterRounds() = default;

void MinimizePointsDifferenceLaterRounds::setObjective(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x, Tournament& tournament) const {
    IloExpr objective(env);

    const int numberRounds = tournament.getNumberRounds() / 2;
    const int numberTeams = tournament.getNumberTeams();

    for (int t = 0; t < numberRounds; t++) {
        const double weight = (t >= numberRounds - 5 && t < numberRounds - 1) ? 1000.0 : static_cast<double>((t * 100000 + 1) * (t * 500 + 1)) / numberRounds; // Higher weight for rounds closer to the end
        for (int i = 0; i < numberTeams; i++) {
            for (int j = 0; j < numberTeams; j++) {
                if (i != j) {
                    const int pointsDifference = std::abs(tournament.getTeamManager()->getTeam(i)->getPoints() - tournament.getTeamManager()->getTeam(j)->getPoints());
                    objective += weight * pointsDifference * x[i][j][t];
                }
            }
        }
    }
    model.add(IloMinimize(env, objective));
    objective.end();
}