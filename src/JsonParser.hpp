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
    JsonParser();
    void initialize(const std::string& basePair, const std::string& tickers);
    void parseTicker(matrix &mat, const std::string& jsonStr);
    double parseResponse(std::string& response, std::string str);
    std::vector<std::string> encodeOrder(std::vector<std::tuple<int,int>>);
    const std::vector<std::string>& getTickers();
    int GetGbpIndex();
private:
    rapidjson::Document doc_;
    std::vector<std::string> idToName_;
    std::unordered_map<std::string, int> nameToId_;
    bool initialized_;
};

