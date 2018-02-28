//
// Created by konstantin on 27/02/18.
//

#pragma once


#include <vector>
#include <string>
#include <unordered_map>

#include <rapidjson/document.h>

#include "matrix.hpp"
#include "order.hpp"
#include "OrderResponse.hpp"

class JsonParser {
public:
    void decodeTickers(matrix& mat, const std::string& jsonStr);
    void decodeResponce(OrderResponse& response, const std::string& str);
    std::string encodeOrder(order order);
private:
    std::vector<std::string> idToName_;
    std::unordered_map<std::string, int> nameToId_;
    bool initialized_;
};

