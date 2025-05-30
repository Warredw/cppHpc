#include "entities/TimeTable.h"
#include <tournament/Tournament.h>
#include "utils/Metrics/SecondaryMetrics.h"

#include "utils/Metrics/ChampionsLeagueMetrics.h"

#ifndef TEAMBOUNDSWRITER_H
#define TEAMBOUNDSWRITER_H

#include "utils/Metrics/TeamBounds.h"


class TeamBoundsWriter {

public:
    static void writeTeamBounds(const std::string& filepath, const std::vector<TeamBounds>& runs);

};



#endif //TEAMBOUNDSWRITER_H
