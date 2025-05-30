#ifndef CSVHELPER_H
#define CSVHELPER_H

#include "entities/TimeTable.h"
#include <tournament/Tournament.h>
#include "utils/Metrics/SecondaryMetrics.h"
#include "utils/Metrics/RandomForestFeatures.h"
#include "utils/Metrics/ChampionsLeagueMetrics.h"
#include "utils/Metrics/TeamBounds.h"
#include "utils/Metrics/Rankings.h"

#include <algorithm>


class Writer {

public:
    static void writeTable(TimeTable& table, const std::string& filepath);
    static void writeRanking(Tournament& tournament, const std::string& filepath);
    static void writeRankingBeforeRound(Tournament& tournament, const std::string& filepath, int roundNumber);
    static void writeChampionsLeaugeMetrics(const std::vector<ChampionsLeagueMetrics>& metrics, const std::string& filepath);


    static void writeRandomForestFeatures(const std::string& filepath, std::vector<RandomForestFeatures>& runs);
    static void writeTeamBounds(const std::string& filepath, const std::vector<TeamBounds>& runs);
    static void writeLastRounds(const std::string& filepath, const std::vector<std::map<int, std::vector<Match>>>& matches);
    static void writeRankings(const std::string& filepath, const std::vector<Rankings>& runs);


};



#endif //CSVHELPER_H
