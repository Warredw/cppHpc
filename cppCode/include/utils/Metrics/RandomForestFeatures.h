#ifndef RANDOMFORESTFEATURES_H
#define RANDOMFORESTFEATURES_H

#include <memory>
#include "entities/Team.h"
#include <map>
#include "tournament/Tournament.h"

class RandomForestFeatures {

    std::map<std::shared_ptr<Team>, int> teamPoints; // Basically the ranking?
    std::map<std::shared_ptr<Team>, int> teamPlaces;
    std::map<std::shared_ptr<Team>, int> distanceToFirst;
    std::map<std::shared_ptr<Team>, int> distanceToLast;
    std::map<std::shared_ptr<Team>, int> distanceToFourth;
    std::map<std::shared_ptr<Team>, int> distanceToSeventh;
    std::map<std::shared_ptr<Team>, int> distanceToSeventeenth;


public:

    std::map<std::shared_ptr<Team>, int> getTeamPoints();
    std::map<std::shared_ptr<Team>, int> getTeamPlaces();
    std::map<std::shared_ptr<Team>, int> getDistanceToFirst();
    std::map<std::shared_ptr<Team>, int> getDistanceToLast();
    std::map<std::shared_ptr<Team>, int> getDistanceToFourth();
    std::map<std::shared_ptr<Team>, int> getDistanceToSeventh();
    std::map<std::shared_ptr<Team>, int> getDistanceToSeventeenth();


    // calculate everything
    void setRandomForestFeatures(Tournament& tournament);

};



#endif //RANDOMFORESTFEATURES_H
