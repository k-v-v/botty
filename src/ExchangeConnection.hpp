//
// Created by konstantin on 27/02/18.
//
#include <string>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/system/error_code.hpp>



class ExchangeConnection
{
public:
    ExchangeConnection(std::string host, std::string port, std::string target);
    ExchangeConnection(const ExchangeConnection& that) = delete;
    ExchangeConnection&operator=(ExchangeConnection const&) = delete;

    boost::system::error_code getTickersJson(std::string& strJson);
    boost::system::error_code getBalanceJson(std::string& strJson);
    boost::system::error_code sendOrder(const std::string& ordJson, std::string& responceJson);
    boost::system::error_code establishConnection();
private:
    std::string host_;
    std::string port_;
    std::string target_;
    boost::asio::io_context ioc_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
};


