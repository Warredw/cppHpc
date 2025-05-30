#include "utils/TeamManager/ChampionsLeagueTeamManager.h"
#include <unordered_map>

ChampionsLeagueTeamManager::ChampionsLeagueTeamManager() {

    const std::unordered_map<std::string, std::tuple<int, double, std::string>> teamData = {
        {"Manchester City", {1, 2000, "England"}},
        {"Bayern Munich", {1, 2050, "Germany"}},
        {"Real Madrid", {1, 2100, "Spain"}},
        {"PSG", {1, 2020, "France"}},
        {"Liverpool", {1, 1980, "England"}},
        {"Inter", {1, 1880, "Italy"}},
        {"Borussia Dortmund", {1, 1900, "Germany"}},
        {"Leipzig", {1, 1800, "Germany"}},
        {"Barcelona", {1, 2080, "Spain"}},

        {"Bayer Leverkusen", {2, 1870, "Germany"}},
        {"Atletico Madrid", {2, 1920, "Spain"}},
        {"Atalanta", {2, 1850, "Italy"}},
        {"Juventus", {2, 1940, "Italy"}},
        {"Benfica", {2, 1740, "Portugal"}},
        {"Arsenal", {2, 1830, "England"}},
        {"Club Brugge", {2, 1720, "Belgium"}},
        {"Shakhtar Donetsk", {2, 1720, "Ukraine"}},
        {"AC Milan", {2, 1860, "Italy"}},

        {"Feyenoord", {3, 1700, "Netherlands"}},
        {"Sporting CP", {3, 1690, "Portugal"}},
        {"PSV", {3, 1680, "Netherlands"}},
        {"Lille", {3, 1670, "France"}},
        {"PAOK", {3, 1660, "Greece"}},
        {"Fenerbahçe", {3, 1650, "Turkey"}},
        {"Maccabi Tel Aviv", {3, 1640, "Israel"}},
        {"Young Boys", {3, 1630, "Switzerland"}},
        {"Celtic", {3, 1620, "Scotland"}},

        {"Galatasaray", {4, 1610, "Turkey"}},
        {"AS Monaco", {4, 1600, "France"}},
        {"Sparta Praha", {4, 1590, "Czech Republic"}},
        {"Aston Villa", {4, 1580, "England"}},
        {"Bologna", {4, 1570, "Italy"}},
        {"Girona", {4, 1560, "Spain"}},
        {"Stuttgart", {4, 1550, "Germany"}},
        {"Sturm Graz", {4, 1540, "Austria"}},
        {"Stade Brestois", {4, 1530, "France"}}
    };

    for (const auto &[team, data] : teamData) {
        int potNumber;
        double rating;
        std::string country;
        std::tie(potNumber, rating, country) = data;
        teams.push_back(std::make_shared<Team>(team, rating, potNumber, country));
    }
}
