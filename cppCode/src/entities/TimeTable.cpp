#include "entities/TimeTable.h"

TimeTable::TimeTable(const int &numberRounds) {
    rounds.resize(numberRounds);
}

// Getters
int TimeTable::getNumberOfRounds() const {
    return static_cast<int>(rounds.size());
}

Round& TimeTable::getRound(const int round) {
    return rounds.at(round);
}

std::vector<Round>& TimeTable::getRounds() {
    return rounds;
}

std::vector<Match>& TimeTable::getMatchesInRound(const int round) {
    return rounds.at(round).getMatches();
}

// Setters
void TimeTable::setRound(const int round, const Round& newRound) {
    rounds.at(round) = newRound;
}

// Other methods
void TimeTable::resizeRounds(const size_t newSize) {
    rounds.resize(newSize);
}

void TimeTable::addMatchToRound(const int round,  const Match& match) {
    rounds.at(round).addMatch(match);
}










