#ifndef TEAMBOUNDS_H
#define TEAMBOUNDS_H

#include <map>
#include <memory>
#include "entities/Team.h"
#include "entities/Round.h"
#include "tournament/Tournament.h"


class TeamBounds {


    std::map<int,std::map<std::shared_ptr<Team>, std::pair<int, int>>> teamBounds;


public:

    // getter
    [[nodiscard]] std::map<int, std::map<std::shared_ptr<Team>, std::pair<int, int>>> getTeamBounds() const;

    // fill the instance variable
    void setTeamBounds(Tournament& tournament, int n);
};



#endif //TEAMBOUNDS_H
