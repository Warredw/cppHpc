#ifndef COMPETITIONMETRICS_H
#define COMPETITIONMETRICS_H

#include<map>
#include <string>
#include <map>

#include "tournament/Tournament.h"


class SecondaryMetrics {

    // basic metrics
    int roundWinnerDecided;
    int numberLeadChanges;
    int numberRelegationChanges;
    int numberChanges;  // = number of times teams swap places (tiebreak included)

    // average point difference between the teams in the second half schedule
    std::map<int,int> averagePointDifference; // key = round number (zero indexed)
    std::map<int,int> intensity; // sum of rating difference between the teams before the round


public:
    SecondaryMetrics();

    // Getters
    [[nodiscard]] int getRoundWinnerDecided() const;
    [[nodiscard]] int getNumberLeadChanges() const;
    [[nodiscard]] int getNumberRelegationChanges() const;
    [[nodiscard]] std::map<int,int> getAveragePointDifference() const;
    [[nodiscard]] std::map<int,int> getIntensity() const;


    // Calculate them
    void calculateBasicMetrics(Tournament& tournament);
    void calculateAveragePointDifference(Tournament& tournament);



};

#endif //COMPETITIONMETRICS_H
