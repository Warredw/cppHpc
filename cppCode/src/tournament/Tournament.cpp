#include "tournament/Tournament.h"

Tournament::Tournament(const int numberTeams, const int numberRounds, const int dynamicRounds)
    : numberTeams(numberTeams), numberRounds(numberRounds), numberDynamicRounds(dynamicRounds), numberNormalRounds(numberRounds - dynamicRounds),
    homeAwayGamesValue(numberTeams, std::vector<std::vector<int>>(numberTeams, std::vector<int>(numberRounds/2))), firstTimeTable(TimeTable(numberRounds - dynamicRounds)), secondTimeTable(TimeTable(dynamicRounds)),
    completeTimeTable(TimeTable(numberRounds)), /*scheduler(nullptr)*/ teamManager(nullptr) {
}

Tournament::~Tournament() {
    //delete scheduler;
    delete teamManager;
}

// Getters
int Tournament::getNumberRounds() const {
    return numberRounds;
}

int Tournament::getNumberTeams() const {
    return numberTeams;
}

TeamManager* Tournament::getTeamManager() const {
    return teamManager;
}

TimeTable &Tournament::getFirstTimeTable() {
    return firstTimeTable;
}

TimeTable &Tournament::getSecondTimeTable() {
    return secondTimeTable;
}

int Tournament::getNumberDynamicRounds() const {
    return numberDynamicRounds;
}

int Tournament::getNumberNormalRounds() const {
    return numberRounds - numberDynamicRounds;
}

TimeTable &Tournament::getCompleteTimeTable() {
    return completeTimeTable;
}

/*
Scheduler* Tournament::getScheduler() const {
    return scheduler;
}
*/

int Tournament::getHomeAwayGamesValue(const int team1, const int team2, const int round) const {
    return homeAwayGamesValue[team1][team2][round];
}

bool Tournament::getHomeAwayValue(const int team1, const int team2) const {

    for(int round = 0; round < numberRounds/2; round++) {
        if(homeAwayGamesValue[team1][team2][round] == 1) {
            return true;
        }
    }
    return false;
}


std::vector<std::vector<int> > Tournament::getfirstHalfHomeAway() {

    std::vector<std::vector<int>> matrix(numberTeams, std::vector<int>(numberTeams, 0));

    for(int r = 0; r < numberRounds/2; r++) {

        for(const auto& match: firstTimeTable.getMatchesInRound(r)) {
            const int i = teamManager->getTeamIndex(match.getHomeTeam());
            const int j = teamManager->getTeamIndex(match.getAwayTeam());

            matrix[i][j] = 1;
        }
    }

    return matrix;
}


// Setters
void Tournament::setFirstTimeTable(const TimeTable &table) {
    this->firstTimeTable = table;
}

void Tournament::setSecondTimeTable(const TimeTable &table) {
    this->secondTimeTable = table;
}

void Tournament::setCompleteTimeTable() {

    const int firstTableRounds = firstTimeTable.getNumberOfRounds();
    const int secondTableRounds = secondTimeTable.getNumberOfRounds();

    for (int i = 0; i < firstTableRounds; i++) {
        completeTimeTable.setRound(i, firstTimeTable.getRound(i));
    }
    for (int i = 0; i < secondTableRounds; i++) {
        completeTimeTable.setRound(i + firstTableRounds, secondTimeTable.getRound(i));
    }
}

void Tournament::setHomeAwayGamesValue(const int team1, const int team2, const int round, const int value) {
    homeAwayGamesValue[team1][team2][round] = value;
}

void Tournament::resetHomeAwayGamesValue() {
    homeAwayGamesValue = std::vector(numberTeams, std::vector(numberTeams, std::vector(numberRounds/2, 0)));
}



// Scheduling functions

/*
void Tournament::makeInitialSchedule(std::mt19937& randomNumberGenerator) {
    scheduler->setInitialSchedule(*this, randomNumberGenerator);
}

void Tournament::makeSecondSchedule() {
    scheduler->setSecondSchedule(*this);
}
*/


// Reset
void Tournament::resetTournament() {
    teamManager->resetTeams();
    firstTimeTable = TimeTable(numberRounds - numberDynamicRounds);
    secondTimeTable = TimeTable(numberDynamicRounds);
    completeTimeTable = TimeTable(numberRounds);
}


void Tournament::setPointsToPenultimateRound() {
    // get the matches of the penultimate round
    const std::vector<Match> matches = getCompleteTimeTable().getMatchesInRound(getNumberRounds()-2);
    for (auto& match : matches) {
        const auto homeTeam = match.getHomeTeam();
        const auto awayTeam = match.getAwayTeam();

        homeTeam->setPoints(match.getPointsHomeTeamAfterMatch());
        awayTeam->setPoints(match.getPointsAwayTeamAfterMatch());

        homeTeam->setRating(match.getRatingHomeTeamAfterMatch());
        awayTeam->setRating(match.getRatingAwayTeamAfterMatch());
    }
}

// parameter is one indexed
std::vector<int> Tournament::getPointsAfterCertainRound(const int roundNumber){
    std::vector<int> points(numberTeams, 0);
    setCompleteTimeTable();
    const std::vector<Match> matches = getCompleteTimeTable().getMatchesInRound(roundNumber-1);
    for (const auto &match : matches) {
        points[teamManager->getTeamIndex(match.getHomeTeam())] = match.getPointsHomeTeamAfterMatch();
        points[teamManager->getTeamIndex(match.getAwayTeam())] = match.getPointsAwayTeamAfterMatch();
    }
    return points;
}
