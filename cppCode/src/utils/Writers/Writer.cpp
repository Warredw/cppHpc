
#include "utils/Writers/Writer.h"

#include <utils/Metrics/RandomForestFeatures.h>

#include "utils/Metrics/RoundStatistics.h"


void Writer::writeTable(TimeTable& table, const std::string& filepath) {

    std::remove(filepath.c_str());;
    std::ofstream file(filepath, std::ios_base::trunc);

    if (!file.is_open()) {
        std::cerr<<"Could not open file"<<std::endl;
        return;
    }
    
    for (int roundIndex = 0; roundIndex < table.getRounds().size(); ++roundIndex) {
        Round round = table.getRounds().at(roundIndex);
        file << "Round " << roundIndex + 1 << std::endl;
        for (const Match& match : round.getMatches()) {
            file << match.getHomeTeam()->getName() << " vs " << match.getAwayTeam()->getName() << std::endl;
        }
        file << std::endl;
    }
    file.close();
}

void Writer::writeRanking(Tournament& tournament, const std::string& filepath) {
    std::ofstream file(filepath, std::ios_base::trunc);

    if (!file.is_open()) {
        std::cerr<<"Could not open file"<<std::endl;
        return;
    }

    std::vector<std::shared_ptr<Team>> teams = tournament.getTeamManager()->getTeams();
    std::sort(teams.begin(), teams.end(), [](const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b) {
        return a->getPoints() > b->getPoints();
    });

    for (const auto& team : teams) {
        file << team->getName() << " (" << team->getPoints() << ") " << team->getRating() <<  std::endl;
    }
    file.close();
}

// roundNumber is one indexed
void Writer::writeRankingBeforeRound(Tournament &tournament, const std::string &filepath, const int roundNumber) {
    std::ofstream file(filepath, std::ios_base::trunc);

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    file << "team,points" << std::endl;

    const auto ranking = RoundStatistics::makeRanking(tournament.getCompleteTimeTable().getRound(roundNumber - 2));

    // Go over the ranking to print the team and the number of points
    for (const auto &entry : ranking) {
        const auto &team = entry.first;
        const int points = entry.second;
        file << team->getName() <<","<< points << std::endl;
    }

    file.close();
}



void Writer::writeChampionsLeaugeMetrics(const std::vector<ChampionsLeagueMetrics> &metrics, const std::string &filepath) {

    std::ofstream file(filepath, std::ios_base::trunc);

    if(!file.is_open()) {
        std::cerr <<"Could not open file" << std::endl;
        return;
    }

    const int numberSimulations = static_cast<int>(metrics.size());

    file << "run";

    // Add columns dynamically for each round
    for (int round = 6; round <= 8; ++round) {
        file << ";number_irrelevant_matches_round_" << round;
        file << ";number_partially_relevant_matches_round_" << round;
        file << ";number_highly_relevant_matches_round_" << round;
        file << ";number_qualifiers_decided_round_" << round;
        file << ";number_playoffs_decided_round_" << round;
        file << ";number_relegation_decided_round_" << round;
        file << ";number_undecided_round_" << round;
    }
    file << std::endl;

    // Now fill the columns with data
    for (int i = 0; i < numberSimulations; ++i) {
        file << i;
        for (int round = 6; round <= 8; ++round) {
            file << ";" << metrics[i].getNumberIrrelevantMatches().at(round);
            file << ";" << metrics[i].getNumberPartiallyRelevantMatches().at(round);
            file << ";" << metrics[i].getNumberHighlyRelevantMatches().at(round);
            file << ";" << metrics[i].getNumberQualifiersDecided().at(round);
            file << ";" << metrics[i].getNumberPlayOffsDecided().at(round);
            file << ";" << metrics[i].getNumberRelegationDecided().at(round);
            file << ";" << metrics[i].getNumberUndecided().at(round);
        }
        file << std::endl;
    }
}

void Writer::writeRandomForestFeatures(const std::string &filepath, std::vector<RandomForestFeatures>& runs) {

    std::ofstream file(filepath, std::ios_base::trunc);

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    // Write header
    file << "run,team,Points,Place,distance_to_1,distance_to_4,distance_to_7,distance_to_17,distance_to_20" << std::endl;

    for (size_t run = 0; run < runs.size(); ++run) {
        const auto& teamPoints = runs[run].getTeamPoints();
        const auto& teamPlaces = runs[run].getTeamPlaces();
        const auto& distanceToFirst = runs[run].getDistanceToFirst();
        const auto& distanceToFourth = runs[run].getDistanceToFourth();
        const auto& distanceToSeventh = runs[run].getDistanceToSeventh();
        const auto& distanceToSeventeenth = runs[run].getDistanceToSeventeenth();
        const auto& distanceToLast = runs[run].getDistanceToLast();

        for (const auto& [teamPtr, points] : teamPoints) {
            if (teamPtr) {
                file << run << ","                              // run number
                     << teamPtr->getName() << ","               // team name
                     << points << ","                           // points
                     << teamPlaces.at(teamPtr) << ","           // place
                     << distanceToFirst.at(teamPtr) << ","      // distance to 1
                     << distanceToFourth.at(teamPtr) << ","     // distance to 4
                     << distanceToSeventh.at(teamPtr) << ","    // distance to 7
                     << distanceToSeventeenth.at(teamPtr) <<","      // distance to 17
                     <<distanceToLast.at(teamPtr) << std::endl;        // distance to 20

            }
        }
    }
    file.close();
}


void Writer::writeTeamBounds(const std::string& filepath, const std::vector<TeamBounds>& runs) {
    std::ofstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    // Write header
    file << "run,team,round,lowest,highest\n";

    for (size_t run = 0; run < runs.size(); ++run) {
        const auto& teamBounds = runs[run].getTeamBounds();

        for (const auto& [round, teamMap] : teamBounds) {
            for (const auto& [teamPtr, bounds] : teamMap) {
                if (teamPtr) {
                    file << run << ","                              // run number
                         << teamPtr->getName() << ","                // team name
                         << round + 1 << ","                             // round
                         << bounds.second << ","                     // lowest
                         << bounds.first << "\n";                    // highest
                }
            }
        }
    }
    file.close();
}

void Writer::writeLastRounds(const std::string& filepath, const std::vector<std::map<int, std::vector<Match>>>& matches) {
    std::ofstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    // Write header
    file << "run,round,home_team,away_team\n";

    for(size_t run = 0; run < matches.size(); ++run) {

        const auto& runMatches = matches[run];
        if(runMatches.empty()) continue;

        // now go over the rounds
        for(int i = 0; i <5; i++) {

            const auto& roundMatches = runMatches.at(i+33);

            // now go over the matches
            for(const auto& match : roundMatches) {
                file << run << ","
                     << 34+i << ","                             // round
                     << match.getHomeTeam()->getName() << ","   // home team name
                     << match.getAwayTeam()->getName() << "\n";   // away team name   // away team name
            }
        }
    }
    file.close();
}


void Writer::writeLastRoundsCL(const std::string& filepath, const std::vector<std::map<int, std::vector<Match>>>& matches) {
    std::ofstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    // Write header
    file << "run,round,home_team,away_team\n";

    for(size_t run = 0; run < matches.size(); ++run) {

        const auto& runMatches = matches[run];
        if(runMatches.empty()) continue;

        // now go over the rounds
        for(int i = 0; i <4; i++) {

            const auto& roundMatches = runMatches.at(i+4);

            // now go over the matches
            for(const auto& match : roundMatches) {
                file << run << ","
                     << 5+i << ","                             // round
                     << match.getHomeTeam()->getName() << ","   // home team name
                     << match.getAwayTeam()->getName() << "\n";   // away team name   // away team name
            }
        }
    }
    file.close();
}

void Writer::writeRankings(const std::string &filepath, const std::vector<Rankings> &runs) {
    std::ofstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    // Write header
    file << "run,round,team,rating,points,saldo\n";
    int runCounter = 0;

    // Iterate over each run
    for (int i = 0; i < runs.size(); ++i) {
        // Iterate over each round in the current run
        const auto &currentInfo = runs[i].getRankingInfo();

        for (const auto &[roundNumber, teamMap] : currentInfo) { // Access rankingInfo via a getter
            // Iterate over each team in the current round
            for (const auto &[team, data] : teamMap) {
                if (!team) {
                    std::cerr << "Error: Null team pointer encountered." << std::endl;
                    continue;
                }

                double rating = std::get<0>(data);
                double points = std::get<1>(data);
                int saldo = std::get<2>(data);

                file << runCounter << "," << roundNumber+1 << "," << team->getName() << ","
                     << rating << "," << points << "," << saldo << "\n";
            }
        }
        runCounter++;
    }

    file.close();
}