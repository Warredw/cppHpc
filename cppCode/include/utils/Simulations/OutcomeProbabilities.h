#ifndef OUTCOMEPROBABILITIES_H
#define OUTCOMEPROBABILITIES_H
#include "entities/Match.h"

class OutcomeProbabilities {

public:
    static double calculateExpectedResult(const Match& match);
    static double calculateEloDifference(const Match& match);
    static double calculateDrawProbability(const Match& match);
    static double calculateWinProbabilityHomeTeam(const Match& match);
    static double calculateLossProbabilityHomeTeam(const Match& match);


    // Poisson functions
    static double calculateExpectedHomeGoals(const Match& match);
    static double calculateExpectedAwayGoals(const Match& match);

    static int getHomeGoals(const Match& match, std::mt19937& randomNumberGenerator);
    static int getAwayGoals(const Match& match, std::mt19937& randomNumberGenerator);
};

#endif //OUTCOMEPROBABILITIES_H
