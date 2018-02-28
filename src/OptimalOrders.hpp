//
// Created by konstantin on 27/02/18.
//

#pragma once

#include <vector>
#include <tuple>
#include "matrix.hpp"

std::vector <std::tuple<int, int>> getOptimalOrder(matrix& max, int maxN, double minProfit);