//
// Created by dimitar on 01.03.18.
//

#ifndef BOOSTHTTP_PATHFINDER_HPP
#define BOOSTHTTP_PATHFINDER_HPP

#include "matrix.hpp"
#include <string>
#include <vector>
#include <algorithm>

typedef short path[NUMBER_CURRENCIES][NUMBER_CURRENCIES][NUMBER_CURRENCIES];

class PathFinder
{
public:
    PathFinder(const matrix m);
    void SetRates(const matrix& m);
    std::vector<short> GetPath();
    std::string GetStrPath();
    void initialize();
private:
    path path_;
    matrix initial_rates_;
    matrix benefits_[NUMBER_CURRENCIES];
    std::vector<short> ConstructPath(short x, short y);
};

#endif //BOOSTHTTP_PATHFINDER_HPP
