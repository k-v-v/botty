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
    auto parseLambda = [&parser, &mat](std::string& jsonStr){
        parser.parseTicker(mat, jsonStr);
    };
    std::cout << "Doing all tickers" << std::endl;

    exchange.getTickersBatch<decltype(parseLambda)>(parseLambda);

    std::cout << std::setprecision(2);
    for(int i=0; i < NUMBER_CURRENCIES; i++)
    {
        std::cout << parser.getTickers()[i] <<":";
        for(int j=0; j < NUMBER_CURRENCIES; j++)
        {
            std::cout << mat[i][j] << " ";
        }
        std::cout <<std::endl;
    }
}
