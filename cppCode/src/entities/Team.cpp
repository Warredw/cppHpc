#include <utility>
#include "entities/Team.h"

// Constructor for Premier League Team  (no association and pot number required)
Team::Team(std::string name, const double rating): name(std::move(name)), rating(rating), initialRating(rating),
    points(0), numberHomeGames(0), numberAwayGames(0), potNumber(0){}

// Constructor for Champions League Team (association and pot number)
Team::Team(std::string name, const double rating, const int potNumber, std::string  association): name(std::move(name)), rating(rating), initialRating(rating),
    points(0), numberHomeGames(0), numberAwayGames(0), potNumber(potNumber), association(std::move(association)){}


// Getters
std::string Team::getName() {
    return name;
}

double Team::getRating() const {
    return rating;
}

int Team::getPoints() const {
    return points;
}

int Team::getHomeAwaBalance() const {
    return numberHomeGames - numberAwayGames;
}

// Setters
void Team::setPoints(const int points) {
    this->points = points;
}

void Team::setRating(const double rating) {
    this->rating = rating;
}

int Team::getPotNumber() const{
    return potNumber;
}

std::string Team::getAssociation() const {
    return association;
}

int Team::getGoalsScored() const {
    return goalsScored;
}

int Team::getGoalsReceived() const {
    return goalsReceived;
}

int Team::getTieBreaker() const {
    return goalsScored - goalsReceived;
}


// Add
void Team::addHomeGame() {
    numberHomeGames++;
}

void Team::addAwayGame() {
    this->numberAwayGames++;
}

void Team::addPoints(const int points) {
    this->points += points;
}

void Team::addToRating(const double ratingChange) {
    this->rating += ratingChange;
}

void Team::addGoalsScored(const int goalsScored) {
    this->goalsScored += goalsScored;
}

void Team::addGoalsReceived(const int goalsReceived) {
    this->goalsReceived += goalsReceived;
}

// Reset
void Team::reset() {
    this->rating  = initialRating;
    this->points = 0;
    this->numberHomeGames = 0;
    this->numberAwayGames = 0;
    this->goalsScored = 0;
    this->goalsReceived = 0;
}

