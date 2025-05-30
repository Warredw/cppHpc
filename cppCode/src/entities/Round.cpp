#include "entities/Round.h"


std::vector<Match>& Round::getMatches() {
    return matches;
}

void Round::addMatch(const Match& match) {
    matches.push_back(match);
}

void Round::dropMatch() {
    if (!matches.empty()) {
        matches.pop_back();
    }
}