//
// Created by konstantin on 27/02/18.
//

#pragma once

#include "matrix.hpp"
#include "JsonParser.hpp"

#include <string>
#include <tuple>
#include <vector>

typedef short path[NUMBER_CURRENCIES][NUMBER_CURRENCIES][NUMBER_CURRENCIES];

class OptimalOrders
{
public:
    std::vector <std::tuple<int, int>> getOptimalOrder(const matrix& max, int maxN, double minProfit, double amount, short GBP_index);
private:
    double max_profit_;
    short max_profit_len_;
    short max_profit_start_;
    path path_;
    matrix benefits_[NUMBER_CURRENCIES] = {{{0}}};
    void initialize(const matrix& initial_rates);
    std::vector <std::tuple<int, int>> ExtractOrdersFromPath();
};