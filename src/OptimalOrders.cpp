//
// Created by konstantin on 27/02/18.
//

#include "OptimalOrders.hpp"

void OptimalOrders::initialize(const matrix& initial_rates)
{
    for (short i = 0; i < NUMBER_CURRENCIES; ++i)
    {
        for (short j = 0; j < NUMBER_CURRENCIES; ++j)
        {
            //the most affordable path of 1 form i to j is to go directly
            if (initial_rates[i][j] > 0)
                path_[1][i][j] = j;
            benefits_[1][i][j] = initial_rates[i][j];
        }
    }
}

std::vector <std::tuple<int, int>> OptimalOrders::getOptimalOrder(const matrix& initial_rates, int maxN, double minProfit, double amount_selling, short GBP_index)
{
    OptimalOrders::initialize(initial_rates);

    max_profit_ = 0;
    max_profit_len_ = 0;
    max_profit_start_ = 0;

    for (short path_len = 2; path_len <= maxN; ++path_len)
        for (short from = 0; from < NUMBER_CURRENCIES; ++from)
            for (short to = 0; to < NUMBER_CURRENCIES; ++to)
            {
                matrix_type *current_benefit = &benefits_[path_len][from][to];
                
                for (short through = 0; through < NUMBER_CURRENCIES; ++through) {

                    double possible_benefit = initial_rates[from][through] * benefits_[path_len - 1][through][to];

                    if (*current_benefit < possible_benefit) {
                        *current_benefit = (float) possible_benefit;
                        path_[path_len][from][to] = through;

                        //if we only consider fro GBP to GBP, then the [from][from] below will be replaced with
                        //[GBP_index][GBP_index] and can be moved outside this loop
                        matrix_type *current_profit_rate_for_GBP = &benefits_[path_len][GBP_index][GBP_index];
                        
                        if (*current_profit_rate_for_GBP * amount_selling - 10 > max_profit_){
                            max_profit_ = *current_profit_rate_for_GBP * amount_selling - 10;
                            max_profit_len_ = path_len;
                            max_profit_start_ = from;

                            if (*current_profit_rate_for_GBP >= minProfit)
                                return OptimalOrders::ExtractOrdersFromPath();
                        }
                    }
                }
            }

    return OptimalOrders::ExtractOrdersFromPath();
}

std::vector <std::tuple<int, int>> OptimalOrders::ExtractOrdersFromPath()
{
    std::vector <std::tuple<int, int>> path_to_return;

    short path_len = max_profit_len_ - 1, to = max_profit_start_,
          from = path_[path_len + 1][to][to],
          prev = to;

    while (path_len >= 0) {
        path_to_return.emplace_back(prev, from);
        prev = from;
        from = path_[path_len][from][to];
        path_len--;
    }

    return path_to_return;
}