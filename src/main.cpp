#include <iostream>


#include "ExchangeConnection.hpp"
#include "JsonParser.hpp"
#include "matrix.hpp"
#include "OptimalOrders.hpp"
#include "OrderResponse.hpp"



void* operator new (size_t size, const char* filename, int line) {
    void* ptr = new char[size];
    return ptr;
}
int main(int argc, char *argv[])
{
    https://api.fixer.io/latest
    ExchangeConnection exchange("api.fixer.io", "80", "/latest");
    JsonParser parser;
    exchange.establishConnection();
    std::string response;

    exchange.getTickerJson("USD",response);
    parser.initialize("USD",response);


    for(auto& e: parser.getCurrencies())
    {
        std::cout << e << std::endl;
    }
    matrix mat;
    std::vector<std::string> jsonTickers;
    jsonTickers.push_back(response);
    parser.decodeTickers(mat, jsonTickers);

}