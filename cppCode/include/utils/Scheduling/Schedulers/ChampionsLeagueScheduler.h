#ifndef CHAMPIONSLEAGUESCHEDULER_H
#define CHAMPIONSLEAGUESCHEDULER_H

#include "utils/Scheduling/Schedulers/Scheduler.h"
#include "entities/Round.h"
#include <set>
#include <map>
#include <algorithm>
#include <tuple>

class Tournament;

class ChampionsLeagueScheduler final: public Scheduler {

    std::vector<std::tuple<int,int>> matchPairings;

public:

    // Setters
    void setMatchPairings(std::vector<std::tuple<int,int>> matchPairings) {
        this->matchPairings = std::move(matchPairings);
    }

    // General functions that needs to be overridden
    void setInitialSchedule(Tournament &tournament, std::mt19937 &randomNumberGenerator) override;
    void setSecondSchedule(Tournament &tournament) const override; // This function will actually be very complex here


    // Function that gets all the possible last round pairings
    static std::vector<std::tuple<int,int>> getEdges(Tournament* tournament, const std::vector<std::tuple<int,int>>& matches);
    static std::vector<Round> generateRounds(Round current, std::vector<bool> used, int idx, Tournament* tournament, std::vector<std::tuple<int,int>> edges);

    // Match Model
    static std::vector<std::vector<IloNumVar>> initializeMatchVariables(const IloEnv& env, int numberRounds, const Tournament& tournament); // will initialize which teams play against each other (home or away)
    static void playOnceHomePot(const IloEnv& env, const IloModel& model , const std::vector<std::vector<IloNumVar>>&x, const Tournament& tournament);
    static void playOnceAwayPot(const IloEnv& env, const IloModel& model , const std::vector<std::vector<IloNumVar>>&x, const Tournament& tournament);

    // Here two different function for when we ge
    [[nodiscard]] static std::vector<std::tuple<int,int>> calculateMatchPairings(const Tournament& tournament, std::mt19937 &randomNumberGenerator);
    //
    //    // Helper Function to check if a certain match is part of the output of the match Model
    static bool isPartOfMatches(int team1, int team2, std::vector<std::tuple<int,int>> matches);
    //    // Helper Function to check if a certain match ist rid of the pots
    static void playFourHomeGames(const IloEnv& env, const IloModel& model , const std::vector<std::vector<IloNumVar>>&x, const Tournament& tournament);
    static void playFourAwayGames(const IloEnv& env, const IloModel& model , const std::vector<std::vector<IloNumVar>>&x, const Tournament& tournament);

    static void playMaxOnce(const IloEnv &env, const IloModel &model, const std::vector<std::vector<IloNumVar>> &x, const Tournament &tournament);
    static void associationConstraint(const IloEnv &env, const IloModel &model, const std::vector<std::vector<IloNumVar>> &x, const Tournament &tournament);

    // Function to get the output of this model played before the last round
    static std::vector<std::tuple<int, int>> getPlayedMatchesExcludingLastRound(Tournament& tournament);


    // add random objective function (to get a different schedule each time)
    static void addObjectiveFunction(const IloEnv &env, const IloModel &model, const std::vector<std::vector<IloNumVar>> &x, const Tournament &tournament, std::mt19937 &randomNumberGenerator);


    // Time Model
    static std::vector<std::vector<std::vector<std::vector<IloNumVar>>>> initializeTimeVariables(const IloEnv& env, int numberRounds, const Tournament& tournament); // will initialize which teams play against each other (home or away)
    static void addOneGamePerWeekConstraint(const IloEnv &env, const IloModel &model, std::vector<std::vector<std::vector<std::vector<IloNumVar>>>> &x, const Tournament &tournament);
    static void addEachMatchUpOnlyOnceConstraint(const IloEnv& env, const IloModel& model, std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x, const Tournament& tournament, std::vector<std::tuple<int,int>> matches);
    static void addNineMatchesPerWeekConstraint(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x, const Tournament& tournament, const std::vector<std::tuple<int,int>>& matches);
    static void addOneHomeGameFirstTwoMatches(const IloEnv& env, const IloModel& model, std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x, const Tournament& tournament, std::vector<std::tuple<int,int>> matches);
    static void addOneHomeGameLastTwoMatches(const IloEnv& env, const IloModel& model, std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x, const Tournament& tournament, std::vector<std::tuple<int,int>> matches);
    static void addMaxTwoHomeGamesInARow(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x, const Tournament& tournament, const std::vector<std::tuple<int,int>>& matches);
    static void addMaxTwoAwayGamesInARow(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x, const Tournament& tournament, const std::vector<std::tuple<int,int>>& matches);
    static void addMilanMadridConstraint(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x, const Tournament& tournament, const std::vector<std::tuple<int,int>>& matches);

};



#endif //CHAMPIONSLEAGUESCHEDULER_H
