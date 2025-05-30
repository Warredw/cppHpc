#include "../../../include/utils/Metrics/Bounds.h"
#include "tournament/Tournament.h"

// Functions related to calculating the lowest and highest possible position of a given team in the tournament
std::map<int, std::vector<Match>> Bounds::getRemainingMatches(Tournament &tournament, const int roundsBeforeTheEnd) {

    // There is a difference between champions league teams and non-champions league teams
    if(tournament.getTeamManager()->getTeam(0)->getAssociation().empty()) { // TO check whether a team is a champions league team
        std::map<int, std::vector<Match>> remainingMatches;

        const int totalRounds = tournament.getCompleteTimeTable().getNumberOfRounds();

        for (int round = totalRounds - roundsBeforeTheEnd; round < totalRounds; round++) {
            remainingMatches[round] = tournament.getCompleteTimeTable().getMatchesInRound(round);
        }


        return remainingMatches;

    }

    // for champions league teams here
    std::map<int, std::vector<Match>> remainingMatches;

    const int totalRounds = 8;


    for (int round = totalRounds - roundsBeforeTheEnd; round < totalRounds; round++) {
        remainingMatches[round] = tournament.getCompleteTimeTable().getMatchesInRound(round);
    }



    return remainingMatches;

}


std::map<std::shared_ptr<Team>, int> Bounds::getInitialPoints(const std::map<int, std::vector<Match>> &remainingMatches) {

    // Store each team's points before the remaining matches
    std::map<std::shared_ptr<Team>, int> pointsBeforeMatch;
    for (auto it = remainingMatches.begin(); it != remainingMatches.end(); ++it) {
        for (const auto &match : it->second) {
            std::shared_ptr<Team> team1 = match.getHomeTeam();
            std::shared_ptr<Team> team2 = match.getAwayTeam();

            if (pointsBeforeMatch.find(team1) == pointsBeforeMatch.end())
                pointsBeforeMatch[team1] = match.getPointsHomeTeamBeforeMatch();
            if (pointsBeforeMatch.find(team2) == pointsBeforeMatch.end())
                pointsBeforeMatch[team2] = match.getPointsAwayTeamBeforeMatch();
        }
    }

    return pointsBeforeMatch;
}


// Function returns 3 decision variables for each remaining match (because there are 3 outcomes possible)
std::vector<IloBoolVarArray> Bounds::defineMatchOutcomeVariables(const IloEnv &env, const IloModel &model, std::map<int, std::vector<Match>> &remainingMatches) {

    // Get the total number of remaining matches
    int numberOfMatchesRemaining = 0;
    for (auto it = remainingMatches.begin(); it != remainingMatches.end(); ++it) {
        numberOfMatchesRemaining += static_cast<int>(it->second.size());
    }

    // Create an array for match outcome variables
    std::vector<IloBoolVarArray> matchOutcomeVars(numberOfMatchesRemaining);

    int matchIndex = 0; // Track index for match outcome variables

    // Iterate through rounds and matches
    for (auto it = remainingMatches.begin(); it != remainingMatches.end(); ++it) {
        std::vector<Match>& matches = it->second;

        for (size_t i = 0; i < matches.size(); i++) {

            // Create an array of 3 Boolean variables for this match.
            IloBoolVarArray outcomeVars(env, 3);
            for (int outcome = 0; outcome < 3; outcome++) {
                outcomeVars[outcome] = IloBoolVar(env);
            }

            // Constraint: Exactly one outcome per match (win, draw, or loss)
            IloExpr sumExpr(env);
            for (int outcome = 0; outcome < 3; outcome++) {
                sumExpr += outcomeVars[outcome];
            }
            model.add(sumExpr == 1);
            sumExpr.end();

            // Store outcome variables for this match
            matchOutcomeVars[matchIndex] = outcomeVars;
            matchIndex++;
        }
    }
    return matchOutcomeVars;
}


std::map<std::shared_ptr<Team>,IloIntVar> Bounds::defineFinalPointsVariables(const IloEnv &env, const IloModel &model, std::vector<IloBoolVarArray> &matchOutcomeVars, std::map<int, std::vector<Match> > &remainingMatches, const Tournament& tournament) {

    // Create the final points array for each team
    std::map<std::shared_ptr<Team>,IloIntVar> finalPoints;

    // Store each team's points before the remaining matches
    std::map<std::shared_ptr<Team>, int> pointsBeforeMatch = getInitialPoints(remainingMatches);

    int matchIndex = 0; // Track match index

    // Define final points for each team
    for (std::shared_ptr<Team>& team: tournament.getTeamManager()->getTeams()) {
        const int initialPoints = pointsBeforeMatch[team];

        finalPoints[team] = IloIntVar(env, initialPoints, static_cast<int>(initialPoints + 3 * remainingMatches.size()));
        model.add(finalPoints[team]);

        IloExpr pointsExpr(env);
        pointsExpr += initialPoints;

        // Accumulate points from remaining matches
        matchIndex = 0; // Reset match index for each team
        for (auto it = remainingMatches.begin(); it != remainingMatches.end(); ++it) {
            for (const auto &match : it->second) {
                    std::shared_ptr<Team> team1 = match.getHomeTeam();
                    std::shared_ptr<Team> team2 = match.getAwayTeam();

                if (team == team1) {
                    pointsExpr += (3 * matchOutcomeVars[matchIndex][0] + 1 * matchOutcomeVars[matchIndex][1]);
                }
                if (team == team2) {
                    pointsExpr += (3 * matchOutcomeVars[matchIndex][2] + 1 * matchOutcomeVars[matchIndex][1]);
                }

                matchIndex++;
            }
        }

        model.add(finalPoints[team] == pointsExpr);
        pointsExpr.end();
    }
    return finalPoints;
}

std::map<std::shared_ptr<Team>, IloBoolVar> Bounds::defineRankingConstraints(const IloEnv &env, const IloModel &model, std::map<std::shared_ptr<Team>, IloIntVar> &finalPoints,
    const std::shared_ptr<Team>& targetTeam, Tournament &tournament,
        const std::map<int, std::vector<Match>> &remainingMatches, const std::map<std::shared_ptr<Team>,int>& initialPoints) {

    std::map<std::shared_ptr<Team>, IloBoolVar> rankingVars;

    // Iterate over all teams and define ranking constraints
    for (auto it = finalPoints.begin(); it != finalPoints.end(); ++it) {
        if (std::shared_ptr<Team> team = it->first; team != targetTeam) {

            // Define binary ranking variable c_m
            rankingVars[team] = IloBoolVar(env);

            const int maxPossiblePointsT = static_cast<int>(initialPoints.at(targetTeam) + 3 * remainingMatches.size());
            const int maxPossiblePointsTeam = static_cast<int>(initialPoints.at(team) + 3 * remainingMatches.size());


            // Constraint 5a: If target team has more points, it is ranked higher
            model.add(finalPoints[targetTeam] - finalPoints[team]
                      <= rankingVars[team] * (maxPossiblePointsT - initialPoints.at(team)));

            // Constraint 5b: If target team has fewer points, it is ranked lower
             model.add(finalPoints[targetTeam] - finalPoints[team]
                >= (1 - rankingVars[team]) * (initialPoints.at(targetTeam) - maxPossiblePointsTeam));
        }
    }
    return rankingVars;
}


int Bounds::solveWorstCaseRank(Tournament &tournament, const std::shared_ptr<Team>& targetTeam, const int roundsBeforeTheEnd) {
    IloEnv env;
    int worstRank = -1;

    try {
        const IloModel model(env);
        IloCplex cplex(model);

        // Step 1: Get Remaining Matches
        std::map<int, std::vector<Match>> remainingMatches = getRemainingMatches(tournament, roundsBeforeTheEnd);

        // Step 2: Define Match Outcome Variables
        std::vector<IloBoolVarArray> matchOutcomeVars = defineMatchOutcomeVariables(env, model, remainingMatches);

        // Step 3: Define Team Final Points Variables
        std::map<std::shared_ptr<Team>, IloIntVar> finalPoints = defineFinalPointsVariables(env, model, matchOutcomeVars, remainingMatches, tournament);

        // Step 4: Define Initial Points
        const std::map<std::shared_ptr<Team>, int> initialPoints = getInitialPoints(remainingMatches);

        // Step 5: Define Ranking Constraints
        const std::map<std::shared_ptr<Team>, IloBoolVar> rankingVars = defineRankingConstraints(env, model, finalPoints, targetTeam, tournament, remainingMatches, initialPoints);

        // Step 6: Create Objective Function: Maximize the worst possible rank
        IloExpr expr(env);
        for (const auto& [team, rankVar] : rankingVars) {
            expr += rankVar;  // Count the number of teams ranked below the target
        }
        model.add(IloMaximize(env, tournament.getNumberTeams() - expr));

        // Step 7: Solve
        if (cplex.solve()) {
            worstRank = static_cast<int>(cplex.getValue(tournament.getNumberTeams() - expr));
            std::cout<<"Bound Solution Found" << std::endl;
        } else {
            std::cerr << "CPLEX Failed to Solve Worst-Case Rank" << std::endl;
        }


        expr.end();
    } catch (IloException &e) {
        std::cerr << "CPLEX Exception: " << e.getMessage() << std::endl;
    }

    env.end();
    return worstRank;
}


int Bounds::solveBestCaseRank(Tournament &tournament, const std::shared_ptr<Team>& targetTeam, const int roundsBeforeTheEnd) {
    IloEnv env;
    int bestRank = -1;

    try {
        const IloModel model(env);
        IloCplex cplex(model);

        // Step 1: Get Remaining Matches
        std::map<int, std::vector<Match>> remainingMatches = getRemainingMatches(tournament, roundsBeforeTheEnd);

        // Step 2: Define Match Outcome Variables
        std::vector<IloBoolVarArray> matchOutcomeVars = defineMatchOutcomeVariables(env, model, remainingMatches);

        // Step 3: Define Team Final Points Variables
        std::map<std::shared_ptr<Team>, IloIntVar> finalPoints = defineFinalPointsVariables(env, model, matchOutcomeVars, remainingMatches, tournament);

        // Step 4: Define Initial Points
        const std::map<std::shared_ptr<Team>, int> initialPoints = getInitialPoints(remainingMatches);

        // Step 5: Define Ranking Constraints (Using dynamically computed Big-M)
        const std::map<std::shared_ptr<Team>, IloBoolVar> rankingVars = defineRankingConstraints(env, model, finalPoints, targetTeam, tournament, remainingMatches, initialPoints);

        // Step 6: Create Objective Function: Minimize the best possible rank
        IloExpr expr(env);
        for (const auto& [team, rankVar] : rankingVars) {
            expr += rankVar;  // Count the number of teams ranked below the target
        }
        model.add(IloMinimize(env, tournament.getNumberTeams() - expr));

        // Step 7: Solve
        if (cplex.solve()) {
            bestRank = static_cast<int>(cplex.getValue(tournament.getNumberTeams() - expr));
        } else {
            std::cerr << "CPLEX Failed to Solve Best-Case Rank" << std::endl;
        }

        expr.end();
    } catch (IloException &e) {
        std::cerr << "CPLEX Exception: " << e.getMessage() << std::endl;
    }
    env.end();
    return bestRank;
}

bool Bounds::matchIsRelevant(const int highestRankPossible, const int lowestRankPossible) {

    // Team can still play for the title
    if(highestRankPossible == 1 && lowestRankPossible != 1) {
        return true;
    }

    // Team can still play for the champions league
    if(highestRankPossible <= 4 && lowestRankPossible > 4) {
        return true;
    }

    // Team can still play for Europa League
    if(highestRankPossible <= 6 && lowestRankPossible > 7) {
        return true;
    }

    // Team can still play for conference league
    if(highestRankPossible <= 7 && lowestRankPossible > 7) {
        return true;
    }

    // Team still has to fight against relegation
    if(highestRankPossible <= 17 && lowestRankPossible > 17) {
        return true;
    }

    return false;
}


bool Bounds::teamIsDecidedChampionsLeague(int best, int worst) {
    return (best <= 8 && worst <= 8) ||     // top 8 secured
           (best >= 9 && worst <= 24) ||    // 9–24 secured
           (best >= 25 && worst >= 25);     // eliminated
}


bool Bounds::teamIsDecidedPremierLeague(const int highestRankPossible, const int lowestRankPossible) {
    // 1. Title (only if 1st is the only possible rank)
    if (highestRankPossible == 1 && lowestRankPossible == 1)
        return true;

    // 2. Champions League (2–4 only, not 1st, not 5th)
    if (highestRankPossible >= 2 && lowestRankPossible <= 4 &&
        highestRankPossible <= 4 && lowestRankPossible >= 2)
        return true;

    // 3. Europa League (5–6 only)
    if (highestRankPossible >= 5 && lowestRankPossible <= 6 &&
        highestRankPossible <= 6 && lowestRankPossible >= 5)
        return true;

    // 4. Conference League (exactly 7th)
    if (highestRankPossible == 7 && lowestRankPossible == 7)
        return true;

    // 5. No prize (8–17 only)
    if (highestRankPossible >= 8 && lowestRankPossible <= 17 &&
        highestRankPossible <= 17 && lowestRankPossible >= 8)
        return true;

    // 6. Relegation (18–20 only)
    if (highestRankPossible >= 18 && lowestRankPossible >= 18)
        return true;

    // Otherwise, team is still undecided
    return false;
}




bool Bounds::matchIsRelevantChampionsLeague(int best, int worst) {
    return !teamIsDecidedChampionsLeague(best, worst);
}

std::string Bounds::matchType(const int highestRankPossibleTeam1, const int lowestRankPossibleTeam1, const int highestRankPossibleTeam2, const int lowestRankPossibleTeam2) {

    bool team1NoStakes = false;
    bool team2NoStakes = false;

    if(highestRankPossibleTeam1 >= 8 && lowestRankPossibleTeam1 <= 17) team1NoStakes = true;
    if(highestRankPossibleTeam2 >= 8 && lowestRankPossibleTeam1 <= 17) team2NoStakes = true;



    if ((highestRankPossibleTeam1 == 1 && lowestRankPossibleTeam1 != 1) && (highestRankPossibleTeam2 == 1 && lowestRankPossibleTeam2 != 1)) {
        return "Title Match";
    }

    if ((highestRankPossibleTeam1 <= 4 && lowestRankPossibleTeam1 > 4) && (highestRankPossibleTeam2 <= 4 && lowestRankPossibleTeam2 > 4)) {
        return "Champions League Match";
    }

    if ((highestRankPossibleTeam1 <= 17 && lowestRankPossibleTeam1 > 17) && (highestRankPossibleTeam2 <= 17 && lowestRankPossibleTeam2 > 17)) {
        return "Relegation Match";
    }

    if(team1NoStakes && team2NoStakes) return "No stakes match";

    if((highestRankPossibleTeam1 == 1 && lowestRankPossibleTeam1 != 1 && team2NoStakes) || (highestRankPossibleTeam2 == 1 && lowestRankPossibleTeam2 != 1 && team1NoStakes)) return "Title vs No Stakes Match";

    if((highestRankPossibleTeam1 <= 4 && lowestRankPossibleTeam1 > 4 && team2NoStakes) || (highestRankPossibleTeam2 <= 4 && lowestRankPossibleTeam2 > 4 && team1NoStakes)) return "Champions League vs No Stakes Match";

    if((lowestRankPossibleTeam1 >= 18 && highestRankPossibleTeam1 > 18 && team2NoStakes) || (lowestRankPossibleTeam2 >= 18 && highestRankPossibleTeam2 > 18 && team1NoStakes)) return "Relegation vs No Stakes Match";

    if((highestRankPossibleTeam1 == 5 || highestRankPossibleTeam1 == 6) && lowestRankPossibleTeam1 > 6 && team2NoStakes) return "Europe League vs No Stakes Match";

    if((highestRankPossibleTeam2 == 5 || highestRankPossibleTeam2 == 6) && lowestRankPossibleTeam2 > 6 && team1NoStakes) return "Europe League vs No Stakes Match";

    if(highestRankPossibleTeam1 == 7 & lowestRankPossibleTeam1 > 7 && team2NoStakes) return "Conference League vs No Stakes Match";

    if(highestRankPossibleTeam2 == 7 & lowestRankPossibleTeam2 > 7 && team1NoStakes) return "Conference League vs No Stakes Match";

    return "Other";
}

