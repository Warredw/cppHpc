#ifndef TIMETABLE_H
#define TIMETABLE_H

#include "entities/Round.h"


class TimeTable {
    std::vector<Round> rounds;

public:
    explicit TimeTable(const int& numberRounds);

    // Getters
    std::vector<Round>& getRounds();
    std::vector<Match>& getMatchesInRound(int round);
    [[nodiscard]] int getNumberOfRounds() const;
    Round& getRound(int round);

    // Setters
    void setRound(int round, const Round& newRound);

    // Other methods
    void resizeRounds(size_t newSize);
    void addMatchToRound(int round,  const Match& match);
};

#endif //TIMETABLE_H
