#include <iostream>


#include "ExchangeConnection.hpp"
#include "JsonParser.hpp"
#include "OptimalOrders.hpp"
#include "OrderResponse.hpp"


int main(int argc, char *argv[])
{
    ExchangeConnection exchange("localhost", "3000", "/posts");
    JsonParser parser;

    while(true)
    {
        std::cout << exchange.establishConnection().message() << std::endl;
    }

}