//
// Created by konstantin on 27/02/18.
//

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/system/error_code.hpp>

#include <string>
#include <string_view>
#include <functional>
#include <thread>
#include <atomic>

class ExchangeConnection
{
    void buildRequests();
public:
    ExchangeConnection(const std::string& host, const std::string& port, const std::string& apiKey);
    ExchangeConnection(const ExchangeConnection& that) = delete;
    ExchangeConnection&operator=(ExchangeConnection const&) = delete;


    boost::system::error_code establishConnection();
    boost::system::error_code getBalanceJson(std::string& strJson);
    boost::system::error_code getMarketJson(std::string& strJson);
    boost::system::error_code sendOrder(const std::string& pair, double amount, std::string &responceJson);
    bool isOpen();
private:
    std::string host_;
    std::string port_;
    std::string apiKey_;
    boost::asio::io_context ioc_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;

    std::vector<boost::beast::http::request<boost::beast::http::string_body>> cached_requests_;
    boost::beast::http::request<boost::beast::http::string_body> getBalanceRequest_;
    boost::beast::http::request<boost::beast::http::string_body> getMarketRequest_;

};


