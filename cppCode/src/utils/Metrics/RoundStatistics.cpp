#include "utils/Metrics/RoundStatistics.h"

#include <algorithm>
#include <limits>
#include <unordered_set>
#include <vector>
#include <memory>
#include <map>

int RoundStatistics::calculateHighestScore(Round &round) {
    int highestScore = 0;
    const std::vector<Match> matches = round.getMatches();

    for(const auto& match: matches) {
        const int homePoints = match.getPointsHomeTeamAfterMatch();
        const int awayPoints = match.getPointsAwayTeamAfterMatch();

        highestScore = std::max({highestScore, homePoints, awayPoints});
    }
    return highestScore;
}

int RoundStatistics::calculateSecondHighestScore(Round &round) {
    int highestScore = 0;
    int secondHighestScore = 0;
    const std::vector<Match> matches = round.getMatches();

    for (const auto& match: matches) {
        const int homePoints = match.getPointsHomeTeamAfterMatch();
        const int awayPoints = match.getPointsAwayTeamAfterMatch();

        // Check home team score
        if (homePoints > highestScore) {
            secondHighestScore = highestScore;
            highestScore = homePoints;
        } else if (homePoints > secondHighestScore && homePoints < highestScore) {
            secondHighestScore = homePoints;
        }

        // Check away team score
        if (awayPoints > highestScore) {
            secondHighestScore = highestScore;
            highestScore = awayPoints;
        } else if (awayPoints > secondHighestScore && awayPoints < highestScore) {
            secondHighestScore = awayPoints;
        }
    }

    if (secondHighestScore == 0) {
        return highestScore; // If there's no second highest, return highest.
    }

    return secondHighestScore;
}

std::unordered_set<std::shared_ptr<Team>> RoundStatistics::calculateLeadersAfterRound(Round &round) {
    std::unordered_set<std::shared_ptr<Team>> leaders;
    const std::vector<Match> matches = round.getMatches();
    int maxPoints = -1;

    for (const auto& match : matches) {
        const int homePoints = match.getPointsHomeTeamAfterMatch();
        const int awayPoints = match.getPointsAwayTeamAfterMatch();

        int matchMaxPoints = std::max(homePoints, awayPoints);

        if (matchMaxPoints > maxPoints) {
            leaders.clear();
            maxPoints = matchMaxPoints;
        }

        if (homePoints == maxPoints) {
            leaders.insert(match.getHomeTeam());
        }
        if (awayPoints == maxPoints) {
            leaders.insert(match.getAwayTeam());
        }
    }
    return leaders;
}


std::unordered_set<std::shared_ptr<Team>> RoundStatistics::calculateRelegatorsAfterRound(Round &round) {
    std::unordered_set<std::shared_ptr<Team>> relegators;
    const std::vector<Match> matches = round.getMatches();

    // Make The ranking
    std::vector<std::pair<std::shared_ptr<Team>, int>> ranking = makeRanking(round);

    // Ensure there are enough teams in the ranking
    if (ranking.size() < 18) {
        return relegators; // Not enough teams to determine relegators
    }

    // Check the points of the team in 18th place
    const int numberPoints18th = ranking[ranking.size() - 3].second;

    // Select all the teams with this number of points or less
    for(const auto& match: matches) {
        const int homeTeamPoints = match.getPointsHomeTeamAfterMatch();
        const int awayTeamPoints = match.getPointsAwayTeamAfterMatch();

        if(homeTeamPoints <= numberPoints18th) {
            relegators.insert(match.getHomeTeam());
        }

        if(awayTeamPoints <= numberPoints18th) {
            relegators.insert(match.getAwayTeam());
        }
    }
    return relegators;
}

int RoundStatistics::calculateLowestScore(Round &round) {
    int lowestScore = std::numeric_limits<int>::max();
    const std::vector<Match> matches = round.getMatches();

    if (matches.empty()) {
        return 0;
    }

    for (const auto& match : matches) {
        const int homePoints = match.getPointsHomeTeamAfterMatch();
        const int awayPoints = match.getPointsAwayTeamAfterMatch();

        lowestScore = std::min({lowestScore, homePoints, awayPoints});
    }

    return lowestScore;
}

int RoundStatistics::calculateSecondLowestScore(Round &round) {
    int lowestScore = std::numeric_limits<int>::max();
    int secondLowestScore = std::numeric_limits<int>::max();

    const std::vector<Match> matches = round.getMatches();

    if (matches.empty()) {
        return 0;
    }

    for (const auto& match : matches) {
        const int homePoints = match.getPointsHomeTeamAfterMatch();
        const int awayPoints = match.getPointsAwayTeamAfterMatch();

        // Check home team score
        if (homePoints < lowestScore) {
            secondLowestScore = lowestScore;
            lowestScore = homePoints;
        } else if (homePoints < secondLowestScore && homePoints > lowestScore) {
            secondLowestScore = homePoints;
        }

        // Check away team score
        if (awayPoints < lowestScore) {
            secondLowestScore = lowestScore;
            lowestScore = awayPoints;
        } else if (awayPoints < secondLowestScore && awayPoints > lowestScore) {
            secondLowestScore = awayPoints;
        }
    }

    if (secondLowestScore == std::numeric_limits<int>::max()) {
        return lowestScore;
    }

    return secondLowestScore;
}

std::vector<std::pair<std::shared_ptr<Team>, int>> RoundStatistics::makeRanking(Round &round) {

    // make the current ranking
    std:vector<std::pair<std::shared_ptr<Team>, int>> teamPoints;

    for(const auto& match: round.getMatches()) {
        teamPoints.emplace_back(match.getHomeTeam(), match.getPointsHomeTeamAfterMatch());
        teamPoints.emplace_back(match.getAwayTeam(), match.getPointsAwayTeamAfterMatch());
    }

    std::sort(teamPoints.begin(), teamPoints.end(), [](const std::pair<std::shared_ptr<Team>, int>& a, const std::pair<std::shared_ptr<Team>, int>& b) {
        return a.second > b.second;
    });

    // Now return
    return teamPoints;
}


std::unordered_set<std::shared_ptr<Team>> RoundStatistics::selectTeamPositions(const std::vector<std::pair<std::shared_ptr<Team>, int>>& ranking, int highPosition, int lowPosition) {
    std::unordered_set<std::shared_ptr<Team>> selectedTeams;

    for(int i = highPosition; i <= lowPosition; i++) {
        selectedTeams.insert(ranking[i].first);
    }
    return selectedTeams;
}
