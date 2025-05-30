#ifndef MATCH_H
#define MATCH_H

#include "entities/Team.h"
#include <memory>

enum class Result {
    WIN,
    DRAW,
    LOSS,
    NOT_PLAYED
};

class Match {
    std::shared_ptr<Team> homeTeam;
    std::shared_ptr<Team> awayTeam;
    Result result;
    int matchIndex; // Used in the champions league scenario

    // information about the match after it is played
    int pointsHomeTeamAfterMatch;
    int pointsAwayTeamAfterMatch;
    double ratingHomeTeamAfterMatch;
    double ratingAwayTeamAfterMatch;

    int goalsHomeTeam;
    int goalsAwayTeam;

    int tiebreakerHomeTeamAfterMatch;
    int tieBreakerAwayTeamAfterMatch;

    // information about the match before it is played
    int pointsHomeTeamBeforeMatch;
    int pointsAwayTeamBeforeMatch;

public:
    // for a non champions league match
    Match(std::shared_ptr<Team> homeTeam, std::shared_ptr<Team>awayTeam);

    // for a champions league match. MatchIndex = 1 or 2 because each round has two game days
    Match(std::shared_ptr<Team> homeTeam, std::shared_ptr<Team> awayTeam, int matchIndex);


    // Getters
    [[nodiscard]] std::shared_ptr<Team> getHomeTeam() const;
    [[nodiscard]] std::shared_ptr<Team> getAwayTeam() const;

    [[nodiscard]] int getPointsHomeTeamAfterMatch() const;
    [[nodiscard]] int getPointsAwayTeamAfterMatch() const;
    [[nodiscard]] double getRatingHomeTeamAfterMatch() const;
    [[nodiscard]] double getRatingAwayTeamAfterMatch() const;
    [[nodiscard]] int getPointsHomeTeamBeforeMatch() const;
    [[nodiscard]] int getPointsAwayTeamBeforeMatch() const;
    [[nodiscard]] Result getResult() const;
    [[nodiscard]] int getTiebreakerHomeTeamAfterMatch() const;
    [[nodiscard]] int getTieBreakerAwayTeamAfterMatch() const;

    // Setters
    void setPointsHomeTeamAfterMatch(int pointsHomeTeamAfterMatch);
    void setPointsAwayTeamAfterMatch(int pointsAwayTeamAfterMatch);
    void setRatingHomeTeamAfterMatch(double ratingHomeTeamAfterMatch);
    void setRatingAwayTeamAfterMatch(double ratingAwayTeamAfterMatch);
    void setRatingHomeTeamBeforeMatch(double ratingHomeTeamBeforeMatch);
    void setRatingAwayTeamBeforeMatch(double ratingAwayTeamBeforeMatch);
    void setPointsHomeTeamBeforeMatch(int pointsHomeTeamBeforeMatch);
    void setPointsAwayTeamBeforeMatch(int pointsAwayTeamBeforeMatch);
    void setResult(Result result);
    void setGoalsHomeTeam(int goals);
    void setGoalsAwayTeam(int goals);
    void setTiebreakerHomeTeamAfterMatch(int tiebreakerHomeTeamAfterMatch);
    void setTieBreakerAwayTeamAfterMatch(int tieBreakerAwayTeamAfterMatch);


    // Operator overloading
    bool operator !=(const Match& match) const {
        return this->homeTeam != match.getHomeTeam() || this->awayTeam != match.getAwayTeam();
    }
};

#endif //MATCH_H
