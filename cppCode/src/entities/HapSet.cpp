#include "entities/HapSet.h"

#include <random>
#include <algorithm>
static constexpr int UNASSIGNED = -1;

HapSet::HapSet(const string& dNotation, const string& type, std::mt19937& randomNumberGenerator) : randomNumberGenerator(randomNumberGenerator) {

    this->type = type;
    hapSetFirstHalf = nullptr;
    hapSetSecondHalf = nullptr;
    this->dNotation = dNotation;
    nrTeams = 20; // we will only take a look at the premier league in the case of DRR
    makeHapSetFirstHalf(); // Initialize the (canonic) HAP-set
    shuffleHapSet(randomNumberGenerator); // In order to randomly assign it over the teams
    makeHapSetSecondHalf(); // = reversing the first half or inverting
    shuffleHapSetSecondHalf(randomNumberGenerator);
}


HapSet::~HapSet() {


    for(int i = 0; i < nrTeams; i++) {
        delete[] hapSetFirstHalf[i];
        delete[] hapSetSecondHalf[i];
    }
    delete[] hapSetFirstHalf;
    delete[] hapSetSecondHalf;
}

int** HapSet::getHapSetFirstHalf() const {
    return hapSetFirstHalf;
}

int** HapSet::getHapSetSecondHalf() const {
    return hapSetSecondHalf;
}

void HapSet::makeHapSetFirstHalf() {
    // 1) Allocate and fill with UNASSIGNED
    hapSetFirstHalf = new int*[nrTeams];
    for(int i = 0; i < nrTeams; i++) {
        hapSetFirstHalf[i] = new int[nrTeams-1];
        std::fill(
          hapSetFirstHalf[i],
          hapSetFirstHalf[i] + (nrTeams-1),
          UNASSIGNED
        );
    }

    // 2) Set the fixed round-1 “break” for team 1 & 2
    hapSetFirstHalf[0][0]         =  1;
    hapSetFirstHalf[0][nrTeams-2] =  1;
    hapSetFirstHalf[1][0]         =  0;
    hapSetFirstHalf[1][nrTeams-2] =  0;

    // 3) Fill team 1 & 2 alternating H/A in the mid rounds
    for(int j = 1; j < nrTeams-2; j++) {
        hapSetFirstHalf[0][j] = (j % 2 == 0 ? 1 : 0);
        hapSetFirstHalf[1][j] = (j % 2 == 0 ? 0 : 1);
    }

    // 4) Parse dNotation → break rounds
    vector<int> roundsWithBreak;
    for(int sum = 0, i = 0; i + 1 < (int)dNotation.size(); i++) {
        sum += (dNotation[i] - '0');
        roundsWithBreak.push_back(sum);
    }

    // 5) Stamp in those breaks, two teams at a time
    vector<int> breakRoundForTeam(nrTeams, -1);
    for(int team = 2, idx = 0; idx < roundsWithBreak.size(); idx += 1, team += 2) {
        int br = roundsWithBreak[idx];
        // both teams get the same break at rounds (br-1, br)
        hapSetFirstHalf[team][  br - 1] = 1;
        hapSetFirstHalf[team][  br    ] = 1;
        hapSetFirstHalf[team+1][br - 1] = 0;
        hapSetFirstHalf[team+1][br    ] = 0;
        breakRoundForTeam[team]   = br;
        breakRoundForTeam[team+1] = br;
    }

    // 6) Now fill *all* the remaining slots by stepping away from each break
    for(int team = 2; team < nrTeams; team++) {
        int br = breakRoundForTeam[team];
        // after the break
        for(int j = br + 1; j < nrTeams-1; j++)
            hapSetFirstHalf[team][j]
              = 1 - hapSetFirstHalf[team][j-1];
        // before the break
        for(int j = br - 2; j >= 0; j--)
            hapSetFirstHalf[team][j]
              = 1 - hapSetFirstHalf[team][j+1];
    }
}

// Function to shuffle the HAP-set

void HapSet::shuffleHapSet(std::mt19937& randomNumberGenerator) {

    // Create an index vector for teams
    vector<int> indices(nrTeams);
    for(int i = 0; i < nrTeams; i++) {
        indices[i] = i;
    }

    //std::shuffle(indices.begin(), indices.end(), randomNumberGenerator);

    // Create a new shuffled HAP-set
    int** shuffledHapSet = new int*[nrTeams];
    for(int i = 0; i < nrTeams; i++) {
        shuffledHapSet[i] = hapSetFirstHalf[indices[i]];
    }

    // Replace original HAP-set with shuffled version
    hapSetFirstHalf = shuffledHapSet;
}


void HapSet::shuffleHapSetSecondHalf(std::mt19937& randomNumberGenerator) {

    // Create an index vector for teams
    vector<int> indices(nrTeams);
    for(int i = 0; i < nrTeams; i++) {
        indices[i] = i;
    }

    //std::shuffle(indices.begin(), indices.end(), randomNumberGenerator);

    // Create a new shuffled HAP-set
    int** shuffledHapSet = new int*[nrTeams];
    for(int i = 0; i < nrTeams; i++) {
        shuffledHapSet[i] = hapSetSecondHalf[indices[i]];
    }

    // Replace original HAP-set with shuffled version
    hapSetSecondHalf = shuffledHapSet;
}


// Mirror the HAP-set to get the HAP-set of the second half
void HapSet::makeHapSetSecondHalf() {

    // Initialize size HAP-set second half
        hapSetSecondHalf = new int*[nrTeams];
        for (int i = 0; i < nrTeams; i++) {
            hapSetSecondHalf[i] = new int[nrTeams - 1];
        }

    if(type == "mirrored") {
        // Go over every value of the first half and take the reverse
        for(int i = 0; i < nrTeams; i++) {
            for(int j = 0; j < nrTeams-1; j++) {
                if(hapSetFirstHalf[i][j] == 1) hapSetSecondHalf[i][j] = 0;
                else hapSetSecondHalf[i][j] = 1;
            }
        }
    }

    else if (type == "inverted") {

        for(int i = 0; i < nrTeams; i++) {
            for(int j = nrTeams-2; j >= 0; j-- ) {
                if(hapSetFirstHalf[i][j] == 1) hapSetSecondHalf[i][nrTeams-2 - j ] = 0;
                else hapSetSecondHalf[i][nrTeams-2-j] = 1;
            }
        }

    }
}


