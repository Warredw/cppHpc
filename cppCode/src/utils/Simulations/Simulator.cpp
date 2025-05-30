#include "utils/Simulations/Simulator.h"


 void Simulator::simulateCompetition(TimeTable& timeTable, std::mt19937& randomNumberGenerator)  {

    std::uniform_real_distribution<> dis(0.0, 1.0);  // Uniform distribution between 0 and 1

    const auto rounds = timeTable.getNumberOfRounds();

    for(int i = 0; i < rounds; i++) {

        auto& matches = timeTable.getMatchesInRound(i);
        for(auto& match : matches) {

            const auto home = match.getHomeTeam();
            const auto away = match.getAwayTeam();
            match.setPointsHomeTeamBeforeMatch(home->getPoints());
            match.setPointsAwayTeamBeforeMatch(away->getPoints());

            /*

            if (const double randomNumber = dis(randomNumberGenerator); randomNumber < OutcomeProbabilities::calculateLossProbabilityHomeTeam(match)) {
                away->addPoints(3);
                match.setPointsAwayTeamAfterMatch(away->getPoints());
                match.setPointsHomeTeamAfterMatch(home->getPoints());

                const double ratingChange = -OutcomeProbabilities::calculateExpectedResult(match) * 20;
                home->addToRating(ratingChange);
                away->addToRating(-ratingChange);
                match.setRatingAwayTeamAfterMatch(away->getRating());
                match.setRatingHomeTeamAfterMatch(home->getRating());
                match.setResult(Result::LOSS);
            }

            else if (randomNumber < OutcomeProbabilities::calculateLossProbabilityHomeTeam(match) + OutcomeProbabilities::calculateDrawProbability(match)) {
                home->addPoints(1);
                away->addPoints(1);

                match.setPointsHomeTeamAfterMatch(home->getPoints());
                match.setPointsAwayTeamAfterMatch(away->getPoints());

                const double ratingChange = (0.5-OutcomeProbabilities::calculateExpectedResult(match)) * 20;
                home->addToRating(-ratingChange);
                away->addToRating(ratingChange);
                match.setRatingAwayTeamAfterMatch(away->getRating());
                match.setRatingHomeTeamAfterMatch(home->getRating());
                match.setResult(Result::DRAW);
            }

            else {
                home->addPoints(3);
                match.setPointsAwayTeamAfterMatch(away->getPoints());
                match.setPointsHomeTeamAfterMatch(home->getPoints());
                const double ratingChange = (1-OutcomeProbabilities::calculateExpectedResult(match)) * 20;
                home->addToRating(ratingChange);
                away->addToRating(-ratingChange);
                match.setRatingAwayTeamAfterMatch(away->getRating());
                match.setRatingHomeTeamAfterMatch(home->getRating());
                match.setResult(Result::WIN);
            }

            */
            const int goalsHomeTeam = OutcomeProbabilities::getHomeGoals(match, randomNumberGenerator);
            const int goalsAwayTeam = OutcomeProbabilities::getAwayGoals(match, randomNumberGenerator);

            match.setGoalsHomeTeam(goalsHomeTeam);
            match.setGoalsAwayTeam(goalsAwayTeam);

            home->addGoalsScored(goalsHomeTeam);
            home->addGoalsReceived(goalsAwayTeam);

            away->addGoalsScored(goalsAwayTeam);
            away->addGoalsReceived(goalsHomeTeam);

            match.setTiebreakerHomeTeamAfterMatch(home->getTieBreaker());
            match.setTieBreakerAwayTeamAfterMatch(away->getTieBreaker());



            if(goalsHomeTeam > goalsAwayTeam) {
                home->addPoints(3);
                match.setPointsHomeTeamAfterMatch(home->getPoints());
                match.setPointsAwayTeamAfterMatch(away->getPoints());
                match.setResult(Result::WIN);
                const double ratingChange = (1-OutcomeProbabilities::calculateExpectedResult(match)) * 20;
                home->addToRating(ratingChange);
                away->addToRating(-ratingChange);
                match.setRatingAwayTeamAfterMatch(away->getRating());
                match.setRatingHomeTeamAfterMatch(home->getRating());


            }
            else if(goalsHomeTeam < goalsAwayTeam) {
                away->addPoints(3);
                match.setResult(Result::LOSS);
                match.setPointsHomeTeamAfterMatch(home->getPoints());
                match.setPointsAwayTeamAfterMatch(away->getPoints());
                const double ratingChange = -OutcomeProbabilities::calculateExpectedResult(match) * 20;
                home->addToRating(ratingChange);
                away->addToRating(-ratingChange);
                match.setRatingAwayTeamAfterMatch(away->getRating());
                match.setRatingHomeTeamAfterMatch(home->getRating());

            }
            else {
                home->addPoints(1);
                away->addPoints(1);
                match.setResult(Result::DRAW);
                match.setPointsHomeTeamAfterMatch(home->getPoints());
                match.setPointsAwayTeamAfterMatch(away->getPoints());
                const double ratingChange = (0.5-OutcomeProbabilities::calculateExpectedResult(match)) * 20;
                home->addToRating(-ratingChange);
                away->addToRating(ratingChange);
                match.setRatingAwayTeamAfterMatch(away->getRating());
                match.setRatingHomeTeamAfterMatch(home->getRating());
            }

        }
    }
}


 void Simulator::simulateCompetition(TimeTable& timeTable, std::mt19937& randomNumberGenerator, int numberRoundsBeforeTheEnd)  {

    std::uniform_real_distribution<> dis(0.0, 1.0);  // Uniform distribution between 0 and 1

    const auto rounds = timeTable.getNumberOfRounds();

    for(int i = 0; i < rounds-numberRoundsBeforeTheEnd; i++) {
        auto& matches = timeTable.getMatchesInRound(i);
        for(auto& match : matches) {

            const auto home = match.getHomeTeam();
            const auto away = match.getAwayTeam();
            match.setPointsHomeTeamBeforeMatch(home->getPoints());
            match.setPointsAwayTeamBeforeMatch(away->getPoints());

            if (const double randomNumber = dis(randomNumberGenerator); randomNumber < OutcomeProbabilities::calculateLossProbabilityHomeTeam(match)) {
                away->addPoints(3);
                match.setPointsAwayTeamAfterMatch(away->getPoints());
                match.setPointsHomeTeamAfterMatch(home->getPoints());

                const double ratingChange = -OutcomeProbabilities::calculateExpectedResult(match) * 20;
                home->addToRating(ratingChange);
                away->addToRating(-ratingChange);
                match.setRatingAwayTeamAfterMatch(away->getRating());
                match.setRatingHomeTeamAfterMatch(home->getRating());
                match.setResult(Result::LOSS);
            }

            else if (randomNumber < OutcomeProbabilities::calculateLossProbabilityHomeTeam(match) + OutcomeProbabilities::calculateDrawProbability(match)) {
                home->addPoints(1);
                away->addPoints(1);

                match.setPointsHomeTeamAfterMatch(home->getPoints());
                match.setPointsAwayTeamAfterMatch(away->getPoints());

                const double ratingChange = (0.5-OutcomeProbabilities::calculateExpectedResult(match)) * 20;
                home->addToRating(-ratingChange);
                away->addToRating(ratingChange);
                match.setRatingAwayTeamAfterMatch(away->getRating());
                match.setRatingHomeTeamAfterMatch(home->getRating());
                match.setResult(Result::DRAW);
            }

            else {
                home->addPoints(3);
                match.setPointsAwayTeamAfterMatch(away->getPoints());
                match.setPointsHomeTeamAfterMatch(home->getPoints());
                const double ratingChange = (1-OutcomeProbabilities::calculateExpectedResult(match)) * 20;
                home->addToRating(ratingChange);
                away->addToRating(-ratingChange);
                match.setRatingAwayTeamAfterMatch(away->getRating());
                match.setRatingHomeTeamAfterMatch(home->getRating());
                match.setResult(Result::WIN);
            }
        }
    }
}

// Necessary For Schilling  (simulating last round a number of times)
void Simulator::simulateMatches(const std::vector<Match>& matches) {

     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_real_distribution<> dis(0.0, 1.0);

     for (const Match& currentMatch: matches) {

         if (const double randomNumber = dis(gen); randomNumber < OutcomeProbabilities::calculateLossProbabilityHomeTeam(currentMatch)) {
             currentMatch.getAwayTeam()->addPoints(3);
         }
         else if (randomNumber < OutcomeProbabilities::calculateLossProbabilityHomeTeam(currentMatch) + OutcomeProbabilities::calculateLossProbabilityHomeTeam(currentMatch)) {
             currentMatch.getHomeTeam()->addPoints(1);
             currentMatch.getAwayTeam()->addPoints(1);
         }
         else currentMatch.getHomeTeam()->addPoints(3);
     }
 }

