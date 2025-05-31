#include "utils/TeamManager/TeamManager.h"
#include "tournament/Tournament.h"
#include <iostream>



TeamManager::TeamManager(): teams(std::vector<std::shared_ptr<Team>>()){
}

// Getters
std::shared_ptr<Team> TeamManager::getTeam(const int index) const{
    return teams[index];
}

std::vector<std::shared_ptr<Team>> TeamManager::getTeams() {
    return teams;
}

int TeamManager::getTeamIndex(const std::shared_ptr<Team>& team) const {
    for(int i = 0; i < teams.size(); i++) {
        if(teams[i] == team) return i;
    }
    throw std::runtime_error("Team not found");
}

int TeamManager::getTeamIndex(const std::string& name) const {
    int index = 0;
    for(const auto& team: teams) {
        if(team->getName() == name) {
            return index;
        }
        index++;
    }
    throw std::runtime_error("Team not found");
}

int TeamManager::getNumberOfTeams() const {
    return static_cast<int>(teams.size());
}

std::vector<std::shared_ptr<Team>> TeamManager::getPotTeams(const int potNumber) const {

    // initialize return type
    std::vector<std::shared_ptr<Team>> potTeams;

    // Go over the teams
    for(const auto& team: teams) {
        if(team->getPotNumber() == potNumber) {
            potTeams.push_back(team);
        }
    }
    return potTeams;
}


std::vector<std::shared_ptr<Team>> TeamManager::getTeamsFromAssociation(const std::string& association) const {
    std::vector<std::shared_ptr<Team>> associationTeams;

    for(const auto& team: teams) {
        if(team->getAssociation() == association) {
            associationTeams.push_back(team);
        }
    }
    return associationTeams;
}

// Other methods
bool TeamManager::teamIsWinner(const std::shared_ptr<Team>& team) const {
    bool isWinner = false;
    int highestScore = 0;

    for(int i = 0; i < teams.size(); i++) {
        if(teams[i]->getPoints() > highestScore) {
            highestScore = teams[i]->getPoints();
        }
    }

    if(team->getPoints() == highestScore) {
        isWinner = true;
    }
    return isWinner;
}

void TeamManager::resetTeams() const {
    for(const auto & team : teams) {
        team->reset();
    }
}


void TeamManager::changeTeamRatings(const std::string &distribution, int value) const {

    if (distribution == "equallyDistanced") {

        int startRating = 2000;

        for(const auto& team: teams) {
            team->setRating(startRating);
            startRating -= value;
        }
    }
}
