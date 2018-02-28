//
// Created by konstantin on 27/02/18.
//
#include <string>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

using namespace boost::asio;

class ExchangeConnection
{
public:
    ExchangeConnection(std::string url);
    ExchangeConnection(const ExchangeConnection& that) = delete;
    ExchangeConnection&operator=(ExchangeConnection const&) = delete;

    boost::system::error_code getTickersJson(std::string & strJson);
    boost::system::error_code getBalanceJson(std::string & strJson);
    boost::system::error_code sendOrder(const std::string& ordJson, std::string& responceJson);
private:
    boost::system::error_code establishConnection();
    std::string url_;
};


