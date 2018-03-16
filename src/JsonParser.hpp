//
// Created by konstantin on 27/02/18.
//

#pragma once

#include "matrix.hpp"
#include "order.hpp"
#include "OrderResponse.hpp"
#include "balance.hpp"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <vector>
#include <string>
#include <unordered_map>



class JsonParser {
public:
    JsonParser();
    void initializeWithBase(const std::string& marketJson, const std::string& baseName);
    void parseMarket(const std::string& marketJson, matrix& mat);
    void parseBalance(const std::string& balanceJson, balance& bal);
    const std::vector<std::string>& getTickers();
private:
    rapidjson::Document doc_;
    std::vector<std::string> idToName_;
    std::unordered_map<std::string, int> nameToId_;
    bool initialized_;
};

