#include <iostream>
#include <iomanip>
#include <boost/log/trivial.hpp>

#include "ExchangeConnection.hpp"
#include "JsonParser.hpp"

#include "matrix.hpp"
#include "OptimalOrders.hpp"
#include "OrderResponse.hpp"


int main(int argc, char *argv[])
{
    https://api.fixer.io/latest
    ExchangeConnection exchange("api.fixer.io", "80", "/latest");
    JsonParser parser;
    exchange.establishConnection();
    std::string response;



    exchange.getTickerJson("USD",response);
    std::cout << response<<std::endl;

    parser.initialize("USD",response);
    exchange.cacheRequests(parser.getTickers());

    matrix mat;

    for (int i=0 ; i < parser.getTickers().size(); i++) {
        std::cout << "Doing " << parser.getTickers()[i] << std::endl;
        exchange.getTickerJsonCached(i, response);
        parser.parseTicker(mat, response);
        //std::cout << " " << response << std::endl;
    }


    OptimalOrders path_finder;

    auto path = path_finder.getOptimalOrder(mat, 10, 200);

    double profit = 1;
    std::tuple<int,int> last;
    for (auto tuple : path)
    {
        profit *= mat[std::get<0>(tuple)][std::get<1>(tuple)];
        std::cout<< std::get<0>(tuple) << " -> " ;
        last = tuple;
    }
    std::cout << std::get<1>(last)  << " " << std::endl << profit;

//    std::cout << std::setprecision(2);
//    for(int i=0; i < NUMBER_CURRENCIES; i++)
//    {
//        std::cout << parser.getTickers()[i] <<":";
//        for(int j=0; j < NUMBER_CURRENCIES; j++)
//        {
//            std::cout << mat[i][j] << " ";
//        }
//        std::cout <<std::endl;
//    }
}