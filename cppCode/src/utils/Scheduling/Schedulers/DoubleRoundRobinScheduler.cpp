#include "utils/Scheduling/Schedulers/DoubleRoundRobinScheduler.h"
#include "tournament/Tournament.h"
#include "entities/HapSet.h"

// these are the static rounds
void DoubleRoundRobinScheduler::setInitialSchedule(Tournament &tournament, std::mt19937& randomNumberGenerator)  {

    IloEnv env;
    IloModel model(env);
    const auto x = initializeDecisionVariables(env, tournament.getNumberRounds()/2, tournament);
    HapSet hapSet("2222222221", "mirrored", randomNumberGenerator); // canonical one for now where we randomly shuffle

    hapSetSecondHalf.resize(tournament.getNumberTeams());
    for (int i = 0; i < tournament.getNumberTeams(); ++i) {
        hapSetSecondHalf[i].resize(tournament.getNumberRounds()/2);
    }

    // mirror the HapSet
    for(int i = 0; i < tournament.getNumberTeams(); i++) {
        for(int s = 0; s < tournament.getNumberRounds()/2; s++) {
            hapSetSecondHalf[i][s] = 1 - hapSet.getHapSetFirstHalf()[i][s];
        }
    }



    addPlayOnceInFirstPartConstraint(env, model, x, tournament.getNumberRounds() / 2, tournament); // each team needs to play once in the first half of the season
    addOneGamePerRoundConstraint(env, model, x, tournament.getNumberRounds() / 2, tournament); // every team needs to have one match in every round
    addHapConstraint(model, env, hapSet, x, true, tournament);
    setOptimizationObjective(env, model, x, tournament, randomNumberGenerator);


    IloCplex cplex(model);


    if (cplex.solve()) {
        std::cout<< "solution found"<< std::endl;
        tournament.setFirstTimeTable(extractTableFromSolution(cplex, x, tournament.getNumberRounds()/2, tournament));
    }
    env.end();
}




void DoubleRoundRobinScheduler::setSecondSchedule(Tournament &tournament)const {

    IloEnv env;
    const IloModel model(env);
    const auto x = initializeDecisionVariables(env, tournament.getNumberRounds()/2, tournament);

    const int reversedRounds = tournament.getNumberRounds()/2 - tournament.getNumberDynamicRounds();


    if(reversedRounds > 0) addReversedConstraints(env, model, x, reversedRounds, tournament);

    addOneGamePerRoundConstraint(env, model, x, tournament.getNumberRounds()/2, tournament);
    addPlayOnceInFirstPartConstraint(env, model, x, tournament.getNumberRounds()/2, tournament); // each team needs to play once in the first half of the

    std::vector<std::vector<int>> firstHalfHomeAway = tournament.getfirstHalfHomeAway();
    addOppositeVenueConstraint(model, env, tournament, x, firstHalfHomeAway);
    addHapSecondHalfConstraint(model, env, hapSetSecondHalf, x, tournament); // Only applicable to the second half of the season (first half is static or reversed)


    schedulingStrategy->setObjective(env, model, x, tournament);
    // set own objective



    IloCplex cplex(model);



    if(cplex.solve()) {
        std::cout << "solution found!" << std::endl;
        tournament.setSecondTimeTable(extractTableFromSolution(cplex,x, tournament.getNumberRounds()/2, tournament));
    }

    else {
        std::cout <<"no solution found!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    env.end();
}




// For a DRR tournament
void DoubleRoundRobinScheduler::addPlayOnceInFirstPartConstraint(const IloEnv& env, const IloModel& model , const std::vector<std::vector<std::vector<IloNumVar>>>&x, const int numberRounds, const Tournament& tournament) {

    for (int i = 0; i < tournament.getNumberTeams(); i++) {
        for(int j = i+1; j < tournament.getNumberTeams(); j++) {

            IloExpr expr(env);

            for(int s = 0 ; s < numberRounds; s++) {
                expr += x[i][j][s] + x[j][i][s];
            }
            model.add(expr == 1);
            expr.end();
        }
    }
}

// Function that will reverse the non-dynamic rounds in the second half
void DoubleRoundRobinScheduler::addReversedConstraints(const IloEnv &env, const IloModel &model, const std::vector<std::vector<std::vector<IloNumVar>>>&x, const int numberRounds, Tournament &tournament) {

    // Go over all the matches in the first timetable that need to be reversed
    for(int round = 0; round < numberRounds; round++) {

        for(const auto& match: tournament.getFirstTimeTable().getMatchesInRound(round)) {

            const int i = tournament.getTeamManager()->getTeamIndex(match.getHomeTeam());
            const int j = tournament.getTeamManager()->getTeamIndex(match.getAwayTeam());

            model.add(x[j][i][round] == 1);
        }
    }
}




void DoubleRoundRobinScheduler::addHapConstraint(const IloModel& model, const IloEnv& env, const HapSet& hapSet, const vector<vector<vector<IloNumVar>>>& x, const bool firstHalf, const Tournament& tournament) {
    const auto& hap = firstHalf ? hapSet.getHapSetFirstHalf() : hapSet.getHapSetSecondHalf();

    for (int i = 0; i < tournament.getNumberTeams(); i++) {
        for (int s = 0; s < tournament.getNumberRounds()/2; s++) {
            IloExpr homeExpr(env), awayExpr(env);
            for (int j = 0; j < tournament.getNumberTeams(); j++) {
                if (i != j) {
                    homeExpr += x[i][j][s];
                    awayExpr += x[j][i][s];
                }
            }
            model.add(IloRange(env, hap[i][s], homeExpr, hap[i][s]));
            model.add(IloRange(env, 1 - hap[i][s], awayExpr, 1 - hap[i][s]));
            homeExpr.end();
            awayExpr.end();
        }
    }
}


void DoubleRoundRobinScheduler::addHapSecondHalfConstraint(const IloModel &model, const IloEnv &env, const std::vector<std::vector<int>>& hap,  const vector<vector<vector<IloNumVar> > > &x, const Tournament &tournament) {

    for(int i = 0; i < tournament.getNumberTeams(); i++) {
        for(int s = 0; s < tournament.getNumberRounds()/2; s++) {

            IloExpr homeExpr(env), awayExpr(env);
            for(int j = 0; j < tournament.getNumberTeams(); j++) {
                if(i != j) {
                    homeExpr += x[i][j][s];
                    awayExpr += x[j][i][s];
                }
            }
            model.add(IloRange(env, hap[i][s], homeExpr, hap[i][s]));
            model.add(IloRange(env, 1 - hap[i][s], awayExpr, 1 - hap[i][s]));
            homeExpr.end();
            awayExpr.end();
        }
    }
}




void DoubleRoundRobinScheduler::addOppositeVenueConstraint(const IloModel& model, const IloEnv& env, const Tournament& tournament, const std::vector<std::vector<std::vector<IloNumVar>>>& x, std::vector<std::vector<int>>& firstHalfHomeAway) {

        for (int i = 0; i < tournament.getNumberTeams(); i++) {
            for (int j = 0; j < tournament.getNumberTeams(); j++) {

                if(i == j) continue;

                IloExpr expr1(env), expr2(env);

                for (int s = 0; s < tournament.getNumberRounds()/2; s++) {
                    expr1 += x[i][j][s];
                    expr2 += x[j][i][s];
                }

                if (firstHalfHomeAway[i][j] == 1) {
                    model.add(expr1 == 0);
                    model.add(expr2 == 1);

                } else if (firstHalfHomeAway[j][i] == 0) {
                    model.add(expr1 == 1);
                    model.add(expr2 == 0);
                }
                expr1.end();
                expr2.end();

            }
        }
}


void DoubleRoundRobinScheduler::setOptimizationObjective(const IloEnv &env, const IloModel &model, const std::vector<std::vector<std::vector<IloNumVar> > > &x, const Tournament &tournament, std::mt19937& randomNumberGenerator) {
    IloExpr expr(env);
    std::uniform_real_distribution<> dis(0.0, 1);

    for (int i = 0; i < tournament.getNumberTeams(); i++) {
        for (int j = 0; j < tournament.getNumberTeams(); j++) {
            for (int s = 0; s < tournament.getNumberRounds()/2; s++) {
                double randomCoefficient = dis(randomNumberGenerator);
                expr += randomCoefficient * x[i][j][s];
            }
        }
    }
    model.add(IloMaximize(env, expr)); // Set the objective to maximize the sum of decision variables with random coefficients
    expr.end();
}

