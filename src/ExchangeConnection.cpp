//
// Created by konstantin on 27/02/18.
//

#include <boost/log/trivial.hpp>
#include "ExchangeConnection.hpp"
#include <boost/lexical_cast.hpp>
#include <iostream>
using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

ExchangeConnection::ExchangeConnection(const std::string& host, const std::string& port,  const std::string& apiKey)
        : host_(host), port_(port), apiKey_(apiKey), resolver_(ioc_), socket_(ioc_)
{
    buildRequests();
}

boost::system::error_code ExchangeConnection::establishConnection()
{
    try {
        auto const results = resolver_.resolve(host_, port_);

        if (results.empty()) {
            BOOST_LOG_TRIVIAL(error) << "Can't resolve host :" << host_ << "\n";
            return boost::asio::error::host_not_found;
        }
        boost::asio::connect(socket_, results.begin(), results.end());
        socket_.set_option(tcp::no_delay(true));
        socket_.set_option(boost::asio::socket_base::keep_alive(true));
        if (!socket_.is_open()) {
            BOOST_LOG_TRIVIAL(info) << "Error opening socket to " << host_ << " :" << port_ << "\n";
            return boost::asio::error::broken_pipe;
        }
        BOOST_LOG_TRIVIAL(info) << "Opened socket to " << host_ << " :" << port_ << "\n";

    } catch (boost::system::system_error &e) {
        BOOST_LOG_TRIVIAL(error) << "Exception in establishing connection to exchange" << e.what() << "\n";
        return boost::asio::error::not_connected;
    }
    return boost::system::error_code{};
}

boost::system::error_code ExchangeConnection::getBalanceJson(std::string &strJson)
{
    try {
        //Send an http request to keep the connection alive
        http::write(socket_, getBalanceRequest_);
        std::cout <<getBalanceRequest_ << std::endl;
        //Buffer to store the reply in
        boost::beast::flat_buffer response_buffer;
        http::response<http::string_body> response;

        http::read(socket_, response_buffer, response);

        //BOOST_LOG_TRIVIAL(info) << "Got balance from server \n"
        //                        << response.body();
        strJson = response.body();
    } catch (boost::system::system_error &e) {
        BOOST_LOG_TRIVIAL(error) << "Exception in get balance " << e.what() << "\n";
        return boost::asio::error::not_connected;
    }
    return boost::system::error_code{};
}
boost::system::error_code ExchangeConnection::getMarketJson(std::string& strJson)
{
    try {
        //Send an http request to keep the connection alive
        http::write(socket_, getMarketRequest_);

        //Buffer to store the reply in
        boost::beast::flat_buffer response_buffer;
        http::response<http::string_body> response;

        http::read(socket_, response_buffer, response);

        //BOOST_LOG_TRIVIAL(info) << "Got balance from server \n"
        //                        << response.body();
        strJson = response.body();
    } catch (boost::system::system_error& e) {
        //BOOST_LOG_TRIVIAL(error) << "Exception in get  " << e.what() << "\n";
        return boost::asio::error::not_connected;
    }
    return boost::system::error_code{};
}
boost::system::error_code ExchangeConnection::sendOrder(const std::string& pair, double amount, std::string& responseJson)
{
    boost::beast::http::request<boost::beast::http::string_body> orderRequest;
    orderRequest.method(http::verb::get);
    orderRequest.version(11);
    //GET /trade/<api_key>/<buy|sell>/<ccy_pair>/<amount>
    //std::cout <<"Sending order with amount " << amount << "   and amountStr= " << boost::lexical_cast<std::string>(amount) << std::endl;
    std::string amountStr = std::move(boost::lexical_cast<std::string>(amount));

    try {
        orderRequest.target("/trade/" + apiKey_ + "/sell/" + pair + "/" + amountStr);
        orderRequest.set(http::field::host, host_);
        orderRequest.set(http::field::user_agent, "Botty");

        //BOOST_LOG_TRIVIAL(info) << "Making order request :\n" << orderRequest << std::endl;


        http::write(socket_, orderRequest);

        boost::beast::flat_buffer response_buffer;
        http::response<http::string_body> response;
        http::read(socket_, response_buffer, response);
        //BOOST_LOG_TRIVIAL(info) << "OrderResponse" << response.body() << "\n";


        responseJson = response.body();
    }catch(std::exception& e)
    {
        BOOST_LOG_TRIVIAL(error) << "Exception in send order  " << e.what() << "\n";
        return boost::asio::error::not_connected;
    }


}
void ExchangeConnection::buildRequests()
{
    getMarketRequest_.method(http::verb::get);
    getMarketRequest_.keep_alive(true);
    getMarketRequest_.version(11);
    getMarketRequest_.target("/market");
    getMarketRequest_.set(http::field::host, host_);
    getMarketRequest_.set(http::field::user_agent, "Botty");

    getBalanceRequest_.method(http::verb::get);
    getBalanceRequest_.keep_alive(true);
    getBalanceRequest_.version(11);
    getBalanceRequest_.target("/account/" + apiKey_);
    getBalanceRequest_.set(http::field::user_agent, "Botty");
    getBalanceRequest_.set(http::field::host, host_);
}