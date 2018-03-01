//
// Created by dimitar on 01.03.18.
//

#ifndef BOOSTHTTP_PATHFINDER_HPP
#define BOOSTHTTP_PATHFINDER_HPP

#include "matrix.hpp"
#include <string>
#include <vector>
#include <algorithm>

typedef short path[MATRIX_DIM][MATRIX_DIM][MATRIX_DIM];

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
    matrix benefits_[MATRIX_DIM];
    std::vector<short> ConstructPath(short x, short y);
};

#endif //BOOSTHTTP_PATHFINDER_HPP
