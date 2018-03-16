//
// Created by konstantin on 27/02/18.
//

#include "JsonParser.hpp"
#include <boost/log/trivial.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <exception>
JsonParser::JsonParser()
{
    initialized_  = false;
}
/**
 * Sample Json
    {
      "GBPJPY": "GBPJPY 145.3421 145.3429",
      "EURGBP": "EURGBP 0.9345 0.9349",
      "EURJPY": "EURJPY 136.7765 136.7771",
      "USDJPY": "USDJPY 105.6014 105.6018",
      "EURUSD": "EURUSD 1.2862 1.2864",
      "GBPUSD": "GBPUSD 1.3755 1.3759"
    }
 * */
void JsonParser::initializeWithBase(const std::string& marketJson, const std::string& baseName)
{
    if(initialized_)
    {
        throw std::invalid_argument("JsonParser is already initialized.");
    }

    rapidjson::Document doc;
    doc.Parse(marketJson.c_str());

    if(!baseName.empty())
    {
        nameToId_[std::string(baseName)] = 0;
        idToName_.push_back(std::string(baseName));
    }

    std::string baseString, quoteString;
    for(auto& e: doc.GetObject())
    {
        //Example str: "GBPJPY"
        std::string str = e.name.GetString();
        baseString = str.substr(0,3);
        quoteString = str.substr(3,3);


        if(nameToId_.find(baseString) == nameToId_.end())
        {
            nameToId_[baseString] = idToName_.size();
            idToName_.push_back(baseString);
        }

        if(nameToId_.find(quoteString) == nameToId_.end())
        {
            nameToId_[quoteString] = idToName_.size();
            idToName_.push_back(quoteString);
        }
    }

    initialized_ = true;
}
void JsonParser::parseMarket(const std::string& marketJson, matrix& mat)
{
    if(initialized_ == false)
    {
        throw std::invalid_argument("JsonParser isn't initialized.");
    }
    rapidjson::Document doc;
    doc.Parse(marketJson.c_str());

    std::string baseStr, quoteStr;
    std::string baseValueString, quoteValueString;

    int baseId, quoteId;
    int spaceLoc;
    double high, low;

    for(int i=0; i<NUMBER_CURRENCIES; i++)
        mat[i][i] = 0;
    for(auto& e: doc.GetObject())
    {
        //Parse only the value of the json field
        std::string str = e.value.GetString();

        baseStr = str.substr(0,3);
        quoteStr = str.substr(3,3);


        baseId = nameToId_[std::string(baseStr)];
        quoteId = nameToId_[std::string(quoteStr)];


        spaceLoc = str.find_last_of(' ');

        baseValueString = str.substr(7, spaceLoc - 7);
        quoteValueString = str.substr(spaceLoc + 1);

        high = boost::lexical_cast<double>(baseValueString);
        low =  boost::lexical_cast<double>(quoteValueString);

        mat[baseId][quoteId] = high;
        mat[quoteId][baseId] = 1/low;
    }
}
void JsonParser::parseBalance(const std::string& balanceJson, balance& bal)
{
    if(initialized_ == false)
    {
        throw std::invalid_argument("JsonParser isn't initialized.");
    }

    rapidjson::Document doc;
    doc.Parse(balanceJson.c_str());

    int currID;
    double currValue;
    std::string currStr;
    for(auto& e: bal)
        e = 0.0;
    for(auto& e: doc.GetObject())
    {
        currStr = e.name.GetString();
        currValue = e.value.GetDouble();

        currID = nameToId_[currStr];
        bal[currID] = currValue;
    }
}
const std::vector<std::string>& JsonParser::getTickers()
{
    if(initialized_ == false)
    {
        throw std::invalid_argument("JsonParser isn't initialized.");
    }
    return idToName_;
}