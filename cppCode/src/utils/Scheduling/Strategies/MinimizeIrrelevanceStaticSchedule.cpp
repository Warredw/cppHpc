#include "utils/Scheduling/Strategies/MinimizeIrrelevanceStaticSchedule.h"

#include <map>

#include "tournament/Tournament.h"
#include "utils/Metrics/RoundStatistics.h"

MinimizeIrrelevanceStaticSchedule::MinimizeIrrelevanceStaticSchedule() = default;

void MinimizeIrrelevanceStaticSchedule::setObjective(
    const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x,  Tournament& tournament) const {

    IloExpr expr(env);
    const int numberOfRounds = tournament.getNumberDynamicRounds();
    const int numberOfTeams = tournament.getNumberTeams();

    // Use the ranking at halfway (round 18) as a proxy for team relevance
    auto halfwayRanking = RoundStatistics::makeRanking(tournament.getFirstTimeTable().getRound(18));
    std::map<std::shared_ptr<Team>, int> halfwayRanks;

    for (int i = 0; i < numberOfTeams; ++i) {
        halfwayRanks[halfwayRanking[i].first] = halfwayRanking[i].second;
    }

    for (int i = 0; i < numberOfTeams; ++i) {
        for (int j = i + 1; j < numberOfTeams; ++j) {
            std::shared_ptr<Team> teamI = tournament.getTeamManager()->getTeam(i);
            std::shared_ptr<Team> teamJ = tournament.getTeamManager()->getTeam(j);

            int rankI = halfwayRanks[teamI];
            int rankJ = halfwayRanks[teamJ];

            double weight = getMatchPenalty(rankI, rankJ); // relevant vs relevant = some risk, irrelevant vs irrelevant = worst

            // Only apply objective in the final round
            int finalRound = numberOfRounds - 1;
            expr += weight * x[i][j][finalRound];
        }
    }

    model.add(IloMinimize(env, expr));
    expr.end();
}


double MinimizeIrrelevanceStaticSchedule::getMatchPenalty(int rankI, int rankJ) {
    // Weight = how important it is to avoid "resolving" this team too early
    auto relevanceWeight = [](int rank) -> double {
        if (rank == 1) return 0.2;       // Title race usually resolved
        if (rank <= 4) return 0.5;       // CL race — some suspense
        if (rank <= 6) return 0.9;       // Europa — often undecided
        if (rank == 7) return 1.2;       // Conference = single spot → very suspenseful
        if (rank <= 13) return 0.3;      // Mid-table — likely irrelevant
        if (rank <= 17) return 0.6;      // Safe zone above relegation
        return 0.8;                      // Bottom 3 — often still in play
    };

    double w1 = relevanceWeight(rankI);
    double w2 = relevanceWeight(rankJ);

    // Penalize relevant vs relevant — they resolve each other
    // Reward mixed matches (1 relevant + 1 irrelevant)
    // Penalize irrelevant vs irrelevant (dead rubber)
    return w1 * w2;
}