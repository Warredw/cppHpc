#include "utils/Scheduling/Schedulers/Scheduler.h"
#include "utils/Scheduling/Strategies/MinimizePointsDifference.h"
#include "entities/HapSet.h"
#include "tournament/DoubleRoundRobin.h"


Scheduler::Scheduler(): schedulingStrategy(nullptr) {};

Scheduler::~Scheduler() {
    delete schedulingStrategy;
}

// Setters
void Scheduler::setSchedulingStrategy(SchedulingStrategy *schedulingStrategy) {
    this->schedulingStrategy = schedulingStrategy;
}

// Constraints
std::vector<std::vector<std::vector<IloNumVar>>> Scheduler::initializeDecisionVariables(const IloEnv& env, const int numberRounds, const Tournament& tournament) {
    std::vector<std::vector<std::vector<IloNumVar>>> x(tournament.getNumberTeams());
    for (int i = 0; i < tournament.getNumberTeams(); i++) {
        x[i].resize(tournament.getNumberTeams());
        for (int j = 0; j < tournament.getNumberTeams(); j++) {
            x[i][j].resize(numberRounds);
            for (int z = 0; z < numberRounds; z++) {
                x[i][j][z] = IloNumVar(env, 0, 1, ILOBOOL);
            }
        }
    }
    return x;
}

void Scheduler::addPlayMaxOnceConstraint(const IloEnv& env, const IloModel& model , const std::vector<std::vector<std::vector<IloNumVar>>>&x, const int numberRounds, const Tournament& tournament) {
    for (int i = 0; i < tournament.getNumberTeams(); i++) {
        for(int j = i+1; j < tournament.getNumberTeams(); j++) {
            IloExpr expr(env);
            for(int s = 0 ; s < numberRounds; s++) {
                expr += x[i][j][s] + x[j][i][s];
            }
            model.add(expr <= 1);
            expr.end();
        }
    }
}

void Scheduler::addOneGamePerRoundConstraint(const IloEnv& env, const IloModel& model , const std::vector<std::vector<std::vector<IloNumVar>>>& x, const int numberRounds, const Tournament& tournament){
    const int nrTeams = tournament.getNumberTeams();

    for(int i = 0; i < nrTeams; i++) {
        for(int w = 0; w < numberRounds; w++) {
            IloExpr gamesForTeamInRound(env);
            for(int j = 0; j < nrTeams; j++) {
                if (i != j) gamesForTeamInRound += x[i][j][w] + x[j][i][w];
            }

            model.add(gamesForTeamInRound == 1);
            gamesForTeamInRound.end();
        }
    }
}

void Scheduler::addHomeAwayConstraint(const IloEnv& env, const IloModel& model , const std::vector<std::vector<std::vector<IloNumVar>>>& x, const int numberRounds, const Tournament& tournament) const{
    const int nrTeams = tournament.getNumberTeams();

    for(int i = 0; i < nrTeams; i++) {
        IloExpr totalHomeGames(env);
        IloExpr totalAwayGames(env);

        for(int j = 0; j < nrTeams; j++) {
            if (i != j) {
                for(int s = 0; s < numberRounds; s++) {
                    totalHomeGames += x[i][j][s];
                    totalAwayGames += x[j][i][s];
                }
            }
        }
        model.add(totalHomeGames - totalAwayGames <= 1);
        model.add(totalAwayGames - totalHomeGames <= 1);

        totalHomeGames.end();
        totalAwayGames.end();
    }
}


// We can't have matches in the second schedule that have already been played in the first schedule
void Scheduler::addNoRepeatOpponentsConstraint(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x, Tournament& tournament) const{
    for (int s = 0; s < tournament.getNumberNormalRounds(); s++) {
        for (const auto& match : tournament.getFirstTimeTable().getMatchesInRound(s)) {
            const int i = tournament.getTeamManager()->getTeamIndex(match.getHomeTeam());
            const int j = tournament.getTeamManager()->getTeamIndex(match.getAwayTeam());
            for (int t = 0; t < tournament.getNumberDynamicRounds(); t++) {
                model.add(x[i][j][t] + x[j][i][t] == 0);
            }
        }
    }
}

// Extract TimeTable
TimeTable Scheduler::extractTableFromSolution(const IloCplex& cplex, const std::vector<std::vector<std::vector<IloNumVar>>>& x, const int numberRounds,  Tournament& tournament) const {

    //tournament.resetHomeAwayGamesValue();

    TimeTable table(numberRounds);
    table.resizeRounds(numberRounds);
    for(int s = 0; s < numberRounds; s++) {
        for(int i = 0; i < tournament.getNumberTeams(); i++) {
            for(int j = 0; j < tournament.getNumberTeams(); j++) {

                if(i != j && cplex.getValue(x[i][j][s]) > 0.5) {

                    const Match match(tournament.getTeamManager()->getTeam(i), tournament.getTeamManager()->getTeam(j));
                    table.addMatchToRound(s,match);
                    match.getHomeTeam()->addHomeGame();
                    match.getAwayTeam()->addAwayGame();

                    //tournament.setHomeAwayGamesValue(i, j, s, 1);

                }
            }
        }
    }
    return table;
}



TimeTable Scheduler::extractTableFromSolutionChampionsLeague(const IloCplex &cplex, const std::vector<std::vector<std::vector<std::vector<IloNumVar> > > > &x, int numberRounds, Tournament &tournament) const {

    TimeTable table(numberRounds);
    table.resizeRounds(numberRounds);

    for(int week = 0; week < numberRounds; week++) {

        if(week != numberRounds-1) {

        }

        for(int i = 0; i < tournament.getNumberTeams(); i++) {

            for(int j = 0; j < tournament.getNumberTeams(); j++) {

                if(i == j) continue;
                if(cplex.getValue(x[i][j][week][0]) > 0.5) {
                    const Match match = Match(tournament.getTeamManager()->getTeam(i), tournament.getTeamManager()->getTeam(j), week);
                    table.addMatchToRound(week, match);
                    match.getHomeTeam()->addHomeGame();
                    match.getAwayTeam()->addAwayGame();
                }

                else if (cplex.getValue(x[i][j][week][1])> 0.5) {
                    const Match match = Match(tournament.getTeamManager()->getTeam(i), tournament.getTeamManager()->getTeam(j), week);
                    table.addMatchToRound(week, match);
                    match.getHomeTeam()->addAwayGame();
                    match.getAwayTeam()->addHomeGame();
                }
            }
        }
    }

    return table;
}

