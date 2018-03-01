//
// Created by dimitar on 01.03.18.
//

#include "PathFinder.hpp"


PathFinder::PathFinder(const matrix m)
{
    /*
     *
     * THIS MIGHT NEED CHANGING AS IT WILL BE TOO SLOW TO COPY OER MEMORY ALL THE TIME
     *
     */
    std::copy(&m[0][0], &m[0][0] + NUMBER_CURRENCIES * NUMBER_CURRENCIES, &initial_rates_[0][0]);

    for (int i = 1; i < NUMBER_CURRENCIES; ++i)
        for (int j = 0; j < NUMBER_CURRENCIES; ++j)
            for (int k = 0; k < NUMBER_CURRENCIES; ++k)
                benefits_[i][j][k] = 0;
}

void PathFinder::initialize()
{
    for (short i = 0; i < NUMBER_CURRENCIES; ++i)
    {
        for (short j = 0; j < NUMBER_CURRENCIES; ++j)
        {
            //the most affordable path of 1 form i to j is to go directly
            if (initial_rates_[i][j] > 0)
                path_[1][i][j] = j;
            benefits_[0][i][j] = initial_rates_[i][j];
        }
    }
}

void PathFinder::SetRates(const matrix &m)
{
    std::copy(&m[0][0], &m[0][0] + NUMBER_CURRENCIES * NUMBER_CURRENCIES, &initial_rates_[0][0]);
}

std::vector<short> PathFinder::GetPath()
{
    double max_profit = 0;
    short max_profit_len = 0, max_profit_start = 0;

    for (short path_len = 1; path_len < NUMBER_CURRENCIES; ++path_len)
    {
        for (short from = 0; from < NUMBER_CURRENCIES; ++from)
        {
            for (short to = 0; to < NUMBER_CURRENCIES; ++to)
            {
                for (short through = 0; through < NUMBER_CURRENCIES; ++through)
                {
                    double possible_benefit = initial_rates_[from][through] * benefits_[path_len - 1][through][to];
                    double* current_benefit = &benefits_[path_len][from][to];

                    if (*current_benefit < possible_benefit)
                    {
                        *current_benefit = possible_benefit;
                        path_[path_len][from][to] = through;

                        if (max_profit < benefits_[path_len][from][from])
                        {
                            max_profit = benefits_[path_len][from][from];
                            max_profit_len = path_len;
                            max_profit_start = from;
                        }
                    }
                }
            }
        }
    }

    return PathFinder::ConstructPath(max_profit_len, max_profit_start);
}

std::string PathFinder::GetStrPath()
{
    return std::string();
}

std::vector<short> PathFinder::ConstructPath(short x, short from) {
    std::vector<short> path_to_return(x);

    short path_len = x, to = from;

    while (path_len > 0)
    {
        path_to_return.push_back(from);
        from = path_[path_len][from][to];
        path_len--;
    }

    std::reverse(path_to_return.begin(), path_to_return.end());

    return path_to_return;
}