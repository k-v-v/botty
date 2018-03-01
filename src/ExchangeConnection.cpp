//
// Created by konstantin on 27/02/18.
//

#include <boost/log/trivial.hpp>
#include "ExchangeConnection.hpp"


using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

ExchangeConnection::ExchangeConnection(std::string host, std::string port, std::string target)
    : host_(host), port_(port), target_(target), resolver_(ioc_), socket_(ioc_)
{
}

boost::system::error_code ExchangeConnection::establishConnection()
{
    try {
        auto const results = resolver_.resolve(host_, port_);

        if (results.empty()) {
            BOOST_LOG_TRIVIAL(error) << "Can't resolve host :" << host_ << "\n";
            return boost::asio::error::host_not_found;
        }


        boost::asio::socket_base::keep_alive keep_alive_option(true);
        //socket_.set_option(keep_alive_option);
        boost::asio::connect(socket_, results.begin(), results.end());

        if (!socket_.is_open()) {
            BOOST_LOG_TRIVIAL(info) << "Error opening socket to " << host_ << " :" << port_ << "\n";
            return boost::asio::error::broken_pipe;
        }
        BOOST_LOG_TRIVIAL(info) << "Opened socket to " << host_ << " :" << port_ << "\n";

//
//        http::request<http::string_body> req{http::verb::get, target_, 11};
//        req.set(http::field::host, host_);
//        req.set(http::field::user_agent, "Botty");
//        req.set(http::field::keep_alive, true);
//        //Send an http request to keep the connection alive
//        http::write(socket_, req);
//
//        //Buffer to store the reply in
//        boost::beast::flat_buffer response_buffer;
//        http::response<http::string_body> response;
//
//        http::read(socket_, response_buffer, response);
//
//        BOOST_LOG_TRIVIAL(info) << "Got response from server \n"
//                                << response.body();
//
    }catch(boost::system::system_error& e)
    {
        BOOST_LOG_TRIVIAL(error) << "Exception in establish connection " << e.what() << "\n";
        return boost::asio::error::not_connected;
    }
    return boost::system::error_code{};
}
boost::system::error_code ExchangeConnection::getBalanceJson(std::string& strJson)
{

    try{
        http::request<http::string_body> req{http::verb::post, target_, 11};

        req.set(http::field::host, host_);
        req.set(http::field::user_agent, "Botty");
        req.set(http::field::keep_alive, true);
        //Set the request body
        req.body() = "GIVE ME MY BALANCE!!!!";
        //Calculate the body size for the header
        req.prepare_payload();

        //Send an http request to keep the connection alive
        http::write(socket_, req);

        //Buffer to store the reply in
        boost::beast::flat_buffer response_buffer;
        http::response<http::string_body> response;

        http::read(socket_, response_buffer, response);

        BOOST_LOG_TRIVIAL(info) << "Got balance from server \n"
                                << response.body();
        strJson = std::move(response.body());
    }catch (boost::system::system_error& e)
    {
        BOOST_LOG_TRIVIAL(error) << "Exception in get balance " << e.what() << "\n";
        return boost::asio::error::not_connected;
    }
    return boost::system::error_code{};
}


boost::system::error_code ExchangeConnection::getTickerJson(std::string ticker, std::string &strJson)
{
    try{
        http::request<http::string_body> req{http::verb::get, target_+"?"+ticker, 11};
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, "Botty");
        req.set(http::field::keep_alive, true);
        //Set the request body
        //req.body() = "GIVE ME ALL THE TICKERS!!!!";
        //Calculate the body size for the header
        //req.prepare_payload();

        //Send an http request to keep the connection alive
        http::write(socket_, req);

        //Buffer to store the reply in
        boost::beast::flat_buffer response_buffer;
        http::response<http::string_body> response;

        http::read(socket_, response_buffer, response);

        BOOST_LOG_TRIVIAL(info) << "Got tickers from server \n"
                                << response.body();
        strJson = std::move(response.body());

    }catch (boost::system::system_error& e)
    {
        BOOST_LOG_TRIVIAL(error) << "Exception in get ticker " << e.what() << "\n";
        return boost::asio::error::not_connected;
    }
    return boost::system::error_code{};
}


boost::system::error_code ExchangeConnection::sendOrder(const std::string &ordJson, std::string &responceJson)
{
    try {
        http::request<http::string_body> req{http::verb::post, target_, 11};
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, "Botty");
        req.set(http::field::keep_alive, true);
        //Set the request body
        req.body() = ordJson;
        //Calculate the body size for the header
        req.prepare_payload();

        //Send an http request to keep the connection alive
        http::write(socket_, req);

        //Buffer to store the reply in
        boost::beast::flat_buffer response_buffer;
        http::response<http::string_body> response;

        http::read(socket_, response_buffer, response);

        BOOST_LOG_TRIVIAL(info) << "Got made order to server \n"
                                << response.body();
        responceJson = std::move(response.body());
    }catch (boost::system::system_error& e)
    {
        BOOST_LOG_TRIVIAL(error) << "Exception in send order " << e.what() << "\n";
        return boost::asio::error::not_connected;
    }
    return boost::system::error_code{};
}