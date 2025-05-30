
#ifndef BOUNDS_H
#define BOUNDS_H
#include <vector>
#include <map>
#include <memory>
#include <map>
#include <vector>
#include <ilcplex/ilocplex.h>


// Everything related to Calculating the Highest and Lowest Possible Rank of a Team after a given round in the tournament

class Tournament;
class Team;
class Match;

class Bounds {

public:
    static std::map<int, std::vector<Match>> getRemainingMatches(Tournament &tournament, int roundsBeforeTheEnd);
    static std::map<std::shared_ptr<Team>, int> getInitialPoints(const std::map<int, std::vector<Match>> &remainingMatches);

    // Second step = make three decision variables for each match
    static std::vector<IloBoolVarArray> defineMatchOutcomeVariables(const IloEnv &env, const IloModel &model, std::map<int, std::vector<Match>> &remainingMatches);
    static std::map<std::shared_ptr<Team>,IloIntVar> defineFinalPointsVariables(const IloEnv &env, const IloModel &model, std::vector<IloBoolVarArray>& matchOutcomeVars, std::map<int, std::vector<Match>> &remainingMatches, const Tournament& tournament);
    static std::map<std::shared_ptr<Team>, IloBoolVar> defineRankingConstraints(const IloEnv &env, const IloModel &model, std::map<std::shared_ptr<Team>, IloIntVar> &finalPoints, const std::shared_ptr<Team>& targetTeam, Tournament &tournament, const std::map<int, std::vector<Match>> &remainingMatches, const std::map<std::shared_ptr<Team>,int>& initialPoints);

    // Functions to calculate the highest and lowest possible rank
    static int solveWorstCaseRank(Tournament& tournament, const std::shared_ptr<Team>& targetTeam, int roundsBeforeTheEnd);
    static int solveBestCaseRank(Tournament &tournament, const std::shared_ptr<Team>& targetTeam,  int roundsBeforeTheEnd);


    // Function to check whether a match is relevant for a team based on the highest and lowest possible rank
    static bool matchIsRelevant(int highestRankPossible, int lowestRankPossible);
    static bool teamIsDecidedChampionsLeague(int highestRankPossible, int lowestRankPossible);
    static bool teamIsDecidedPremierLeague(int highestRankPossible, int lowestRankPossible);
    static bool matchIsRelevantChampionsLeague(int highestRankPossible, int lowestRankPossible);
    static std::string matchType(int highestRankPossibleTeam1, int lowestRankPossibleTeam1, int highestRankPossibleTeam2, int lowestRankPossibleTeam2);



};



#endif //BOUNDS_H
