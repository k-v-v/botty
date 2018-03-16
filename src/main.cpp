#include <iostream>
#include <iomanip>
#include <boost/log/trivial.hpp>

#include "ExchangeConnection.hpp"
#include "JsonParser.hpp"

#include "matrix.hpp"
#include "OptimalOrders.hpp"
#include "OrderResponse.hpp"


double print_path(std::vector<std::tuple<int,int>> path, matrix mat);

int main(int argc, char *argv[])
{
    //The parser
    JsonParser parser;
    //Holds the current account balance
    balance bal;

    ExchangeConnection exchange("localhost", "8080", "06a094");
    exchange.establishConnection();

    std::string marketStr;
    exchange.getMarketJson(marketStr);
    parser.initializeWithBase(marketStr, "GBP");

    matrix mat;
    while(true) {
        BOOST_LOG_TRIVIAL(info) << "Started ordering" << std::endl;
        std::string balStr;
        exchange.getBalanceJson(balStr);

        parser.parseBalance(balStr, bal);
        exchange.getMarketJson(marketStr);
        parser.parseMarket(marketStr, mat);

        /*
        for (auto &row: mat) {
            for (auto e:row)
                std::cout << e << " ";
            std::cout << std::endl;
        }
        */
        OptimalOrders path_finder;
        auto start = std::chrono::system_clock::now();
        auto path = path_finder.getOptimalOrder(mat, 3, 200);
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        //std::cout << "elapsed time for path finding: " << elapsed_seconds.count() << "s\n";

        path_finder.printPath(path, parser.getTickers());
        double profit = path_finder.getProfit(path, mat);


        double amount = bal[0];
        std::string pair;
        auto &tickers = parser.getTickers();

        for (const auto &e: path) {
            pair = tickers[std::get<0>(e)] + tickers[std::get<1>(e)];
            exchange.sendOrder(pair, amount, balStr);
            parser.parseBalance(balStr, bal);

            //std::cout << "amount was" << amount << std::endl;
            //std::cout << "Balance after transaction is :" << std::endl;
            //for (auto v: bal)
            //    std::cout << v << std::endl;

            amount = bal[std::get<1>(e)];
        }

        BOOST_LOG_TRIVIAL(info) << "Balance now ordering, GBP=" << bal[0]<< std::endl;
        //std::cout << " Balance after :" << amount << std::endl;
        //std::cout << profit * 100 << "%" << std::endl;
    }
}
