//
// Created by konstantin on 27/02/18.
//

#include "JsonParser.hpp"

std::string JsonParser::encodeOrder(order order) {
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

void JsonParser::decodeResponce(OrderResponse &response, const std::string &str) {
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

void JsonParser::decodeTickers(matrix &mat, const std::vector<std::string>& json_tickers) {
    int i = 0;
    for (auto&& jsonTicker : json_tickers) {
        rapidjson::Document response;
        response.Parse(jsonTicker.c_str());

        //for each child at index j of response containing a value
        //  if j == i
        //      mat(i,j) = 1
        //  else
        //      mat(i,j) = (chil at at i).GetDouble();
        //  j++

        i++;
    }
}