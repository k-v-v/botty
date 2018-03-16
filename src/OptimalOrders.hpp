//
// Created by konstantin on 27/02/18.
//

#pragma once

#include "matrix.hpp"

#include <string>
#include <tuple>
#include <vector>

typedef short path[NUMBER_CURRENCIES][NUMBER_CURRENCIES][NUMBER_CURRENCIES];
class OptimalOrders
{
public:
    std::vector <std::tuple<int, int>> getOptimalOrder(const matrix& max, int maxN, double minProfit);
    double getProfit(const std::vector<std::tuple<int, int>>& path,const matrix& mat)const;
    void printPath(const std::vector<std::tuple<int, int>>& path,const std::vector<std::string>& currencies)const;
private:
    double max_profit_;
    short max_profit_len_;
    short max_profit_start_;
    int baseId_;
    path path_;
    matrix benefits_[NUMBER_CURRENCIES] = {{{0}}};
    void initialize(const matrix& initial_rates);
    std::vector <std::tuple<int, int>> ExtractOrdersFromPath();
};