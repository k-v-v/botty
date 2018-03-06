//
// Created by konstantin on 27/02/18.
//
#include <string>
#include <string_view>
#include <functional>
#include <thread>
#include <atomic>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/http.hpp>
#include <boost/system/error_code.hpp>


class ExchangeConnection
{
public:
    ExchangeConnection(std::string host, std::string port, std::string target);
    ExchangeConnection(const ExchangeConnection& that) = delete;
    ExchangeConnection&operator=(ExchangeConnection const&) = delete;
    boost::system::error_code getTickerJson(const std::string& ticker, std::string& strJson);
    template<typename Func>
    boost::system::error_code getTickersBatch(Func func){
        using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
        namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

        for(auto& req : cached_requests_)
        {
            http::write(socket_, req);
        }


        for(auto _: cached_requests_)
        {
            boost::beast::flat_buffer response_buffer;
            http::response<http::string_body> response;

            http::read(socket_, response_buffer, response);
            func(response.body());
        }
        return boost::system::error_code();
    }
    boost::system::error_code getBalanceJson(std::string& strJson);
    boost::system::error_code sendOrder(std::string_view ordJson, std::string& responceJson);
    boost::system::error_code establishConnection();

    boost::system::error_code getTickerJsonCached(int tickerId, std::string &strJson);
    void cacheRequests(const std::vector<std::string>& tickers);
    bool isOpen();
private:
    std::vector<boost::beast::http::request<boost::beast::http::string_body>> cached_requests_;
    std::string host_;
    std::string port_;
    std::string target_;
    boost::asio::io_context ioc_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
};


