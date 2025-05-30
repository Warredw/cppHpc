#ifndef TEAM_H
#define TEAM_H
#include <string>
#include "HapSet.h"

class Team {

    std::string name;
    double rating;
    double initialRating;
    int points;
    int goalsScored;
    int goalsReceived;
    int numberHomeGames;
    int numberAwayGames;
    int potNumber;
    std::string association;


public:

    // constructor for premier league teams (no potNumber and association required)
    Team(std::string name, double rating);

    // Other constructor for championsLeagueTeams
    Team(std::string name, double rating, int potNumber, std::string  association);

    // Getters
    std::string getName();
    [[nodiscard]] double getRating() const;
    [[nodiscard]] int getHomeAwaBalance() const;
    [[nodiscard]] int getPoints() const;
    [[nodiscard]] int getPotNumber() const;
    [[nodiscard]] std::string getAssociation() const;
    [[nodiscard]] int getGoalsScored() const;
    [[nodiscard]] int getGoalsReceived() const;
    [[nodiscard]]int getTieBreaker() const;

    // Setters
    void setPoints(int points);
    void setRating(double rating);


    // Add
    void addHomeGame();
    void addAwayGame();
    void addPoints(int points);
    void addToRating(double ratingChange);
    void addGoalsScored(int goalsScored);
    void addGoalsReceived(int goalsReceived);

    // Reset
    void reset();


    // Operator Overloading to compare two teams
    bool operator==(Team& team) const {
        return this->name == team.getName();
    }
};

#endif //TEAM_H
