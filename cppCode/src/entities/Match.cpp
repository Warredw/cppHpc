#include "entities/Match.h"


Match::Match(std::shared_ptr<Team> homeTeam, std::shared_ptr<Team> awayTeam): homeTeam(std::move(homeTeam)),
    awayTeam(std::move(awayTeam)),result(Result::NOT_PLAYED), matchIndex(0),pointsHomeTeamAfterMatch(0),
    pointsAwayTeamAfterMatch(0), ratingHomeTeamAfterMatch(0), ratingAwayTeamAfterMatch(0),pointsHomeTeamBeforeMatch(0),pointsAwayTeamBeforeMatch(0) {}

Match::Match(std::shared_ptr<Team> homeTeam, std::shared_ptr<Team> awayTeam, const int matchIndex):
    homeTeam(std::move(homeTeam)), awayTeam(std::move(awayTeam)), result(Result::NOT_PLAYED), matchIndex(matchIndex), pointsHomeTeamAfterMatch(0),
    pointsAwayTeamAfterMatch(0), ratingHomeTeamAfterMatch(0), ratingAwayTeamAfterMatch(0), pointsHomeTeamBeforeMatch(0), pointsAwayTeamBeforeMatch(0) {}

// Getters
std::shared_ptr<Team> Match::getHomeTeam() const {
    return homeTeam;
}

std::shared_ptr<Team> Match::getAwayTeam() const {
    return awayTeam;
}

int Match::getPointsHomeTeamAfterMatch() const {
    return pointsHomeTeamAfterMatch;
}

int Match::getPointsAwayTeamAfterMatch() const {
    return pointsAwayTeamAfterMatch;
}

double Match::getRatingHomeTeamAfterMatch() const {
    return ratingHomeTeamAfterMatch;
}

double Match::getRatingAwayTeamAfterMatch() const {
    return ratingAwayTeamAfterMatch;
}

int Match::getPointsHomeTeamBeforeMatch() const {
    return pointsHomeTeamBeforeMatch;
}

int Match::getPointsAwayTeamBeforeMatch() const {
    return pointsAwayTeamBeforeMatch;
}

Result Match::getResult() const {
    return result;
}

int Match::getTiebreakerHomeTeamAfterMatch() const {
    return tiebreakerHomeTeamAfterMatch;
}

int Match::getTieBreakerAwayTeamAfterMatch() const {
    return tieBreakerAwayTeamAfterMatch;
}

// Setters
void Match::setResult(const Result result) {
    this->result = result;
}

void Match::setPointsHomeTeamAfterMatch(const int pointsHomeTeamAfterMatch) {
    this->pointsHomeTeamAfterMatch = pointsHomeTeamAfterMatch;
}

void Match::setPointsAwayTeamAfterMatch(const int pointsAwayTeamAfterMatch) {
    this->pointsAwayTeamAfterMatch = pointsAwayTeamAfterMatch;
}

void Match::setRatingHomeTeamAfterMatch(const double ratingHomeTeamAfterMatch) {
    this->ratingHomeTeamAfterMatch = ratingHomeTeamAfterMatch;
}

void Match::setRatingAwayTeamAfterMatch(const double ratingAwayTeamAfterMatch) {
    this->ratingAwayTeamAfterMatch = ratingAwayTeamAfterMatch;
}

void Match::setRatingHomeTeamBeforeMatch(const double ratingHomeTeamBeforeMatch) {
    this->ratingHomeTeamAfterMatch = ratingHomeTeamBeforeMatch;
}

void Match::setRatingAwayTeamBeforeMatch(const double ratingAwayTeamBeforeMatch) {
    this->ratingAwayTeamAfterMatch = ratingAwayTeamBeforeMatch;
}

void Match::setPointsHomeTeamBeforeMatch(const int pointsHomeTeamBeforeMatch) {
    this->pointsHomeTeamBeforeMatch = pointsHomeTeamBeforeMatch;
}

void Match::setPointsAwayTeamBeforeMatch(const int pointsAwayTeamBeforeMatch) {
    this->pointsAwayTeamBeforeMatch = pointsAwayTeamBeforeMatch;
}

void Match::setGoalsHomeTeam(const int goals) {
    this->goalsHomeTeam = goals;
}

void Match::setGoalsAwayTeam(const int goals) {
    this->goalsAwayTeam = goals;
}

void Match::setTiebreakerHomeTeamAfterMatch(const int tiebreakerHomeTeamAfterMatch) {
    this->tiebreakerHomeTeamAfterMatch = tiebreakerHomeTeamAfterMatch;
}

void Match::setTieBreakerAwayTeamAfterMatch(const int tieBreakerAwayTeamAfterMatch) {
    this->tieBreakerAwayTeamAfterMatch = tieBreakerAwayTeamAfterMatch;
}






