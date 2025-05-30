#ifndef CHAMPIONSLEAGUEMETRICS_H
#define CHAMPIONSLEAGUEMETRICS_H

#include <vector>
#include <map>

class Tournament;

class ChampionsLeagueMetrics {

    std::map<int,int> numberIrrelevantMatches;
    std::map<int,int> numberHighlyRelevantMatches;
    std::map<int,int> numberPartiallyRelevantMatches;


    std::map<int,int> numberQualifiersDecided;
    std::map<int,int> numberPlayOffsDecided;
    std::map<int,int> numberRelegationDecided;
    std::map<int,int> numberUndecided;



public:

    // Getters
    [[nodiscard]] std::map<int,int> getNumberIrrelevantMatches() const;
    [[nodiscard]] std::map<int,int> getNumberHighlyRelevantMatches() const;
    [[nodiscard]] std::map<int,int> getNumberPartiallyRelevantMatches() const;
    [[nodiscard]] std::map<int,int> getNumberQualifiersDecided() const;
    [[nodiscard]] std::map<int,int> getNumberPlayOffsDecided() const;
    [[nodiscard]] std::map<int,int> getNumberRelegationDecided() const;
    [[nodiscard]] std::map<int,int> getNumberUndecided() const;


    void calculateMetrics(Tournament& tournament, int numberRoundsBeforeTheEnd);


};



#endif //CHAMPIONSLEAGUEMETRICS_H
