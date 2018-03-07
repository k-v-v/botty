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

//    OptimalOrders path_finder;
//    std::cout<<"Testing with a dummy matrix"<<std::endl;
//    matrix dummy = {{1, 1.7, 2}, {1, 1, 1.5}, {1.6, 1, 1}};
//    auto path = path_finder.getOptimalOrder(dummy, 3, 200);
//    auto profit = print_path(path, dummy);
//
//    std::cout<< profit*100 << "%" <<std::endl;


    https://api.fixer.io/latest
    ExchangeConnection exchange("api.fixer.io", "80", "/latest");
    JsonParser parser;
    exchange.establishConnection();
    std::string response;

    exchange.getTickerJson("USD",response);

    parser.initialize("USD",response);
    exchange.cacheRequests(parser.getTickers());



    matrix mat;
    auto parseLambda = [&parser, &mat](std::string& jsonStr){
        parser.parseTicker(mat, jsonStr);
    };
    std::cout << "Doing all tickers" << std::endl;

    boost ::system::error_code ec = exchange.getTickersBatch<decltype(parseLambda)>(parseLambda);
    if(ec)
    {
        BOOST_LOG_TRIVIAL(error) << "getTickersBatch error =" << ec.message() << "\n";
    }

    OptimalOrders path_finder;
    auto start = std::chrono::system_clock::now();
    auto path = path_finder.getOptimalOrder(mat, 5, 200);
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "elapsed time for path finding: " << elapsed_seconds.count() << "s\n";

    double profit = print_path(path, mat);

    std::cout<< profit*100 << "%" <<std::endl;

    /*
    std::cout << std::setprecision(2);
    for(int i=0; i < NUMBER_CURRENCIES; i++)
    {
        std::cout << parser.getTickers()[i] <<":";
        for(int j=0; j < NUMBER_CURRENCIES; j++)
        {
            std::cout << mat[i][j] << " ";
        }
        std::cout <<std::endl;
    }*/
}

double print_path(std::vector<std::tuple<int,int>> path, matrix mat) {
    double profit = 1;
    std::tuple<int,int> last;
    for (auto tuple : path)
    {
        profit *= mat[std::get<0>(tuple)][std::get<1>(tuple)];
        std::cout << std::get<0>(tuple) << " -> " ;
        last = tuple;
    }
    std::cout << std::get<1>(last)  << " " << std::endl;
    return profit;
}
