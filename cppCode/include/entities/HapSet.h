#ifndef HAPSET_H
#define HAPSET_H

#include<iostream>
#include<vector>
#include <fstream>
#include <random>
#include <ilcplex/ilocplex.h>

using std::string;
using std::vector;


class HapSet {

    int** hapSetFirstHalf;
    int** hapSetSecondHalf;
    int nrTeams;
    string dNotation;
    string type;

    std::mt19937& randomNumberGenerator;

public:
    explicit HapSet(const string& dNotation, const string& type, std::mt19937& randomNumberGenerator);    // Shuffle indices randomly;
    ~HapSet();
    void makeHapSetFirstHalf();
    void shuffleHapSet(std::mt19937& randomNumberGenerator);
    void shuffleHapSetSecondHalf(std::mt19937& randomNumberGenerator);

    void makeHapSetSecondHalf();

    [[nodiscard]] int** getHapSetSecondHalf () const;
    [[nodiscard]] int** getHapSetFirstHalf () const;

};

#endif //HAPSET_H
