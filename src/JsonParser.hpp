//
// Created by konstantin on 27/02/18.
//

#pragma once


#include <vector>
#include <string>
#include <unordered_map>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "matrix.hpp"
#include "order.hpp"
#include "OrderResponse.hpp"

class JsonParser {
public:
    JsonParser::JsonParser();
    void JsonParser::initialize(const std::string basePair, const std::string& tickers);
    void JsonParser::decodeTickers(matrix& mat, const std::vector<std::string>& jsonStr)const;
    void JsonParser::decodeResponce(OrderResponse& response, const std::string& str)const;
    std::string JsonParser::encodeOrder(order order)const;
    const std::vector<std::string>& JsonParser::getCurrencies()const;
private:
    std::vector<std::string> idToName_;
    std::unordered_map<std::string, int> nameToId_;
    bool initialized_;
};

