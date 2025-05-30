#ifndef RANKINGS_H
#define RANKINGS_H
#include <map>
#include <memory>
#include "entities/Team.h"
#include "tournament/Tournament.h"

class Rankings {


    // int = round with null index!
    std::map<int, std::map<std::shared_ptr<Team>, std::tuple<double, int, int>>> rankingInfo;



public:

    void setRanking(Tournament& tournament);
    std::map<int, std::map<std::shared_ptr<Team>, std::tuple<double, int, int>>> getRankingInfo() const;




};



#endif //RANKINGS_H
