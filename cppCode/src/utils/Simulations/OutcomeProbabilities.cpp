#include "utils/Simulations/OutcomeProbabilities.h"

double OutcomeProbabilities::calculateEloDifference(const Match& match) {
    const double eloDifference = match.getHomeTeam()->getRating() - match.getAwayTeam()->getRating() + 51.3;
    return eloDifference;
}

double OutcomeProbabilities::calculateExpectedResult(const Match& match) {
    const double expectedResult = 1 / (pow(10, -calculateEloDifference(match) / 400) + 1);
    return expectedResult;
}

double OutcomeProbabilities::calculateDrawProbability(const Match& match) {
    const double eloDiffSquared = pow(calculateEloDifference(match), 2);
    const double drawProbability = (29.0 / 100) * exp(-eloDiffSquared / (2 * pow(285, 2)));
    return drawProbability;
}

double OutcomeProbabilities::calculateWinProbabilityHomeTeam(const Match& match) {
    const double winProbabilityHomeTeam = calculateExpectedResult(match) - (calculateDrawProbability(match) / 2);
    return winProbabilityHomeTeam;
}

double OutcomeProbabilities::calculateLossProbabilityHomeTeam(const Match& match) {
    const double lossProbabilityHomeTeam = 1 - calculateExpectedResult(match) - (calculateDrawProbability(match) / 2);
    return lossProbabilityHomeTeam;
}


double OutcomeProbabilities::calculateExpectedHomeGoals(const Match &match) {

    // Calculate win probability home team
    const double winProbabilityHomeTeam = calculateWinProbabilityHomeTeam(match);
    // Calculated expected Goals
    const double expectedHomeGoals = 0.28004 + 7.44861*winProbabilityHomeTeam - 34.68423 * pow(winProbabilityHomeTeam,2) + 89.64274*  pow(winProbabilityHomeTeam, 3)
                            - 103.75235*  pow(winProbabilityHomeTeam,4) + 45.39971 * pow(winProbabilityHomeTeam,5);

    return expectedHomeGoals;
}

double OutcomeProbabilities::calculateExpectedAwayGoals(const Match &match) {

    // Calculate win probability home team
    const double winProbabilityHomeTeam = calculateWinProbabilityHomeTeam(match);

    // calculate expected goals
    const double expectedAwayGoals = 2.67454 - 1.40135 * winProbabilityHomeTeam - 15.35034 * pow(winProbabilityHomeTeam,2) + 39.28812 * pow(winProbabilityHomeTeam, 3)
        - 35.34970 * pow(winProbabilityHomeTeam,4) + 10.51070* pow(winProbabilityHomeTeam,5);

    return expectedAwayGoals;
}

int OutcomeProbabilities::getHomeGoals(const Match &match, std::mt19937& randomNumberGenerator) {

    const double ExpectedHomeGoals = calculateExpectedHomeGoals(match);

    // Sample from Poisson distribution
    std::poisson_distribution<> poisson(ExpectedHomeGoals);
    const int homeGoalsSampled = poisson(randomNumberGenerator);

    return homeGoalsSampled;
}

int OutcomeProbabilities::getAwayGoals(const Match &match, std::mt19937& randomNumberGenerator) {

    const double ExpectedAwayGoals = calculateExpectedAwayGoals(match);

    // Sample from Poisson distribution
    std::poisson_distribution<> poisson(ExpectedAwayGoals);
    const int awayGoalsSampled = poisson(randomNumberGenerator);

    return awayGoalsSampled;
}



