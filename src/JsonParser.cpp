//
// Created by konstantin on 27/02/18.
//

#include "JsonParser.hpp"
#include <iostream>
#include <boost/log/trivial.hpp>




JsonParser::JsonParser()
{
    initialized_  = false;
}


void JsonParser::initialize(const std::string basePair, const std::string& tickers)
{
    if (initialized_)
        return;

    rapidjson::Document doc;
    doc.Parse(tickers.c_str());

    idToName_.push_back(basePair);
    nameToId_.insert(std::make_pair(basePair, idToName_.size() - 1));
    for (auto& e: doc["rates"].GetObject())
    {
        idToName_.push_back(e.name.GetString());
        nameToId_.insert(std::make_pair(e.name.GetString(), idToName_.size() - 1));
    }
    initialized_ = true;
}
std::string JsonParser::encodeOrder(order order)const
{
    rapidjson::Document doc;
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> result_writer(buffer);
    rapidjson::Value from(rapidjson::kNumberType),
                     to(rapidjson::kNumberType),
                     amount(rapidjson::kNumberType);

    from.SetInt(order.from);
    to.SetInt(order.to);
    amount.SetDouble(order.amount);

    doc.SetObject();
    doc.AddMember("from", from, doc.GetAllocator());
    doc.AddMember("to", to, doc.GetAllocator());
    doc.AddMember("amount", amount, doc.GetAllocator());

    buffer.Clear();
    doc.Accept(result_writer);

    return (std::string) buffer.GetString();
}

void JsonParser::parseResponse(OrderResponse &response, const std::string &str)const
{
    rapidjson::Document doc;
    bool success;
    double amount;
    const char* success_tag = "was_successful";
    const char* amount_tag = "amount_converted";
    doc.Parse(str.c_str());


    if (!doc.HasMember(success_tag) || !doc.HasMember(amount_tag))
        printf("couldn\'t parse response, returning empty OrderResponse");

    success = doc[success_tag].GetBool();
    amount = doc[amount_tag].GetDouble();

    response.amount = amount;
    response.status = success ? OrderResponse::order_successful : OrderResponse::order_unsuccessful;
}

void JsonParser::parseTicker(matrix &mat, const std::string& jsonStr)const
{
    //{"base":"USD","date":"2018-03-02","rates":{"AUD":1.2902,"BGN":1.5885,"BRL":3.2567,"CAD":1.288,"CHF":0.93502,"CNY":6.3451,"CZK":20.638,"DKK":6.0493,"EUR":0.81222,"GBP":0.72547,"HKD":7.8295,"HRK":6.0465,"HUF":254.94,"IDR":13778.0,"ILS":3.4526,"INR":65.237,"ISK":100.63,"JPY":105.4,"KRW":1084.1,"MXN":18.928,"MYR":3.9203,"NOK":7.8117,"NZD":1.3819,"PHP":52.009,"PLN":3.4068,"RON":3.7845,"RUB":57.168,"SEK":8.2554,"SGD":1.3207,"THB":31.49,"TRY":3.8173,"ZAR":11.945}}

    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());

    std::string baseString = doc["base"].GetString();

    int baseID = nameToId_.at(baseString);

    BOOST_LOG_TRIVIAL(error) << "Got base string: " << baseString << " with id: " << baseID << "\n";

    for(auto& pair: doc["rates"].GetObject())
    {
        std::string otherString = pair.name.GetString();
        int otherId = nameToId_.at(otherString);
        mat[baseID][otherId] = pair.value.GetDouble();
    }

}
const std::vector<std::string>& JsonParser::getTickers()const
{
    return idToName_;
}