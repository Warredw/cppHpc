
#include "utils/Metrics/RandomForestFeatures.h"
#include "utils/Metrics/RoundStatistics.h"
#include "utils/Metrics/RoundStatistics.h"

// Getters
std::map<std::shared_ptr<Team>, int> RandomForestFeatures::getTeamPoints() {
    return teamPoints;
}

std::map<std::shared_ptr<Team>, int> RandomForestFeatures::getTeamPlaces() {
    return teamPlaces;
}

std::map<std::shared_ptr<Team>, int> RandomForestFeatures::getDistanceToFirst() {
    return distanceToFirst;
}

std::map<std::shared_ptr<Team>, int> RandomForestFeatures::getDistanceToLast() {
    return distanceToLast;
}

std::map<std::shared_ptr<Team>, int> RandomForestFeatures::getDistanceToFourth() {
    return distanceToFourth;
}

std::map<std::shared_ptr<Team>, int> RandomForestFeatures::getDistanceToSeventh() {
    return distanceToSeventh;
}

std::map<std::shared_ptr<Team>, int> RandomForestFeatures::getDistanceToSeventeenth() {
    return distanceToSeventeenth;
}

// Setters
void RandomForestFeatures::setRandomForestFeatures(Tournament &tournament) {

    // get the ranking at half time
    std::vector<std::pair<std::shared_ptr<Team>,int>> halfTimeRanking = RoundStatistics::makeRanking(tournament.getCompleteTimeTable().getRounds()[tournament.getNumberRounds()/2]);

    const int first = halfTimeRanking[0].second;
    const int last = halfTimeRanking[halfTimeRanking.size()-1].second;


    const int fourth = halfTimeRanking[3].second;
    const int seventh = halfTimeRanking[6].second;
    const int seventeenth = halfTimeRanking[16].second;



    int rankingIndex = 1;

    // Go over all the teams;
    for (const auto& teamPair : halfTimeRanking) {

        const auto& team = teamPair.first;
        // Perform operations on the team
        const int points = teamPair.second;

        teamPoints[team] = points;
        teamPlaces[team] = rankingIndex;
        distanceToFirst[team] = std::abs(first - points);
        distanceToLast[team] = std::abs(points - last);
        distanceToFourth[team] = std::abs(points - fourth);
        distanceToSeventh[team] = std::abs(points - seventh);

        distanceToSeventeenth[team] = std::abs(points - seventeenth);


        rankingIndex++;
    }

}
