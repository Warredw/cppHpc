#ifndef TOURNAMENT_H
#define TOURNAMENT_H
#include "entities/TimeTable.h"
#include "utils/TeamManager/TeamManager.h"
#include "utils/Scheduling/Schedulers/Scheduler.h"
#include "utils/Simulations/Simulator.h"


class Tournament {

protected:
    int numberTeams;
    int numberRounds;
    int numberDynamicRounds;
    int numberNormalRounds;

    std::vector<std::vector<std::vector<int>>> homeAwayGamesValue; // stores whether team i played at home against team j in a certain round
    TimeTable firstTimeTable; // static rounds
    TimeTable secondTimeTable; // dynamic rounds
    TimeTable completeTimeTable; // Combined (easier to loop over)

    // Utility Objects
    //Scheduler* scheduler; // will generate the schedule
    TeamManager* teamManager; // will manage the teams

public:
    Tournament(int numberTeams, int numberRounds, int dynamicRounds);

    virtual ~Tournament();

    // Getters
    [[nodiscard]] int getNumberRounds() const;
    [[nodiscard]] int getNumberTeams() const;
    [[nodiscard]] int getNumberDynamicRounds() const;
    [[nodiscard]] int getNumberNormalRounds() const;
    [[nodiscard]] TeamManager* getTeamManager() const;
    TimeTable &getFirstTimeTable();
    TimeTable &getSecondTimeTable();
    TimeTable &getCompleteTimeTable();
    //[[nodiscard]] Scheduler* getScheduler() const;
    [[nodiscard]] int getHomeAwayGamesValue(int team1, int team2, int round) const;
    [[nodiscard]] bool getHomeAwayValue(int team1, int team2) const; // this is true when team i played at home against team j
    std::vector<std::vector<int>> getfirstHalfHomeAway();


    // Setters
    void setFirstTimeTable(const TimeTable& table); // static rounds
    void setSecondTimeTable(const TimeTable& table); // dynamic rounds
    void setCompleteTimeTable(); // combine static and dynamic rounds to get the full tournament
    void setHomeAwayGamesValue(int team1, int team2, int round, int value);
    void resetHomeAwayGamesValue();

    // Scheduling functions
    virtual void makeInitialSchedule(std::mt19937& randomNumberGenerator) = 0;
    virtual void makeSecondSchedule() = 0;

    // Reset tournament (in order to be able to start a new simulation round)
    void resetTournament();
    // Reset the tournament to a particular Round
    void setPointsToPenultimateRound(); // needed to calculate Schilling For Example
    std::vector<int> getPointsAfterCertainRound(int numberRoundsBeforeTheEnd);
};

#endif //TOURNAMENT_H
