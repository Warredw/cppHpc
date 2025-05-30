
#include "utils/TeamManager/PremierLeague.h"
#include <unordered_map>

PremierLeague::PremierLeague(): TeamManager() {

    const std::unordered_map<std::string, double> teamRatings = {
        {"Arsenal", 1947},
        {"Liverpool", 1901},
        {"Man City", 2051},
        {"Chelsea", 1810},
        {"Newcastle United", 1802},
        {"Bournemouth", 1691},
        {"Tottenham", 1791},
        {"Aston Villa", 1770},
        {"Nottingham Forest", 1648},
        {"Fulham", 1716},
        {"Crystal Palace", 1760},
        {"Brighton", 1713},
        {"Man United", 1779},
        {"Brentford", 1711},
        {"Everton", 1707},
        {"West Ham United", 1726},
        {"Wolverhampton", 1678},
        {"Leicester", 1644},
        {"Ipswich", 1568},
        {"Southampton", 1600}
    };


    for (const auto &[fst, snd]: teamRatings) {
        teams.push_back(std::make_shared<Team>(fst, snd));
    }
}
